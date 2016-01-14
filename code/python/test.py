from dtmf import dtmf_decoder

N = 160
sample_rate = 5000
threshold = 10000


with open('test.txt') as f:
    data = [float(x)*1024 for x in f]

dtmf = dtmf_decoder(N, sample_rate, threshold)

for x in data:
    ready = dtmf.update(x)

    if(ready):
        result = dtmf.getDigit()
        if(result):
            print result,
