from goertzel import goertzel

class dtmf_decoder():

    lowfreqs = [697, 770, 852, 941]
    hifreqs = [1209, 1336, 1477]
    digits = [
        [1,2,3],
        [4,5,6],
        [7,8,9],
        ['*',0,'#']
    ]

    def __init__(self, N, fs, threshold):
        self.N = N
        self.sample_rate = fs
        self.threshold = threshold

        self.l = []
        self.h = []
        for f in self.lowfreqs:
            x = goertzel(f,N,fs)
            self.l.append(x)
        for f in self.hifreqs:
            x = goertzel(f,N,fs)
            self.h.append(x)

    def update(self,sample):
        for x in self.l+self.h:
            n = x.update(sample)

            if n == self.N:
                x.calculateMagnitude()
                x.reset()

        if n == self.N:
            return True
        else:
            return False

    def getDigit(self):
        row,maxl = max(enumerate(self.l), key=lambda x: x[1].magnitude)
        col,maxh = max(enumerate(self.h), key=lambda x: x[1].magnitude)

        #print row, int(maxl.magnitude)
        #print col, int(maxh.magnitude)

        if maxl.magnitude > self.threshold and maxh.magnitude > self.threshold:
            return str(self.digits[row][col])
        else:
            return False
