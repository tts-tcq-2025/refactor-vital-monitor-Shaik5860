#include <gtest/gtest.h>
#include "./monitor.h"

// Functional Tests
TEST(EvaluateVitals, NormalValues) {
    std::map<VitalType, float> readings = {
        {VitalType::Temperature, 98.6},
        {VitalType::PulseRate, 80},
        {VitalType::Spo2, 95}
    };
    std::map<VitalType, VitalRange> limits = {
        {VitalType::Temperature, {95,102}},
        {VitalType::PulseRate, {60,100}},
        {VitalType::Spo2, {90,100}}
    };

    auto violations = evaluateVitals(readings, limits);
    EXPECT_TRUE(violations.empty());
}

TEST(EvaluateVitals, CriticalTemperature) {
    std::map<VitalType, float> readings = {
        {VitalType::Temperature, 104}
    };
    std::map<VitalType, VitalRange> limits = {
        {VitalType::Temperature, {95,102}}
    };

    auto violations = evaluateVitals(readings, limits);
    ASSERT_EQ(violations.size(),1);
    EXPECT_EQ(violations[0].vital, VitalType::Temperature);
    EXPECT_EQ(violations[0].message, "above-max");
}

TEST(EvaluateVitals, MultipleViolations) {
    std::map<VitalType, float> readings = {
        {VitalType::Temperature, 94},
        {VitalType::PulseRate, 50},
        {VitalType::Spo2, 85}
    };
    std::map<VitalType, VitalRange> limits = {
        {VitalType::Temperature, {95,102}},
        {VitalType::PulseRate, {60,100}},
        {VitalType::Spo2,{90,100}}
    };

    auto violations = evaluateVitals(readings, limits);
    ASSERT_EQ(violations.size(),3);
}

// OO Tests
TEST(PatientProfile, PediatricAdjustment) {
    VitalRegistry registry;
    registry.addVitalLimit(VitalType::PulseRate, 60,100);
    registry.addVitalLimit(VitalType::Temperature, 95,102);

    PatientProfile child(10);
    child.personalizeLimits(registry);

    EXPECT_EQ(registry.getLimits().at(VitalType::PulseRate).minValue,70);
    EXPECT_EQ(registry.getLimits().at(VitalType::Temperature).maxValue,100);
}

TEST(PatientProfile, SeniorAdjustment) {
    VitalRegistry registry;
    registry.addVitalLimit(VitalType::PulseRate, 60,100);

    PatientProfile senior(70);
    senior.personalizeLimits(registry);

    EXPECT_EQ(registry.getLimits().at(VitalType::PulseRate).maxValue,90);
}
