//clockwとanticwはモータから見た（足回りの中心から見た）タイヤの回転方向を示している
//clockwは時計回り、anticwは反時計回り

#include "mbed.h"
#include "mbed_wait_api.h"
#include <cstdio>
#include <stdexcept>
#include "QEI.h"
#include "time.h"
#include "sonMD.h"

#define ueMD 0x50
#define migiMD 0x52
#define sitaMD 0x54
#define hidariMD 0x56

#define clockw 0xba
#define anticw 0x45
#define shortbreak 0x80

#define Rturn 0x70
#define Lturn 0x8f

I2C              i2c(PB_9,PB_8);
UnbufferedSerial raspi(PA_0,PA_1,9600);
QEI              ueMD_encoder(PA_13, PA_14, NC, 2048, QEI::X2_ENCODING);
QEI              sitaMD_encoder(PB_4, PB_5, NC, 2048, QEI::X2_ENCODING);
//QEI 任意の名前( A相のピン, B相のピン, Z相のピン, 分解能, 逓倍);
DigitalOut       green(PB_10), 
                 red(PA_8), 
                 blue(PA_9);

DigitalOut       sig(PC_11);
DigitalIn        led(PD_2);

void send(char add,char dat);
void autorun(int raspi_dat);//中央を自動でとる
void led_enable(void);

int main(){
    raspi.format(8, BufferedSerial::None, 1);
    static char data;
    int res = 0;
    int autorun_state;
    sig = 0;
    while (true) {
        res = raspi.read(&data,4);
        if(led == 0){//12V on
            green = 1;
            red = 1;
            blue = 0;
        }
        else{//12V off
            green = 0;
            red = 1;
            blue = 1;
        }
        if(res == 1){
            printf("%d\n",data);
            switch(int(data)){
                case 1://go to ue
                    send(ueMD,shortbreak);
                    send(migiMD,anticw);
                    send(sitaMD,shortbreak);
                    send(hidariMD,clockw);
                    break;
                case 2://go to sita
                    send(ueMD,shortbreak);
                    send(migiMD,clockw);
                    send(sitaMD,shortbreak);
                    send(hidariMD,anticw);
                    break;
                case 3://go to migi
                    send(ueMD,clockw);
                    send(migiMD,shortbreak);
                    send(sitaMD,anticw);
                    send(hidariMD,shortbreak); 
                    break;
                case 4://go to hidari
                    send(ueMD,anticw);
                    send(migiMD,shortbreak);
                    send(sitaMD,clockw);
                    send(hidariMD,shortbreak);
                    break;             
                case 5://turn clockwise
                    send(ueMD,Rturn);
                    send(migiMD,Rturn);
                    send(sitaMD,Rturn);
                    send(hidariMD,Rturn);
                    break;
                case 6://HIBARI starts running from leftside
                    send(ueMD,clockw);
                    send(migiMD,anticw);
                    send(sitaMD,anticw);
                    send(hidariMD,clockw);
                    break;
                case 7://turn anti-clockwise
                    send(ueMD,Lturn);
                    send(migiMD,Lturn);
                    send(sitaMD,Lturn);
                    send(hidariMD,Lturn);
                    break;
                case 8://HIBARI starts running from rightside
                    send(ueMD,anticw);
                    send(migiMD,anticw);
                    send(sitaMD,clockw);
                    send(hidariMD,clockw);
                    break;
                default:
                    send(ueMD,shortbreak);
                    send(migiMD,shortbreak);
                    send(sitaMD,shortbreak);
                    send(hidariMD,shortbreak); 
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
