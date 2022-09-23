#include "mbed.h"
#include "sonMD.h"
#include "PS3.h"
#include "QEI.h"
#include <mbed_wait_api.h>

DigitalOut sig(D13);
PS3 ps3(A0,A1);
sonMD right_outside(D2,D3,0.00015);//右の外側
sonMD right_inside(D4,D5,0.00015);//右の内側
sonMD left_inside(D6,D7,0.00015);//左の内側
sonMD left_outside(D8,D9,0.00015);//左の外側
//sonMD name(PWMピン,PWMピン,周期)

void getdata(void);
void wait_ms(int t);
int ue,sita,maru,batu,sankaku,start;

int main(){
    sig = 0;
    float dat = 0.76;
    bool state = 0;
    bool spin = 0;
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
    sankaku=ps3.getButtonState(PS3::sankaku);
}

void wait_ms(int t){
    wait_us(t*1000);
}