#include <gtest/gtest.h>
#include "./monitor.h"

// Functional Tests
TEST(EvaluateVitals, NormalValues) {
    std::map<std::string, float> readings = {{"temperature", 98.6}, {"pulseRate", 80}, {"spo2", 95}};
    std::map<std::string, VitalRange> limits = {{"temperature", {95,102}}, {"pulseRate", {60,100}}, {"spo2",{90,100}}};
    auto violations = evaluateVitals(readings, limits);
    EXPECT_TRUE(violations.empty());
}

TEST(EvaluateVitals, CriticalTemperature) {
    std::map<std::string, float> readings = {{"temperature", 104}};
    std::map<std::string, VitalRange> limits = {{"temperature", {95,102}}};
    auto violations = evaluateVitals(readings, limits);
    ASSERT_EQ(violations.size(),1);
    EXPECT_EQ(violations[0].vitalName, "temperature");
    EXPECT_EQ(violations[0].message, "above-max");
}

TEST(EvaluateVitals, MultipleViolations) {
    std::map<std::string, float> readings = {{"temperature", 94}, {"pulseRate", 50}, {"spo2", 85}};
    std::map<std::string, VitalRange> limits = {{"temperature", {95,102}}, {"pulseRate", {60,100}}, {"spo2",{90,100}}};
    auto violations = evaluateVitals(readings, limits);
    ASSERT_EQ(violations.size(),3);
}

// OO Tests
TEST(PatientProfile, PediatricAdjustment) {
    VitalRegistry registry;
    registry.addVitalLimit("pulseRate", 60,100);
    registry.addVitalLimit("temperature", 95,102);

    PatientProfile child(10);
    child.personalizeLimits(registry);
    EXPECT_EQ(registry.getLimits().at("pulseRate").minValue,70);
    EXPECT_EQ(registry.getLimits().at("temperature").maxValue,100);
}

TEST(PatientProfile, SeniorAdjustment) {
    VitalRegistry registry;
    registry.addVitalLimit("pulseRate", 60,100);
    PatientProfile senior(70);
    senior.personalizeLimits(registry);
    EXPECT_EQ(registry.getLimits().at("pulseRate").maxValue,90);
}

