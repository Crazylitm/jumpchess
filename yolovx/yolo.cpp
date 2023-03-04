#include"yolo.h"
using namespace std;
using namespace cv;
using namespace cv::dnn;

bool Yolo::readModel(Net& net, string& netPath, bool isCuda = false) {
	try {
		net = readNet(netPath);
	}
	catch (const std::exception&) {
		return false;
	}
	//cuda
	if (isCuda) {
		net.setPreferableBackend(cv::dnn::DNN_BACKEND_CUDA);
		net.setPreferableTarget(cv::dnn::DNN_TARGET_CUDA);
	}
	//cpu
	else {
		net.setPreferableBackend(cv::dnn::DNN_BACKEND_DEFAULT);
		net.setPreferableTarget(cv::dnn::DNN_TARGET_CPU);
	}
	return true;
}
bool Yolo::Detect(Mat& SrcImg, Net& net, vector<Output>& output,int iType) {
	Mat blob;
	int col = SrcImg.cols;
	int row = SrcImg.rows;
	int maxLen = MAX(col, row);
	Mat netInputImg = SrcImg.clone();
    std::vector<std::string> className;
    if(iType == 1){
        className = className_jump_chess;
    }else if(iType ==2){
        className = className_no_chess;
    }else{
        className = className_jump_chess;
    }
	if (maxLen > 1.2 * col || maxLen > 1.2 * row) {
		Mat resizeImg = Mat::zeros(maxLen, maxLen, CV_8UC3);
		SrcImg.copyTo(resizeImg(Rect(0, 0, col, row)));
		netInputImg = resizeImg;
	}
	blobFromImage(netInputImg, blob, 1 / 255.0, cv::Size(netWidth, netHeight), cv::Scalar(0, 0, 0), true, false);
	//如果在其他设置没有问题的情况下但是结果偏差很大，可以尝试下用下面两句语句
	//blobFromImage(netInputImg, blob, 1 / 255.0, cv::Size(netWidth, netHeight), cv::Scalar(104, 117, 123), true, false);
	//blobFromImage(netInputImg, blob, 1 / 255.0, cv::Size(netWidth, netHeight), cv::Scalar(114, 114,114), true, false);
	net.setInput(blob);
	std::vector<cv::Mat> netOutputImg;
	//vector<string> outputLayerName{"345","403", "461","output" };
	//net.forward(netOutputImg, outputLayerName[3]); //获取output的输出

    net.setPreferableBackend(DNN_BACKEND_CUDA);
    net.setPreferableTarget(DNN_TARGET_CUDA);
    double start_time = cv::getTickCount();
	net.forward(netOutputImg, net.getUnconnectedOutLayersNames());
    double end_time = cv::getTickCount();
    double freq = cv::getTickFrequency();
    //printf("cost time:  %.5f ms \n",1000*(end_time-start_time)/freq);
#if CV_VERSION_MAJOR==4&&CV_VERSION_MINOR==6
	std::sort(netOutputImg.begin(), netOutputImg.end(), [](Mat& A, Mat& B) {return A.size[1] > B.size[1]; });//opencv 4.6
#endif
	std::vector<int> classIds;//结果id数组
	std::vector<float> confidences;//结果每个id对应置信度数组
	std::vector<cv::Rect> boxes;//每个id矩形框
	float ratio_h = (float)netInputImg.rows / netHeight;
	float ratio_w = (float)netInputImg.cols / netWidth;
	int net_width = className.size() + 5;  //输出的网络宽度是类别数+5
	float* pdata = (float*)netOutputImg[0].data;
	for (int stride = 0; stride < strideSize; stride++) {    //stride
		int grid_x = (int)(netWidth / netStride[stride]);
		int grid_y = (int)(netHeight / netStride[stride]);
		for (int anchor = 0; anchor < 3; anchor++) {	//anchors
			const float anchor_w = netAnchors[stride][anchor * 2];
			const float anchor_h = netAnchors[stride][anchor * 2 + 1];
			for (int i = 0; i < grid_y; i++) {
				for (int j = 0; j < grid_x; j++) {
					float box_score = pdata[4]; ;//获取每一行的box框中含有某个物体的概率
					if (box_score >= boxThreshold) {
						cv::Mat scores(1, className.size(), CV_32FC1, pdata + 5);
						Point classIdPoint;
						double max_class_socre;
						minMaxLoc(scores, 0, &max_class_socre, 0, &classIdPoint);
						max_class_socre = (float)max_class_socre;
						if (max_class_socre >= classThreshold) {
							//rect [x,y,w,h]
							float x = pdata[0];  //x
							float y = pdata[1];  //y
							float w = pdata[2];  //w
							float h = pdata[3];  //h
							int left = (x - 0.5 * w) * ratio_w;
							int top = (y - 0.5 * h) * ratio_h;
							classIds.push_back(classIdPoint.x);
							confidences.push_back(max_class_socre * box_score);
							boxes.push_back(Rect(left, top, int(w * ratio_w), int(h * ratio_h)));
						}
					}
					pdata += net_width;//下一行
				}
			}
		}
	}

	//执行非最大抑制以消除具有较低置信度的冗余重叠框（NMS）
	vector<int> nms_result;
	NMSBoxes(boxes, confidences, nmsScoreThreshold, nmsThreshold, nms_result);
	for (int i = 0; i < nms_result.size(); i++) {
		int idx = nms_result[i];
        //
        if(confidences[idx] <0.6)
            continue;
		Output result;
		result.id = classIds[idx];
		result.confidence = confidences[idx];
		result.box = boxes[idx];
		output.push_back(result);
	}
	if (output.size())
		return true;
	else
		return false;
}

