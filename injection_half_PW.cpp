#include "mbed.h"
#include "sonMD.h"
#define wingPWadd 0x58
#define shotpower 0.5

UnbufferedSerial raspi(PA_0,PA_1,9600);
sonMD            inside(PA_6,PB_6,0.00015);
sonMD            outside(PA_9,PA_8,0.00015);
sonMD            inside_reload(PB_9,PB_8,0.00015);
sonMD            outside_reload(PC_9,PC_8,0.00015);
sonMD            leg(PA_11,PB_2,0.00015);
sonMD            leg_reload(PA_10,PC_8,0.00015);
I2C              i2c(PC_7,PC_6);
DigitalIn        uelimitswitch(PC_1);
DigitalIn        sitalimitswitch(PC_0);

void send(char add, char dat);

int main(){
    raspi.format(8,BufferedSerial::None,1);
    int res;
    static char data;
    while (true) {
        res = raspi.read(&data,4);
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
                    inside.move_p1(0.02,shotpower);
                    outside.move_p1(0.02,shotpower);
                    leg.move_p1(0.02,shotpower);
                    break;
                case 10://sikaku,reload back
                    inside_reload.move_p2(0.02,0.2);
                    outside_reload.move_p2(0.02,0.2);
                    leg_reload.move_p2(0.02,0.2);
                    break;
                case 11://sankaku,reload go
                    inside_reload.move_p1(0.02,0.1);
                    outside_reload.move_p1(0.02,0.1);
                    leg_reload.move_p1(0.02,0.1);
                    break;
                    
                default:
                    inside.stop();
                    outside.stop();
                    leg.stop();
                    inside_reload.stop();
                    outside_reload.stop();
                    leg_reload.stop();
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