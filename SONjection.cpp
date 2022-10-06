#include "mbed.h"
#include "sonMD.h"
#include "PS3.h"
#include "QEI.h"
#include <mbed_wait_api.h>

PS3              ps3(A0,A1);

DigitalOut       sig(D13);

UnbufferedSerial raspi(D0,D1,9600);

sonMD            right_outside(D2,D3,0.00015);//右の外側
sonMD            right_inside(NC,NC,0.00015);//右の内側
sonMD            left_inside(D4,NC,0.00015);//左の内側
sonMD            left_outside(D5,NC,0.00015);//左の外側

sonMD            ro_reload(D6,NC,0.00015);
sonMD            ri_reload(D7,NC,0.00015);
sonMD            li_reload(D8,NC,0.00015);
sonMD            lo_reload(D9,NC,0.00015);
//sonMD name(PWMピン,PWMピン,周期)

void getdata(void);
void wait_ms(int t);
int maru,batu,sankaku,start;

int main(){
    sig = 0;
    float dat = 0.76;
    bool  state = 0;
    bool  spin = 0;
    while (true){
        getdata();
        if(maru == 1){
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
        else if(0){
            ro_reload.move_p1(0.02,0.1);
            ri_reload.move_p1(0.02,0.1);
            li_reload.move_p1(0.02,0.1);
            lo_reload.move_p1(0.02,0.1);
        }
        else{
            right_outside.stop();
            right_inside.stop();
            left_inside.stop();
            left_outside.stop();
        }
    }
}

void getdata(void){
    maru = ps3.getButtonState(PS3::maru);
    batu = ps3.getButtonState(PS3::batu);
    sankaku = ps3.getButtonState(PS3::sankaku);
}

void wait_ms(int t){
    wait_us(t*1000);
}