void Yolo::drawPred(Mat& img, vector<Output> result, vector<Scalar> color,int iType) {
    //{ "B", "G", "O", "R", "R2", "W"};
    int iB=0,iG=0,iO=0,iR=0,iR2=0,iW=0,iOther=0,iN=0;
    std::vector<std::string> className;
    if(iType == 1){
        className = className_jump_chess;
    }else if(iType ==2){
        className = className_no_chess;
    }else{
        className = className_jump_chess;
    }
	for (int i = 0; i < result.size(); i++) {
		int left, top;
		left = result[i].box.x;
		top = result[i].box.y;
		int color_num = i;

		rectangle(img, result[i].box, color[result[i].id], 1, 8);

        char buf[10];
        sprintf(buf, "%.2f", result[i].confidence);
        string label = className[result[i].id] + ":" +
                       "("+to_string((result[i].box.x))+":"+to_string((result[i].box.y))+
                       ":"+to_string((result[i].box.width))+":"+to_string((result[i].box.height))+
                       ")";
        if(iType == 2)
        {
            label = className[result[i].id];
        }


		int baseLine;
		Size labelSize = getTextSize(label, FONT_HERSHEY_SIMPLEX, 0.5, 2, &baseLine);
		top = max(top, labelSize.height);
		//rectangle(frame, Point(left, top - int(1.5 * labelSize.height)), Point(left + int(1.5 * labelSize.width), top + baseLine), Scalar(0, 255, 0), FILLED);
		putText(img, label, Point(left, top), FONT_HERSHEY_SIMPLEX, 0.5, color[result[i].id], 2);

        //summary info
        if(iType == 1){
            switch (result[i].id) {
                case 0:
                    iB++;
                    break;
                case 1:
                    iG++;
                    break;
                case 2:
                    iO++;
                    break;
                case 3:
                    iR++;
                    break;
                case 4:
                    iR2++;
                    break;
                case 5:
                    iW++;
                    break;
                default :
                    iOther++;
                    break;
            }
        }else if (iType == 2){
            iN++;
        }


	}

    //summary info
    if(iB >0 ){
        string label_B = string("B: ") + to_string(iB);
        putText(img, label_B, Point(20, 100), FONT_HERSHEY_SIMPLEX, 0.5, color[0], 2);
    }
    if(iG >0){
        string label_G = string("G: ") + to_string(iG);
        putText(img, label_G, Point(20, 120), FONT_HERSHEY_SIMPLEX, 0.5, color[1], 2);
    }
    if(iO >0){
        string label_O = string("O: ") + to_string(iO);
        putText(img, label_O, Point(20, 140), FONT_HERSHEY_SIMPLEX, 0.5, color[2], 2);
    }
    if(iR >0){
        string label_R = string("R: ") + to_string(iR);
        putText(img, label_R, Point(20, 160), FONT_HERSHEY_SIMPLEX, 0.5, color[3], 2);
    }
    if(iR2 >0){
        string label_R2 = string("R2: ") + to_string(iR2);
        putText(img, label_R2, Point(20, 180), FONT_HERSHEY_SIMPLEX, 0.5, color[4], 2);
    }
    if(iW >0){
        string label_W = string("W: ") + to_string(iW);
        putText(img, label_W, Point(20, 200), FONT_HERSHEY_SIMPLEX, 0.5, color[5], 2);
    }
    if(iN >0){
        string label_W = string("N: ") + to_string(iN);
        putText(img, label_W, Point(20, 220), FONT_HERSHEY_SIMPLEX, 0.5, color[5], 2);
    }

}
