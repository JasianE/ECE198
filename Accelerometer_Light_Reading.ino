#include <Arduino.h>

// Pins 
const int zPin = A3;
const int xPin = A1;
const int yPin = A2;
const int lightPin = A0;

const int SAMPLES = 10;

// Smoothing 
double AlphaAcc = 0.5;        
float xSmoothedVoltage = 0.0;
float ySmoothedVoltage = 0.0;
float zSmoothedVoltage = 0.0;

// Calibration
// Offsets in volts (0 g per axis)
const float offsetX = 1.315;
const float offsetY = 1.2825;
const float offsetZ = 1.3067;

// Sensitivities in V/g per axis
const float sensX = 0.255;     // V per g
const float sensY = 0.2675;
const float sensZ = 0.2933;

// Reading the voltage on each pin
float readAveragedVoltage(int pin, int samples) {
  long sum = 0;
  for (int i = 0; i < samples; i++) {
    sum += analogRead(pin);
    delayMicroseconds(200);
  }
  //Read from the Arduino using 5V ref
  return (sum / (float)samples) * (5.0 / 1023.0);
}

// Smmoth the voltages
float smooth(float newVal, float prevVal, double alpha) {
  return alpha * newVal + (1.0 - alpha) * prevVal;
}

void setup() {
  Serial.begin(9600);
  delay(500);

  // Initialize smoothed voltages from initial readings
  xSmoothedVoltage = readAveragedVoltage(xPin, SAMPLES);
  ySmoothedVoltage = readAveragedVoltage(yPin, SAMPLES);
  zSmoothedVoltage = readAveragedVoltage(zPin, SAMPLES);
}

void loop() {
  // Reading the voltage
  float xV_raw = readAveragedVoltage(xPin, SAMPLES);
  float yV_raw = readAveragedVoltage(yPin, SAMPLES);
  float zV_raw = readAveragedVoltage(zPin, SAMPLES);

  //Smooth the voltages
  xSmoothedVoltage = smooth(xV_raw, xSmoothedVoltage, AlphaAcc);
  ySmoothedVoltage = smooth(yV_raw, ySmoothedVoltage, AlphaAcc);
  zSmoothedVoltage = smooth(zV_raw, zSmoothedVoltage, AlphaAcc);

  // Convert to g
  float gx = (xSmoothedVoltage - offsetX) / sensX;
  float gy = (ySmoothedVoltage - offsetY) / sensY;
  float gz = (zSmoothedVoltage - offsetZ) / sensZ;

  //Calculate magnitude of accleration
  float mag_g = sqrt(gx * gx + gy * gy + gz * gz);

  // Magnitude of acceleration calculated by subtracting 1g from accel
  float motion_g = mag_g - 1.0;
  if (motion_g < 0) motion_g = 0;  // clamp to avoid tiny negative noise

  float motion_ms2 = motion_g * 9.81f;
  int value = analogRead(0);
  Serial.println(value);

  // Print magnitude of acceleration excluding gravity
  Serial.println(motion_ms2);

  // Degugging
  /*
  Serial.print("gx: "); Serial.print(gx);
  Serial.print(" gy: "); Serial.print(gy);
  Serial.print(" gz: "); Serial.print(gz);
  Serial.print(" mag_g: "); Serial.print(mag_g);
  Serial.print(" motion_ms2: "); Serial.println(motion_ms2);
  */

  delay(500);
}
