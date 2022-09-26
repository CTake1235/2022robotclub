#include "mbed.h"
#include "sonMD.h"
#include "PS3.h"
#include <mbed_wait_api.h>

DigitalOut sig(D13);
PS3 ps3(A0,A1);
sonMD right_outside(D2,D3,0.00015);//右の外側
sonMD right_inside(D4,D5,0.00015);//右の内側
sonMD left_inside(D6,D7,0.00015);//左の内側
sonMD left_outside(D8,D9,0.00015);//左の外側

sonMD ro_reload(D10,D11,0.00015);
sonMD ri_reload(PA_13,PA_14,0.00015);
sonMD li_reload(PB_15,PB_1,0.00015);
sonMD lo_reload(PB_2,PB_12,0.00015);
//sonMD name(PWMピン,PWMピン,周期)

void getdata(void);
void wait_ms(int t);
int ue,sita,maru,batu,sankaku,sikaku,select,start;

int main(){
    sig = 0;
    float dat = 0;
    bool state = 0;
    bool spin = 0;
    float reloaddat = 0;
    bool reloadstate = 0;
    while (true){
        getdata();
        if(select == 1){
            sig = 1;
        }
        else if(start == 1){
            sig = 0;
        }
        else if(maru == 1){
            printf("injecting...\n");
            wait_ms(300);
            state = 1;
        }
        else if(batu == 1){
            printf("stopping to inject...\n");
            wait_ms(300);
            state = 0;
        }
        else if(sankaku == 1){
            printf("changing spin:%d\n",spin);
            wait_ms(300);
            spin = !spin;
            printf("changed:%d\n",spin);
        }
        else if(sikaku == 1){
            printf("reloading...\n");
            wait_ms(300);
            reloadstate = 1;
        }
        else if(ue == 1){
            wait_ms(300);
            dat += 0.05;
            printf("dat=%lf\n",dat);
        }
        else if (sita == 1 && dat > 0){
            wait_ms(300);
            dat -= 0.05;
            printf("dat=%lf\n",dat);
        }

        if(state == 1 && spin == 1){
            right_outside.move_p2(0.02, dat);
            right_inside.move_p2(0.02, dat);
            left_inside.move_p2(0.02, dat);
            left_outside.move_p2(0.02, dat);
        }
        else if(state == 1 && spin == 0){
            right_outside.move_p1(0.02,dat);
            right_inside.move_p1(0.02,dat);
            left_inside.move_p1(0.02,dat);
            left_outside.move_p1(0.02,dat);
        }
        else{
            right_outside.stop();
            right_inside.stop();
            left_inside.stop();
            left_outside.stop();
        }

        if(reloadstate == 1 && spin == 0){
            ro_reload.move_p2(0.02,reloaddat);
            ri_reload.move_p2(0.02,reloaddat);
            li_reload.move_p2(0.02,reloaddat);
            lo_reload.move_p2(0.02,reloaddat);
        }
        else if(reloadstate == 1 && spin == 1){
            ro_reload.move_p1(0.02,reloaddat);
            ri_reload.move_p1(0.02,reloaddat);
            li_reload.move_p1(0.02,reloaddat);
            lo_reload.move_p1(0.02,reloaddat);
        }
        else{
            ro_reload.stop();
            ri_reload.stop();
            li_reload.stop();
            lo_reload.stop();

        }
    }
}

void getdata(void){
    ue = ps3.getButtonState(PS3::ue);
    sita = ps3.getButtonState(PS3::sita);

    maru = ps3.getButtonState(PS3::maru);
    batu = ps3.getButtonState(PS3::batu);
    sankaku = ps3.getButtonState(PS3::sankaku);

    sikaku = ps3.getButtonState(PS3::sikaku);

    select = ps3.getSELECTState();
    start = ps3.getSTARTState();
}

void wait_ms(int t){
    wait_us(t*1000);
}