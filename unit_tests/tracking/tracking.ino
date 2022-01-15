#include <SparkFun_TB6612.h>
#include <SoftwareSerial.h>
#include "ir.h"
//#include "tracking.h"
#include "defines.h"
#include "motor.h"

const int TRACKING = 0;
const int ON_BLOCK = 1;
const int TURN = 2;
const int END = 3;

const int commands[10] = {R,R,U,R,R,U,L,F,L,S};
int command_pos = 0;
int state = TRACKING;
int onblock_v = 150;
int turn_v = 120;

void setup() {
    Serial.begin(9600);
}
 
void loop() {
    int irRes = readIR();
    
    if(state == TRACKING){
      
        if(irRes%100 > 3) state = ON_BLOCK;
        else{
            tracking(irRes/100);
            Serial.print(irRes/100);
            //delay(50);
            //brake(motor1, motor2);
            //delay(10);
        }
    }
    
    else if(state==ON_BLOCK){
        irRes = readIR();
        if(irRes%100 >= 3) {
            MotorWriting(onblock_v, onblock_v);
            delay(30);
            //MotorWriting(0,0);
            //delay(10);
        }
        if(irRes%100 < 3){
            MotorWriting(120,120);
            delay(50);
            MotorWriting(0,0);
            delay(100);
            state = TURN;
            if(commands[command_pos] == R){
                MotorWriting(turn_v, 0);
                delay(500);
            }
            else if(commands[command_pos] == L){
                MotorWriting(0, turn_v);
                delay(500);
            }
            
        }
    }
    else if(state==TURN){
        irRes = readIR();
        if(commands[command_pos] == F){
            MotorWriting(120, 120);
            delay(50);
            state = TRACKING;
            command_pos++;
        }
        else if(commands[command_pos] == R){
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
        else if(commands[command_pos] == L){
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
        else if(commands[command_pos] == U){
            if(irRes/100 != 0 && irRes/100 != 1){
                MotorWriting(-100, 100);
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
        else{
            state == END;
        }
        //command_pos++;
    }
    else if(state == END) MotorWriting(0, 0);
}
