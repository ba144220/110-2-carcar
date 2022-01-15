#include <SPI.h>
#include <MFRC522.h>
#include<SoftwareSerial.h>
#include <defines.h>

byte* rfid(byte& idSize) {
    // 確認是否有新卡片
    if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
      byte *id = mfrc522.uid.uidByte;   // 取得卡片的UID
      idSize = mfrc522.uid.size;   // 取得UID的長度

      //Serial.print("PICC type: ");      // 顯示卡片類型
      // 根據卡片回應的SAK值（mfrc522.uid.sak）判斷卡片類型
      //MFRC522::PICC_Type piccType = mfrc522.PICC_GetType(mfrc522.uid.sak);
      //Serial.println(mfrc522.PICC_GetTypeName(piccType));
      //#ifdef DEBUG
      Serial.print("UID Size: ");       // 顯示卡片的UID長度值
      Serial.println(idSize);   
      for (byte i = 0; i < idSize; i++) {  // 逐一顯示UID碼
        Serial.print("id[");
        Serial.print(i);
        Serial.print("]: ");
        Serial.println(id[i], HEX);       // 以16進位顯示UID值  

      }
      Serial.println();
      //#endif
      mfrc522.PICC_HaltA();  // 讓卡片進入停止模式
      return id;
    }
    return 0;
}

void send_byte(byte *id, byte& idSize){
    for (byte i = 0; i < idSize; i++){
        BT.write(id[i]);
    }

    Serial.print("Sent id: ");
    for (byte i = 0; i < idSize; i++){
        Serial.print(id[i], HEX);
    }
    Serial.println();
}
