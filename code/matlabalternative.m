[y, fs] = audioread('random.wav');
plot(y);

tsamp = 0.01;
t = linspace(0,tsamp,tsamp*fs+1);
M = length(t);
N = floor(length(y)/M);
ys = reshape(y(1:(N*M)),M,N); %prints the waveform of the .wav file and  
 
f_low = [697 770 852];
f_high = [1209 1336 1477];
omega = 2*pi*f_low';
w = exp(1j*omega*t);
yLow = abs(w*ys)/M;
plot(yLow');

omega = 2*pi*f_high';
w = exp(1j*omega*t);
yHigh = abs(w*ys)/M;
plot(yHigh');

ndx = 1:3;
y1 = ndx*(yLow>0.07);
subplot(2,1,1);
plot(y1);
ylabel('low freq index');
subplot(2,1,2);
y2 = ndx*(yHigh>0.07);
plot(y2);
ylabel('high freq index');

% look for zero to positive differences
% and then a few samples later
ndx = find(diff(y1)>0);
n1 = y1(ndx+2);
n2 = y2(ndx+2);
[n1' n2'];

key = ['147'; '258'; '369'];

for k=1:length(n1)
    pn(k) =key(n2(k),n1(k));
end
disp('phone number:');
disp(pn);