#pragma once
#include <string>
#include <map>
#include <vector>

enum class VitalType {
    Temperature,
    PulseRate,
    Spo2
};

struct VitalRange {
    float minValue;
    float maxValue;
};

struct VitalViolation {
    VitalType vital;
    float actualValue;
    VitalRange range;
    std::string message;
};

std::vector<VitalViolation> evaluateVitals(
    const std::map<VitalType, float>& patientReadings,
    const std::map<VitalType, VitalRange>& allowedLimits
);

class VitalRegistry {
public:
    void addVitalLimit(VitalType vital, float minValue, float maxValue);
    void adjustMin(VitalType vital, float newMin);
    void adjustMax(VitalType vital, float newMax);
    const std::map<VitalType, VitalRange>& getLimits() const;

private:
    std::map<VitalType, VitalRange> vitalLimits;
};

class PatientProfile {
public:
    PatientProfile(int ageYears);
    void personalizeLimits(VitalRegistry& registry) const;

private:
    int ageYears;
};

// Aspect-Oriented: Alerts / Logging
namespace aspect {
    void notify(const std::vector<VitalViolation>& violations);
}

