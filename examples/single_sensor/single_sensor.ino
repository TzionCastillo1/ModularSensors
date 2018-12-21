/*****************************************************************************
single_sensor.ino
Written By:  Sara Damiano (sdamiano@stroudcenter.org)
Development Environment: PlatformIO
Hardware Platform: EnviroDIY Mayfly Arduino Datalogger
Software License: BSD-3.
  Copyright (c) 2017, Stroud Water Research Center (SWRC)
  and the EnviroDIY Development Team

This example sketch is written for ModularSensors library version 0.19.2

This sketch is an example of getting data from a single sensor, in this case, a
MaxBotix Ultrasonic Range Finder
DISCLAIMER:
THIS CODE IS PROVIDED "AS IS" - NO WARRANTY IS GIVEN.
*****************************************************************************/

// ==========================================================================
// Include the base required libraries
// ==========================================================================
#include <Arduino.h>  // The base Arduino library
#include <EnableInterrupt.h>  // for external and pin change interrupts
#include <SensorBase.h>
#include <VariableBase.h>

// The library version this example was written for
const char *libraryVersion = "0.19.2";
// The name of this file
const char *sketchName = "single_sensor.ino";

// ==========================================================================
// Set up the sensor object
// ==========================================================================

// ==========================================================================
//    Maxbotix HRXL
// ==========================================================================

// Define a serial port for receiving data - in this case, using software serial
// Because the standard software serial library uses interrupts that conflict
// with several other libraries used within this program, we must use a
// version of software serial that has been stripped of interrupts and define
// the interrrupts for it using the enableInterrup library.

// If enough hardware serial ports are available on your processor, you should
// use one of those instead.  If the proper pins are avaialbe, AltSoftSerial
// by Paul Stoffregen is also superior to SoftwareSerial for this sensor.
// Neither hardware serial nor AltSoftSerial require any modifications to
// deal with interrupt conflicts.

#if defined ARDUINO_SAMD_ZERO
// On an Arduino Zero or Feather M0, we'll create serial 3 on SERCOM2
#include "wiring_private.h" // pinPeripheral() function
Uart Serial3(&sercom2, 5, 2, SERCOM_RX_PAD_3, UART_TX_PAD_2);
void SERCOM2_Handler()
{
    Serial3.IrqHandler();
}
HardwareSerial &sonarSerial = Serial3;

#elif defined ARDUINO_SODAQ_AUTONOMO
// Serial3 is already defined on the Autonomo
HardwareSerial &sonarSerial = Serial3;

#else
// Set up a serial port for serial communication - in this case, using SoftwareSerial_ExtInts
#include <SoftwareSerial_ExtInts.h>  // for the stream communication
const int SonarData = 11;     // data  pin
SoftwareSerial_ExtInts sonarSerial(SonarData, -1);  // No Tx pin is required, only Rx
#endif


#include <sensors/MaxBotixSonar.h>
const int SonarTrigger = -1;   // Trigger pin
const int8_t SonarPower = 22;   // excite (power) pin

// Create a new instance of the sonar sensor;
MaxBotixSonar sonar(sonarSerial, SonarPower, SonarTrigger) ;

// Create a new instance of the range variable;
MaxBotixSonar_Range sonar_range(&sonar);

// Create a function to calculate the water depth from the sonar range
// For this example, we'll assume that the sonar is mounted 5m above the stream bottom
float calcDepth(void)
{
    float mountHeight = 5000;
    float sonarRange = sonar_range.getValue();
    return mountHeight - sonarRange;
}
// Create a calculated variable for the water depth
// Variable calcVar(functionName, VariableName, VariableUnit, Resolution, UUID, Code);
// VariableName must be a value from http://vocabulary.odm2.org/variablename/
// VariableUnit must be a value from http://vocabulary.odm2.org/units/
Variable waterDepth(calcDepth, "waterDepth", "millimeter", 0, "", "sonarDepth");

// ==========================================================================
// Board setup info
// ==========================================================================
const long serialBaud = 57600;  // Baud rate for the primary serial port for debugging
const int8_t greenLED = 8;  // Pin for the green LED
const int8_t redLED = 9;  // Pin for the red LED

// Flashes to Mayfly's LED's
void greenredflash(int numFlash = 4)
{
  for (int i = 0; i < numFlash; i++) {
    digitalWrite(greenLED, HIGH);
    digitalWrite(redLED, LOW);
    delay(75);
    digitalWrite(greenLED, LOW);
    digitalWrite(redLED, HIGH);
    delay(75);
  }
  digitalWrite(redLED, LOW);
}

// ==========================================================================
// Main setup function
// ==========================================================================
void setup()
{
    // Start the primary serial connection
    Serial.begin(serialBaud);

    // Print a start-up note to the first serial port
    Serial.print(F("Now running "));
    Serial.println(sketchName);

    Serial.print(F("Using ModularSensors Library version "));
    Serial.println(MODULAR_SENSORS_VERSION);

    if (String(MODULAR_SENSORS_VERSION) !=  String(libraryVersion))
        Serial.println(F(
            "WARNING: THIS EXAMPLE WAS WRITTEN FOR A DIFFERENT VERSION OF MODULAR SENSORS!!"));

    // Start the stream for the sonar
    sonarSerial.begin(9600);

    // Allow interrupts for software serial
    #if defined SoftwareSerial_ExtInts_h
    enableInterrupt(SonarData, SoftwareSerial_ExtInts::handle_interrupt, CHANGE);
    #endif
    #if defined NeoSWSerial_h
    enableInterrupt(SonarData, NeoSWSISR, CHANGE);
    #endif

    #if defined ARDUINO_SAMD_ZERO
    // Assign pins to SERCOM functionality
    pinPeripheral(2, PIO_SERCOM);
    pinPeripheral(5, PIO_SERCOM);
    #endif

    // Set up pins for the LED's
    pinMode(greenLED, OUTPUT);
    pinMode(redLED, OUTPUT);
    // Blink the LEDs to show the board is on and starting up
    greenredflash();

    // Print a start-up note to the first serial port
    Serial.println(F("Single Sensor Example - Sonar Ranging"));

    // Set up the sensor
    sonar.setup();
}


// ==========================================================================
// Main loop function
// ==========================================================================
void loop()
{
    // Turn on the LED to show we're taking a reading
    digitalWrite(greenLED, HIGH);

    // Send power to the sensor
    sonar.powerUp();

    // Wake up the sensor
    sonar.wake();

    // Update the sensor value
    sonar.update();

    // Print the sonar result
    Serial.print("Current sonar range: ");
    Serial.println(sonar_range.getValueString());
    Serial.print("Calculated water depth: ");
    Serial.println(waterDepth.getValueString());

    // Put the sensor back to sleep
    sonar.sleep();

    // Cut the sensor power
    sonar.powerDown();

    // Turn off the LED to show we're done with the reading
    digitalWrite(greenLED, LOW);

    // Wait for the next reading
    delay(5000);
}
