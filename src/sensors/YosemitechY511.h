/**
 * @file YosemitechY511.h
 * @copyright 2020 Stroud Water Research Center
 * Part of the EnviroDIY ModularSensors library for Arduino
 * @author Sara Geleskie Damiano <sdamiano@stroudcenter.org>
 *
 * @brief Contains the YosemitechY511 sensor subclass and the variable
 * subclasses YosemitechY511_Turbidity and YosemitechY511_Temp.
 *
 * These are for the Yosemitech Y511 Turbidity Sensor with Wiper.
 *
 * This depends on the YosemitechParent super class.
 *
 * Documentation for the Modbus Protocol commands and responses can be found
 * within the documentation in the YosemitechModbus library at:
 * https://github.com/EnviroDIY/YosemitechModbus
 */
/* clang-format off */
/**
 * @defgroup y511_group Yosemitech Y511 Wipered Turbidity
 * Classes for the @ref y511_page
 *
 * @copydoc y511_page
 *
 * @ingroup yosemitech_group
 */
/* clang-format on */
/* clang-format off */
/**
 * @page y511_page Yosemitech Y511 Wipered Turbidity
 *
 * @tableofcontents
 *
 * @section y511_datasheet Sensor Datasheet
 * - [Manual](https://github.com/EnviroDIY/YosemitechModbus/tree/master/doc/Y511-Turbidity+Wiper_UserManual-v1.1.pdf)
 * - [Modbus Instructions](https://github.com/EnviroDIY/YosemitechModbus/tree/master/doc/Y511-Turbidity+Wiper-v1.7_ModbusInstructions.pdf)
 *
 * @section y511_sensor The Yosemitech Y511 Wipered Turbidity Sensor
 * @ctor_doc{YosemitechY511, byte modbusAddress, Stream* stream, int8_t powerPin, int8_t powerPin2, int8_t enablePin, uint8_t measurementsToAverage}
 * @subsection y511_timing Sensor Timing
 * - Time before sensor responds after power - 500ms
 * - Time between "StartMeasurement" command and stable reading - 22sec
 *
 * @section y511_turb Turbidity Output
 * @variabledoc{YosemitechY511,Turbidity}
 *   - Range is 0.1 to 1000 NTU
 *   - Accuracy is ± 5 % or 0.3 NTU
 *   - Result stored in sensorValues[0]
 *   - Resolution is 0.01 NTU
 *   - Reported as Nephelometric Turbidity Units (NTU)
 *   - Default variable code is Y511Turbidity
 *
 * @section y511_temp Temperature Output
 * @variabledoc{YosemitechY511,Temp}
 *   - Range is 0°C to + 50°C
 *   - Accuracy is ± 0.2°C
 *   - Result stored in sensorValues[1]
 *   - Resolution is 0.1 °C
 *   - Reported as degrees Celsius (°C)
 *   - Default variable code is Y511Temp
 *
 * The reported resolution (32 bit) gives far more precision than is significant
 * based on the specified accuracy of the sensor, so the resolutions kept in the
 * string representation of the variable values is based on the accuracy not the
 * maximum reported resolution of the sensor.
 *
 *
 * ___
 * @section y511_examples Example Code
 * The Yosemitech Y511 wipered turbidity sensor is used in the @menulink{y511}
 * example.
 *
 * @menusnip{y511}
 */
/* clang-format on */

// Header Guards
#ifndef SRC_SENSORS_YOSEMITECHY511_H_
#define SRC_SENSORS_YOSEMITECHY511_H_

// Included Dependencies
#include "sensors/YosemitechParent.h"

// Sensor Specific Defines

/// Sensor::_numReturnedValues; the Y511 can report 2 values.
#define Y511_NUM_VARIABLES 2
/**
 * @brief Sensor::_warmUpTime_ms; Y511 warms up in 8000ms.
 *
 * 500 ms to receive commands, but if activating brush warmup+stabilization must
 * >20s
 */
#define Y511_WARM_UP_TIME_MS 8000
/**
 * @brief Sensor::_stabilizationTime_ms; Y511 is stable after 40s.
 *
 * warmup+stabilization > 48 s for consecutive readings to give different
 * results
 */
#define Y511_STABILIZATION_TIME_MS 40000
/**
 * @brief Sensor::_measurementTime_ms; Y511 take 4s to complete a measurement.
 *
 * Could potentially be lower with a longer stabilization time; more testing
 * needed.
 */
#define Y511_MEASUREMENT_TIME_MS 4000

/// Decimals places in string representation; turbidity should have 2.
#define Y511_TURB_RESOLUTION 2
/// Variable number; turbidity is stored in sensorValues[0].
#define Y511_TURB_VAR_NUM 0

/// Decimals places in string representation; temperature should have 1.
#define Y511_TEMP_RESOLUTION 1
/// Variable number; temperature is stored in sensorValues[1].
#define Y511_TEMP_VAR_NUM 1

/* clang-format off */
/**
 * @brief The Sensor sub-class for the
 * [Yosemitech Y511-A optical turbidity sensor with wiper](@ref y511_page).
 *
 * @ingroup y511_group
 */
