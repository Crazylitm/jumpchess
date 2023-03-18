cmake_minimum_required(VERSION 3.7)
set(CMAKE_CXX_STANDARD 14)
project(yolo)
find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )
add_library(${PROJECT_NAME} SHARED yolovx/yolo.cpp)
set(SOURCE_FILES   utils/MultipleImageWindow.cpp utils/MultipleImageWindow.h
        main4.cpp jump.h jump.cpp
        CheckersUI.cpp CheckersUI.h
        CheckersMapLimitCheck.cpp CheckersMapLimitCheck.h
        ChessInclude.h CheckersImageObject.cpp
        CheckersImageObject.h MouseMoveChessOpt.cpp
        MouseMoveChessOpt.h ChinessJumpChessControl.cpp
        ChinessJumpChessControl.h ChessNode.cpp ChessNode.h
        ChessCamera.cpp ChessCamera.h
        FrameProcessor/FrameProcessor.h FrameProcessor/BGFGSegmentor.cpp
        FrameProcessor/BGFGSegmentor.h FrameProcessor/Camera_OutPut_UI.cpp FrameProcessor/Camera_OutPut_UI.h
        yolovx/yolo.cpp yolovx/yolo.h FrameProcessor/Chessboardinfo.cpp FrameProcessor/Chessboardinfo.h utils/inifile.cpp utils/inifile.h
        utils/inifile.cpp utils/inifile.h UI.h utils/SaveData2INIFile.cpp utils/SaveData2INIFile.h)
add_executable(jump0 ${SOURCE_FILES}   jump.cpp)
target_link_libraries( jump0 ${OpenCV_LIBS} yolo)


