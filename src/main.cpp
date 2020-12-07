#include <Arduino.h>
#include <DRVL298NMotorShield.h>
#include <Servo.h>


#define CDEMOTEURDIR 5
#define CDEMOTEURPWM 6
#define brocheCapteurDroit A0
#define brocheCapteurGauche A1
#define TE 500; // Durée d'échantillonage en milliseconde
unsigned long old_time = millis();
DRVL298NMotorShield drv(CDEMOTEURDIR, CDEMOTEURPWM, 7, 8, true, false);
Servo my_servo;
void setup() {
  // put your setup code here, to run once:
  pinMode(brocheCapteurDroit, INPUT);
  pinMode(brocheCapteurGauche, INPUT);
  my_servo.attach(9);
  Serial.begin(9600);
  Serial.println("Démarrage ...");
}

void loop() {
  // put your main code here, to run repeatedly:
  
  if ((millis() - old_time) >= 500) {
    int lumD = analogRead(brocheCapteurDroit);
    int lumG = analogRead(brocheCapteurGauche);
    int ecart = lumG - lumD;
    //lumD = map(lumD, 0, 1024, 0, 5);
    //lumG = map(lumG, 0, 1024, 0, 5);
    Serial.print(lumG);
    Serial.print("  |  ");
    Serial.print(lumD);
    Serial.print("  |  ");
    Serial.println(lumG - lumD);

    if (ecart > 30) {
      drv.setSpeeds(-130, 0);
      my_servo.write(-90);
    }
    else if (ecart < -30) {
      drv.setSpeeds(130, 0);
      my_servo.write(90);
    }
    else {
      drv.setSpeeds(0, 0);
      my_servo.write(0);
    }
    old_time = millis();
  }

}