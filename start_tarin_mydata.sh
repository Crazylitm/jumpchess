
#yolov3 darnet 的训练命令
#不带GPU的训练
./darknet detector train myData/my_data.data myData/my_yolov3_voc.cfg myData/darknet53.conv.74

#带GPU的训练
./darknet detector train myData/my_data.data myData/my_yolov3_voc.cfg myData/darknet53.conv.74 -gups 0,1,2,3


#从停止处重新训练

./darknet detector train myData/my_data.data myData/my_yolov3_voc.cfg darknet53.conv.74 -gups 0,1,2,3 myData/weights/my_yolov3.backup -gpus 0,1,2,3

#测试

./darknet detect myData/my_yolov3_voc.cfg myData/weights/my_yolov3_voc.backup  myData/JPEGImages/IMG_20190928_220044.jpg



#yolov5x 的训练##############################
python train.py --data JumpChess_YoloV5.yaml --cfg yolov5x.yaml --weights '' --batch-size 16

# Train YOLOv5s on COCO128 for 3 epochs
python train.py --img 640 --batch 16 --epochs 3 --data coco128.yaml --weights yolov5s.pt

#一个非常牛逼的lableimage的标注工具
https://app.roboflow.com/crazyjumpchess/jumpchess/2
账号：用户名：litianmin@gmail.com 密码: aa**!!
              litm7@qq.com   aa**!!
              tianmin.litianmin@taobao.com  aa**!!

#train 把batch从16改成2就不会killed
python train.py --data /home/litm/soft/yolov5/Jumplabel2_robotflow_data/data.yaml --cfg /home/litm/soft/yolov5/Jumplabel2_robotflow_data/yolov5x.yaml --weights /home/litm/soft/yolov5/Jumplabel2_robotflow_data/yolov5x.pt --batch-size 2  --device 0,1,2,3

#test
python test.py --weights /home/litm/soft/yolov5/runs/train/exp17/weights/best.pt --data /home/litm/soft/yolov5/Jumplabel2_robotflow_data/data.yaml --img 640



#推理图像
python detect.py --source /home/litm/git_code/jump0/inputImage/IMG_20190928_215308.jpg --weights /home/litm/soft/yolov5/runs/train/exp18/weights/best.pt 
--output /home/litm/soft/yolov5/Jumplabel2_robotflow_data/inference/output/1_img/
#推理目录
python detect.py --source inference/1_input/2_imgs --weights ./weights/yolov5s.pt --output inference/2_output/2_imgs
#检测中有些图片置信度比较低的可以通过--conf-thres参数过滤掉
python detect.py --source inference/1_input/2_imgs --weights ./weights/yolov5s.pt --output inference/2_output/2_imgs --conf-thres 0.8
#推理视频
python detect.py --source test.mp4 --weights ./weights/yolov5s.pt --output test_result/3_video
# 示例语法
python detect.py --source ./file.jpg  # 图片 
                          ./file.mp4  # 视频
                          ./dir  # 目录
                          0  # 网络摄像头
      'rtsp://170.93.143.139/rtplive/470011e600ef003a004ee33696235daa' # rtsp流
      'http://112.50.243.8/PLTV/88888888/224/3221225900/1.m3u8'  # http流
      
################################################################################################################   
#2023-01-24  手工达标265张图片
class: 
0:B
1:G
2:O
3:R
4:R2
5:W
#train 把batch从16改成2就不会killed
python train.py --data /home/litm/soft/yolov5/2023.1.24.JumpChess_Images_robotflow_data_train/data.yaml --cfg /home/litm/soft/yolov5/2023.1.24.JumpChess_Images_robotflow_data_train/yolov5x.yaml --weights /home/litm/soft/yolov5/2023.1.24.JumpChess_Images_robotflow_data_train/yolov5x.pt --batch-size 2  --device 0

python train.py --data /home/litm/soft/yolov5/2023.1.27.JumpChess_Images_robotflow_data_train/data.yaml --cfg /home/litm/soft/yolov5/2023.1.24.JumpChess_Images_robotflow_data_train/yolov5x.yaml --weights /home/litm/soft/yolov5/2023.1.24.JumpChess_Images_robotflow_data_train/yolov5x.pt --batch-size 2   --device 0
--device 0,1,2,3


