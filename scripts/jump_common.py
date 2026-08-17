#!/usr/bin/env python3
"""对弈脚本共享工具：棋盘常量、坐标换算、鼠标控制（预编译 Swift 助手）。

原来每个脚本各自用 `swift clk.swift` / `swift -e` 每次落子现场编译/启动 Swift 进程，
速度极慢。这里把 click / move / activate 统一到一个 Swift 源码 `jumpctl.swift`，
首次使用时 `swiftc` 编译一次得到二进制，之后直接 exec 该二进制（无编译开销）。
"""
import os
import subprocess
import time

SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))
REPO_DIR = os.path.dirname(SCRIPT_DIR)
BUILD_DIR = os.path.join(REPO_DIR, 'build')
_JUMPCTL_SRC = os.path.join(SCRIPT_DIR, 'jumpctl.swift')
_JUMPCTL_BIN = os.path.join(SCRIPT_DIR, '.jumpctl')

# 棋盘常量（与 C++ CheckersUI.h 对齐）
BEGX, BEGY = 600, 30          # RED_TOP_5_1_X / RED_TOP_5_1_Y
XWIDTH, YHIGH = 30, 52        # 每格 x/y 像素间距
WIN_X, WIN_Y = 0, 53          # jump0 窗口内容区在屏幕上的原点（标题栏偏移）
CHESS_RADIUS = 19


def x_dis(x, y):
    # 与 C++ getDistanceFromRED_5_1_point_X() 等价：y - 2*x + 9
    return y - 2 * x + 9


def board_to_screen(bx, by):
    px = BEGX + x_dis(bx, by) * XWIDTH
    py = BEGY + (by - 1) * YHIGH
    return WIN_X + px, WIN_Y + py


def _ensure_compiled():
    """仅在源文件比二进制新（或二进制缺失）时编译一次。"""
    if os.path.exists(_JUMPCTL_BIN):
        try:
            if os.path.getmtime(_JUMPCTL_SRC) <= os.path.getmtime(_JUMPCTL_BIN):
                return _JUMPCTL_BIN
        except OSError:
            pass
    subprocess.run(['swiftc', _JUMPCTL_SRC, '-o', _JUMPCTL_BIN], check=True)
    return _JUMPCTL_BIN


def click(x, y):
    subprocess.run([_ensure_compiled(), 'click', str(int(x)), str(int(y))], check=True)


def move_cursor(x1, y1, x2, y2):
    subprocess.run([_ensure_compiled(), 'move',
                    str(int(x1)), str(int(y1)),
                    str(int(x2)), str(int(y2))], check=True)


def activate_jump0():
    subprocess.run([_ensure_compiled(), 'activate'], check=True)


def launch_jump0():
    """若 jump0 未运行则启动并置前。"""
    r = subprocess.run(['pgrep', '-x', 'jump0'], capture_output=True, text=True)
    if not r.stdout.strip():
        subprocess.Popen(['./jump0'], cwd=BUILD_DIR,
                         stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
        time.sleep(4)
        activate_jump0()
        time.sleep(0.5)
