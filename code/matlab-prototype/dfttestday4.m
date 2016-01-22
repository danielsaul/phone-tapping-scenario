%DTMF frequency
lfg = [697 770 852 941]; % Low frequency group
hfg = [1209 1336 1477];  % High frequency group
f   = [reshape(ones(3,1)*lfg,1,12); repmat(hfg,1,4)];% Generate 12 frequency pairs
dtmf = [ ['1', '2', '3'];
         ['4', '5', '6'];
         ['7', '8', '9'];
         ['*', '0', '#'];
       ];
symbols = {'1','2','3','4','5','6','7','8','9','*','0','#'};
estim_f=[
         ,
         775,
         850,
         950,
         1225,
         1350,
         1475];
%Goertzel Algorithm
N1=(length(Y)-224-69)./11;
Fs=5000;
original_f =[lfg(:);hfg(:)]; % Original frequencies
%first number
N1r=200;
N0=224;
t=N0:N1r+N0;   
Y1=Y(1,t);
figure
plot(t,Y1)
title('First Tone')
dft_Y1=fft(Y1);
hi=abs(dft_Y1);
f1=0:Fs/N1r:Fs;
figure
stem(f1,hi)
hgca = gca;
hgca.XTick = estim_f;
hgca.XTickLabel = estim_f;
xlim([650 1550]);
ylabel('DFT Magnitude');
xlabel('Frequency (Hz)')
toneChoice= DTMFdecoder(hi);
toneChoice1=toneChoice;
title(['Symbol " ', symbols{toneChoice1},' ": [',num2str(f(1,toneChoice1)),',',num2str(f(2,toneChoice1)),']'])
display( ['The first digit of the phone number is :', symbols{toneChoice1}])
%second number
N0=N1r+N0;
t=N0:N1r+N0;   
Y1=Y(1,t);
figure
plot(t,Y1)
title('Second Tone')
dft_Y1=fft(Y1);
hi=abs(dft_Y1);
f1=0:Fs/N1r:Fs;
figure
stem(f1,hi)
hgca = gca;
hgca.XTick = estim_f;
hgca.XTickLabel = estim_f;
xlim([650 1550]);
ylabel('DFT Magnitude');
xlabel('Frequency (Hz)')
toneChoice= DTMFdecoder(hi);
toneChoice2=toneChoice;
title(['Symbol " ', symbols{toneChoice2},' ": [',num2str(f(1,toneChoice2)),',',num2str(f(2,toneChoice2)),']'])
display( ['The second digit of the phone number is :', symbols{toneChoice2}])
%third number
N0=N1r+N0;
t=N0:N1r+N0;   
Y1=Y(1,t);
figure
plot(t,Y1)
title('Third Tone')
dft_Y1=fft(Y1);
hi=abs(dft_Y1);
f1=0:Fs/N1r:Fs;
figure
stem(f1,hi)
hgca = gca;
hgca.XTick = estim_f;
hgca.XTickLabel = estim_f;
xlim([650 1550]);
ylabel('DFT Magnitude');
xlabel('Frequency (Hz)')
toneChoice= DTMFdecoder(hi);
toneChoice3=toneChoice;
title(['Symbol " ', symbols{toneChoice3},' ": [',num2str(f(1,toneChoice3)),',',num2str(f(2,toneChoice3)),']'])
display( ['The third digit of the phone number is :', symbols{toneChoice3}])
%fourth number
N0=N1r+N0;
t=N0:N1r+N0;   
Y1=Y(1,t);
figure
plot(t,Y1)
title('Fourth Tone')
dft_Y1=fft(Y1);
hi=abs(dft_Y1);
f1=0:Fs/N1r:Fs;
figure
stem(f1,hi)
hgca = gca;
hgca.XTick = estim_f;
hgca.XTickLabel = estim_f;
xlim([650 1550]);
ylabel('DFT Magnitude');
xlabel('Frequency (Hz)')
toneChoice= DTMFdecoder(hi);
toneChoice4=toneChoice;
title(['Symbol " ', symbols{toneChoice4},' ": [',num2str(f(1,toneChoice4)),',',num2str(f(2,toneChoice4)),']'])
display( ['The fourth digit of the phone number is :', symbols{toneChoice4}])
%fifth number
N0=N1r+N0;
t=N0:N1r+N0;   
Y1=Y(1,t);
figure
plot(t,Y1)
title('Fifth Tone')
dft_Y1=fft(Y1);
hi=abs(dft_Y1);
f1=0:Fs/N1r:Fs;
figure
stem(f1,hi)
hgca = gca;
hgca.XTick = estim_f;
hgca.XTickLabel = estim_f;
xlim([650 1550]);
ylabel('DFT Magnitude');
xlabel('Frequency (Hz)')
toneChoice= DTMFdecoder(hi);
toneChoice5=toneChoice;
title(['Symbol " ', symbols{toneChoice5},' ": [',num2str(f(1,toneChoice5)),',',num2str(f(2,toneChoice5)),']'])
display( ['The fifth digit of the phone number is :', symbols{toneChoice5}])
%sixth number
N0=N1r+N0;
t=N0:N1r+N0;   
Y1=Y(1,t);
figure
plot(t,Y1)
title('Sixth Tone')
dft_Y1=fft(Y1);
hi=abs(dft_Y1);
f1=0:Fs/N1r:Fs;
figure
stem(f1,hi)
hgca = gca;
hgca.XTick = estim_f;
hgca.XTickLabel = estim_f;
xlim([650 1550]);
ylabel('DFT Magnitude');
xlabel('Frequency (Hz)')
toneChoice= DTMFdecoder(hi);
toneChoice6=toneChoice;
title(['Symbol " ', symbols{toneChoice6},' ": [',num2str(f(1,toneChoice6)),',',num2str(f(2,toneChoice6)),']'])
display( ['The sixth digit of the phone number is :', symbols{toneChoice6}])
%seventh number
N0=N1r+N0;
t=N0:N1r+N0;   
Y1=Y(1,t);
figure
plot(t,Y1)
title('Seventh Tone')
dft_Y1=fft(Y1);
hi=abs(dft_Y1);
f1=0:Fs/N1r:Fs;
figure
stem(f1,hi)
hgca = gca;
hgca.XTick = estim_f;
hgca.XTickLabel = estim_f;
xlim([650 1550]);
ylabel('DFT Magnitude');
xlabel('Frequency (Hz)')
toneChoice= DTMFdecoder(hi);
toneChoice7=toneChoice;
title(['Symbol " ', symbols{toneChoice7},' ": [',num2str(f(1,toneChoice7)),',',num2str(f(2,toneChoice7)),']'])
display( ['The seventh digit of the phone number is :', symbols{toneChoice7}])
%eighth number
N0=N1r+N0;
t=N0:N1r+N0;   
Y1=Y(1,t);
figure
plot(t,Y1)
title('Eighth Tone')
dft_Y1=fft(Y1);
hi=abs(dft_Y1);
f1=0:Fs/N1r:Fs;
figure
stem(f1,hi)
hgca = gca;
hgca.XTick = estim_f;
hgca.XTickLabel = estim_f;
xlim([650 1550]);
ylabel('DFT Magnitude');
xlabel('Frequency (Hz)')
toneChoice= DTMFdecoder(hi);
toneChoice8=toneChoice;
title(['Symbol " ', symbols{toneChoice8},' ": [',num2str(f(1,toneChoice8)),',',num2str(f(2,toneChoice8)),']'])
display( ['The eighth digit of the phone number is :', symbols{toneChoice8}])
%ninth number
N0=N1r+N0;
t=N0:N1r+N0;   
Y1=Y(1,t);
figure
plot(t,Y1)
title('Ninth Tone')
dft_Y1=fft(Y1);
hi=abs(dft_Y1);
f1=0:Fs/N1r:Fs;
figure
stem(f1,hi)
hgca = gca;
hgca.XTick = estim_f;
hgca.XTickLabel = estim_f;
xlim([650 1550]);
ylabel('DFT Magnitude');
xlabel('Frequency (Hz)')
toneChoice= DTMFdecoder(hi);
toneChoice9=toneChoice;
title(['Symbol " ', symbols{toneChoice9},' ": [',num2str(f(1,toneChoice9)),',',num2str(f(2,toneChoice9)),']'])
display( ['The ninth digit of the phone number is :', symbols{toneChoice9}])
%tenth number
N0=N1r+N0;
t=N0:N1r+N0;   
Y1=Y(1,t);
figure
plot(t,Y1)
title('Tenth Tone')
dft_Y1=fft(Y1);
hi=abs(dft_Y1);
f1=0:Fs/N1r:Fs;
figure
stem(f1,hi)
hgca = gca;
hgca.XTick = estim_f;
hgca.XTickLabel = estim_f;
xlim([650 1550]);
ylabel('DFT Magnitude');
xlabel('Frequency (Hz)')
toneChoice= DTMFdecoder(hi);
toneChoice10=toneChoice;
title(['Symbol " ', symbols{toneChoice10},' ": [',num2str(f(1,toneChoice10)),',',num2str(f(2,toneChoice10)),']'])
display( ['The tenth digit of the phone number is :', symbols{toneChoice10}])
%eleventh number
N0=N1r+N0;
t=N0:N1r+N0;   
Y1=Y(1,t);
figure
plot(t,Y1)
title('Eleventh Tone')
dft_Y1=fft(Y1);
hi=abs(dft_Y1);
f1=0:Fs/N1r:Fs;
figure
stem(f1,hi)
hgca = gca;
hgca.XTick = estim_f;
hgca.XTickLabel = estim_f;
xlim([650 1550]);
ylabel('DFT Magnitude');
xlabel('Frequency (Hz)')
toneChoice= DTMFdecoder(hi);
toneChoice11=toneChoice;
title(['Symbol " ', symbols{toneChoice11},' ": [',num2str(f(1,toneChoice11)),',',num2str(f(2,toneChoice11)),']'])
display( ['The eleventh digit of the phone number is :', symbols{toneChoice11}])
display( ['The telephone number is :',symbols{toneChoice1},symbols{toneChoice2},symbols{toneChoice3},symbols{toneChoice4},symbols{toneChoice5},symbols{toneChoice6},symbols{toneChoice7},symbols{toneChoice8},symbols{toneChoice9},symbols{toneChoice10},symbols{toneChoice11}])
