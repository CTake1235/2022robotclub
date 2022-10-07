#include "mbed.h"
#include "sonMD.h"
#define wingPWadd 0x58

UnbufferedSerial raspi(PA_0,PA_1,9600);
sonMD            inside(PA_6,PB_6,0.00015);
sonMD            outside(PA_9,PA_8,0.00015);
sonMD            inside_reload(PB_9,PB_8,0.00015);
sonMD            outside_reload(PC_9,PC_8,0.00015);
I2C              i2c(PC_6,PC_7);
DigitalIn        uelimitswitch(PC_1);
DigitalIn        sitalimitswitch(PC_0);

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
    wait_us(15000);
}