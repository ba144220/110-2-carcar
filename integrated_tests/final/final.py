import serial
from time import sleep
import sys
import numpy as np
import requests
import socketio

IDLE = 0
UID = 1
state = IDLE

COM_PORT = '/dev/tty.HC-39-SPPDev'  # 請自行修改序列埠名稱
BAUD_RATES = 9600

ser = serial.Serial(COM_PORT, BAUD_RATES)





# ================================================================
# Scoreboard
#   add_UID(UID_str)
#     UID_str : UID string ("0x" excluded)
#   getCurrentScore()
#     return current score (int)
# ================================================================


class Scoreboard:
    '''
    The Scoreboard class connects to the server socket and enables updating score by sending UID.
    '''
    def __init__(self, filepath, teamName, host="http://192.168.50.163:3000"):

        # INIT VARIABLES

        self.totalScore = 0
        self.team = teamName
        self.game = 0
        self.ip = host
        #self.ip = 'https://creative.ntuee.org'

        print(f"{self.team} wants to play!")
        print(f"connecting to server......{self.ip}")

        # CONNECT WITH SERVER SOCKET

        # create socket.io instance and connect to server
        self.socket = Socket(self.ip)

        # GET INFORMATION FROM SERVER
        
        res = requests.get(self.ip + '/game_status')
        if res.status_code == 404:
            raise ConnectionError('Response[404] from /game_status')
        playing_team = res.json()['current_team']
        if playing_team is None:
            print("Game is starting.....")
            self.socket.start_game({ 
                'gamemode': self.game, 
                'team': self.team })
        elif playing_team != teamName:
            self.socket.disconnect()
            raise ConnectionError(f"{res.json()['current_team']} is current playing.\nPlease wait {res.json()['time_remain']} seconds for retry.")          

    def add_UID(self, UID_str):
        '''Send {UID_str} to server to update score. Returns nothing.'''
        UID_type = type(UID_str)
        UID_len = len(UID_str)
        print("In add_UID, UID = {}".format(UID_str))
        if(UID_type != type('')):
            print("    UID type error! (Your type: {})".format(UID_type.__name__))
        if(UID_len != 8):
            print("    UID length error! (Your length: {})".format(UID_len))
        self.socket.add_UID({'uid_str': UID_str})

    def getCurrentScore(self):
        try:
            r = requests.get(self.ip+"/current_score")
            return r.json()['current_score']
        except:
            print("Failed to fetch current score")
            return None
        # return int(self.totalScore)


# ================================================================
# Socket interface
#   mySocket = Socket(ip)
# ================================================================


class Socket(socketio.ClientNamespace):
    sio = socketio.Client()

    def __init__(self, ip):
        #***********************#
        # pass in the namespace '/'
        # https://python-socketio.readthedocs.io/en/latest/client.html#class-based-namespaces
        #***********************#
        super().__init__('/')
        self.ip = ip
        try:
            self.sio.connect(self.ip)
        except Exception as e:
            print('Failed to connect to server, ', e)
            sys.exit(1)
        self.sio.register_namespace(self)

    def on_connect(self):
        print("connected")
    
    def on_invalid_mode(self):
        print("Error:invalid gamemode!!")

    def on_game_end(self, data=None):
        print("game_end")
        self.disconnect()

    def on_game_started(self, data):
        print("Game started!")
        print("Playing game as\nTeam: {}\nMode: {}".format(
            data['current_team'], data['gamemode']))
        print("Please checkout {} for more information.".format(self.ip))

    def on_UID_added(self, message):
        print(message)

    def start_game(self, gamenum):
        self.emit("start_game", gamenum)

    def add_UID(self, UID_str):
        self.emit("add_UID", UID_str)
         
    #secret backdoor for TA
    def stop_game(self):
        self.emit("stop_game")


if __name__ == '__main__':

    

    # bluetooth init
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
                myScoreboard = Scoreboard(None,'test 01/15 16:25',"http://192.168.50.163:3000")
                sleep(3)

            while ser.in_waiting:
                response = ser.readline().decode()  # 接收回應訊息並解碼
                print('Arduino回應：', response)

    while state == UID:
        while ser.in_waiting:

            response = ser.readline().decode()  # 接收回應訊息並解碼
            print('UID回應：', response[0:8])    

            myScoreboard.add_UID(response[0:8]) 
            print('目前分數： ',myScoreboard.getCurrentScore())


















