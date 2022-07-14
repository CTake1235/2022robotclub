//clockwとanticwはモータから見た（足回りの中心から見た）タイヤの回転方向を示している
//clockwは時計回り、anticwは反時計回り

#include "mbed.h"
#include"PS3.h"
#include"mbed_wait_api.h"
#include <stdexcept>
#define ueMD 0x60
#define migiMD 0x14
#define sitaMD 0x10
#define hidariMD 0x16
I2C i2c (D14,D15);
PS3 ps3 (A0,A1);
DigitalOut sig(D13);
int select,start,ue,migi,sita,hidari,L1,R1;
void getdata(void);
int send(char add,char dat);

int main(){
    char clockw=0xff;
    char anticw=0x00;
    char sb=0x80;//ショートブレーキ用
    sig=0;
    while (true) {
        getdata();
        if(select==1){
            sig=1;
        }
        else if(start == 1){
            sig=0;
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
