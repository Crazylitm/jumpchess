# jumpchess

机械手臂控制的人机中国跳棋博弈程序（Chinese Checkers with Robot Arm）

有兴趣的可以添加钉钉号：simomo

---

## 项目目标

| 阶段 | 目标 | 状态 |
|------|------|------|
| 第一阶段 | 纯软件跳棋棋盘 UI，支持完整走棋交互 | ✅ 完成 |
| 第二阶段 | 摄像头识别物理棋盘，映射到软件数据结构 | ✅ 完成 2023.3.11 |
| 第三阶段 | 软件指挥机械手臂准确移动棋子 | 进行中 |
| 第四阶段 | 完成人机对弈全流程 | 待完成 |
| 第五阶段 | 升级对弈算法，增强棋力 | 待完成 |

---

## 项目结构

```
jumpchess/
├── jump.cpp / jump.h          # 主程序入口，OpenCV 窗口事件循环
├── CheckersUI.cpp / .h        # 棋盘 UI 核心：绘制、鼠标响应、棋子移动
├── CheckersMapLimitCheck.cpp  # 坐标合法性检查、六边形距离计算
├── ChinessJumpChessControl.cpp# 跳棋规则引擎：合法跳跃路径计算
├── ChessCamera.cpp / .h       # 摄像头采集与 YOLO 识别联动
├── FrameProcessor/            # 图像处理：背景分割、棋子检测
│   ├── BGFGSegmentor          # 背景前景分割
│   ├── Chessboardinfo         # 棋盘信息管理
│   └── Camera_OutPut_UI       # 摄像头输出 UI
├── yolovx/
│   ├── yolo.h                 # YOLO 推理接口（已升级为 YOLO11n）
│   └── yolo.cpp               # YOLO 推理实现（anchor-free 解析）
├── models/
│   ├── best.onnx              # 当前生效模型：YOLO11n（mAP50=99.4%）
│   └── best_yolov5_backup.onnx# 旧 YOLOv5n 模型备份
├── yolov5_dataset/            # 训练数据集（185张训练图，6类棋子）
│   └── data.yaml              # 数据集配置
├── models/runs/
│   └── yolo11n_chess/         # YOLO11n 训练结果
├── build/                     # CMake 编译输出目录
│   └── jump0                  # 可执行文件
└── utils/                     # 工具类
```

---

## 编译与运行

### 依赖环境

- macOS（Apple Silicon 已验证，M3）
- CMake >= 3.7，C++14
- OpenCV（含 dnn 模块）
- Python 3.12 + ultralytics（用于训练/导出 YOLO 模型）

### 编译

```bash
cd build
cmake ..
make -j4
```

### 运行

```bash
./build/jump0
```

**退出方式：** 按 `Q` 键 或 `ESC` 键退出程序（窗口左下角有提示）

---

## 棋盘坐标系说明

棋盘使用逻辑坐标 `(x, y)`，范围 `x: 1~17, y: 1~17`：

```
坐标原点（5,1）= 屏幕像素 (600, 30)
像素公式：screen_x = 600 + x_dis(x,y) * 30
          screen_y = 30  + (y-1)     * 52
```

6色棋子起始位置：

| 颜色 | 起始区域 | 目标对角 |
|------|---------|---------|
| RED（红） | 顶部 (x=5-8, y=1-4) | 底部 ORANGE 区 |
| ORANGE（橙）| 底部 (x=10-13, y=14-17) | 顶部 RED 区 |
| GREEN（绿） | 右上 (x=10-13, y=5-8) | 左中 WHITE 区 |
| WHITE（白） | 左中 (x=5-8, y=10-13) | 右上 GREEN 区 |
| ROSERED（玫红）| 右中 (x=14-17, y=10-13) | 左下 BLUE 区 |
| BLUE（蓝） | 左下 (x=1-4, y=5-8) | 右中 ROSERED 区 |

---

## 核心数据结构

```cpp
// 棋盘状态双轨存储
int CircleMap[MAX_CHESS][3];                      // 快速数组：[x, y, color]
map<int, list<CircleReturn>> MapChessControlMemory; // 精确路径图

// 更新棋子状态（两个存储保持同步）
void CheckersUI::updateCircleMap(int i, Point p, ChessColor color);

// 获取合法跳跃路径（BFS）
void ChinessJumpChessControl::ProbableJumpPathALLShow(Point cur, int i, int type, list<Point>* out);
```

---

## YOLO 识别模块

### 升级历史

| 版本 | 时间 | mAP50 | 备注 |
|------|------|-------|------|
| YOLOv5n | 2023 | ~85% | 原始版本，anchor-based |
| **YOLO11n** | 2026.03 | **99.4%** | 当前版本，anchor-free |

### YOLO11n 模型信息

```
架构：  YOLO11n（Ultralytics，101层，258万参数，6.3 GFLOPs）
输入：  640×640 RGB
输出：  [1, 10, 8400]  即 [batch, 4+6classes, proposals]
6类：   B(蓝) G(绿) O(橙) R(红) R2(玫红) W(白)
训练集：185张训练图 + 53张验证图
```

