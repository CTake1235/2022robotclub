#include "mbed.h"
#include "sonMD.h"
#include "PS3.h"
#include "QEI.h"
#include <mbed_wait_api.h>
#include "time.h"


UnbufferedSerial data_from_legnwing(D0,D1,9600);

sonMD            right_outside(D2,D3,0.00015);//右の外側
sonMD            right_inside(D4,D5,0.00015);//右の内側
sonMD            left_inside(D6,D7,0.00015);//左の内側
sonMD            left_outside(D8,D9,0.00015);//左の外側

sonMD            ro_reload(D10,D11,0.00015);
sonMD            ri_reload(D12,D13,0.00015);
sonMD            li_reload(PB_8,PB_9,0.00015);
sonMD            lo_reload(PA_5,PB_6,0.00015);

void wait_ms(int t);
void reload(void);
void reload_back(void);

double shotPW = 0.90;
double spindelay = 0.02;
double reloadPW = 0.16;

int main(){
    int         res;
    static char data;
    bool state = 0;
    bool rlstate = 0;
    data_from_legnwing.read(&data,9);
    while (true){
        res = data_from_legnwing.read(&data,9);
        if(res == 1){
            switch (int(data)) {
            case 9://shot
                state = 1;
                break;
            case 10:
                reload_back();//reload back
                break;
            case 11:
                reload();//reload
                break;
            case 12:
                state = 0;
                break;
            default:
                ro_reload.stop();
                ri_reload.stop();
                li_reload.stop();
                lo_reload.stop();
            }
        }

        if(state == 1){
            right_outside.move_p2(spindelay,shotPW);
            right_inside.move_p2(spindelay,shotPW);
            left_inside.move_p2(spindelay,shotPW);
            left_outside.move_p2(spindelay,shotPW);
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