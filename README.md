# ESP32 UDP Broadcast Example

This project demonstrates how to use an ESP32 to broadcast data over UDP. It also includes a simple PowerShell script to listen to the UDP messages on a PC.

---

## Features

- Connects ESP32 to a Wi-Fi network.
- Broadcasts messages over UDP.
- Reads data from a secondary serial interface (`Serial2`) and sends it via UDP.
- PowerShell script can receive and display UDP broadcast messages.

---

## Hardware Required

- ESP32 development board
- USB cable to program the ESP32
- Optional: device connected to ESP32 `Serial2` pins (16 = RX, 17 = TX)

---

## Software Required

- Arduino IDE with ESP32 board support
- PowerShell (for receiving UDP messages)

---

## Wiring

