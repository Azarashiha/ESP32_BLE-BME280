import sys
import datetime
import serial
import datetime
import csv
import time
import split
import json
import re
#シリアルポートは閉じてください。受信できません。
ser = serial.Serial('COM3',115200, timeout=1) #ポートの情報を記入


while(True):
    try:
        time.sleep(1)
        data=ser.read_all()
        #print(data)
        value = ser.readline().decode('utf-8')
        print("-"*30)
        print(value)
        
        date = datetime.datetime.now().strftime('%Y-%m-%d %H:%M:%S')
        print(date,value)
        #post=value.find('Advertised')

        value=value.replace('\r\n',"")
        value=(value.split(","))
        print(value)
        a={
        "temp": value[0],
        "humi": value[1],
        "press": value[2],
        "len": value[3]
        }
        data = json.dumps(a, ensure_ascii=True, indent=4, sort_keys=True, separators=(',',':'))

        
        file = open('data/test.json', 'w')
        file.write(data)
        file.close()
    except:
        print('a')
