//
// Created by litm on 2020/1/26.
//
#include "CheckersUI.h"

int main(int, char *argv[]) {
    namedWindow(WINDOW_NAME_CHESS);
    CheckersUI ui;
    ui.Triangle();
    ui.DrawBackground();
    ui.DrawButton(1);
    ui.InitChess();  // Registers mouse callback + draws chess pieces + builds jump graph
    Mat bg = ui.getmat();
    imshow(WINDOW_NAME_CHESS, bg);

    // Event loop: ESC or Q quits; also exits if the window is closed via the OS
    while (true) {
        int key = waitKey(30);
        if (key == 27 || key == 'q' || key == 'Q') break; // ESC or Q
        // Detect OS window-close button (title-bar X)
        if (getWindowProperty(WINDOW_NAME_CHESS, WND_PROP_VISIBLE) < 1) break;
    }
    destroyAllWindows();
    return 0;
}
