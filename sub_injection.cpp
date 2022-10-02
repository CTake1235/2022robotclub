#include "mbed.h"
#include "sonMD.h"
#include "PS3.h"
#include "QEI.h"
#include <mbed_wait_api.h>
#include "time.h"


UnbufferedSerial raspi(D0,D1,9600);

sonMD            right_outside(D2,D3,0.00015);//右の外側
sonMD            right_inside(D4,D5,0.00015);//右の内側
sonMD            left_inside(D6,D7,0.00015);//左の内側
sonMD            left_outside(D8,D9,0.00015);//左の外側

sonMD            ro_reload(D10,D11,0.00015);
sonMD            ri_reload(D12,D13,0.00015);
sonMD            li_reload(PC_8,PC_6,0.00015);
sonMD            lo_reload(PA_11,PB_2,0.00015);

QEI              ro_rori(PB_12,PB_1,NC, 2048, QEI::X2_ENCODING);
QEI              ri_rori(PC_5, PA_12, NC, 2048, QEI::X2_ENCODING);//PC_5
QEI              li_rori(PB_15, PB_14, NC, 2048, QEI::X2_ENCODING);
QEI              lo_rori(PB_13, PC_4, NC, 2048, QEI::X2_ENCODING);
void wait_ms(int t);
void reload(void);
void reload_back(void);

double shotPW = 0.95;
double spindelay = 0.02;
double reloadPW = 0.1;

int main(){
    int         res;
    static char data;
    bool state = 0;
    bool rlstate = 0;
    //raspi.format(8, BufferedSerial::None, 1);
    while (true){
        //res = raspi.read(&data,9);
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
            clock_t cstart, cend;
            int pulse[4];//ro,ri,li,lo
            int rpm[4];//same as pulse

            right_outside.move_p2(spindelay,shotPW);
            right_inside.move_p2(spindelay,shotPW);
            left_inside.move_p2(spindelay,shotPW);
            left_outside.move_p2(spindelay,shotPW);

            cstart = clock();

            pulse[0] = ro_rori.getPulses();
            pulse[1] = ri_rori.getPulses();
            pulse[2] = li_rori.getPulses();
            pulse[3] = lo_rori.getPulses();

            cend = clock();
            double time = cend - cstart;
            for(int i = 0; i < 4; i++){
                rpm[i] = (pulse[i]/4096)*(time*60/CLOCKS_PER_SEC);
                if(rpm[i] > 1500){//maybe
                    shotPW -= 0.02;                    
                }
            }
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