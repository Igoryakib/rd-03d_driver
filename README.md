# Ai-Thinker RD-03D mmWave Radar Driver for ESP32

[![ESP-IDF](https://img.shields.io/badge/ESP--IDF-v5.0%2B-blue.svg)](https://idf.espressif.com/)
[![FreeRTOS](https://img.shields.io/badge/FreeRTOS-Kernel-orange.svg)](https://www.freertos.org/)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![Language: C99](https://img.shields.io/badge/Language-C99-green.svg)](https://en.wikipedia.org/wiki/C99)

A robust, thread-safe, non-blocking embedded C driver for the **Ai-Thinker RD-03D** 24GHz FMCW mmWave radar sensor on the **ESP32** microcontroller family, built using the **ESP-IDF** framework and **FreeRTOS**.

This driver is designed as a foundational component for distributed multi-target radar nodes in IoT and Wi-Fi Mesh networks.

---

## 🚀 Key Features

- **Layered Architecture:** Strict separation between Low-Level hardware communication (`rd_03d_low`) and High-Level user API (`rd_03d_high`).
- **Encapsulation (Opaque Pointers):** Device state, UART parameters, and queues are encapsulated within the handle structure (`rd_handle_init_s`) to prevent direct memory manipulation.
- **Thread Safety via FreeRTOS:** Native integration with FreeRTOS queues (`QueueHandle_t`) for thread-safe asynchronous data transfer between UART ISR/tasks and application logic.
- **Non-blocking UART I/O:** Configurable timeout handling (`pdMS_TO_TICKS`) prevents system hangs or deadlocks if the sensor is disconnected or damaged.
- **Endianness Management:** Bitwise transformation handling conversion from the sensor's native Big-Endian binary protocol to the ESP32 Little-Endian architecture.
- **Run-time Configuration Pipeline:** Automatic cascading initialization (`CONF_ENABLE` ➔ `MODE_MULTI` ➔ `CONF_END`) with full ACK status verification.
- **Buffer Safety:** Strict validation of frame lengths (12–30 bytes) and 32-bit frame headers/tails to prevent buffer overflows and CPU HardFaults.

---

## 🛠 Hardware Connections

| ESP32 Pin (Default) | RD-03D Sensor Pin | Description |
|:-------------------:|:-----------------:|:------------|
| **GPIO 17**         | **RX**            | UART TX from ESP32 to Sensor RX |
| **GPIO 16**         | **TX**            | UART RX to ESP32 from Sensor TX |
| **5V / VIN**        | **VCC**           | Sensor Power Supply (5V recommended) |
| **GND**             | **GND**           | Common Ground |

> **Note:** The default UART baud rate for the RD-03D module is **256000 bps** (8 data bits, 1 stop bit, no parity).

---

## 📂 Project Structure

```text
├── include/
│   ├── rd_03d_defs.h      # Register definitions, opcodes, frame constants
│   ├── rd_03d_low.h       # Low-level UART & framing API declarations
│   └── rd_03d_high.h      # High-level initialization and user data structures
├── src/
│   ├── rd_03d_low.c       # Binary frame parser, Endianness shift, command logic
│   └── rd_03d_high.c      # Device handle initialization, FreeRTOS queue binding
├── CMakeLists.txt         # ESP-IDF component CMake registration
└── README.md
