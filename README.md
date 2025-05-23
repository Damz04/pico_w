# Home Alarm System on Raspberry Pi Zero 2 W and Pico W

## Overview

This project is a real-time sensor-based alarm monitoring system developed using Flask,
MQTT, SQLite, Javascript, C, and Pushover notification service. It is deployed on a Raspberry
Pi Zero 2 W and includes both a frontend dashboard and a backend system to monitor
distance data from a connected device (Raspberry Pico W) and trigger alerts when objects
get too close. This repository holds the code for the Raspberry Pico W.

## Raspberry Pi Pico W Functionality

The Raspberry Pi Pico W in this project functions as the sensor interface and IoT edge
device. It handles real-time distance measurements using an ultrasonic sensor and
communicates with the Raspberry Pi Zero 2 W (which acts as the backend server and
dashboard host) over MQTT via Wi-Fi. Below is a breakdown of its responsibilities:

### 1. Ultrasonic Sensor Integration:
 - The Pico W reads distance values using a TRIG and ECHO pin setup.
 - It calculates the time delay to determine the distance to an object in meters.

### 2. Wi-Fi Connectivity:
 - The Pico W connects to a predefined Wi-Fi network using the CYW43 driver.
 - It retries connection attempts and prints IP information on success.

### 3. MQTT Communication:
 - On successful Wi-Fi connection, the Pico initializes an MQTT client.
 - It publishes:
 - motion/distance: Regular distance readings.
 - device/status: "online" on startup and "offline" as a will message.
 - It subscribes to:
 - device/alarm: To receive and update the current alarm state.
 - device/alarm/request: To trigger a state synchronization with the server.

### 4. LED and Buzzer Feedback:
 - Based on the measured distance, the Pico W controls:
 - Green LED: Safe distance.
 - Blue LED and intermittent buzzing: Medium range (warning).
 - Red LED and continous buzzing: Danger zone (object is too close).
 - If the alarm is disabled, all outputs are turned off.

### 5. Alarm Synchronization:
 - At startup, the Pico W requests the current alarm state.
 - On receiving "on" or "off" via MQTT, it updates its local flag to control response behavior.

> To view the repository for the Raspberry Pi Zero 2 W, follow this url: https://github.com/Damz04/pi_zero

---

## 🚀 How to Set Up and Build

Follow these steps to set up your environment, build the project, and flash it to the Pico W.

### 1. Clone this Repository

```bash
git clone https://github.com/Damz04/pico_w.git
cd pico_w

```

### 2. Clone and Initialize the Pico SDK

The Raspberry Pi Pico SDK is required to build this project. Clone it into the root of this project folder:

```bash
git clone https://github.com/raspberrypi/pico-sdk.git
cd pico-sdk
git submodule update --init
cd ..

```

### 3. Set the PICO_SDK_PATH Environment Variable

You must tell the build system where to find the SDK. Run this in the terminal:

```bash
export PICO_SDK_PATH=$(pwd)/pico-sdk
echo 'export PICO_SDK_PATH=$(pwd)/pico-sdk' >> ~/.bashrc
source ~/.bashrc

```

### 4. Install Required Tools

Ensure the following packages are installed:

```bash
sudo apt update
sudo apt install cmake build-essential picotool

```

## 🛠️ Build the Project

From the root of the project folder:

```bash
mkdir build
cd build
cmake ..
make

```

After building, you should see a .uf2 file in the build/ directory — this is the firmware file you’ll flash to the Pico W.

## 🔌 Flash the Firmware to the Pico W

1. Press and hold the BOOTSEL button on your Pico W.

2. While holding it, connect the Pico W to your computer via USB.

3. It should appear as a USB drive on your system.

4. Copy the generated .uf2 file to the Pico using drag-and-drop

The device will reboot and begin running the firmware.

## 📡 Network & MQTT Notes

The Pico W must be configured with valid Wi-Fi credentials in the code (edit with your own Wi-Fi credentials before building).

Ensure the MQTT broker is accessible on your network and that setup_mqtt_client in mqtt.c is configured with the correct IP.

The backend (e.g., Raspberry Pi Zero 2 W) should be running and listening to the appropriate topics.
