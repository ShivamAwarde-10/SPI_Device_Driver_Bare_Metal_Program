# STM32F429ZI SPI Driver Development (Bare-Metal)

# Overview

This project implements a custom SPI (Serial Peripheral Interface) device driver from scratch for the STM32F429ZI microcontroller using Embedded C and direct register-level programming. The driver was developed without using STM32 HAL or any vendor-provided libraries to gain a deeper understanding of peripheral programming and ARM Cortex-M4 architecture.

The driver supports both polling and interrupt-based communication and provides a reusable hardware abstraction layer for SPI peripherals.

# Hardware & Software

# Hardware

* STM32F429ZI Nucleo-144 Development Board
* ARM Cortex-M4 MCU

# Software

* STM32CubeIDE
* Embedded C
* STM32F429ZI Reference Manual & Datasheet

## Features

* SPI Master and Slave Mode Configuration
* Full-Duplex Communication
* Half-Duplex Communication
* Simplex Receive-Only Mode
* Configurable Baud Rate
* Configurable Clock Polarity (CPOL)
* Configurable Clock Phase (CPHA)
* 8-bit and 16-bit Data Frame Support
* Software Slave Management (SSM)
* Polling-Based Data Transmission and Reception
* Interrupt-Based Data Transmission and Reception
* NVIC Interrupt Configuration
* Interrupt Priority Configuration
* Overrun Error (OVR) Handling
* Callback-Based Event Notification
* Modular Driver Architecture

## Driver Architecture

The driver is organized into three layers:

# 1. MCU Header Layer

stm32f429xx.h

Contains:

* Peripheral Base Addresses
* Register Definition Structures
* Bit Position Macros
* NVIC Register Definitions
* Clock Control Macros

# 2. Driver Header Layer

stm32f429xx_spi_driver.h

Contains:

* Configuration Structures
* Handle Structures
* Driver Macros
* API Declarations

# 3. Driver Source Layer

stm32f429xx_spi_driver.c

Contains:

* SPI Initialization
* Polling APIs
* Interrupt APIs
* NVIC Configuration
* Error Handling
* Callback Implementation

## Implemented APIs

# Initialization
SPI_Init()
SPI_DeInit()
SPI_PeriClockControl()

# Polling Communication
SPI_SendData()
SPI_ReceiveData()

# Interrupt Communication
SPI_SendDataIT()
SPI_ReceiveDataIT()
SPI_IRQHandling()

# Peripheral Control
SPI_PeripheralControl()
SPI_SSIConfig()
SPI_SSOEConfig()

# Interrupt Configuration
SPI_IRQInterruptConfig()
SPI_IRQPriorityConfig()

# Error Handling
SPI_ClearOVRFlag()

# Callback
SPI_ApplicationEventCallback()

# SPI Communication Flow

# Polling Mode
Application
    |
SPI_SendData()
    |
Check TXE Flag
    |
Write Data to DR
    |
Transmission Complete

# Interrupt Mode

Application
    |
SPI_SendDataIT()
    |
Enable TXE Interrupt
    |
SPI IRQ Occurs
    |
SPI_IRQHandling()
    |
Callback Notification

# Learning Outcomes

* Bare-Metal Embedded Driver Development
* Register-Level Peripheral Programming
* ARM Cortex-M4 Architecture
* SPI Protocol Internals
* Interrupt Handling using NVIC
* Embedded Software Layering
* Hardware Abstraction Design
* Callback-Based Driver Architecture

# Project Highlights

* Developed without HAL or LL libraries.
* Direct manipulation of SPI and NVIC registers.
* Supports both blocking and non-blocking communication.
* Modular and reusable driver design.
* Built to understand real-world embedded device driver development.

# Author

Shivam Awarde

Embedded Systems Engineer (Fresher)

Skills:
Embedded C | Data Structures | STM32 | ARM Cortex-M4 | SPI | Embedded Linux | Device Driver Development | Interrupt Programming
