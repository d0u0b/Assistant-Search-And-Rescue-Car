# Assistant Search & Rescue Car
"Assistant Search & Rescue Car" is a combination of Himax WE-I Plus EVB and ESP32.
Using TensorFlow Lite for Microcontroller (TFLM) and sensors, It has functions detecting terrain, detecting survivors, and calculating distance.

## 📝 Table of Contents
- [Introduction](#introduction)
  - [System Architecture](#system-architecture)
- [Hardware and Software Setup](#hardware-and-software-setup)
  - [Required Hardware](#required-hardware)
  - [Required Software](#required-software)
  - [Hardware Connection](#hardware-connection)
- [Usage](#usage)
- [DEMO](#demo)
- [Acknowledgments](#acknowledgments)

## 📖 Introduction
Assistant Search & Rescue Car can search for survivors in small or collapsed buildings for disaster rescuers.

### 🛠️ System Architecture


## 🧰 Hardware and Software Setup

### ⚙️ Required Hardware
- 1 Himax WE-I Plus EVB
- 1 ESP32
- 1 L298N
- 1 MPU6050 Accelerometer and Gyroscope
- 1 Two Wheels Smart Robot Car Chassis Kit
- 2 Ultrasonic sensor
- 2 Lithium rechargeable batteries

### 👨‍💻 Required Software
- Python 3.8

### ⛓️ Hardware Connection
![](https://i.imgur.com/QYPl87s.jpg)

#### Hardware Command Table

|   Command Name   | Value | WE-I Plus | ESP-32 | Server |
|:----------------:|:-----:|:---------:|:------:|:------:|
| PERSON_DETECT    | 0x81  | ✔         | ✔      |        |
| GET_ATTITUDE    | 0x82  |           | ✔      | ✔      |
| GET_ULTRASONIC   | 0x83  |           | ✔      | ✔      |
| GET_IP           | 0x91  |           | ✔      | ✔      |
| PERSON_FINDED    | 0x92  |           | ✔      | ✔      |
| GET_IMG          | 0x93  | ✔         | ✔      | ✔      |
| CONTINUE_FIND    | 0xA1  |           | ✔      | ✔      |
## ✍️ Usage

## 💻 DEMO

## 🙏 Acknowledgments

make file的ROOT_PATH須更改