/* clang-format on */
class YosemitechY511 : public YosemitechParent {
 public:
    // Constructors with overloads
    /**
     * @brief Construct a new Yosemitech Y511 object.
     *
     * @param modbusAddress The modbus address of the sensor.
     * @param stream An Arduino data stream for modbus communication.  See
     * [notes](https://github.com/EnviroDIY/ModularSensors/wiki/Arduino-Streams)
     * for more information on what streams can be used.
     * @param powerPin The pin on the mcu controlling power to the Y511.  Use -1
     * if it is continuously powered.
     * @param powerPin2 The pin on the mcu controlling power to the RS485
     * adapter, if it is different from that used to power the sensor.  Use -1
     * or omit if not applicable.
     * @param enablePin The pin on the mcu controlling the direction enable on
     * the RS485 adapter, if necessary; use -1 or omit if not applicable.  An
     * RS485 adapter with integrated flow control is strongly recommended.
     * @param measurementsToAverage The number of measurements to take and
     * average before giving a "final" result from the sensor; optional with a
     * default value of 1.
     */
    YosemitechY511(byte modbusAddress, Stream* stream, int8_t powerPin,
                   int8_t powerPin2 = -1, int8_t enablePin = -1,
                   uint8_t measurementsToAverage = 1)
        : YosemitechParent(modbusAddress, stream, powerPin, powerPin2,
                           enablePin, measurementsToAverage, Y511,
                           "YosemitechY511", Y511_NUM_VARIABLES,
                           Y511_WARM_UP_TIME_MS, Y511_STABILIZATION_TIME_MS,
                           Y511_MEASUREMENT_TIME_MS) {}
    /**
     * @copydoc YosemitechY511::YosemitechY511
     */
    YosemitechY511(byte modbusAddress, Stream& stream, int8_t powerPin,
                   int8_t powerPin2 = -1, int8_t enablePin = -1,
                   uint8_t measurementsToAverage = 1)
        : YosemitechParent(modbusAddress, stream, powerPin, powerPin2,
                           enablePin, measurementsToAverage, Y511,
                           "YosemitechY511", Y511_NUM_VARIABLES,
                           Y511_WARM_UP_TIME_MS, Y511_STABILIZATION_TIME_MS,
                           Y511_MEASUREMENT_TIME_MS) {}
    /**
     * @brief Destroy the Yosemitech Y511 object
     */
    ~YosemitechY511() {}
};


/* clang-format off */
/**
 * @brief The Variable sub-class used for the
 * [turbidity output](@ref y511_turb) from a
 * [Yosemitech Y511-A optical turbidity sensor with wiper](@ref y511_page).
 *
 * @ingroup y511_group
 */
/* clang-format on */
class YosemitechY511_Turbidity : public Variable {
 public:
    /**
     * @brief Construct a new YosemitechY511_Turbidity object.
     *
     * @param parentSense The parent YosemitechY511 providing the result
     * values.
     * @param uuid A universally unique identifier (UUID or GUID) for the
     * variable; optional with the default value of an empty string.
     * @param varCode A short code to help identify the variable in files;
     * optional with a default value of "Y511Turbidity".
     */
    explicit YosemitechY511_Turbidity(YosemitechY511* parentSense,
                                      const char*     uuid    = "",
                                      const char*     varCode = "Y511Turbidity")
        : Variable(parentSense, (const uint8_t)Y511_TURB_VAR_NUM,
                   (uint8_t)Y511_TURB_RESOLUTION, "turbidity",
                   "nephelometricTurbidityUnit", varCode, uuid) {}
    /**
     * @brief Construct a new YosemitechY511_Turbidity object.
     *
     * @note This must be tied with a parent YosemitechY511 before it can be
     * used.
     */
    YosemitechY511_Turbidity()
        : Variable((const uint8_t)Y511_TURB_VAR_NUM,
                   (uint8_t)Y511_TURB_RESOLUTION, "turbidity",
                   "nephelometricTurbidityUnit", "Y511Turbidity") {}
    /**
     * @brief Destroy the YosemitechY511_Turbidity object - no action needed.
     */
    ~YosemitechY511_Turbidity() {}
};


/* clang-format off */
/**
 * @brief The Variable sub-class used for the
 * [temperature output](@ref y511_temp) from a
 * [Yosemitech Y511-A optical turbidity sensor with wiper](@ref y511_page).
 *
 * @ingroup y511_group
 */
/* clang-format on */
class YosemitechY511_Temp : public Variable {
 public:
    /**
     * @brief Construct a new YosemitechY511_Temp object.
     *
     * @param parentSense The parent YosemitechY511 providing the result
     * values.
     * @param uuid A universally unique identifier (UUID or GUID) for the
     * variable; optional with the default value of an empty string.
     * @param varCode A short code to help identify the variable in files;
     * optional with a default value of "Y511Temp".
     */
    explicit YosemitechY511_Temp(YosemitechY511* parentSense,
                                 const char*     uuid    = "",
                                 const char*     varCode = "Y511Temp")
        : Variable(parentSense, (const uint8_t)Y511_TEMP_VAR_NUM,
                   (uint8_t)Y511_TEMP_RESOLUTION, "temperature",
                   "degreeCelsius", varCode, uuid) {}
    /**
     * @brief Construct a new YosemitechY511_Temp object.
     *
     * @note This must be tied with a parent YosemitechY511 before it can be
     * used.
     */
    YosemitechY511_Temp()
        : Variable((const uint8_t)Y511_TEMP_VAR_NUM,
                   (uint8_t)Y511_TEMP_RESOLUTION, "temperature",
                   "degreeCelsius", "Y511Temp") {}
    /**
     * @brief Destroy the YosemitechY511_Temp object - no action needed.
     */
    ~YosemitechY511_Temp() {}
};

#endif  // SRC_SENSORS_YOSEMITECHY511_H_
