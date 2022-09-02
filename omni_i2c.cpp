//clockwとanticwはモータから見た（足回りの中心から見た）タイヤの回転方向を示している
//clockwは時計回り、anticwは反時計回り

#include "mbed.h"
#include"PS3.h"
#include"mbed_wait_api.h"
#include <stdexcept>
#include "QEI.h"
#define ueMD 0x60
#define migiMD 0x14
#define sitaMD 0x10
#define hidariMD 0x16
I2C i2c (D14,D15);
PS3 ps3 (A0,A1);
DigitalOut sig(D13);//緊急停止用
PwmOut green(D3),red(D5),blue(D6);
QEI encoder( D8, D9, D10, 2048, QEI::X2_ENCODING);
//QEI 任意の名前( A相のピン, B相のピン, Z相のピン, 分解能, 逓倍);
int select,start,ue,migi,sita,hidari,L1,R1,sankaku,batu;
void getdata(void);
int send(char add,char dat);
void autorun(void);//中央を自動でとる
void ledcheck(void);

int main(){
    char clockw=0xc9;
    char anticw=0x36;
    char sb=0x80;//ショートブレーキ用
    sig=0;
    ledcheck();
    while (true) {
        getdata();
        if(select == 1){
            sig=1;
        }
        else if(start == 1){
            sig=0;
        }
        else if(sankaku == 1){
            autorun();
        }
        else if(ue == 1){
            send(ueMD,sb);
            send(migiMD,anticw);
            send(sitaMD,sb);
            send(hidariMD,clockw);

        }
        /*else if(migi == 1){
            send(ueMD,clockw);
            send(migiMD,sb);
            send(sitaMD,anticw);
            send(hidariMD,sb);
        }*/
        else if(sita == 1){
            send(ueMD,sb);
            send(migiMD,clockw);
            send(sitaMD,sb);
            send(hidariMD,anticw);
        }
        /*else if(hidari == 1){
            send(ueMD,anticw);
            send(migiMD,sb);
            send(sitaMD,clockw);
            send(hidariMD,sb);
        }*/
        else if(R1 == 1){
            send(ueMD,clockw);
            send(migiMD,clockw);
            send(sitaMD,clockw);
            send(hidariMD,clockw);
        }
        else if(L1 == 1){
            send(ueMD,anticw);
            send(migiMD,anticw);
            send(sitaMD,anticw);
            send(hidariMD,anticw);
        }
        else{
            send(ueMD,sb);
            send(migiMD,sb);
            send(sitaMD,sb);
            send(hidariMD,sb); 
        }
    }
}
void getdata(void){
    ue=ps3.getButtonState(PS3::ue);
    migi=ps3.getButtonState(PS3::migi);
    sita=ps3.getButtonState(PS3::sita);
    hidari=ps3.getButtonState(PS3::hidari);

    sankaku=ps3.getButtonState(PS3::sankaku);
    batu=ps3.getButtonState(PS3::batu);

    R1=ps3.getButtonState(PS3::R1);
    L1=ps3.getButtonState(PS3::L1);

    select=ps3.getSELECTState();
    start=ps3.getSTARTState();
}

int send(char add,char dat){  
    i2c.start();
    i2c.write(add);
    i2c.write(dat);
    i2c.stop();
    wait_us(15000);
    return 0;
}

void autorun(void){
    encoder.reset();
    int pulse=0;
    while(true){
        getdata();
        pulse = encoder.getPulses();
        printf("%d\n",pulse);
        if(pulse >= 31403 || pulse <= -31403){//数値はあとで計測する！！！
            break;
        }
        else if(batu == 1){
            pulse=0;
            break;
        }
        else{
            send(migiMD,0xf0);
            send(hidariMD,0x0f);
        }
        wait_us(20000);
    }
}

void ledcheck(){
    while (true) {
        for(double i=0;;i++){
            green=0.5*sin(i*PI/180)+0.5;
            red=0.5*sin((i+120)*PI/180)+0.5;
            blue=0.5*sin((i+240)*PI/180)+0.5;
            wait_us(8000);
        }
    }
}