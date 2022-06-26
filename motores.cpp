#include "definicoes_sistema.h"
#include "motores.h"

ESC myESCR (motor1, 1000, 2000, 500);
ESC myESCL (motor2, 1000, 2000, 500);

Motors::Motors()
{
  
}

void Motors::setupMOT(){
  
  pinMode(Rele1, OUTPUT);
  pinMode(Rele2, OUTPUT);
  pinMode(Rele3, OUTPUT);
  pinMode(Rele4, OUTPUT);
  pinMode(motor1, OUTPUT);
  pinMode(motor2, OUTPUT);
  pinMode(buttonL,INPUT);
  pinMode(buttonR,INPUT);
 
}

void Motors::armMOT(){
  Serial.print("Armando motores");
  myESCR.arm();
  myESCL.arm();
}

void Motors::disarmMOT(){
  Serial.print("Desarmando motores");
  myESCR.stop();
  myESCL.stop();
}

void Motors::setMotR_front(){
  digitalWrite(Rele2, HIGH);
  digitalWrite(Rele1, HIGH);
}

void Motors::setMotR_back(){
  digitalWrite(Rele2, LOW);
  digitalWrite(Rele1, LOW);
}

void Motors::setMotL_back(){
  digitalWrite(Rele3, LOW);
  digitalWrite(Rele4, LOW);
}

void Motors::setMotL_front(){
  digitalWrite(Rele3, HIGH);
  digitalWrite(Rele4, HIGH);
}

void Motors::setMotR_vel(int vel){
  //Serial.print("Setando velocidade motor direito");
  //Serial.print(vel);
  if (vel>=0){
    //Serial.print("Setando velocidade motor direito para frente");
    setMotR_front();
    vel = map(vel, 0, 100, MIN_SPEED, MAX_SPEED);
    myESCR.speed(vel);
    
  }
  else {
    setMotR_back();
    vel = -vel;
    vel = map(vel, 0, 100, MIN_SPEED, MAX_SPEED);
    myESCR.speed(vel);
  }
}

void Motors::setMotL_vel(int vel){
  if (vel>=0){
    setMotL_front();
    vel = map(vel, 0, 100, MIN_SPEED, MAX_SPEED);
    myESCL.speed(vel);
  }
  else {
    setMotL_back();
    vel = -vel;
    vel = map(vel, 0, 100, MIN_SPEED, MAX_SPEED);
    myESCL.speed(vel);
  }
}

void Motors::ModAuto(){
 
  int ButL = digitalRead(buttonL);
  int ButR = digitalRead(buttonR);
  
  if ((ButL == 0) && (ButR == 0)){
    //Serial.println("Indo pra frente");
    Serial.println(ButL);
    Serial.println(ButR);
    setMotR_vel(50);
    setMotL_vel(50);
  }
  else{
    setMotL_vel(-50);
    setMotR_vel(30);
    delay(500);
    }
    
  }
  
  
  
