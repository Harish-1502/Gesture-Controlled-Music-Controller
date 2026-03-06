#include <Arduino.h>
#include <Preferences.h>
#include "sensors.h"
#include "gestures.h"
#include "ble_media.h"
#include "power.h"


Preferences pref;
int F1_ON  = 0, F1_OFF = 0;  
int F2_ON  = 0, F2_OFF = 0;   
static bool f1On=false, f2On=false, flex_Cal_mode=false;
struct Cal { int f1Relax, f1Bent, f2Relax, f2Bent; } cal;

void setup() {
  Serial.begin(115200);
  Serial.println("Starting BLE work!");

  power_init();
  ble_media_init();

  if (!sensors_init()) {
      Serial.println("Failed to initialize sensors. Halting.");
      while (true) {
          delay(1000);
      }
  }
  gestures_init();

  // if(!loadCalibration){
  //   startCalibration();
  // } else{
  //   print("Data loaded on setup")
  // }
}

void loop() {
  power_update();

  if (!power_is_activated()) {
      return;
  }

  const SensorSample sample = sensors_read();
  const GestureEvent event = gestures_detect(sample);

  if (event != GestureEvent::None && ble_media_is_connected()) {
      ble_media_send(event);
  }
}

// ==============================   DEBUG   ============================
    // if(millis() - timer > 3000){ // print data every second
    // int flexValue1 = analogRead(flexSensor1);
    // int flexValue2 = analogRead(flexSensor2);

    // Serial.print("Flex Sensor 1 Value: ");
    // Serial.println(flexValue1);

    // Serial.print("Flex Sensor 2 Value: ");
    // Serial.println(flexValue2);
    // Serial.print(F("TEMPERATURE: "));Serial.println(mpu.getTemp());
    // Serial.print(F("ACCELERO  X: "));Serial.print(mpu.getAccX());
    // Serial.print("\tY: ");Serial.print(mpu.getAccY());
    // Serial.print("\tZ: ");Serial.println(mpu.getAccZ());
  
    // Serial.print(F("GYRO      X: "));Serial.print(mpu.getGyroX());
    // Serial.print("\tY: ");Serial.print(mpu.getGyroY());
    // Serial.print("\tZ: ");Serial.println(mpu.getGyroZ());
  
    // Serial.print(F("ACC ANGLE X: "));Serial.print(mpu.getAccAngleX());
    // Serial.print("\tY: ");Serial.println(mpu.getAccAngleY());
    
    // Serial.print(F("ANGLE     X: "));Serial.print(mpu.getAngleX());
    // Serial.print("\tY: ");Serial.print(mpu.getAngleY());
    // Serial.print("\tZ: ");Serial.println(mpu.getAngleZ());
    // Serial.println(F("=====================================================\n"));
    // timer = millis();

    //   Serial.println("2: Fully bent!");
    //   // bleKeyboard.write('F');  // Send a key only when fully bent
    //   // delay(500); 
    //   digitalWrite(lightPin, HIGH);
    // } else if (flexValue2 >= 1700 && flexValue2 < 2000) {
    //   Serial.println("2: Partially bent.");
    //   digitalWrite(lightPin, LOW);
    // } else if (flexValue2 < 1700) {
    //   Serial.println("2: Unbent / relaxed.");
    //   digitalWrite(lightPin, LOW);
  // }

// int readFlexAvg(int flexPin){
//   int arrayValue[5];

//   for(int i=0;i<5;i++){
//     arrayValue[i] = analogRead(flexPin);
//   }

//   for(int i = 0; i < 5; i++)
//     for(int j = i + 1; j < 5; j++)
//       if(arrayValue[j] < arrayValue[i]){
//         int temp = arrayValue[i];
//         arrayValue[i] = arrayValue[j];
//         arrayValue[j] = temp; 
//       }
  
//   if(flex_cal_mode)
//     return arrayValue[5/2];
//   else{
//     int median = arrayValue[5/2];
//     static float ema1=0.0f, ema2=0.0f;
    
//     if (flexPin==flexSensor1){ 
//       ema1 = 0.7f*ema1 + 0.3f*median;
//       return (int)ema1;
//     } else{                  
//       ema2 = 0.7f*ema2 + 0.3f*median;
//       return (int)ema2; 
//     }
//   }
// }

// void saveCalibration(){
//   pref.begin("mp3-controller", false);  

//   pref.putInt("f1Relax", cal.f1Relax);
//   pref.putInt("f1Bent", cal.f1Bent);
//   pref.putInt("f2Relax", cal.f2Relax);
//   pref.putInt("f2Bent", cal.f2Bent);

//   pref.end();
// }

// bool loadCalibration(){
//   pref.begin("mp3-controller", true);  

//   bool ok = pref.isKey("f1Relax") && pref.isKey("f2Relax") &&
//   pref.isKey("f1Bent") && pref.isKey("f2Bent");

//   if(ok){
//     Serial.printf("Loading from namespace");
//     pref.begin("mp3-controller", true);

//     cal.f1Relax = pref.getInt("f1Relax");
//     cal.f1Bent = pref.getInt("f1Bent");
//     cal.f2Relax = pref.getInt("f2Relax");
//     cal.f2Bent = pref.getInt("f2Bent");

//     pref.end();
//   }

//   return ok;
// }

// void flash(){
//   digitalWrite(lightPin, HIGH);
//   delay(50);
//   digitalWrite(lightPin, LOW);
// }

// int interp(int relax, int bent, float pct) {  
//   return (int)(relax + pct * (bent - relax));  
// }

// void startCalibration(){
//   Serial.printf("Starting calibration");
//   flex_cal_mode = true;
//   Serial.println("Step 1: Keep hand RELAXED (no bends)...");
//   delay(5000);
//   cal.f1Relax = readFlexAvg(flexSensor1);
//   cal.f2Relax = readFlexAvg(flexSensor2);
//   Serial.printf("Relaxed  -> F1:%d  F2:%d\n", cal.f1Relax, cal.f2Relax);
//   flash(); 
  
//   Serial.println("Step 2: Bend FLEX 1 fully...");
//   delay(5000);
//   cal.f1Bent = readFlexAvg(flexSensor1);
//   Serial.printf("F1 Bent  -> %d\n", cal.f1Bent);
//   flash();

//   Serial.println("Step 3: Bend FLEX 2 fully...");
//   delay(5000);
//   cal.f2Bent = readFlexAvg(flexSensor2);
//   Serial.printf("F2 Bent  -> %d\n", cal.f2Bent);
//   flash();

//   flex_cal_mode = false;
//   F1_ON  = interp(cal.f1Relax, cal.f1Bent, 0.90f); 
//   F2_ON  = interp(cal.f2Relax, cal.f2Bent, 0.90f);

//   F1_OFF = (int)(F1_ON * 0.85f);  
//   F2_OFF = (int)(F2_ON * 0.85f);   
//   Serial.printf("Loaded calibration. F1_ON:%d F1_OFF:%d  F2_ON:%d F2_OFF:%d\n",
//                 F1_ON, F1_OFF, F2_ON, F2_OFF);
  
//   Serial.println("Saved to NVS. Calibration complete.\n");
// }