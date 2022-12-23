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

struct LinAcc {
  float x;
  float y;
  float z;
} linAcc;

struct LinVel {
  float x = 0;
  float y = 0;
  float z = 0;
} linVel;

struct Pos {
  float x = 0;
  float y = 0;
  float z = 0;
} pos;



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


  // OscWiFi.publish(OSC_SERVER_HOST, OSC_SERVER_PORT, "/quat", quat.x, quat.y, quat.z, quat.w);
  OscWiFi.publish(OSC_SERVER_HOST, OSC_SERVER_PORT, "/euler", euler.x, euler.y, euler.z);
  // OscWiFi.publish(OSC_SERVER_HOST, OSC_SERVER_PORT, "/rpy", rpy.r, rpy.p, rpy.y);
  // OscWiFi.publish(OSC_SERVER_HOST, OSC_SERVER_PORT, "/acc", acc.x, acc.y, acc.z);
  // OscWiFi.publish(OSC_SERVER_HOST, OSC_SERVER_PORT, "/lin_acc", linAcc.x, linAcc.y, linAcc.z);
  // OscWiFi.publish(OSC_SERVER_HOST, OSC_SERVER_PORT, "/lin_vel", linVel.x, linVel.y, linVel.z);
  OscWiFi.publish(OSC_SERVER_HOST, OSC_SERVER_PORT, "/pos", pos.x, pos.y, pos.z);
}


unsigned long prevTime = millis();
unsigned long currTime;

void loop() {
  if (mpu.update()) {
    currTime = millis();

    // print_roll_pitch_yaw();
    // print_acc();
    print_vel_and_pos();

    // quat.x = mpu.getQuaternionX();
    // quat.y = mpu.getQuaternionY();
    // quat.z = mpu.getQuaternionZ();
    // quat.w = mpu.getQuaternionW();

    euler.x = mpu.getEulerX();
    euler.y = mpu.getEulerY();
    euler.z = mpu.getEulerZ();
  
    // rpy.r = mpu.getRoll();
    // rpy.p = mpu.getPitch();
    // rpy.y = mpu.getYaw();

    linAcc.x = mpu.getLinearAccX();
    linAcc.y = mpu.getLinearAccY();
    linAcc.z = mpu.getLinearAccZ();

    unsigned long timeDiff = currTime - prevTime;
    linVel.x += mpu.getLinearAccX() * timeDiff / 1000;
    linVel.y += mpu.getLinearAccY() * timeDiff / 1000;
    linVel.z += mpu.getLinearAccZ() * timeDiff / 1000;

    pos.x += linVel.x * timeDiff / 1000;
    pos.y += linVel.y * timeDiff / 1000;
    pos.z += linVel.z * timeDiff / 1000;

    prevTime = currTime;
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


void print_acc() {
  Serial.print("AccX, AccY, AccZ: ");
  Serial.print(mpu.getAccX(), 2);
  Serial.print(", ");
  Serial.print(mpu.getAccY(), 2);
  Serial.print(", ");
  Serial.print(mpu.getAccZ(), 2);
  Serial.print("  ");
  Serial.print("lin_acc = ");
  Serial.print(mpu.getLinearAccX(), 2);
  Serial.print(", ");
  Serial.print(mpu.getLinearAccY(), 2);
  Serial.print(", ");
  Serial.println(mpu.getLinearAccZ(), 2);
}


void print_vel_and_pos() {
  Serial.print("vx, vy, vz: ");
  Serial.print(linVel.x, 2);
  Serial.print(", ");
  Serial.print(linVel.y, 2);
  Serial.print(", ");
  Serial.print(linVel.z, 2);
  Serial.print("  ");
  Serial.print("pos = ");
  Serial.print(pos.x, 2);
  Serial.print(", ");
  Serial.print(pos.y, 2);
  Serial.print(", ");
  Serial.println(pos.z, 2);
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
