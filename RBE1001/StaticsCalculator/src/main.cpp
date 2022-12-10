#include <Arduino.h>
#include <RBE1001Lib.h>
#include <button.h>
#include <iostream>
#include <string>


Button bootButton(BOOT_FLAG_PIN);

float g = 9.81; // m/s^2

void RWD(float alpha_deg, float a, float b, float h, float mu, float m){
  float alpha = alpha_deg * 3.1415926 / 180;
  float mgcos = m*g*cos(alpha);
  float mgsin = m*g*sin(alpha);
  float total_force = mgsin;
  float NR = mgcos - (a*mgcos-h*mgsin)/(a+b);
  float NF = (a*mgcos-h*mgsin)/(a+b);
  float AvailableF = 0;
  float AvailableR = mu * NR;
  Serial.printf("Alpha: %f \t Force Needed: %f \t Normal Rear: %f \t Normal Front: %f \t Available Rear: %f \t Available Front %f \n",
  alpha_deg, mgsin, NR, NF, AvailableR, AvailableF);
}

void FWD(float alpha_deg, float a, float b, float h, float mu, float m){
  float alpha = alpha_deg * 3.1415926 / 180;
  float mgcos = m*g*cos(alpha);
  float mgsin = m*g*sin(alpha);
  float total_force = mgsin;
  float NR = mgcos - (a*mgcos-h*mgsin)/(a+b);
  float NF = (a*mgcos-h*mgsin)/(a+b);
  float AvailableF = mu*NF;
  float AvailableR = 0;
  Serial.printf("Alpha: %f \t Force Needed: %f \t Normal Rear: %f \t Normal Front: %f \t Available Rear: %f \t Available Front %f \n",
  alpha_deg, mgsin, NR, NF, AvailableR, AvailableF);
}

void AWD(float alpha_deg, float a, float b, float h, float mu, float m){
  float alpha = alpha_deg * 3.1415926 / 180;
  float mgcos = m*g*cos(alpha);
  float mgsin = m*g*sin(alpha);
  float total_force = mgsin;
  float NR = mgcos - (a*mgcos-h*mgsin)/(a+b);
  float NF = (a*mgcos-h*mgsin)/(a+b);
  float AvailableF = mu*NF;
  float AvailableR = mu*NR;
  Serial.printf("Alpha: %f \t Force Needed: %f \t Normal Rear: %f \t Normal Front: %f \t Available Rear: %f \t Available Front %f \n",
  alpha_deg, mgsin, NR, NF, AvailableR, AvailableF);
}


/*void printResults(float alpha_deg, float a, float b, float h, float mu, float m){
  Serial.printf("Alpha: %f \t Force Needed: %f \t Normal Rear: %f \t Normal Front: %f \t Available Rear: %f \t Available Front %f");
}*/

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  bootButton.Init();
}

void loop() {
  // These are the hard-coded values that should be changed to change the calculation
  float a = 0.2;
  float b = 0.3;
  float h = 0.2;
  float mu = 0.9;
  float m = 6;

  if (bootButton.CheckButtonPress()) //If the button was pressed, calculate 0-60 degrees
  {
    //RWD(10, a, b, h, mu, m);
    for (int i = 0; i <= 60; i++) {
      RWD(i, a, b, h, mu, m);
    }
  }
}