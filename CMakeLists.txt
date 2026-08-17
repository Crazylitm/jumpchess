cmake_minimum_required(VERSION 3.7)
set(CMAKE_CXX_STANDARD 14)
project(yolo)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

# YOLO 推理库（ONNX DNN），单独编译避免 yolo.cpp 被重复编进可执行文件
add_library(yolo SHARED yolovx/yolo.cpp)
target_link_libraries(yolo ${OpenCV_LIBS})

set(SOURCE_FILES
    jump.cpp
    CheckersUI.cpp
    CheckersMapLimitCheck.cpp
    ChinessJumpChessControl.cpp
    ChessNode.cpp
    ChessCamera.cpp
    FrameProcessor/BGFGSegmentor.cpp
    FrameProcessor/Camera_OutPut_UI.cpp
    utils/inifile.cpp
    utils/SaveData2INIFile.cpp
)

add_executable(jump0 ${SOURCE_FILES})
target_link_libraries(jump0 ${OpenCV_LIBS} yolo)
