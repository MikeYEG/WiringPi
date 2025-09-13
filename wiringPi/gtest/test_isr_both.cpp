#include <gtest/gtest.h>
#include <time.h>
#include <wiringPi.h>

#define TRIGGER_PIN  26  // BCM 12
#define LISTENER_PIN 31  // BCM 1

/**
 * Tests ISR for both edges.
 */
class ISRBoth : public ::testing::Test {
 public:
  static void SetUpTestSuite() {
    wiringPiSetup();
    pinMode(TRIGGER_PIN, OUTPUT);
    digitalWrite(TRIGGER_PIN, LOW);
    wiringPiISR(LISTENER_PIN, INT_EDGE_BOTH, ISRBoth::TriggerCount);
    delay(100);
  }

  static uint32_t count;
  static void TriggerCount() { ++count; }

 protected:
  virtual void SetUp() { count = 0; }

  virtual void TearDown() {
    digitalWrite(TRIGGER_PIN, LOW);
    delay(10);
  }
};

uint32_t ISRBoth::count = 0;

TEST_F(ISRBoth, Fundamental) {
  EXPECT_EQ(count, 0u);
  digitalWrite(TRIGGER_PIN, HIGH);
  delay(10);
  EXPECT_EQ(count, 1u);
  digitalWrite(TRIGGER_PIN, LOW);
  delay(10);
  EXPECT_EQ(count, 2u);
}

TEST_F(ISRBoth, ThreeTriggers) {
  for (int i = 0; i < 5; ++i) {
    digitalWrite(TRIGGER_PIN, (i + 1) % 2);
    delay(10);
  }
  EXPECT_EQ(count, 5u);
}