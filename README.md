# 🏠 Raspberry Pi Pico W – Home Alarm System

This repository contains the firmware for a home alarm system using a Raspberry Pi Pico W. The device uses an ultrasonic distance sensor to detect proximity, communicates with a backend over MQTT, and activates a buzzer and LEDs based on alarm status and distance thresholds.

---

## 📋 Project Description

This code is for a home alarm setup using a Raspberry Pi Pico W connected to an ultrasonic sensor, a buzzer, and some LED lights. It measures distance using the sensor and passes the result on through MQTT to the backend running on a Raspberry Pi Zero 2 W.

- If the **alarm is enabled** from the frontend:
  - When something gets within **50 cm**:
    - The **buzzer beeps intermittently**
    - The **blue LED** turns on
  - When something gets within **20 cm**:
    - The **alarm is triggered**
    - The **buzzer sounds continuously**
    - The **red LED** turns on
    - A **Pushover notification** is triggered in the backend

> For further explanation of the backend functionality, see the corresponding repository.

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