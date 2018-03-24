#include "mbed.h"
#include <string>
#include <mpr121.h>
#include "uLCD_4DGL.h"

DigitalOut led1(LED1);
DigitalOut led2(LED2);
DigitalOut led3(LED3);
DigitalOut led4(LED4);

Serial pc(USBTX, USBRX);

uLCD_4DGL uLCD(p28, p27, p30); // serial tx, serial rx, reset pin;

int count = 0;

InterruptIn interrupt(p26);
// Setup the i2c bus on pins 9 and 10
I2C i2c(p9, p10);
// Setup the Mpr121){
// constructor(i2c object, i2c address of the mpr121)
Mpr121 mpr121(&i2c, Mpr121::ADD_VSS);
 
//buffer to hold characters
string buffer = "";
string answer = "";

RawSerial  dev(p13,p14);

void dev_recv()
{
    while(dev.readable()) {
        uLCD.printf("%s \n", dev.getc());
    }
}

void pc_recv()
{
    for(int i = 0; i < answer.length(); i++){
        dev.putc(answer[i]);
        pc.putc(answer[i]);
    }
    dev.putc('\n');
    pc.putc('\n');
    pc.putc('\r');
}

void interpretLetter() {
    if(buffer == "12"){
        answer.append("A");
    }
    else if(buffer == "2111"){
        answer.append("B");
    }
    else if(buffer == "2121"){
        answer.append("C");
    }
    else if(buffer == "211"){
        answer.append("D");
    }
    else if(buffer == "1"){
        answer.append("E");
    }
    else if(buffer == "1121"){
        answer.append("F");
    }
    else if(buffer == "221"){
        answer.append("G");
    }
    else if(buffer == "1111"){
        answer.append("H");
    }
    else if(buffer == "11"){
        answer.append("I");
    }
    else if(buffer == "1222"){
        answer.append("J");
    }
    else if(buffer == "212"){
        answer.append("K");
    }
    else if(buffer == "1211"){
        answer.append("L");
    }
    else if(buffer == "22"){
        answer.append("M");
    }
    else if(buffer == "21"){
        answer.append("N");
    }
    else if(buffer == "222"){
        answer.append("O");
    }
    else if(buffer == "1221"){
        answer.append("P");
        }
    else if(buffer == "2212"){
        answer.append("Q");
        }
    else if(buffer == "121"){
        answer.append("R");
        }
    else if(buffer == "111"){
        answer.append("S");
        }
    else if(buffer == "2"){
        answer.append("T");
        }
    else if(buffer == "112"){
        answer.append("U");
        }
    else if(buffer == "1112"){
        answer.append("V");
        }
    else if(buffer == "122"){
        answer.append("W");
        }
    else if(buffer == "2112"){
        answer.append("X");
        }
    else if(buffer == "2122"){
        answer.append("Y");
        }
    else if(buffer == "2211"){
        answer.append("Z");
        }
}

// Key hit/release interrupt routine
void fallInterrupt() {
      count++;
      int key_code=0;
      int i=0;
      int value=mpr121.read(0x00);
      value +=mpr121.read(0x01)<<8;
      // LED demo mod
      i=0;
      // puts key number out to LEDs for demo
      for (i=0; i<12; i++) {
        if (((value>>i)&0x01)==1) key_code=i+1;
      }
      led4=key_code & 0x01;
      led3=(key_code>>1) & 0x01;
      led2=(key_code>>2) & 0x01;
      led1=(key_code>>3) & 0x01;
      if(count%2 == 1){
        if(key_code == 3){
            interpretLetter();
            buffer = "";
        }
        else if(key_code == 4){
            answer += ' ';
            buffer = "";
        }
        else if(key_code == 5){
            uLCD.printf("%s \n", answer.c_str());
            //pc.printf("%s \n\r", answer.c_str());
            pc_recv();
            answer = "";
        }
        else{
            char c = '0' + key_code;
            buffer += c;
            //pc.printf("%s", buffer.c_str());
        }
    }
}
 
int main() {
  interrupt.fall(&fallInterrupt);
  interrupt.mode(PullUp);
  dev.attach(&dev_recv, Serial::RxIrq);
  while (1) {}
}
