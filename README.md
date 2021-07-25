# Assistant Search & Rescue Car
Assistant Search & Rescue Car (ASRC) can search for survivors in small or collapsed buildings for disaster rescuers.

## 📝 Table of Contents
- [Introduction](#-introduction)
  - [System Architecture](#%EF%B8%8F-system-architecture)
- [Hardware and Software Setup](#-hardware-and-software-setup)
  - [Required Hardware](#-required-hardware)
  - [Required Software](#-required-software)
  - [Hardware Connection](#%EF%B8%8F-hardware-connection)
  - [Software Setup](#%EF%B8%8F-software-setup)
- [Usage](#%EF%B8%8F-usage)
- [DEMO](#-demo)

## 📖 Introduction
"Assistant Search & Rescue Car" is a combination of Himax WE-I Plus EVB and ESP32.
Using TensorFlow Lite for Microcontroller (TFLM) and sensors, It has functions detecting obstacle, detecting survivors, and calculating distance.


### 🛠️ System Architecture
![image](https://user-images.githubusercontent.com/43933375/126891478-6ee90f6d-012f-4749-b60c-5cc852e481a6.png)


## 🧰 Hardware and Software Setup

### 👨‍🔧 Required Hardware
- 1 Himax WE-I Plus EVB
- 1 ESP32
- 1 L298N
- 1 MPU6050 Accelerometer and Gyroscope
- 1 Two Wheels Smart Robot Car Chassis Kit
- 2 Ultrasonic sensor
- 2 Lithium rechargeable batteries

### 👨‍💻 Required Software
- Python 3.8
- Virtualenv
- Arduino 1.8.13
- NodeJs 12.22.1 (Optional)

### ⛓️ Hardware Connection
![image](https://user-images.githubusercontent.com/43933375/126891421-e636d7a5-ad17-45ee-bace-04df83d1da05.png)

#### Hardware Command Table

|   Command Name   | Value | WE-I Plus |  ESP32 | Server |
|:----------------:|:-----:|:---------:|:------:|:------:|
| PERSON_DETECT    | 0x81  | ✔         | ✔      |        |
| GET_ATTITUDE     | 0x82  |           | ✔      | ✔      |
| GET_ULTRASONIC   | 0x83  |           | ✔      | ✔      |
| GET_IP           | 0x91  |           | ✔      | ✔      |
| PERSON_FINDED    | 0x92  |           | ✔      | ✔      |
| GET_IMG          | 0x93  | ✔         | ✔      | ✔      |
| CONTINUE_FIND    | 0xA1  |           | ✔      | ✔      |

### ⚙️ Software Setup
#### Step 1: Prepare environment.
- Clone this project into `Synopsys_SDK/User_Project`.
- Download `Arduino_librarys.zip` from Releases and extract into the Arduino library folder.
- Open the Arduino Manage Libraries, Search and Download `HCSR04 ultrasonic sensor` library.

#### Step 2: Burn program into hardware.
- Open `./we-i`, build and burn into WE-I plus EVB.
- Open `./esp/esp.ino`, burn into ESP-32

#### Step 3: Install Server's dependency. **(Optional)**
If you download the server application, you don't need to install dependency.
- Open `./server` in command line.
- Run `npm install` to install dependency.

## ✍️ Usage
### Step 1: Open server application
Use two methods to open the application.
- Download application from releases and open `ASRC Server` file.
- install npm dependency and run `npm start` command.

### Step 2: Open the ASRC
Connect the ASRC Car to the power supply and wait for the MPU6050 calibration to be completed.

### Step 3: Start
Press `啟動車子` button，ASRC will start looking for survivors.
ASRC draws a route when walking, and avoids obstacles if it encounters obstacles.
When ASRC detects the survivor, it will stop and take a photo, and then send it back to the server application to display the photo.

## 💻 DEMO
We will update the DEMO video after this contest. 🤗
