import math

class goertzel():

    def __init__(self, freq, N, fs):
        self.freq = freq
        self.N = N
        self.sample_rate = fs

        self.computeCoefficient()
        self.reset()


    def computeCoefficient(self):
        k = round((self.N*self.freq)/self.sample_rate)
        w = (2*math.pi*k)/self.N
        self.coeff = 2*math.cos(w)

    def update(self,sample):
        self.Q[0] = self.coeff * self.Q[1] - self.Q[2] + sample
        self.Q[2] = self.Q[1]
        self.Q[1] = self.Q[0]

        self.n += 1

        return self.n

    def calculateMagnitude(self):
        self.magnitude = self.Q[1]**2 + self.Q[2]**2 - (self.Q[1]*self.Q[2]*self.coeff)

        return self.magnitude

    def reset(self):
        self.n = 0
        self.Q = [0,0,0]
