#!/bin/bash
# 训练完成后：导出 ONNX 并替换模型文件
set -e

VENV=/Users/mike/claude-work/jumpchess/yolov5_env
RUNS=/Users/mike/claude-work/jumpchess/models/runs/yolo11n_chess
MODEL_OUT=/Users/mike/claude-work/jumpchess/models/best.onnx

source "$VENV/bin/activate"

BEST_PT="$RUNS/weights/best.pt"
if [ ! -f "$BEST_PT" ]; then
    echo "ERROR: best.pt not found at $BEST_PT"
    exit 1
fi

echo "==> Exporting $BEST_PT to ONNX (opset=12, simplify=True)..."
python -c "
from ultralytics import YOLO
model = YOLO('$BEST_PT')
model.export(format='onnx', opset=12, simplify=True, imgsz=640)
print('Export done.')
"

ONNX_FILE="${BEST_PT%.pt}.onnx"
if [ ! -f "$ONNX_FILE" ]; then
    echo "ERROR: .onnx not found at $ONNX_FILE"
    exit 1
fi

# 备份旧模型
cp "$MODEL_OUT" "${MODEL_OUT%.onnx}_yolov5_backup.onnx" 2>/dev/null || true
cp "$ONNX_FILE" "$MODEL_OUT"
echo "==> Model replaced: $MODEL_OUT"
ls -lh "$MODEL_OUT"

# 重新编译
echo "==> Rebuilding C++..."
cd /Users/mike/claude-work/jumpchess/build && make -j4
echo "==> ALL DONE. YOLO11n is live."
