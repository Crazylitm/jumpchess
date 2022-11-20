cmake_minimum_required(VERSION 3.7)
set(CMAKE_CXX_STANDARD 14)
project(jump0)
find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )
set(SOURCE_FILES   utils/MultipleImageWindow.cpp utils/MultipleImageWindow.h  main4.cpp jump.h jump.cpp CheckersUI.cpp CheckersUI.h CheckersMapLimitCheck.cpp CheckersMapLimitCheck.h ChessInclude.h CheckersImageObject.cpp CheckersImageObject.h MouseMoveChessOpt.cpp MouseMoveChessOpt.h ChinessJumpChessControl.cpp ChinessJumpChessControl.h ChessNode.cpp ChessNode.h ChessCamera.cpp ChessCamera.h FrameProcessor/FrameProcessor.h FrameProcessor/BGFGSegmentor.cpp FrameProcessor/BGFGSegmentor.h)
add_executable(jump0 ${SOURCE_FILES}   jump.cpp)
target_link_libraries( jump0 ${OpenCV_LIBS} )


