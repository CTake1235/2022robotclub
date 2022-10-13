#include "mbed.h"
#include "sonMD.h"
#include <cstdio>
#include "time.h"

#define wingPWadd 0x58

UnbufferedSerial raspi(PA_0,PA_1,9600);

I2C              i2c(PB_9,PB_8);

sonMD            inside(PA_11,PB_2,0.00015);
sonMD            outside(PA_10,PB_5,0.00015);
sonMD            inside_reload(PA_9,PA_8,0.00015);
sonMD            outside_reload(PA_6,PB_6,0.00015);    

DigitalIn        sitalimitswitch(PC_0);

void send(char add, char dat);

int main(){
    raspi.format(8,BufferedSerial::None,1);
    clock_t start,end;
    int res,time;
    static char data; 
    float shotpower = 0.9;
    float reloadpower = 0.5;
    bool state = false;
    while (true) {
        printf("shot:%lf,reload:%lf\n",shotpower,reloadpower);
        res = raspi.read(&data,4);
        if(res == 1){
            switch (int(data)){
                case 1://increase shotpower
                    start = clock();
                    shotpower += 0.1;
                    printf("shotpower:%lf\n",shotpower);
                    while(true){
                        end = clock();
                        time = end - start;
                        if(double(time)*60/CLOCKS_PER_SEC >= 0.3){
                            break;
                        }
                    }
                    break;
                case 2://decrease shotpower
                    start = clock();
                    shotpower -= 0.1;
                    printf("shotpower:%lf\n",shotpower);
                    while(true){
                        end = clock();
                        time = end - start;
                        if(double(time)*60/CLOCKS_PER_SEC >= 0.3){
                            break;
                        }
                    }
                    break;
                case 3://increase reloadpower
                    start = clock();
                    reloadpower += 0.1;
                    printf("reload:%lf\n",reloadpower);
                    while(true){
                        end = clock();
                        time = end - start;
                        if(double(time)*60/CLOCKS_PER_SEC >= 0.3){
                            break;
                        }
                    }
                    break;
                case 4://decrease reloadpower
                    start = clock();
                    reloadpower -= 0.1;
                    printf("reload:%lf",reloadpower);
                    while(true){
                        end = clock();
                        time = end - start;
                        if(double(time)*60/CLOCKS_PER_SEC >= 0.3){
                            break;
                        }
                    }
                    break;
                case 7://L1,open wing
                    send(wingPWadd,0xff);
                    break;
                case 8://L2,close wing
                    if(sitalimitswitch == 0){
                        send(wingPWadd,0x00);
                    }
                    break;
                case 9://maru,inject
                    state = true;
                    break;
                case 10://sikaku,reload back
                    inside_reload.move_p1(0.02,0.5);
                    outside_reload.move_p1(0.02,0.5);
                    break;
                case 11://sankaku,reload go
                    inside_reload.move_p2(0.02,0.5);
                    outside_reload.move_p2(0.02,0.5);
                    break;
                case 12:
                    state = false;
                    break;
                default:
                    inside.stop();
                    outside.stop();
                    inside_reload.stop();
                    outside_reload.stop();
                    send(wingPWadd,0x80);
                    break;
            }
            if(state == true){//for injection
                inside.move_p1(0.02,shotpower);
                outside.move_p1(0.02,shotpower);
            }
        }
        else{
            inside.stop();
            outside.stop();
            inside_reload.stop();
            outside_reload.stop();
            send(wingPWadd,0x80); 
        }
    }
}

void send(char add, char dat){
    i2c.start();
    i2c.write(add);
    i2c.write(dat);
    i2c.stop();
    wait_us(25000);
}