/**
 * @file MeterTeros11.h
 * @copyright 2020 Stroud Water Research Center
 * Part of the EnviroDIY ModularSensors library for Arduino
 * @author Written By: Anthony Aufdenkampe <aaufdenkampe@limno.com>
 * Edited by Sara Geleskie Damiano <sdamiano@stroudcenter.org>
 *
 * @brief Contains the MeterTeros11 sensor subclass and the variable subclasses
 * MeterTeros11_Ea, MeterTeros11_Temp, and MeterTeros11_VWC.
 *
 * These are for the Meter Teros 11 Advanced Soil Moisture probe.
 *
 * This depends on the EnviroDIY SDI-12 library and the SDI12Sensors super
 * class.
 *
 * Documentation for the SDI-12 Protocol commands and responses for the Meter
 * Teros 11 can be found at:
 * http://publications.metergroup.com/Integrator%20Guide/18224%20TEROS%2011-12%20Integrator%20Guide.pdf
 *
 * For Ea and VWC:
 *     Resolution is 0.001 m3/m3 (0.1% VWC) from 0 – 70% VWC
 *     Accuracy for Generic calibration equation: ± 0.03 m3/m3
 *                                               (± 3% VWC) typical
 *     Accuracy for Medium Specific Calibration: ± 0.01 to 0.02 m3/m3
 *                                              (± 1-2% VWC)
 *     Range is 0 – 1 m3/m3 (0 – 100% VWC)
 *
 * For Temperature:
 *     Resolution is 0.1°C
 *     Accuracy is ± 0.5°C, from - 40°C to 0°C
 *     Accuracy is ± 0.3°C, from 0°C to + 60°C
 *
 * Warm-up time in SDI-12 mode: 245ms typical, assume stability at warm-up
 * Measurement duration: 25 ms to 50 ms
 *
 * Supply Voltage (VCC to GND), 4.0 to 15.0 VDC
 * Digital Input Voltage (logic high), 2.8 to 3.9 V (3.6 typical)
 * Digital Output Voltage (logic high), 3.6 typical
 */

// Header Guards
#ifndef SRC_SENSORS_METERTEROS11_H_
#define SRC_SENSORS_METERTEROS11_H_

// Debugging Statement
// #define MS_MeterTeros11_DEBUG

#ifdef MS_METERTEROS11_DEBUG
#define MS_DEBUGGING_STD "MeterTeros11"
#endif

// Included Dependencies
#include "ModSensorDebugger.h"
#undef MS_DEBUGGING_STD
#include "VariableBase.h"
#include "sensors/SDI12Sensors.h"

// Sensor Specific Defines
/// Sensor::_numReturnedValues; the Teros 11 can report 3 values.
#define TEROS11_NUM_VARIABLES 3
/// Sensor::_warmUpTime_ms; the Teros 11 warms up in 250ms.
#define TEROS11_WARM_UP_TIME_MS 250
/// Sensor::_stabilizationTime_ms; the Teros 11 is stable after 50ms.
#define TEROS11_STABILIZATION_TIME_MS 50
/// Sensor::_measurementTime_ms; the Teros 11 takes 50ms to complete a
/// measurement.
#define TEROS11_MEASUREMENT_TIME_MS 50

/// Decimals places in string representation; EA should have 5.
#define TEROS11_EA_RESOLUTION 5
// adding extra digit to resolution for averaging
/// Variable number; EA is stored in sensorValues[0].
#define TEROS11_EA_VAR_NUM 0

/// Decimals places in string representation; temperature should have 2.
#define TEROS11_TEMP_RESOLUTION 2
// adding extra digit to resolution for averaging
/// Variable number; temperature is stored in sensorValues[1].
#define TEROS11_TEMP_VAR_NUM 1

/// Decimals places in string representation; VWC should have 3.
#define TEROS11_VWC_RESOLUTION 3
// adding extra digit to resolution for averaging
/// Variable number; VWC is stored in sensorValues[2].
#define TEROS11_VWC_VAR_NUM 2

// The main class for the Decagon 5TM
class MeterTeros11 : public SDI12Sensors {
 public:
    // Constructors with overloads
    MeterTeros11(char SDI12address, int8_t powerPin, int8_t dataPin,
                 uint8_t measurementsToAverage = 1)
        : SDI12Sensors(SDI12address, powerPin, dataPin, measurementsToAverage,
                       "MeterTeros11", TEROS11_NUM_VARIABLES,
                       TEROS11_WARM_UP_TIME_MS, TEROS11_STABILIZATION_TIME_MS,
                       TEROS11_MEASUREMENT_TIME_MS) {}
    MeterTeros11(char* SDI12address, int8_t powerPin, int8_t dataPin,
                 uint8_t measurementsToAverage = 1)
        : SDI12Sensors(SDI12address, powerPin, dataPin, measurementsToAverage,
                       "MeterTeros11", TEROS11_NUM_VARIABLES,
                       TEROS11_WARM_UP_TIME_MS, TEROS11_STABILIZATION_TIME_MS,
                       TEROS11_MEASUREMENT_TIME_MS) {}
    MeterTeros11(int SDI12address, int8_t powerPin, int8_t dataPin,
                 uint8_t measurementsToAverage = 1)
        : SDI12Sensors(SDI12address, powerPin, dataPin, measurementsToAverage,
                       "MeterTeros11", TEROS11_NUM_VARIABLES,
                       TEROS11_WARM_UP_TIME_MS, TEROS11_STABILIZATION_TIME_MS,
                       TEROS11_MEASUREMENT_TIME_MS) {}
    // Destructor
    ~MeterTeros11() {}

