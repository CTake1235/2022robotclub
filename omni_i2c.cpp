//clockwとanticwはモータから見た（足回りの中心から見た）タイヤの回転方向を示している
//clockwは時計回り、anticwは反時計回り

#include "mbed.h"
#include "PS3.h"
#include "mbed_wait_api.h"
#include <stdexcept>
#include "QEI.h"
#include "time.h"
#include "sonMD.h"
#define ueMD 0x50
#define migiMD 0x52
#define sitaMD 0x54
#define hidariMD 0x56
I2C              i2c(PB_9,PB_8);
UnbufferedSerial raspi(PA_0,PA_1,9600);
QEI              ueMD_encoder(PA_13, PA_14, NC, 2048, QEI::X2_ENCODING);
QEI              sitaMD_encoder(PB_4, PB_5, NC, 2048, QEI::X2_ENCODING);
//QEI 任意の名前( A相のピン, B相のピン, Z相のピン, 分解能, 逓倍);
DigitalOut       green(PC_8), 
                 red(PC_6), 
                 blue(PC_9);

DigitalOut sig(PC_11);
DigitalIn led(PD_2);

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
        raspi.read(&data,4);
        if(led == 1){
            green = 1;
            red = 1;
            blue = 0;
        }
        else{
            green = 1;
            red = 1;
            blue = 1;
        }
        if(res == 1){
            switch(int(data)){
                case 1://move to ue
                    send(ueMD,sb);
                    send(migiMD,anticw);
                    send(sitaMD,sb);
                    send(hidariMD,clockw);
                    break;
                case 2://move to sita
                    send(ueMD,sb);
                    send(migiMD,clockw);
                    send(sitaMD,sb);
                    send(hidariMD,anticw);
                    break;
                case 3://move to migi
                    send(ueMD,clockw);
                    send(migiMD,sb);
                    send(sitaMD,anticw);
                    send(hidariMD,sb); 
                    break;
                case 4://move to hidari
                    send(ueMD,anticw);
                    send(migiMD,sb);
                    send(sitaMD,clockw);
                    send(hidariMD,sb);
                    break;             
                case 5://turn clockwise
                    send(ueMD,clockw);
                    send(migiMD,clockw);
                    send(sitaMD,clockw);
                    send(hidariMD,clockw);
                    break;
                case 7://turn anti-clockwise
                    send(ueMD,anticw);
                    send(migiMD,anticw);
                    send(sitaMD,anticw);
                    send(hidariMD,anticw);
                    break;
                case 11:
                    autorun(data);
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
    ueMD_encoder.reset();
    sitaMD_encoder.reset();
    int uepulse = 0;int sitapulse = 0;
    while(true){
        uepulse = ueMD_encoder.getPulses();
        sitapulse = sitaMD_encoder.getPulses();
        switch (raspi_dat) {
            case 11:
                if(uepulse <= 31403 || uepulse >= -31403 || sitapulse <= 31403 || sitapulse >= -31403){//数値はあとで計測する！！！
                send(migiMD,0xf0);
                send(hidariMD,0x0f);
                cend = clock();
                time = cend - start;
                rpm = (uepulse/4096)*(time*60/CLOCKS_PER_SEC);
                //pulse/4096で回転数、time/CLOCKS_PER_SECで秒、60をかけて毎分になおす
                printf("%lf",rpm);
                }
            default:
                break;
        }
    }
}