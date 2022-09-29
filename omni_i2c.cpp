//clockwとanticwはモータから見た（足回りの中心から見た）タイヤの回転方向を示している
//clockwは時計回り、anticwは反時計回り

#include "mbed.h"
#include "PS3.h"
#include "mbed_wait_api.h"
#include <stdexcept>
#include "QEI.h"
#include "time.h"
#include "sonMD.h"
#define ueMD 0x60
#define migiMD 0x50
#define sitaMD 0x70
#define hidariMD 0x10
I2C              i2c(D14,D15);
UnbufferedSerial raspi(D0,D1,9600);
DigitalOut       sig(D13);//緊急停止用
QEI              encoder( D9, D10, NC, 2048, QEI::X2_ENCODING);
//QEI 任意の名前( A相のピン, B相のピン, Z相のピン, 分解能, 逓倍);
DigitalOut       green(D6), red(D7), blue(D8);


void send(char add,char dat);
void autorun(int raspi_dat);//中央を自動でとる
void led_enable(void);

int main(){
    raspi.format(8, BufferedSerial::None, 1);
    char clockw = 0xc9;
    char anticw = 0x36;
    static char data;
    int res;
    int autorun_state;
    char sb = 0x80;//ショートブレーキ用
    sig = 0;
    while (true) {
        led_enable();
        raspi.read(&data,9);
        if(res == 1){
            switch(data){
                case 1://starting autorun(option)
                    autorun(data);
                    break;
                case 2:
                    autorun(data);
                    break;
                case 3://move to ue
                    send(ueMD,sb);
                    send(migiMD,anticw);
                    send(sitaMD,sb);
                    send(hidariMD,clockw);
                    break;
                case 4://move to migi 
                    send(ueMD,clockw);
                    send(migiMD,sb);
                    send(sitaMD,anticw);
                    send(hidariMD,sb); 
                    break;
                case 5://move to sita
                    send(ueMD,sb);
                    send(migiMD,clockw);
                    send(sitaMD,sb);
                    send(hidariMD,anticw);
                    break;
                case 6://move to hidari
                    send(ueMD,anticw);
                    send(migiMD,sb);
                    send(sitaMD,clockw);
                    send(hidariMD,sb);
                    break;
                case 7://turn clockwise
                    send(ueMD,clockw);
                    send(migiMD,clockw);
                    send(sitaMD,clockw);
                    send(hidariMD,clockw);
                    break;
                case 8://turn anti_clockwise
                    send(ueMD,anticw);
                    send(migiMD,anticw);
                    send(sitaMD,anticw);
                    send(hidariMD,anticw);
                    break;
                default:
                    send(ueMD,sb);
                    send(migiMD,sb);
                    send(sitaMD,sb);
                    send(hidariMD,sb); 
                    break;
            }
        }
    }
}

void send(char add,char dat){  
    i2c.start();
    i2c.write(add);
    i2c.write(dat);
    i2c.stop();
    wait_us(25000);
}

void autorun(int raspi_dat){
    clock_t cstart,cend;
    double time,rpm;
    int start;
    cstart = clock();
    encoder.reset();
    int pulse=0;
    while(true){
        pulse = encoder.getPulses();
        printf("%d\n",pulse);
        switch (raspi_dat) {
            case 1:
                if(pulse <= 31403 || pulse >= -31403){//数値はあとで計測する！！！
                send(migiMD,0xf0);
                send(hidariMD,0x0f);
                cend = clock();
                time = cend - start;
                rpm = (pulse/4096)*(time*60/CLOCKS_PER_SEC);
                //pulse/4096で回転数、time/CLOCKS_PER_SECで秒、60をかけて毎分になおす
                printf("%lf",rpm);
                }
            case 2:
                break;
        }
    }
}

void led_enable(void){
    green = 1;
    red = 1;
    blue = 0;
}