各类别精度：

| 类别 | Precision | Recall | mAP50 | mAP50-95 |
|------|-----------|--------|-------|----------|
| B 蓝 | 97.3% | 99.7% | 99.2% | 88.1% |
| G 绿 | 98.3% | 99.6% | 99.5% | 88.8% |
| O 橙 | 99.8% | 98.3% | 99.5% | 89.6% |
| R 红 | 94.6% | 100%  | 99.0% | 88.8% |
| R2 玫红 | 99.8% | 99.4% | 99.5% | 88.6% |
| W 白 | 99.8% | 97.8% | 99.4% | 86.1% |
| **全部** | **98.3%** | **99.1%** | **99.4%** | **88.3%** |

### ONNX 输出解析（v11 vs v5 对比）

```cpp
// ── YOLOv5（旧，已废弃） ──────────────────────────────────────────
// 输出：[1, 25200, 5+nc]，需 anchor 解码 + obj_conf 过滤
// net_width = nc + 5;
// for stride × anchor × grid: decode(pdata[0..4], anchor_w/h)

// ── YOLO11n / YOLOv8（当前） ─────────────────────────────────────
// 输出：[1, 4+nc, 8400]，anchor-free，直接转置读取
Mat output0 = netOutputImg[0].reshape(1, {4 + nc, 8400});
Mat output_t;
transpose(output0, output_t);   // → [8400, 4+nc]
// 每行：[cx, cy, w, h, cls0, cls1, ..., cls(nc-1)]
// 类别分即最终置信度，无需乘 obj_conf
```

### 重新训练模型

```bash
cd /Users/mike/claude-work/jumpchess
source yolov5_env/bin/activate

# 训练（50 epochs，CPU，Apple M3 约 40 分钟）
yolo train model=yolo11n.pt \
    data=yolov5_dataset/data.yaml \
    epochs=50 imgsz=640 batch=8 \
    project=models/runs name=yolo11n_chess

# 导出 ONNX（opset=12 兼容 OpenCV DNN）
yolo export model=models/runs/yolo11n_chess/weights/best.pt \
    format=onnx opset=12 simplify=True

# 替换模型
cp models/runs/yolo11n_chess/weights/best.onnx models/best.onnx
```

---

## 本次代码修改记录（2026.03）

### 1. `CheckersUI.cpp` — 棋盘数字渲染 Bug 修复

**问题：** 棋子移动后，棋盘格子上的 CircleMap 编号消失，被棋子图像覆盖后不重绘。

**根本原因：**
- `chessmapmat_no_chess_org`（背景底图）在 `InitChess()` 之前保存，不含编号
- `printChess()` 恢复原位时从无编号背景恢复 → 擦掉编号
- `UpdateChessBoard()` 从无编号背景重建 → 所有编号消失

**修复方案：**

```cpp
// 修复1：UpdateChessBoard() — 全量重绘后补绘所有编号
chessmapmat = mat;
for(int ni=0; ni<MAX_CHESS; ni++){
    if(checker.IsLegalPosition(nx, ny)){
        putText(chessmapmat, text, Point(np.x-10, np.y+2), 1, 1, Scalar(0,0,0));
    }
}

// 修复2：鼠标移动后补绘出发位置和目标位置编号
printChess(oneMouseDownPose, curChessPoint, old_curColor);
// 补绘出发位置编号（格子恢复后保持编号）
sprintf(ntext, "%d", old_cur_i);
putText(chessmapmat, ntext, Point(oneMouseDownPose.x-10, oneMouseDownPose.y+2), ...);
// 补绘目标位置编号（叠加在棋子图像上方）
sprintf(ntext, "%d", cur_i);
putText(chessmapmat, ntext, Point(curChessPoint.x-10, curChessPoint.y+2), ...);
```

**效果：** 棋子移动后，格子编号始终可见；编号叠加在棋子图像上方，不被遮挡。

---

### 2. `CheckersUI.cpp` — 退出提示

在窗口左下角增加退出提示文字，`DrawBackground()` 每次刷新时自动绘制：

```cpp
putText(chessmapmat, "Press Q to quit",
        Point(10, 880), FONT_HERSHEY_SIMPLEX,
        0.6, Scalar(30,30,30), 1, LINE_AA);
```

---

### 3. `yolovx/yolo.h` + `yolovx/yolo.cpp` — YOLO v5 → YOLO11n 升级

**`yolo.h` 主要变更：**

```cpp
// 删除（YOLOv5 anchor 参数，v11 不需要）
- const float netAnchors[3][6] = { ... };
- const int strideSize = 3;
- const float netStride[4] = { 8, 16.0, 32, 64 };

// 保留（接口不变）
const int netWidth  = 640;
const int netHeight = 640;
float classThreshold    = 0.25f;
float nmsThreshold      = 0.45f;
```

**`yolo.cpp` 核心变更（`Detect()` 函数）：**