python train.py --data /home/litm/soft/yolov5/myData/no_chess_image_for_train/2023.2.14.traindata1/data.yaml --cfg /home/litm/soft/yolov5/myData/no_chess_image_for_train/2023.2.14.traindata1/yolov5x.yaml --weights /home/litm/soft/yolov5/2023.1.24.JumpChess_Images_robotflow_data_train/yolov5x.pt --batch-size 2  --device 0
#推理图像

#Good Result
python detect.py --source /home/litm/git_code/jump0/inputImage/IMG_20190928_215308.jpg --weights /home/litm/soft/yolov5/runs/train/exp22/weights/best.pt 

python detect.py --source /home/litm/git_code/2023.1.23_roboflow.com.my_jump_chess_picture/test/2022-12-20-223219_jpg.rf.7f25a10b238c84872e6e89d5dae44801.jpg --weights /home/litm/soft/yolov5/runs/train/exp22/weights/best.pt 

python detect.py --source /home/litm/Pictures/Webcam/2022-12-20-221323.jpg --weights /home/litm/soft/yolov5/runs/train/exp22/weights/best.pt 

python detect.py --source 0 --weights /home/litm/soft/yolov5/runs/train/exp22/weights/best.pt


#export.py 的操作
python export.py --weights /home/litm/soft/yolov5/runs/train/exp22/weights/best.pt --img 640 --include onnx
python export.py --weights /home/litm/soft/yolov5/runs/train/exp22/weights/best.pt --img 640 --include onnx --dynamic
#使用https://netron.app/查看了静态的onnx和动态的onnx的网络结果有区别，具体差异不清楚
#目录在
/home/litm/soft/yolov5/runs/train/exp22/weights

/home/litm/soft/yolov5/2023.1.24.JumpChess_Images_robotflow_data_train/train_result_pt_modle_backup/export_sh_readme.txt

#export参考的文章连接
https://github.com/UNeedCryDear/yolov5-opencv-dnn-cpp
https://blog.csdn.net/weixin_43199832/article/details/122323192

https://github.com/ppogg/YOLO-Streaming
####c++ 调用 pytorch yolov5x.pt模型的方法的探索文章
https://blog.csdn.net/weixin_43199832/article/details/122323192
#两个使用c++调用pytorych的代码实例
https://www.pythonheidong.com/blog/article/896609/c049f91338cf8b56acbc/
http://www.manongjc.com/detail/14-wqycgvipwndihev.html
#yolov5的如果做预测的主战
https://github.com/ultralytics/yolov5/blob/master/detect.py
#yolov7的主站，目前我自己还没有尝试这个7,
https://github.com/WongKinYiu/yolov7
https://github.com/AlexeyAB/darknet
#yolov5的主站
https://github.com/ultralytics/yolov5
#参考opencv调用视频流的c++代码
https://learnopencv.com/read-write-and-display-a-video-using-opencv-cpp-python/

#代码实例，一个网友写的c++调用的案例
https://github.com/hpc203/yolov5-dnn-cpp-python
https://github.com/hpc203/yolov5-v6.1-opencv-onnxrun
#这个代码，可以学习opencv直接视频流在c++代码中如何交互预测
https://learnopencv.com/read-write-and-display-a-video-using-opencv-cpp-python/
https://zhuanlan.zhihu.com/p/484247759
#这个网友的zhihui
https://www.pianshen.com/article/27791819829/
#可以查看pt模型格式的可视化网站
https://netron.app/
https://github.com/lutzroeder/netron

#export.py转onnx模型c++调用
https://blog.csdn.net/qq_22487889/article/details/128011883
https://blog.csdn.net/xiaojunjun200211/article/details/121784214
https://blog.csdn.net/qq128252/article/details/127105463
#onnx的主站点
https://onnxruntime.ai/
https://github.com/microsoft/onnxruntime
https://github.com/microsoft/onnxruntime-inference-examples
#里面有c++代码
https://github.com/microsoft/onnxruntime/releases


############################
#相机标定相关文档
https://zhuanlan.zhihu.com/p/433912754  机械臂的手眼标定 opencv实现
开始检测图像