    bool addSingleMeasurementResult(void) override;
};


// Defines the Ea/Matric Potential Variable
class MeterTeros11_Ea : public Variable {
 public:
    /**
     * @brief Construct a new MeterTeros11_Ea object.
     *
     * @param parentSense The parent MeterTeros11 providing the result values.
     * @param uuid A universally unique identifier (UUID or GUID) for the
     * variable.  Default is an empty string.
     * @param varCode A short code to help identify the variable in files.
     * Default is SoilEa.
     */
    explicit MeterTeros11_Ea(MeterTeros11* parentSense, const char* uuid = "",
                             const char* varCode = "SoilEa")
        : Variable(parentSense, (const uint8_t)TEROS11_EA_VAR_NUM,
                   (uint8_t)TEROS11_EA_RESOLUTION, "permittivity",
                   "faradPerMeter", varCode, uuid) {}
    /**
     * @brief Construct a new MeterTeros11_Ea object.
     *
     * @note This must be tied with a parent MeterTeros11 before it can be used.
     */
    MeterTeros11_Ea()
        : Variable((const uint8_t)TEROS11_EA_VAR_NUM,
                   (uint8_t)TEROS11_EA_RESOLUTION, "permittivity",
                   "faradPerMeter", "SoilEa") {}
    /**
     * @brief Destroy the MeterTeros11_Ea object - no action needed.
     */
    ~MeterTeros11_Ea() {}
};


// Defines the Temperature Variable
class MeterTeros11_Temp : public Variable {
 public:
    /**
     * @brief Construct a new MeterTeros11_Temp object.
     *
     * @param parentSense The parent MeterTeros11 providing the result values.
     * @param uuid A universally unique identifier (UUID or GUID) for the
     * variable.  Default is an empty string.
     * @param varCode A short code to help identify the variable in files.
     * Default is SoilTemp.
     */
    explicit MeterTeros11_Temp(MeterTeros11* parentSense, const char* uuid = "",
                               const char* varCode = "SoilTemp")
        : Variable(parentSense, (const uint8_t)TEROS11_TEMP_VAR_NUM,
                   (uint8_t)TEROS11_TEMP_RESOLUTION, "temperature",
                   "degreeCelsius", varCode, uuid) {}
    /**
     * @brief Construct a new MeterTeros11_Temp object.
     *
     * @note This must be tied with a parent MeterTeros11 before it can be used.
     */
    MeterTeros11_Temp()
        : Variable((const uint8_t)TEROS11_TEMP_VAR_NUM,
                   (uint8_t)TEROS11_TEMP_RESOLUTION, "temperature",
                   "degreeCelsius", "SoilTemp") {}
    /**
     * @brief Destroy the MeterTeros11_Temp object - no action needed.
     */
    ~MeterTeros11_Temp() {}
};


// Defines the Volumetric Water Content Variable
class MeterTeros11_VWC : public Variable {
 public:
    /**
     * @brief Construct a new MeterTeros11_VWC object.
     *
     * @param parentSense The parent MeterTeros11 providing the result values.
     * @param uuid A universally unique identifier (UUID or GUID) for the
     * variable.  Default is an empty string.
     * @param varCode A short code to help identify the variable in files.
     * Default is SoilVWC.
     */
    explicit MeterTeros11_VWC(MeterTeros11* parentSense, const char* uuid = "",
                              const char* varCode = "SoilVWC")
        : Variable(parentSense, (const uint8_t)TEROS11_VWC_VAR_NUM,
                   (uint8_t)TEROS11_VWC_RESOLUTION, "volumetricWaterContent",
                   "percent", varCode, uuid) {}
    /**
     * @brief Construct a new MeterTeros11_VWC object.
     *
     * @note This must be tied with a parent MeterTeros11 before it can be used.
     */
    MeterTeros11_VWC()
        : Variable((const uint8_t)TEROS11_VWC_VAR_NUM,
                   (uint8_t)TEROS11_VWC_RESOLUTION, "volumetricWaterContent",
                   "percent", "SoilVWC") {}
    /**
     * @brief Destroy the MeterTeros11_VWC object - no action needed.
     */
    ~MeterTeros11_VWC() {}
};

#endif  // SRC_SENSORS_METERTEROS11_H_
