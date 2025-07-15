#include "sensor.h"

void Sensor::begin() {
    // Initialize sensor hardware here
    // For now, just set some mock values
    mockValues[static_cast<int>(SensorType::Temperature)] = 25.0;
    mockValues[static_cast<int>(SensorType::Humidity)] = 60.0;
    mockValues[static_cast<int>(SensorType::Pressure)] = 1013.25;
}

const char* Sensor::sensorName(int index) const {
    switch (static_cast<SensorType>(index)) {
        case SensorType::Temperature: return "temperature";
        case SensorType::Humidity: return "humidity";
        case SensorType::Pressure: return "pressure";
        default: return "unknown";
    }
}

float Sensor::readValue(int index) const {
    if (index >= 0 && index < sensorCount()) {
        // Add some random variation to mock values for testing
        return mockValues[index] + (random(-100, 100) / 100.0);
    }
    return 0.0;
}
