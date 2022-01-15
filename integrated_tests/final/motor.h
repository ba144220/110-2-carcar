#include "defines.h"

const int offsetA = 1;  // 正反轉設定A，可能值為1或-1。
const int offsetB = -1;  // 正反轉設定B，可能值為1或-1。

Motor motor1 = Motor(AIN1, AIN2, PWMA, offsetA, HIGH);
Motor motor2 = Motor(BIN1, BIN2, PWMB, offsetB, HIGH);

void MotorWriting(double vL, double vR){

    if(vR > 255) vR = 255;
    else if(vR < -255) vR = -255;

    if(vL > 255) vL = 255;
    else if(vL < -255) vL = -255;
   
    motor1.drive(vL);
    motor2.drive(vR);
    //Serial.print("  vR = ");
    //Serial.print(vR);
    //Serial.print("  vL = ");
    //Serial.println(vL);
}

void tracking(int irRes){
    static int lastDetected = 2 ;

    if(irRes>=0){
        lastDetected = irRes;
    }
    

  if(lastDetected==0){
    MotorWriting( 0 , 120 );
  }else if(lastDetected==1){
    MotorWriting(120 , 180 );
  }else if(lastDetected==2){
    MotorWriting( 200 , 200  );
  }else if(lastDetected==3){
    MotorWriting( 180 , 120 );
  }else if(lastDetected==4){
   MotorWriting( 120 , 0 );
  }
  
}
