#include <SparkFun_TB6612.h>

#include "ir.h"

#define BIG_MAP
#define FAST // FAST, MEDIUM, SLOW


#include "rfid.h"
#include "defines.h"
#include "motor.h"



byte idSize = 4;
byte* id;

char BluetoothData; // the data given from Computer
 
const int _IDLE = 0;
const int TRACKING = 1;
const int ON_BLOCK = 2;
const int TURN = 3;
const int END = 4;
const int DIRECT_CONTROL = 5;
const int READ_CMD = 6;

#ifdef E_MAP
int commands[200] = {_R,_R,_U,_R,_R,_U,_L,_F,_L,_S}; // commands for E-shape map
#endif

#ifdef BIG_MAP
int commands[200] = {   _L, _L, _R, _U,

                        _L, _L, _R, _R, _U, 
                    
                        _R, _F, _L, _U,

                        _L, _F, _U,

                        _L, _L, _R, _R, _U,

                        _L, _L, _R, _F, _R, _L, _L, _R, _R, _U,
                        
                        _F, _F, _U, 
                        _R, _F, _L, _F, _U,
                        _R, _U,
                        _R, _F, _F, _F, _L, _U,
                        _F, _R, _R, _U, _S
                        
                        
                        
                        }; // commands for E-shape map

#endif
int command_pos = 0;
int state = _IDLE;

#ifdef FAST
int onblock_v = 220;
#endif


#ifdef MEDIUM
int onblock_v = 150;
#endif


#ifdef FAST
int turn_v = 140;
#endif

#ifdef MEDIUM
int turn_v = 120;
#endif


#ifdef FAST
int uturn_v = 150;
#endif

#ifdef MEDIUM
int uturn_v = 100;
#endif

void setup() {
  BT.begin(9600);
  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(BUTTON1, INPUT);
  delay(1000);
}
 
void loop() {
    int irRes = readIR();
    if(state == _IDLE){
      
        digitalWrite(LED1, HIGH);
        String readString;
        while (BT.available()) {
          delay(3);  //delay to allow buffer to fill 
          if (BT.available() >0) {
            char c = BT.read();  //gets one byte from serial buffer
            readString += c; //makes the string readString
          } 
        }

        if(readString=="start"){
          state = TRACKING;
          BT.println("Change state to TRACKING");
        }else if(readString=="direct"){
          state = DIRECT_CONTROL;
          BT.println("Change state to CIRECT_CONTROL");
        }else if(readString=="cmd"){
          state = READ_CMD;
          BT.println("Change state to READ_CMD");
        }else if (readString.length() >0) {
          Serial.println(readString); //see what was received
          BT.print("Echo: ");
          BT.println(readString);
        }

        if(digitalRead(BUTTON1)==HIGH){
          for(int i=4; i>=0; i--){
            digitalWrite(LED1, LOW);
            delay(500);
            digitalWrite(LED1, HIGH);
            delay(500);
          }
          digitalWrite(LED1, LOW);
          state = TRACKING;
        }
    
        
       
      

    }else if(state == TRACKING){

        if(commands[command_pos]==_U){
          if(rfid(idSize)){
            digitalWrite(LED2, HIGH);
          }
        }else{
          digitalWrite(LED2, LOW);
        }
        
      
        if(irRes%100 > 3){
          state = ON_BLOCK;
        }
        else{
          tracking(irRes/100);
        }
    }
    
    else if(state==ON_BLOCK){
        irRes = readIR();
        if(irRes%100 >= 3) {
            MotorWriting(onblock_v, onblock_v);
            delay(30);
        }
        if(irRes%100 < 3){
            MotorWriting(120,120);
            delay(50);
            MotorWriting(0,0);
            
            #ifdef MEDIUM
            delay(100);
            #endif
            #ifdef SLOW
            delay(100);
            #endif
            
            state = TURN;
            if(commands[command_pos] == _R){
                MotorWriting(turn_v, 0);
                delay(500);
            }
            else if(commands[command_pos] == _L){
                MotorWriting(0, turn_v);
                delay(500);
            }
        }
    }
    else if(state==TURN){
        irRes = readIR();
        if(commands[command_pos] == _F){
            MotorWriting(120, 120);
            delay(50);
            state = TRACKING;
            command_pos++;
        }
        else if(commands[command_pos] == _R){
            if(irRes/100 != 4 && irRes/100 != 3){
                MotorWriting(turn_v,0);
                delay(50);
                //MotorWriting(0,0);
                //delay(8);
            }
            else{
                MotorWriting(0,0);
                delay(50);
                state = TRACKING;
                command_pos++;
            }
        }
        else if(commands[command_pos] == _L){
            if(irRes/100 != 0 && irRes/100 != 1){
                MotorWriting(0,turn_v);
                delay(50);
                //MotorWriting(0,0);
                //delay(8);
            }
            else{
                MotorWriting(0,0);
                delay(50);
                state = TRACKING;
                command_pos++;
            }
        }
        else if(commands[command_pos] == _U){
            if(irRes/100 != 0 && irRes/100 != 1){
                MotorWriting(-1*uturn_v, uturn_v);
                delay(50);
            }
            else{
                MotorWriting(0,0);
                delay(50);
                state = TRACKING;
                command_pos++;
            }
        }
        else if(commands[command_pos] == _S){
            state = END;
            command_pos = 0;
        }
       
    }
    else if(state == END){
      MotorWriting(0, 0);
      delay(100);
      state = _IDLE;
    }else if(state == DIRECT_CONTROL){
        String readString;
        while (BT.available()) {
          delay(3);  //delay to allow buffer to fill 
          if (BT.available() >0) {
            char c = BT.read();  //gets one byte from serial buffer
            readString += c; //makes the string readString
          } 
        }
        if (readString.length() >0) {
          Serial.println(readString); //see what was received
          BT.println(readString);
        }
        for(int i=0; i<readString.length(); i++){
          if(readString[i]=='q'){
            state = _IDLE;
          }else if(readString[i]=='r'){
            MotorWriting(100,-100);
            delay(100);
            MotorWriting(0,0);
          }else if(readString[i]=='l'){
            MotorWriting(-100,100);
            delay(100);
            MotorWriting(0,0);
          }else if(readString[i]=='f'){
            MotorWriting(200,200);
            delay(300);
            MotorWriting(0,0);
          }else if(readString[i]=='b'){
            MotorWriting(-100,-100);
            delay(200);
            MotorWriting(0,0);
          }
        }
    }else if(state==READ_CMD){
        String readString;
        while (BT.available()) {
          delay(3);  //delay to allow buffer to fill 
          if (BT.available() >0) {
            char c = BT.read();  //gets one byte from serial buffer
            readString += c; //makes the string readString
          } 
        }
        if (readString.length() >0) {
          Serial.println(readString); //see what was received
          BT.println(readString);
        }
        
        for(int i=0; i<readString.length(); i++){
          if(readString[i]=='q'){
            state = _IDLE;
            command_pos = 0;
          }else if(readString[i]=='r'){
            commands[command_pos] = _R;
            command_pos++;
          }else if(readString[i]=='l'){
            commands[command_pos] = _L;
            command_pos++;
          }else if(readString[i]=='f'){
            commands[command_pos] = _F;
            command_pos++;
          }else if(readString[i]=='u'){
            commands[command_pos] = _U;
            command_pos++;
          }else if(readString[i]=='s'){
            commands[command_pos] = _S;
            command_pos++;
          }  
          
        } 
    }
}
