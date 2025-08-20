#include "monitor.h"
#include <iostream>
#include <thread>
#include <chrono>

using std::cout;
using std::flush;
using std::this_thread::sleep_for;
using std::chrono::seconds;

// Functional Core
std::vector<VitalViolation> evaluateVitals(
    const std::map<VitalType, float>& patientReadings,
    const std::map<VitalType, VitalRange>& allowedLimits
) {
    std::vector<VitalViolation> violations;
    for (const auto& [vital, actualValue] : patientReadings) {
        auto iterator = allowedLimits.find(vital);
        if (iterator == allowedLimits.end()) continue; // skip unknown vitals

        const auto& range = iterator->second;
        if (actualValue < range.minValue) {
            violations.push_back({vital, actualValue, range, "below-min"});
        } else if (actualValue > range.maxValue) {
            violations.push_back({vital, actualValue, range, "above-max"});
        }
    }
    return violations;
}

// OO: VitalRegistry
void VitalRegistry::addVitalLimit(VitalType vital, float minValue, float maxValue) {
    vitalLimits[vital] = {minValue, maxValue};
}

void VitalRegistry::adjustMin(VitalType vital, float newMin) {
    auto iterator = vitalLimits.find(vital);
    if (iterator != vitalLimits.end()) it->second.minValue = newMin;
}

void VitalRegistry::adjustMax(VitalType vital, float newMax) {
    auto iterator = vitalLimits.find(vital);
    if (iterator != vitalLimits.end()) it->second.maxValue = newMax;
}

const std::map<std::string, VitalRange>& VitalRegistry::getLimits() const {
    return vitalLimits;
}

// OO: PatientProfile
PatientProfile::PatientProfile(int age) : ageYears(age) {}

void PatientProfile::personalizeLimits(VitalRegistry& registry) const {
    if (ageYears <= 12) { // pediatric
        registry.adjustMax(VitalType::Temperature, 100);
        registry.adjustMin(VitalType::PulseRate, 70);
    } else if (ageYears >= 65) { // senior
        registry.adjustMax(VitalType::PulseRate, 90);
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
