#pragma once
#include <Arduino.h>

enum class SensorType {
    Temperature,
    Humidity,
    Pressure,
    Count
};

class Sensor {
public:
    void begin();
    int sensorCount() const { return static_cast<int>(SensorType::Count); }
    const char* sensorName(int index) const;
    float readValue(int index) const;

private:
    // Add actual sensor hardware interface here later
    // This is just a mock implementation for now
    float mockValues[static_cast<int>(SensorType::Count)] = {0};
};
