#include "vitals.hpp"
#include <iostream>
#include <thread>
#include <chrono>

using std::cout;
using std::flush;
using std::this_thread::sleep_for;
using std::chrono::seconds;

// Functional Core
std::vector<VitalViolation> evaluateVitals(
    const std::map<std::string, float>& patientReadings,
    const std::map<std::string, VitalRange>& allowedLimits
) {
    std::vector<VitalViolation> violations;
    for (const auto& [vitalName, actualValue] : patientReadings) {
        auto it = allowedLimits.find(vitalName);
        if (it == allowedLimits.end()) continue; // skip unknown vitals

        const auto& range = it->second;
        if (actualValue < range.minValue) {
            violations.push_back({vitalName, actualValue, range, "below-min"});
        } else if (actualValue > range.maxValue) {
            violations.push_back({vitalName, actualValue, range, "above-max"});
        }
    }
    return violations;
}

// OO: VitalRegistry
void VitalRegistry::addVitalLimit(const std::string& vitalName, float minValue, float maxValue) {
    vitalLimits[vitalName] = {minValue, maxValue};
}

void VitalRegistry::adjustMin(const std::string& vitalName, float newMin) {
    auto it = vitalLimits.find(vitalName);
    if (it != vitalLimits.end()) it->second.minValue = newMin;
}

void VitalRegistry::adjustMax(const std::string& vitalName, float newMax) {
    auto it = vitalLimits.find(vitalName);
    if (it != vitalLimits.end()) it->second.maxValue = newMax;
}

const std::map<std::string, VitalRange>& VitalRegistry::getLimits() const {
    return vitalLimits;
}

// OO: PatientProfile
PatientProfile::PatientProfile(int age) : ageYears(age) {}

void PatientProfile::personalizeLimits(VitalRegistry& registry) const {
    if (ageYears <= 12) { // pediatric
        registry.adjustMax("temperature", 100);
        registry.adjustMin("pulseRate", 70);
    } else if (ageYears >= 65) { // senior
        registry.adjustMax("pulseRate", 90);
    }
}

// Aspect-Oriented: Alerts
namespace aspect {
    void notify(const std::vector<VitalViolation>& violations) {
        if (violations.empty()) {
            cout << "All vitals are normal.\n";
            return;
        }

        for (const auto& v : violations) {
            cout << v.vitalName << " is " << v.message 
                 << " (" << v.actualValue << ")\n";
            // blinking effect
            for (int i = 0; i < 3; ++i) {
                cout << "\r* " << flush;
                sleep_for(seconds(1));
                cout << "\r *" << flush;
                sleep_for(seconds(1));
            }
            cout << "\n";
        }
    }
}
