#include <SoftwareSerial.h>// import the serial library
 
SoftwareSerial BT(13, 12); // RX, TX
char BluetoothData; // the data given from Computer
 
void setup() 
{
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("Test");
  BT.begin(9600);
  BT.println("Bluetooth On please press 1 or 0 blink LED ..");

}
 
void loop() 
{
  // put your main code here, to run repeatedly:
  if (BT.available())
  {
    BluetoothData=BT.read();
    Serial.println(BluetoothData);
    BT.println(BluetoothData);
  }
delay(100);// prepare for next data ...
}