```cpp
// 旧 YOLOv5：三层 anchor 嵌套循环 + obj_conf 过滤（约60行）
for (int stride = 0; stride < strideSize; stride++)
    for (int anchor = 0; anchor < 3; anchor++)
        for (int i ... j ...): float box_score = pdata[4]; ...

// 新 YOLO11n：转置后线性扫描（约20行）
Mat output0 = netOutputImg[0].reshape(1, {4 + nc, 8400});
Mat output_t;
transpose(output0, output_t);           // [8400, 4+nc]
for (int i = 0; i < 8400; i++, pdata += (4+nc)) {
    float cx=pdata[0], cy=pdata[1], w=pdata[2], h=pdata[3];
    // pdata[4..9] 直接是各类别置信度
}
```

---

### 4. `models/best.onnx` — 模型文件替换

| | YOLOv5n（旧） | YOLO11n（新） |
|--|--------------|--------------|
| 文件大小 | 7.2 MB | 10.1 MB |
| mAP50 | ~85% | **99.4%** |
| 输出形状 | [1, 25200, 11] | [1, 10, 8400] |
| 推理方式 | anchor-based | anchor-free |

旧模型已备份至 `models/best_yolov5_backup.onnx`。

---

## Python 自动对弈脚本

通过 macOS CGEvent 模拟鼠标点击，驱动 jump0 程序自动下棋。

### 前置条件

```bash
# 1. 开启辅助功能权限
# 系统设置 → 隐私与安全 → 辅助功能 → 允许终端/Claude

# 2. 确认权限
swift -e 'import ApplicationServices; print(AXIsProcessTrusted())'
# 输出 true 则正常

# 3. 鼠标点击工具
# /tmp/clk.swift  — 发送单次左键点击
# /tmp/activate_jump0.swift — 将 jump0 窗口置前
```

### 2人对弈（RED vs ORANGE）

```bash
python3 /tmp/jump_game2.py
```

**特性：**
- 贪心评分 + 状态历史防循环
- 最多 400 手，超出判平局
- 实测：RED 在约 125 手内胜出

### 3v3 团队对弈

```bash
python3 -u /tmp/jump_game3v3.py 2>&1 | tee /tmp/3v3_log.txt
```

**队伍分配：**

| 队伍 | 颜色 | 目标 |
|------|------|------|
| Team A | RED + GREEN + ROSERED | 分别到达对角三个角 |
| Team B | ORANGE + WHITE + BLUE | 分别到达对角三个角 |

**AI 算法（v2，修复版）：**

```
1. 六边形精确距离：hex_dist(dx,dy) = max(|dx|,|dy|) 同号 / |dx|+|dy| 异号
2. 最优目标分配：贪心为每颗棋分配最近的未占目标格，最小化总路程
3. 方向惩罚：单棋距目标增加 n 格 → 扣 n×15 分（防止倒退）
4. 逐棋反振荡：记录每棋上一步位置，禁止原路返回
5. 跳棋链奖励：仅 +2 分（打破平分时偏好长跳，不覆盖方向评分）
6. 保护目标区：已入目标区的棋子离开扣 500 分
```

**Bug 修复记录（v1 → v2）：**

- **v1 问题：** `dist * 3` 跳棋奖励太大，反向 12 格长跳得 +36 分，覆盖 -10 的方向损失，导致棋子来回振荡
- **v2 修复：** 奖励改为 +2；新增方向惩罚 `n×15`；新增 `piece_history` 逐棋位置跟踪

**实测结果（2026.03.31）：**

```
Team A 🔴🟢🌸 胜出！总手数: 549，用时: 13分37秒

完成顺序：
  1st  RED     9分04秒
  2nd  ROSERED 9分45秒
  3rd  ORANGE  10分15秒（Team B）
  4th  BLUE    10分54秒（Team B）
  5th  GREEN   13分37秒（压轴完赛，Team A 胜定）
```

---

## 常见问题

**Q: 运行时鼠标不动？**
A: 检查辅助功能权限：系统设置 → 隐私与安全 → 辅助功能 → 允许对应终端程序

**Q: 识别精度下降？**
A: 重新训练模型，参考"重新训练模型"章节。训练数据在 `yolov5_dataset/`。

**Q: 编译报错找不到 OpenCV？**
A: `brew install opencv`，或手动指定 `cmake -DOpenCV_DIR=/path/to/opencv ..`

**Q: 编译报 YOLO 相关错误？**
A: 确认 `yolovx/yolo.h` 和 `yolovx/yolo.cpp` 是最新版（YOLO11n 格式，无 `netAnchors` 变量）

---

## 开发日志

| 日期 | 内容 |
|------|------|
| 2020.02 | 初始版本：纯软件跳棋 UI |
| 2023.01 | 接入 Roboflow 数据集，训练 YOLOv5n |
| 2023.03.11 | 完成摄像头识别棋盘第二阶段目标 |
| 2026.03.31 | 修复棋子编号渲染 Bug；升级 YOLO11n（mAP50=99.4%）；增加 AI 自动对弈脚本（2人/3v3） |
