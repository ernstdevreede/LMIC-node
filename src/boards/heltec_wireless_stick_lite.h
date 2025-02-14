/*******************************************************************************
 * 
 *  File:         heltec_wireless_stick_lite.h
 * 
 *  Function:     Board Support File for Heltec Wireless Stick Lite.
 * 
 *  Copyright:    Copyright (c) 2021 Leonel Lopes Parente
 * 
 *  License:      MIT License. See accompanying LICENSE file.
 * 
 *  Author:       Leonel Lopes Parente
 * 
 *  Description:  This board has onboard USB (provided by onboard USB to serial).
 *                It supports automatic firmware upload and serial over USB. 
 *                No onboard display. Optionally an external display con be connected.
 * 
 *                The standard I2C pins defined in the BSP do not match the 
 *                GPIO pins that the display is connected to. Therefore the
 *                the I2C Wire object is explicitly initialized with the
 *                correct pins (see boardInit() below).
 * 
 *                WARNING: Vext and the standard I2C SDA pin are both defined as GPIO21.
 * 
 *                Connect the LoRa module and optional display
 *                according to below connection details.
 * 
 *                CONNECTIONS AND PIN DEFINITIONS:
 *                
 *                Indentifiers between parentheses are defined in the board's 
 *                Board Support Package (BSP) which is part of the Arduino core. 
 * 
 *                Leds                GPIO 
 *                ----                ----
 *                LED   <――――――――――>  25  (LED_BUILTIN)  Active-high
 *   
 *                I2C [display]       GPIO
 *                ----                ----
 *                SDA   <――――――――――>   4  NOT SDA!
 *                SCL   <――――――――――>  15  NOT SCL!
 *                -                   21  (SDA, Vext) used for VExt!!!
 *                -                   22  (SCL)
 *
 *                SPI/LoRa            GPIO
 *                ---                 ----
 *                MOSI  <――――――――――>  27  (MOSI)
 *                MISO  <――――――――――>  19  (MISO)
 *                SCK   <――――――――――>   5  (SCK)
 *                NSS   <――――――――――>  18  (SS)
 *                RST   <――――――――――>  14  (RST_LoRa)
 *                DIO0  <――――――――――>  26  (DIO0)
 *                DIO1  <――――――――――>  35  (DIO1)
 *                DIO2  <――――――――――>  34  (DIO2)
 * 
 *                Other               GPIO
 *                -----               ----
 *                VExt  <――――――――――>  21 (Vext, SDA) Active-low
 * 
 *  Docs:         https://docs.platformio.org/en/latest/boards/espressif32/heltec_wireless_stick_lite.html
 *
 *  Identifiers:  LMIC-node:
 *                    board-id:      heltec_wireless_stick_lite
 *                PlatformIO
 *                    board:         heltec_wireless_stick_lite
 *                    platform:      espressif32
 *                Arduino
 *                    board:         ARDUINO_HELTEC_WIRELESS_STICK_LITE
 *                    architecture:  ARDUINO_ARCH_ESP32 
 * 
 ******************************************************************************/

#pragma once

#ifndef HELTEC_WIRELESS_STICK_LITE_H_
#define HELTEC_WIRELESS_STICK_LITE_H_

#include "lmic-node.h"

#define DEVICEID_DEFAULT "wireless-sticklt"   // Default deviceid value

// Wait for Serial
// Can be useful for boards with MCU with integrated USB support.
// #define WAITFOR_SERIAL_SECONDS_DEFAULT 10   // -1 waits indefinitely  

// LMIC Clock Error
// This is only needed for slower 8-bit MCUs (e.g. 8MHz ATmega328 and ATmega32u4).
// Value is defined in parts per million (of MAX_CLOCK_ERROR).
// #define LMIC_CLOCK_ERROR_PPM 0

// Pin mappings for LoRa tranceiver
const lmic_pinmap lmic_pins = {
    .nss = 18,
    .rxtx = LMIC_UNUSED_PIN,
    .rst =14,
    .dio = { /*dio0*/ 26, /*dio1*/ 35, /*dio2*/ 34 }
#ifdef MCCI_LMIC
    ,
    .rxtx_rx_active = 0,
    .rssi_cal = 10,
    .spi_freq = 8000000     /* 8 MHz */
#endif    
};

#ifdef USE_SERIAL
    HardwareSerial& serial = Serial;
#endif    

#ifdef USE_LED
    EasyLed led(25, EasyLed::ActiveLevel::High);
#endif

#ifdef USE_DISPLAY
    // Create U8x8 instance for SSD1306 OLED display (no reset) using hardware I2C.
    U8X8_SSD1306_128X64_NONAME_HW_I2C display(/*rst*/ U8X8_PIN_NONE, /*scl*/ 15, /*sda*/ 4);
#endif


bool boardInit(InitType initType)
{
    // This function is used to perform board specific initializations.
    // Required as part of standard template.

    // InitType::Hardware        Must be called at start of setup() before anything else.
    // InitType::PostInitSerial  Must be called after initSerial() before other initializations.    

    bool success = true;
    switch (initType)
    {
        case InitType::Hardware:
            // Note: Serial port and display are not yet initialized and cannot be used use here.

            #ifdef USE_DISPLAY
                // Initialize I2C Wire object with GPIO pins the display is connected to.
                // These pins will be remembered and will not change if any library
                // later calls Wire.begin() without parameters.
                Wire.begin(/*sda*/ 4, /*scl*/ 15);
            #endif
            break;

        case InitType::PostInitSerial:
            // Note: If enabled Serial port and display are already initialized here.
            // No actions required for this board.
            break;       
    }
    return success;
}


#endif  // HELTEC_WIRELESS_STICK_LITE_H_