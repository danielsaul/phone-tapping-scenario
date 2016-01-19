import serial
from twilio.rest import TwilioRestClient
from dtmf import dtmf_decoder

#account_sid = ""
#auth_token = ""
#client = TwilioRestClient(account_sid, auth_token)

ser = serial.Serial('/dev/tty.usbmodem1421',921600,timeout=2)

N = 140
sample_rate = 4000
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

    try:
        x = int(s)
    except ValueError:
        continue

    if not newTone:

        if lower < x < upper:
            cnt += 1
        else:
            if cnt > 10:
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
        #numbercc = "+44"+number[1:]
        #call = client.calls.create(to=numbercc, from_="+441202835206", url="http://dansaul.co.uk/twilio.xml", method="GET")
        #msg = client.messages.create(to=numbercc, from_="+441202835206", body="With love from Team 3 <3")
        #print call.sid
