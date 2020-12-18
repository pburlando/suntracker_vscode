/**
 * @file main.cpp
 * @author patrice.burlando@ac-nice.fr
 * @brief Projet de tracker solaire en première spécialité SI
 *        L'objectif est de recharger un téléphone avec un panneau 
 *        Photovoltaïque qui s'oriente vers le soleil automatiquement
 * @version 0.1
 * @date 2020-12-07
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#include <Arduino.h>
#include "DRVL298NMotorShield.h"


#define CDEMOTEURDIR 5
#define CDEMOTEURPWM 6
#define brocheCapteurDroit A1
#define brocheCapteurGauche A0
#define brocheMesureTensionPPV A2
#define brocheMesureCourantPPV A3
#define TE 500; // Durée d'échantillonage en milliseconde

unsigned long old_time = millis();

DRVL298NMotorShield drv(CDEMOTEURDIR, CDEMOTEURPWM, 7, 8, true, false);  //**  Pilotage du moteur à courant continu

float u_ppv_v(int u_ppv_raw);
float i_ppv_mA(int i_ppv_raw);

void setup() {
  // put your setup code here, to run once:
  pinMode(brocheCapteurDroit, INPUT);
  pinMode(brocheCapteurGauche, INPUT);
  Serial.begin(9600);
  Serial.println("Démarrage ...");
}


void loop() {
  /**
   * @brief Boucle de régulation tout ou rien de la position azimutale du panneau
   *        monitoring des informations sur le port série toutes les 0,5s
   *        ecoute du port série toutes les 0,1s
   * 
   */
  
  if ((millis() - old_time) >= 500) {
    int lumD = analogRead(brocheCapteurDroit);
    int lumG = analogRead(brocheCapteurGauche);
    int u_ppv = analogRead(brocheMesureTensionPPV);
    int i_ppv = analogRead(brocheMesureCourantPPV);
    int ecart = lumG - lumD;

    if (ecart > 30) {
      drv.setSpeeds(-130, 0);
    }
    else if (ecart < -30) {
      drv.setSpeeds(130, 0);
    }
    else {
      drv.setSpeeds(0, 0);
    }

    //** printing raw values : lumG lumD Ecart_G-D tensionPPV courantPPV 
    Serial.print(lumG);
    Serial.print(", ");
    Serial.print(lumD);
    Serial.print(", ");
    Serial.print(lumG - lumD);
    Serial.print(", ");
    Serial.print(u_ppv_v(u_ppv));
    Serial.print(", ");
    Serial.println(i_ppv_mA(i_ppv));

    old_time = millis();
  }

}


/**
 * @brief retourne la mesure de la tension du panneau photovoltaïque en Volt
 * 
 * @param u_ppv_raw 
 * @return float 
 */
float u_ppv_v(int u_ppv_raw) {
  return u_ppv_raw * 5 * 9.81 / 1024 /2;

}

/**
 * @brief retourne la mesure du courant délivré par le panneau photovoltaïque en milliAmpère
 * 
 * @param i_ppv_raw 
 * @return float 
 */
float i_ppv_mA(int i_ppv_raw) {
  return -0.5725 * i_ppv_raw + 378.4;
}
