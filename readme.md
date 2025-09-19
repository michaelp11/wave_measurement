# Wave Height Measurement with Arduino

Measure the wave motion on bodies of water with a low-cost Arduino setup using a wooden frame and a floating mechanism.

## Table of Contents

- [Overview](#overview)
- [How it works](#how-it-works)
- [Hardware Setup](#hardware-setup)
- [Software Usage](#software-usage)
- [Configuration via WiFi](#configuration-via-wifi)
- [Data Output](#data-output)
- [License](#license)

---

## Overview

This project enables you to measure wave heights, frequencies, and other characteristics on water surfaces. The system uses an Arduino board (e.g., Arduino Nano) with a WiFi module and an accelerometer to track the movement of a floating arm.

---

## How it works

1. A wooden pole (stake) is fixed into the ground at the edge of the water body.
2. At the top of the pole, a hinge (pivot) is mounted.
3. A freely swinging arm is attached to the hinge.
4. A floating body (e.g., a plastic bottle) is fixed to the distal end of the arm, so it rests on the water surface.
5. An Arduino (with an accelerometer) is attached to the arm to measure its tilt angle with respect to the gravitational vector.
6. Using trigonometry, the vertical position of the arm's floating end with respect to the pivot can be calculated, giving a live measurement of the water surface.

---

## Hardware Setup

**Needed:**
- 1x Wooden pole/stake (length depending on setup)
- 1x Hinge (robust enough for movement)
- 1x Wooden or metal arm (of known length, e.g., 82 cm; see `ARM_LENGTH`)
- 1x Plastic bottle or other floating body
- 1x Arduino (Nano or similar, WiFi-capable, e.g., using WiFiNINA)
- 1x Accelerometer (compatible with Arduino)
- Required wires, screws, mounting parts

**Mechanical Assembly:**
1. Drive the pole firmly into the ground at the water's edge.
2. Mount the hinge at the desired pivot height above the water.
3. Attach the arm securely to the hinge.
4. Affix the float to the opposite end of the arm. Ensure it will always float but not be submerged.
5. Mount the Arduino and accelerometer securely to the arm, paying attention to orientation.

---

## Software Usage

The Arduino collects acceleration data to infer the tilt (theta) of the arm. By knowing:
- *Pivot height above water* (`PIVOT_HEIGHT`)
- *Arm length* (`ARM_LENGTH`)
- Current arm angle (`theta`, from accelerometer)

…the vertical position (height) of the floating end above the floor (or in relation to the water surface) is determined.

### Calculating Height

The height is computed using: Vertical Height = PIVOT_HEIGHT - ARM_LENGTH * cos(theta)

- `theta` is given in radians.

The software collects samples, calculates averages, amplitudes, and frequencies of wave motion automatically.

---

## Configuration via WiFi

1. Power on the Arduino setup. It creates its own WiFi access point:

    - **SSID:** HOEHENMESSER
    - **Password:** 12345678

You can change this stuff if you need to in Webserver.cpp
2. Connect your PC or phone to this WiFi.
3. Open a web browser and go to the device IP: **http://192.168.2.1/**

4. Enter (or update) the following parameters:
    - **Pivot height:** Distance from ground to hinge (in centimeters), i.e., `PIVOT_HEIGHT`
    - **Arm length:** Distance from hinge to float (in centimeters), i.e., `ARM_LENGTH`
    - Any other required calibration parameters.

5. The device will save your settings and start/continue measurement.

---

## Data Output

The collected measurements include:
- Maximum, minimum, and average height
- Amplitudes (average, min, max)
- Wave count and frequency
- Timestamps

The data can be viewed live over WiFi or saved for further analysis. See code implementation for details.

---

## License

You can do whatever you want with this. But I don't take any responsibility for the outcome if u know what I mean.

---

### Notes

- Make sure the mechanical assembly is robust, and the arm swings freely.
- The float should always rest on the water surface without submerging the arm itself.
- For best results, recalibrate parameters whenever the device is moved to a new location.