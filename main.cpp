#include "mbed.h"

DigitalIn  Switch(SW3);
DigitalOut redLED(LED1);
DigitalOut greenLED(LED2);
AnalogOut Aout(DAC0_OUT);
Serial pc( USBTX, USBRX );
AnalogIn Ain(A0);

float ADCdata[1000];
int sample = 1000;
int i;

BusOut display(D6, D7, D9, D10, D11, D5, D4, D8);
char table[10] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F};

int main(){
  for(i = 0;i <= sample;i++){
        ADCdata[i] = Ain;
        wait(1./sample);
    }
  for(i = 0;i <=sample;i++){
        pc.printf("%1.3f\r\n",ADCdata[i]);
        wait(1./sample);
    }
  
  float f = 0;
  for(i = 1;i <=sample;i++){
        if(ADCdata[i]>ADCdata[i-1] && ADCdata[i]>ADCdata[i+1]){
            f++;
        }    
    }
  
  while(1){
    
    for(i = 0;i <=sample;i++){
        Aout = 0.5 + 0.5*sin(i*f*3.14159/sample/4);
    }
    
    int f1 = f/100;
    int f2 = (f/10)-(f1*10);
    int f3 = f-(f1*100)-(f2*10);
    int f3dot = table[f3] + 0x80;

    if( Switch == 1 ){
      greenLED = 0;
      redLED = 1;
      display = 0000;
    }
    else{
      redLED = 0;
      greenLED = 1;
      while(1){
      display = table[f1]; //7-segments
      wait(1);
      display = table[f2]; //7-segments
      wait(1);
      display = f3dot; //7-segments
      wait(1);
      if( Switch == 1 ){
          break;
      }
      }
    }
  }
}