#include "mbed.h"
#include "sonMD.h"
#include "PS3.h"
#include "QEI.h"
#include <mbed_wait_api.h>

#define shotPW    0.76 
#define spindelay 0.02
#define reloadPW  0.30


UnbufferedSerial raspi(D0,D1,9600);
sonMD            right_outside(D2,NC,0.00015);//右の外側
sonMD            right_inside(D3,NC,0.00015);//右の内側
sonMD            left_inside(D4,NC,0.00015);//左の内側
sonMD            left_outside(D5,NC,0.00015);//左の外側

sonMD            ro_reload(D6,D7,0.00015);
sonMD            ri_reload(D8,D9,0.00015);
sonMD            li_reload(D10,D11,0.00015);
sonMD            lo_reload(D12,D13,0.00015);

QEI              ro_rori(PC_8, PC_6, NC, 2048, QEI::X2_ENCODING);
QEI              ri_rori(PC_5, PA_12, NC, 2048, QEI::X2_ENCODING);
QEI              li_rori(PB_12, PB_2, NC, 2048, QEI::X2_ENCODING);
QEI              lo_rori(PB_1, PB_15, NC, 2048, QEI::X2_ENCODING);

void wait_ms(int t);
void shot_all(void);
void stop_all(void);
void reload(void);
void reload_back(void);
int maru,batu,sankaku,start;

int main(){
    sig = 0;
    int         res;
    static char data;

    bool state = 0;
    bool rlstate = 0;
    while (true){
        res = raspi.read(&data,9);
        if(res == 1){
            switch (int(data)) {
            case 1:
                state = 1;//shot
                break;
            case 2:
                reload();//reload
                break;
            
            case 3:
                reload_back();//reload back
                break;
            case 4:
                state = 0;
            }
        }

        if(state == 1){
            right_outside.move_p1(spindelay,shotPW);
            right_inside.move_p1(spindelay,shotPW);
            left_inside.move_p1(spindelay,shotPW);
            left_outside.move_p1(spindelay,shotPW);
        }
        else{
            right_outside.stop();
            right_inside.stop();
            left_inside.stop();
            left_outside.stop();
        }
    }
}

void wait_ms(int t){
    wait_us(t*1000);
}

void reload(void){
    ro_reload.move_p1(spindelay,reloadPW);
    ri_reload.move_p1(spindelay,reloadPW);
    li_reload.move_p1(spindelay,reloadPW);
    lo_reload.move_p1(spindelay,reloadPW);
}

void reload_back(void){
    ro_reload.move_p2(spindelay,reloadPW);
    ri_reload.move_p2(spindelay,reloadPW);
    li_reload.move_p2(spindelay,reloadPW);
    lo_reload.move_p2(spindelay,reloadPW);
}