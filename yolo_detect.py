# server/yolo_detect.py
import argparse
import time
import cv2
import paho.mqtt.client as mqtt
from ultralytics import YOLO   # ultralytics package (YOLOv8) OR adapt to YOLOv5

# --- CLI args ---
parser = argparse.ArgumentParser()
parser.add_argument("--source", default="http://ESP32_CAM_IP:81/stream", help="video source (http/rtsp/file)")
parser.add_argument("--model", default="ml/models/best_fire_model.pt", help="path to model")
parser.add_argument("--mqtt", default="mqtt://localhost", help="mqtt broker host or ip")
parser.add_argument("--mqtt_topic", default="drone/launch", help="mqtt topic to publish on fire")
parser.add_argument("--confidence", type=float, default=0.5)
args = parser.parse_args()

# --- MQTT setup ---
import urllib.parse
mqtt_host = "localhost"
mqtt_port = 1883
# if args.mqtt like "tcp://host:1883", parse host
if args.mqtt.startswith("mqtt://") or args.mqtt.startswith("tcp://"):
    host = args.mqtt.split("://")[1]
    parts = host.split(":")
    mqtt_host = parts[0]
    if len(parts) > 1:
        mqtt_port = int(parts[1])
else:
    mqtt_host = args.mqtt

client = mqtt.Client("yolo_fire_detector")
client.connect(mqtt_host, mqtt_port, 60)
client.loop_start()

# --- Load model ---
model = YOLO(args.model)  # for ultralytics YOLOv8
# model = torch.hub.load('ultralytics/yolov5','custom', path=args.model) # if using yolov5

cap = cv2.VideoCapture(args.source)
if not cap.isOpened():
    print("Cannot open source", args.source)
    exit(1)

fire_detected_cooldown = 5.0
last_fire_time = 0

while True:
    ret, frame = cap.read()
    if not ret:
        time.sleep(0.1)
        continue

    # Run inference (Ultralytics)
    results = model(frame, imgsz=640)  # returns list with .boxes for each result
    # For YOLOv8, iterate:
    fire_conf = 0.0
    for r in results:
        boxes = r.boxes
        for box in boxes:
            cls = int(box.cls.cpu().numpy()[0])
            conf = float(box.conf.cpu().numpy()[0])
            # Assume class 0 == fire (label mapping depends on training)
            if cls == 0 and conf >= args.confidence:
                fire_conf = max(fire_conf, conf)

    if fire_conf >= args.confidence:
        now = time.time()
        if now - last_fire_time > fire_detected_cooldown:
            print(f"[DETECT] fire detected conf={fire_conf:.2f} -> publishing launch")
            client.publish(args.mqtt_topic, "AUTO")
            last_fire_time = now

    # OPTIONAL: display (for debug)
    cv2.imshow("frame", frame)
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

cap.release()
cv2.destroyAllWindows()
client.loop_stop()
client.disconnect()
