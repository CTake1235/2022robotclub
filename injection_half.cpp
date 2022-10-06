#include "mbed.h"
#include "sonMD.h"
#define wingPWadd 0x58

sonMD            inside(D3,NC,0.00015);
sonMD            outside(D4,NC,0.00015);
sonMD            inside_reload(D5,NC,0.00015);
sonMD            outside_reload(D6,NC,0.00015);
UnbufferedSerial raspi(D0,D1,9600);
I2C              i2c(D14,D15);
DigitalIn        uelimitswitch(D5);
DigitalIn        sitalimitswitch(D6);

void send(char add, char dat);

int main(){
    int res;
    static char data;
    while (true) {
        res = raspi.read(&data,9);
        if(res == 1){
            switch (int(data)){
                case 7://L1,open wing
                    if(uelimitswitch == 0){
                        send(wingPWadd,0xff);
                    }
                    break;
                case 8://L2,close wing
                    if(sitalimitswitch == 0){
                        send(wingPWadd,0x00);
                    }
                    break;
                case 9://maru,inject
                    inside.move_p1(0.02,0.7);
                    outside.move_p1(0.02,0.7);
                    break;
                case 10://sikaku,reload back
                    inside.move_p2(0.02,0.1);
                    outside.move_p2(0.02,0.1);
                    break;
                case 11://sankaku,reload go
                    inside_reload.move_p1(0.02,0.1);
                    outside_reload.move_p1(0.02,0.1);
                    break;
                default:
                    inside.stop();
                    outside.stop();
                    inside_reload.stop();
                    outside_reload.stop();
                    send(wingPWadd,0x80);
                    break;
            }
        }
    }
}

void send(char add, char dat){
    i2c.start();
    i2c.write(add);
    i2c.write(dat);
    i2c.stop();
    wait_us(1500);
}