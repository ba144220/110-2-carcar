import serial
from time import sleep
import sys
IDLE = 0
UID = 1
state = IDLE

COM_PORT = '/dev/tty.HC-39-SPPDev'  # 請自行修改序列埠名稱
BAUD_RATES = 9600
ser = serial.Serial(COM_PORT, BAUD_RATES)

try:
    ser.write("hi".encode('utf-8'))
    sleep(0.5)
    while ser.in_waiting:
        response = ser.readline().decode()  # 接收回應訊息並解碼

    while state == IDLE:
        # 接收用戶的輸入值並轉成小寫
            command = input('輸入指令：').lower()
            ser.write(command.encode('utf-8'))
            sleep(0.5)
            if command == "start":
                state = UID
                print("Change state to UID")
  
            while ser.in_waiting:
                response = ser.readline().decode()  # 接收回應訊息並解碼
                print('Arduino回應：', response)

    while state == UID:
        while ser.in_waiting:
            response = ser.readline().decode()  # 接收回應訊息並解碼
            print('UID回應：', response)        

except KeyboardInterrupt:
    ser.close()
    print('再見！')