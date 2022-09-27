//clockwとanticwはモータから見た（足回りの中心から見た）タイヤの回転方向を示している
//clockwは時計回り、anticwは反時計回り

#include "mbed.h"
#include"PS3.h"
#include"mbed_wait_api.h"
#include <stdexcept>
#include "QEI.h"
#include "time.h"
#include "sonMD.h"
#define ueMD 0x60
#define migiMD 0x50
#define sitaMD 0x70
#define hidariMD 0x10
I2C i2c (D14,D15);
PS3 ps3 (A0,A1);
DigitalOut sig(D13);//緊急停止用
QEI encoder( D9, D10, NC, 2048, QEI::X2_ENCODING);
//QEI 任意の名前( A相のピン, B相のピン, Z相のピン, 分解能, 逓倍);
sonMD right(D3,NC,0.00015);
sonMD left(D4,NC,0.00015);
sonMD reload(D5,NC,0.00015);
DigitalOut green(D6),red(D7),blue(D8);
int select,start,ue,migi,sita,hidari,L1,R1,sankaku,batu,maru,sikaku;
void getdata(void);
int send(char add,char dat);
void autorun(void);//中央を自動でとる
void led_enable(void);

int main(){
    char clockw=0xc9;
    char anticw=0x36;
    char sb=0x80;//ショートブレーキ用
    sig=0;
    while (true) {
        led_enable();
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
        else if(migi == 1){
            send(ueMD,clockw);
            send(migiMD,sb);
            send(sitaMD,anticw);
            send(hidariMD,sb);
        }
        else if(sita == 1){
            send(ueMD,sb);
            send(migiMD,clockw);
            send(sitaMD,sb);
            send(hidariMD,anticw);
        }
        else if(hidari == 1){
            send(ueMD,anticw);
            send(migiMD,sb);
            send(sitaMD,clockw);
            send(hidariMD,sb);
        }
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
        else if(maru == 1){
            right.move_p1(0.02,0.5);
            left.move_p1(0.02,0.5);
        }
        else if(sikaku == 1){
            reload.move_p1(0.02,0.2);
        }
        else{
            send(ueMD,sb);
            send(migiMD,sb);
            send(sitaMD,sb);
            send(hidariMD,sb); 

            right.stop();
            left.stop();
            reload.stop();
        }
    }
}
void getdata(void){
    select=ps3.getSELECTState();
    start=ps3.getSTARTState();

    ue=ps3.getButtonState(PS3::ue);
    migi=ps3.getButtonState(PS3::migi);
    sita=ps3.getButtonState(PS3::sita);
    hidari=ps3.getButtonState(PS3::hidari);

    sankaku=ps3.getButtonState(PS3::sankaku);
    batu=ps3.getButtonState(PS3::batu);

    R1=ps3.getButtonState(PS3::R1);
    L1=ps3.getButtonState(PS3::L1);

    maru=ps3.getButtonState(PS3::maru);
    sikaku=ps3.getButtonState(PS3::sikaku);
}

int send(char add,char dat){  
    i2c.start();
    i2c.write(add);
    i2c.write(dat);
    i2c.stop();
    wait_us(50000);
    return 0;
}

void autorun(void){
    clock_t cstart,cend;
    double time,rpm;
    cstart = clock();
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
            cend = clock();
            time = cend - start;
            rpm = (pulse/4096)*(time*60/CLOCKS_PER_SEC);
            //pulse/4096で回転数、time/CLOCKS_PER_SECで秒、60をかけて毎分になおす
            printf("%lf",rpm);
        }
        wait_us(20000);
    }
}

void led_enable(void){
    green = 1;
    red = 1;
    blue = 0;
}