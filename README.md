# Real-Time Asset Tracking & Geofencing System
A concurrent, RTOS-managed embedded system that ingests real-time aircraft telemetry from a host simulation environment over UART, performs fixed-point kinematic calculations, interfaces with an external FPGA hardware accelerator over SPI, and streams outbound telemetry.

## System Architecture Overview

The project is organized as a monorepo splitting responsibilities between the high-level data simulator (**Host**) and the real-time bare-metal microcontroller (**Target**).
