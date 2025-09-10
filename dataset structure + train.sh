ml/dataset/
  images/
    train/
      fire/        (if using COCO-like folders) OR just images in train/
      nonfire/
    val/
  labels/
    train/
    val/

# data.yaml
train: ml/dataset/images/train
val: ml/dataset/images/val
names: ['fire','nonfire']
nc: 2

#!/bin/bash
# Example using ultralytics YOLOv8
# Install: pip install ultralytics

DATA=data.yaml
MODEL=yolov8n.pt
EPOCHS=50
BATCH=16

python -m ultralytics.train data=$DATA model=$MODEL epochs=$EPOCHS imgsz=640 batch=$BATCH project=ml/models name=fire_detector
# After training best model in ml/models/fire_detector/weights/best.pt -> move/rename to ml/models/best_fire_model.pt

# server/requirements.txt
opencv-python
paho-mqtt
ultralytics  # for YOLOv8
torch>=1.12   # if using YOLOv5 ensure correct torch & cuda versions if on Jetson
numpy

