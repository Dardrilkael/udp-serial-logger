# ESP32 Serial-to-UDP Bridge

This project allows an ESP32 to act as a **wireless Serial bridge**, forwarding serial data over UDP.  
It enables remote monitoring and communication with devices (e.g., another ESP32 or microcontroller) without needing a direct USB connection.

---

## Features

- Reads data from a device connected to `Serial2` on the ESP32.
- Forwards received serial data over UDP to a remote listener.
- Allows remote monitoring of devices without physical connection.
- Supports both text and raw binary data.

---

## Hardware Required

- ESP32 development board  
- USB cable to program the ESP32  
- Device connected to ESP32 `Serial2` pins (another ESP32, sensor, or microcontroller)  

---

## Wiring

TX (ESP32 pin 17) → RX of external device
RX (ESP32 pin 16) ← TX of external device

Ensure the ESP32 is connected to Wi-Fi; no extra wiring is needed for UDP.

---

## Arduino Code Overview

- Initializes Wi-Fi and UDP broadcasting.  
- Reads data from `Serial2` and sends it via UDP.  
- Supports sending messages like `Serial.print()`:



