#include "mbed.h"
#include <math.h>

Serial pc( USBTX, USBRX );
AnalogOut Aout(DAC0_OUT);
AnalogIn Ain(A0);
BusOut display(D6, D7, D9, D10, D11, D5, D4, D8);
DigitalIn  Switch(SW3);
DigitalOut redLED(LED1);
DigitalOut greenLED(LED2);

char table[10] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F};
char last_table[10] ={ 0xBF, 0x86, 0xDB, 0xCF, 0xE6, 0xED, 0xFD, 0x87, 0xFF, 0xEF};
int sample = 1000;
float ADCdata[1000];
int freq = 0;
const double PI = 3.14159;

int main(){
    int i, d[4];
    
    // STEP1 : INPUT
    for (i = 0; i < sample; i++){
        ADCdata[i] = Ain;
        wait(1./sample);
    }
    
    // STEP2 : FREQUENCY CALCULATE
    for (i = 2; i < sample-2; i++) {
        if ( ADCdata[i]<=0.5 && ADCdata[i - 1] >= ADCdata[i] && ADCdata[i + 1] >= ADCdata[i]) {
            freq ++;
        }
    }
    // t = N * 1/100 = 1000 * 1/1000, N = 1 / (1 / 100) = 100
    
    // STEP3 : OUTPUT data
    for (i = 0; i < sample; i++){
        pc.printf("%1.3f\r\n", ADCdata[i]);
        wait(0.1);
    }

    // STEP4: SHOW ON DISPLAYER & PICOSCOPE
    while(1) {

        if (Switch == 0) {
            greenLED = 1; 
            redLED = 0;
            
            d[0] = freq % 10;
            d[1] = (freq - d[0])/ 10 % 10;
            d[2] = (freq - d[0] - d[1]) /100 % 10;
            display = table[d[2]]; 
            wait(1);
            display = table[d[1]]; 
            wait(1);
            display = last_table[d[0]];
            wait(1);
            
        }
        else {
            redLED = 1;
            greenLED = 0;
            display = 0x0;

        }
        for (i = 0; i < 40; i++) {
            Aout = 0.5 + 0.5 * sin(2*PI * i/40);
            wait(1. / 40. / freq);
        }
    }
}
