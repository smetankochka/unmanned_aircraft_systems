// Basic demo for accelerometer readings from Adafruit MPU6050

#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

Adafruit_MPU6050 mpu;

float offs_x = 0.03,offs_y = -0.04,offs_z = 0.8;

void setup(void) {
  Serial.begin(115200);
  while (!Serial)
    delay(10); // will pause Zero, Leonardo, etc until serial console opens

  Serial.println("Adafruit MPU6050 test!");

  // Try to initialize!
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  Serial.println("MPU6050 Found!");

  mpu.setAccelerometerRange(MPU6050_RANGE_2_G);

  mpu.setGyroRange(MPU6050_RANGE_500_DEG);

  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

  delay (100);

  sensors_event_t a, g, temp;
  float sum_x = 0, sum_y = 0, sum_z = 0;

  for (int i = 0; i<500; i++) {
     mpu.getEvent(&a, &g, &temp);  
     sum_x += a.acceleration.x;
     sum_y += a.acceleration.y;
     sum_z += a.acceleration.z;
     delay (5);
  }

  offs_x = sum_x/500;
  offs_y = sum_y/500;
  offs_z = (sum_z/500)-10;
}


float x = 0, y = 0, z = 0;

float angle_acc = 0, filt_angle;
float tmp[10];
int arr = 0;

void loop() {

  /* Get new sensor events with the readings */
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);


  angle_acc = -degrees(atan ((a.acceleration.y-offs_y)/(a.acceleration.z-offs_z) / 0.99));

  filt_angle = filt_angle*0.9 + angle_acc*0.1;
  float sum = 0;
  for (int i = 0; i<9; i++) {
    tmp[i] = tmp[i + 1];
    sum += tmp[i + 1];
  }
  tmp[9] = angle_acc;

  arr += 1;

  Serial.println((sum + angle_acc)/10);

  delay(50);
}