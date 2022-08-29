#include "mbed.h"
#include "PS3.h"
#include "mbed_wait_api.h"
#include "cmath"
#define MDadd 0x16
void getdata(void);
PS3 ps3(A0,A1);
I2C i2c(D14,D15);
int send(char add,char dat);
DigitalOut sig(D13);//緊急停止
int maru,select,start;//ボタン割り当て
bool spin=0;
int main(){
    sig=0;
    while(true){
        getdata();
        if(select==1){
            sig=1;
        }else if(start==1){
            sig=0;
        }else if(maru==1 && spin==0){
            spin=1;
            send(MDadd,0x95);
            wait_us(500000);
        }else if(maru==1 && spin==1){
            spin=0;
            send(MDadd,0x80);
            wait_us(500000);
        }
    }
}
void getdata(void){
    maru=ps3.getButtonState(PS3::maru);
    select=ps3.getSELECTState();
    start=ps3.getSTARTState();
}
int send(char add,char dat){
    i2c.start();
    i2c.write(add);
    i2c.write(dat);
    i2c.stop();
    wait_us(25000);
    return 0;
}

