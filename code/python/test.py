from dtmf import dtmf_decoder

N = 150
sample_rate = 5000
threshold = 1000000
sample_max = 1024
sample_threshold = 180

lower = (sample_max/2)-sample_threshold
upper = (sample_max/2)+sample_threshold

with open('test.txt') as f:
    data = [float(x)*1024/1.5 for x in f]


dtmf = dtmf_decoder(N, sample_rate, threshold)

cnt = 0
newTone = False
number = ""

for t,x in enumerate(data):

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
        break
