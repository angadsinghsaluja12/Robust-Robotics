# Robust-Robotics
Project Title – Drone-Based Pneumatic Fire Extinguisher Ball System
Team ID : TH1189
Team Name: Robust Robotics

1. Overview:
A drone-mounted pneumatic launcher that deploys fire extinguisher balls. Video stream is analyzed with a YOLO-based model to detect fire; on detection or on manual button press, an MQTT command triggers the pneumatic solenoid to deploy.

2. Problem & Solution:
Problem: Rapid fire outbreaks at crowded events (e.g., Kumbh) and community cooking areas risk lives; response times are critical.
Solution: A drone + pneumatic launcher system with AI-based detection to deliver extinguisher balls remotely, minimizing human exposure and reducing response time.

3. Logic & Workflow:
• Camera (ESP32-CAM) streams video to detection server (Jetson/Laptop).  
• YOLO model runs inference; on 'fire' detection publishes MQTT message.  
• Valve ESP32 subscribes to MQTT and triggers solenoid to launch the fire ball.  
• Operator ESP32 with single button can force a launch (manual override).  

4. Tech Stack:
ESP32 (Arduino), ESP32-CAM, MQTT broker (Mosquitto), Python (OpenCV, Ultralytics/YOLO), Jetson Nano or Laptop, Pneumatic solenoid hardware.

5. How to run (high level):
• Flash `valve_receiver.ino` (valve controller) and `button_sender.ino` (operator) with Wi-Fi/MQTT details.
• Run mosquitto broker (or use cloud MQTT).
• Run `python server/yolo_detect.py --model ml/models/best_fire_model.pt`.
• Start esp32_cam_streamer to publish video to the server or use RTSP/HTTP stream.

6. Future Scope:
Autonomous swarm drones, advanced fire-classification, integration with emergency networks.

Repo: (provide GitHub URL)

Structure:
Drone-Fireball-Pneumatic/
├─ README.txt
├─ LICENSE
├─ firmware/
│  ├─ button_sender/                # ESP32 (operator) - single button to send manual launch
│  │  └─ button_sender.ino
│  ├─ valve_receiver/               # ESP32 (valve controller) - controls solenoid valve
│  │  └─ valve_receiver.ino
│  └─ esp32_cam/                    # ESP32-CAM streamer (optional)
│     └─ esp32_cam_streamer.ino
├─ server/
│  ├─ requirements.txt
│  ├─ yolo_detect.py                # YOLO detection + MQTT publisher
│  └─ train_yolo.sh                 # Example training script using Ultralytics YOLO
├─ ml/
│  ├─ dataset/
│  │  ├─ images/
│  │  │  ├─ train/
│  │  │  │  ├─ fire/                 # images of fires
│  │  │  │  └─ nonfire/              # images without fire
│  │  │  └─ val/
│  │  │     ├─ fire/
│  │  │     └─ nonfire/
│  │  └─ labels/                     # YOLO .txt label files if using YOLOv5 style
│  └─ models/
│     └─ best_fire_model.pt          # trained model (commit large models separately / via release)
└─ docs/
   └─ wiring-diagram.png
