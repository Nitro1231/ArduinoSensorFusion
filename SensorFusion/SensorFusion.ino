#include "Key.h"
#include "MPU9250.h"
#include "ArduinoOSCWiFi.h"


MPU9250 mpu;

struct Quat {
  float x;
  float y;
  float z;
  float w;
} quat;

struct Euler {
  float x;
  float y;
  float z;
} euler;

struct RPY {
  float r;
  float p;
  float y;
} rpy;


void setup() {
  Serial.begin(115200);
  Wire.begin();
  delay(2000);


  WiFi.begin(WiFi_SSID, WiFi_PASSWORD);
  Serial.println("Connecting to the WiFi");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());


  if (!mpu.setup(0x68)) {  // change to your own MPU9250 address
    while (true) {
      Serial.println("MPU connection failed.");
      delay(1000);
    }
  }

  Serial.println("Accel Gyro calibration will start in 5sec.");
  Serial.println("Please leave the device still on the flat plane.");
  mpu.verbose(true);
  delay(5000);
  mpu.calibrateAccelGyro();

  Serial.println("Mag calibration will start in 5sec.");
  Serial.println("Please Wave device in a figure eight until done.");
  delay(5000);
  mpu.calibrateMag();

  print_calibration();
  mpu.verbose(false);

  OscWiFi.publish(OSC_SERVER_HOST, OSC_SERVER_PORT, "/quat", quat.x, quat.y, quat.z, quat.w);
  // OscWiFi.publish(OSC_SERVER_HOST, OSC_SERVER_PORT, "/euler", euler.x, euler.y, euler.z);
  // OscWiFi.publish(OSC_SERVER_HOST, OSC_SERVER_PORT, "/rpy", rpy.r, rpy.p, rpy.y);
}


void loop() {
  // delay(500);
  if (mpu.update()) {
    // static uint32_t prev_ms = millis();
    // if (millis() > prev_ms + 25) {
    //   print_roll_pitch_yaw();
    //   prev_ms = millis();
    // }

    quat.x = mpu.getQuaternionX();
    quat.y = mpu.getQuaternionY();
    quat.z = mpu.getQuaternionZ();
    quat.w = mpu.getQuaternionW();

    // euler.x = mpu.getEulerX();
    // euler.y = mpu.getEulerY();
    // euler.z = mpu.getEulerZ();
    // rpy.r = mpu.getRoll();
    // rpy.p = mpu.getPitch();
    // rpy.y = mpu.getYaw();
  }
  OscWiFi.update();
}


void print_roll_pitch_yaw() {
  Serial.print("Yaw, Pitch, Roll: ");
  Serial.print(mpu.getYaw(), 2);
  Serial.print(", ");
  Serial.print(mpu.getPitch(), 2);
  Serial.print(", ");
  Serial.print(mpu.getRoll(), 2);
  Serial.print("  ");
  Serial.print("Mag : ");
  Serial.print(mpu.getMagX(), 2);
  Serial.print(", ");
  Serial.print(mpu.getMagY(), 2);
  Serial.print(", ");
  Serial.print(mpu.getMagZ(), 2);
  Serial.print(", ");
  Serial.print("lin_acc = ");
  Serial.print(mpu.getLinearAccX(), 2);
  Serial.print(", ");
  Serial.print(mpu.getLinearAccY(), 2);
  Serial.print(", ");
  Serial.println(mpu.getLinearAccZ(), 2);
}


void print_calibration() {
  Serial.println("< calibration parameters >");
  Serial.println("accel bias [g]: ");
  Serial.print(mpu.getAccBiasX() * 1000.f / (float)MPU9250::CALIB_ACCEL_SENSITIVITY);
  Serial.print(", ");
  Serial.print(mpu.getAccBiasY() * 1000.f / (float)MPU9250::CALIB_ACCEL_SENSITIVITY);
  Serial.print(", ");
  Serial.print(mpu.getAccBiasZ() * 1000.f / (float)MPU9250::CALIB_ACCEL_SENSITIVITY);
  Serial.println();
  Serial.println("gyro bias [deg/s]: ");
  Serial.print(mpu.getGyroBiasX() / (float)MPU9250::CALIB_GYRO_SENSITIVITY);
  Serial.print(", ");
  Serial.print(mpu.getGyroBiasY() / (float)MPU9250::CALIB_GYRO_SENSITIVITY);
  Serial.print(", ");
  Serial.print(mpu.getGyroBiasZ() / (float)MPU9250::CALIB_GYRO_SENSITIVITY);
  Serial.println();
  Serial.println("mag bias [mG]: ");
  Serial.print(mpu.getMagBiasX());
  Serial.print(", ");
  Serial.print(mpu.getMagBiasY());
  Serial.print(", ");
  Serial.print(mpu.getMagBiasZ());
  Serial.println();
  Serial.println("mag scale []: ");
  Serial.print(mpu.getMagScaleX());
  Serial.print(", ");
  Serial.print(mpu.getMagScaleY());
  Serial.print(", ");
  Serial.print(mpu.getMagScaleZ());
  Serial.println();
}
