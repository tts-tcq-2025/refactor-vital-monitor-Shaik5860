#pragma once
#include <string>
#include <map>
#include <vector>

struct VitalRange {
    float minValue;
    float maxValue;
};

struct VitalViolation {
    std::string vitalName;
    float actualValue;
    VitalRange range;
    std::string message;
};

// Functional Core
std::vector<VitalViolation> evaluateVitals(
    const std::map<std::string, float>& patientReadings,
    const std::map<std::string, VitalRange>& allowedLimits
);

// Object-Oriented Classes
class VitalRegistry {
public:
    void addVitalLimit(const std::string& vitalName, float minValue, float maxValue);
    void adjustMin(const std::string& vitalName, float newMin);
    void adjustMax(const std::string& vitalName, float newMax);
    const std::map<std::string, VitalRange>& getLimits() const;

private:
    std::map<std::string, VitalRange> vitalLimits;
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
