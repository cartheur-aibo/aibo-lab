#include "HelloBody.h"

// This file is pseudo-OPEN-R scaffolding.
// To make it build, graft this behavior into a known-good Sony sample object
// from your OPEN-R SDK installation.

HelloBody::HelloBody() {}
HelloBody::~HelloBody() {}

void HelloBody::DoInit() {
    // Allocate resources, open primitive locators, initialize observers.
}

void HelloBody::DoStart() {
    SetStartupExpression();
    // Register/listen for touch sensor events.
}

void HelloBody::DoStop() {
    // Stop observers and pending motions.
}

void HelloBody::DoDestroy() {
    // Release resources.
}

void HelloBody::OnSensorEvent() {
    // If head/back touch sensor is active:
    NodHeadOnce();
    // Return quickly. Do not block inside this callback.
}

void HelloBody::SetStartupExpression() {
    // Set Illume-Face/body LEDs here.
}

void HelloBody::NodHeadOnce() {
    // Send a small, safe head joint motion.
    // Keep amplitude conservative while testing.
}
