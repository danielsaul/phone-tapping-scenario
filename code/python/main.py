import serial
from dtmf import dtmf_decoder

ser = serial.Serial('/dev/ttyUSB0',19200,timeout=2)

N = 150
sample_rate = 8000
threshold = 1000000
sample_max = 1024
sample_threshold = 180

lower = (sample_max/2)-sample_threshold
upper = (sample_max/2)+sample_threshold


dtmf = dtmf_decoder(N, sample_rate, threshold)

cnt = 0
newTone = False
number = ""

while(1):

    s = ser.readline()
    x = int(s)

    if not newTone:

        if lower < x < upper:
            cnt += 1
        else:
            if cnt > 20:
                newTone = True
            if cnt > 400:
                number = ""
            cnt = 0

    if not newTone: continue

    ready = dtmf.update(x)

    if(ready):
        result = dtmf.getDigit()
        if(result):
            #print "--> " + result
            number += result
        newTone = False

    if len(number) == 11:
        print number
        number = ""
