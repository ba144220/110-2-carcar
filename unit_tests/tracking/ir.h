#include "defines.h"

int readIR(){
  bool ir[5];
  
  ir[0] = digitalRead(IR1);
  ir[1] = digitalRead(IR2);
  ir[2] = digitalRead(IR3);
  ir[3] = digitalRead(IR4);
  ir[4] = digitalRead(IR5);
  #ifdef DEBUG
  Serial.print(" IR1:");
  Serial.print(ir[0]);  
  Serial.print(" IR2:");
  Serial.print(ir[1]); 
  Serial.print(" IR3:");
  Serial.print(ir[2]);  
  Serial.print(" IR4:");
  Serial.print(ir[3]);  
  Serial.print(" IR5:");
  Serial.print(ir[4]);  

  Serial.println();
  #endif


  int numDetected=0;
  int result = -1;
  int irNum[5] = {0,4,1,3,2};


  for(int i=0; i<5; i++){
    if(ir[irNum[i]]){
      numDetected++;
      result = irNum[i];
    }
  }

  return 100*result + numDetected;

}
