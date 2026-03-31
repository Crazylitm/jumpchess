#include "yolo.h"
using namespace std;
using namespace cv;
using namespace cv::dnn;

// ── 加载 ONNX 模型 ────────────────────────────────────────────────────
bool Yolo::readModel(Net& net, string& netPath, bool isCuda) {
    try {
        net = readNet(netPath);
    } catch (const std::exception&) {
        return false;
    }
    if (isCuda) {
        net.setPreferableBackend(DNN_BACKEND_CUDA);
        net.setPreferableTarget(DNN_TARGET_CUDA);
    } else {
        net.setPreferableBackend(DNN_BACKEND_DEFAULT);
        net.setPreferableTarget(DNN_TARGET_CPU);
    }
    return true;
}

// ── YOLO11 / YOLOv8 检测 ─────────────────────────────────────────────
// 输出形状: [1, 4+nc, 8400]  转置后 → [8400, 4+nc]
// 每行:    [cx, cy, w, h, cls0, cls1, ..., cls(nc-1)]
// 无 anchor 解码, 无 obj_conf, 类别分即最终置信度
bool Yolo::Detect(Mat& SrcImg, Net& net, vector<Output>& output, int iType) {
    std::vector<std::string> className;
    if      (iType == 1) className = className_jump_chess;
    else if (iType == 2) className = className_no_chess;
    else                 className = className_jump_chess;
    int nc = (int)className.size();

    int col = SrcImg.cols;
    int row = SrcImg.rows;
    int maxLen = MAX(col, row);
    Mat netInputImg = SrcImg.clone();

    // letterbox: 短边补黑以保持宽高比
    float ratio_h = (float)row / netHeight;
    float ratio_w = (float)col / netWidth;
    if (maxLen > 1.2f * col || maxLen > 1.2f * row) {
        Mat resizeImg = Mat::zeros(maxLen, maxLen, CV_8UC3);
        SrcImg.copyTo(resizeImg(Rect(0, 0, col, row)));
        netInputImg = resizeImg;
        ratio_h = (float)maxLen / netHeight;
        ratio_w = (float)maxLen / netWidth;
    }

    Mat blob;
    blobFromImage(netInputImg, blob, 1.0/255.0,
                  cv::Size(netWidth, netHeight),
                  cv::Scalar(0, 0, 0), true, false);
    net.setInput(blob);

    std::vector<cv::Mat> netOutputImg;
    net.forward(netOutputImg, net.getUnconnectedOutLayersNames());

    // YOLO11/v8 输出: [1, 4+nc, 8400] → 转置 → [8400, 4+nc]
    int total_boxes = netOutputImg[0].size[2];          // 通常 8400
    Mat output0 = netOutputImg[0].reshape(1, {4 + nc, total_boxes});
    Mat output_t;
    transpose(output0, output_t);                        // → [total_boxes, 4+nc]

    std::vector<int>    classIds;
    std::vector<float>  confidences;
    std::vector<Rect>   boxes;

    float* pdata = (float*)output_t.data;
    for (int i = 0; i < total_boxes; i++, pdata += (4 + nc)) {
        float cx = pdata[0];
        float cy = pdata[1];
        float w  = pdata[2];
        float h  = pdata[3];

        // 找最大类别得分
        float maxScore  = -1.0f;
        int   maxCls    = -1;
        for (int c = 0; c < nc; c++) {
            if (pdata[4 + c] > maxScore) {
                maxScore = pdata[4 + c];
                maxCls   = c;
            }
        }

        if (maxScore < classThreshold) continue;

        int left   = (int)((cx - 0.5f * w) * ratio_w);
        int top    = (int)((cy - 0.5f * h) * ratio_h);
        int width  = (int)(w * ratio_w);
        int height = (int)(h * ratio_h);
        classIds.push_back(maxCls);
        confidences.push_back(maxScore);
        boxes.push_back(Rect(left, top, width, height));
    }

    // NMS
    vector<int> nms_result;
    NMSBoxes(boxes, confidences, nmsScoreThreshold, nmsThreshold, nms_result);
    for (int idx : nms_result) {
        if (confidences[idx] < 0.6f) continue;
        Output result;
        result.id         = classIds[idx];
        result.confidence = confidences[idx];
        result.box        = boxes[idx];
        output.push_back(result);
    }
    return !output.empty();
}

// ── 画框 + 标签 ───────────────────────────────────────────────────────
void Yolo::drawPred(Mat& img, vector<Output> result, vector<Scalar> color, int iType) {
    std::vector<std::string> className;
    if      (iType == 1) className = className_jump_chess;
    else if (iType == 2) className = className_no_chess;
    else                 className = className_jump_chess;

    int iB=0,iG=0,iO=0,iR=0,iR2=0,iW=0,iOther=0,iN=0;

    for (int i = 0; i < (int)result.size(); i++) {
        int left = result[i].box.x;
        int top  = result[i].box.y;

        rectangle(img, result[i].box, color[result[i].id], 1, 8);

        char buf[16];
        snprintf(buf, sizeof(buf), "%.2f", result[i].confidence);
        string label;
        if (iType == 2) {
            label = className[result[i].id];
        } else {
            label = className[result[i].id] + ":"
                  + "(" + to_string(result[i].box.x)   + ":"
                        + to_string(result[i].box.y)   + ":"
                        + to_string(result[i].box.width)+ ":"
                        + to_string(result[i].box.height) + ")";
        }

        int baseLine;
        Size labelSize = getTextSize(label, FONT_HERSHEY_SIMPLEX, 0.5, 2, &baseLine);
        top = max(top, labelSize.height);
        putText(img, label, Point(left, top), FONT_HERSHEY_SIMPLEX,
                0.5, color[result[i].id], 2);

        if (iType == 1) {
            switch (result[i].id) {
                case 0: iB++;  break;
                case 1: iG++;  break;
                case 2: iO++;  break;
                case 3: iR++;  break;
                case 4: iR2++; break;
                case 5: iW++;  break;
                default: iOther++; break;
            }
        } else if (iType == 2) {
            iN++;
        }
    }

    // 汇总计数
    int ty = 100;
    auto put_summary = [&](const string& s, int classId){
        putText(img, s, Point(20, ty), FONT_HERSHEY_SIMPLEX, 0.5, color[classId], 2);
        ty += 20;
    };
    if (iB  > 0) put_summary("B: "  + to_string(iB),  0);
    if (iG  > 0) put_summary("G: "  + to_string(iG),  1);
    if (iO  > 0) put_summary("O: "  + to_string(iO),  2);
    if (iR  > 0) put_summary("R: "  + to_string(iR),  3);
    if (iR2 > 0) put_summary("R2: " + to_string(iR2), 4);
    if (iW  > 0) put_summary("W: "  + to_string(iW),  5);
    if (iN  > 0) {
        putText(img, "N: " + to_string(iN), Point(20, ty),
                FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0,0,0), 2);
    }
}
