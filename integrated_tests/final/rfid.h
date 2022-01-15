#include <SPI.h>
#include <MFRC522.h>
#include <SoftwareSerial.h>
#include "defines.h"

MFRC522 mfrc522(SS_PIN, RST_PIN);
SoftwareSerial BT(RX, TX); // RX, TX

void send_byte(byte *id, byte& idSize){
    for (byte i = 0; i < idSize; i++){
 
        if(id[i] < 16){
          BT.print("0");
        }
        BT.print(id[i], HEX);
        
    }
    BT.println();

    Serial.print("Sent id: ");
    for (byte i = 0; i < idSize; i++){
        if(id[i] < 16){
          Serial.print("0");
        }
        Serial.print(id[i], HEX);
    }
    Serial.println();
}


bool rfid(byte& idSize) {
    // 確認是否有新卡片
    if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
      byte *id = mfrc522.uid.uidByte;   // 取得卡片的UID
      idSize = mfrc522.uid.size;   // 取得UID的長度
      Serial.print("UID Size: ");       // 顯示卡片的UID長度值
      Serial.println(idSize);   
      for (byte i = 0; i < idSize; i++) {  // 逐一顯示UID碼
        Serial.print("id[");
        Serial.print(i);
        Serial.print("]: ");
        if(id[i] < 16){
          Serial.print("0");
        }
        Serial.println(id[i], HEX);       // 以16進位顯示UID值  

      }
      Serial.println();
      //#endif
      send_byte(id, idSize);
      mfrc522.PICC_HaltA();  // 讓卡片進入停止模式
      return true;
    }
    return false;
}
