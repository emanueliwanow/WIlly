#ifndef MOTORES_H_INCLUDED
#define MOTORES_H_INCLUDED
#include "ESC.h" // RC_ESP library installed by Library Manager




class Motors {
  private: 
    //ESC myESCR (byte, int , int, int);
    //ESC myESCL (byte, int, int, int);
  public:
    Motors();
    
    void setupMOT();
    void armMOT();
    void disarmMOT();
    void setMotR_front();
    void setMotR_back();
    void setMotL_back();
    void setMotL_front();
    void setMotR_vel(int vel);
    void setMotL_vel(int vel);
    void ModAuto();

  
};

#endif 
