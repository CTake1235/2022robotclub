#include "mbed.h"
#include "sonMD.h"
#include "PS3.h"
#include "QEI.h"

#define PowerWindMD 0x54//change me

UnbufferedSerial raspi(D0,D1,9600);
UnbufferedSerial send_sub(A0,A1,9600);

I2C              i2c(D14,D15);

sonMD            rightleg(D5,D6,0.00015);
sonMD            leftleg(D8,D9,0.00015); 
sonMD            right_reload(D10,D11,0.00015);
sonMD            left_reload(D12,D13,0.00015);

DigitalIn        limitswitch_up(D2);
DigitalIn        limitswitch_down(D3);

void  wait_ms(int t);
void send(char add, char dat);//for opening wing

int  maru,batu,sankaku;

int main(){
    raspi.format(8, BufferedSerial::None, 1);
    int res = 0;
    static char data;
    int state = 0;

    double shot_power = 0.6;

    bool PowerWindUp = 0;
    bool PowerWindDown = 0;
    while (true) {
        raspi.read(&data,9);
        if(res == 1){
            switch(int(data)){
                case 5://R1,open wing
                    PowerWindUp = 1;
                    break;
                case 6://R2,close wing
                    PowerWindDown = 1;
                    break;
                case 9://maru,inject
                    rightleg.move_p1(0.02,shot_power);
                    leftleg.move_p1(0.02,shot_power);
                    send_sub.write(&data,9);
                    break;
                default:

                    break;
            }
        }

        if(PowerWindUp == 1 && limitswitch_up == 0){//open wing
            send(PowerWindUp,0xff);
        }
        else if(PowerWindDown == 1 && limitswitch_down == 0){//close wing
            send(PowerWindDown,0x00);
        }
        else{
            send(PowerWindMD,0x80);
        }
    }
}


void wait_ms(int t){
    wait_us(t * 1000);
}

void send(char add ,char dat){
    i2c.start();
    i2c.write(add);
    i2c.write(dat);
    i2c.stop();
    wait_us(25000);
}