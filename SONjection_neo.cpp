#include "mbed.h"
#include "sonMD.h"
#include "PS3.h"
#include "QEI.h"
#include <mbed_wait_api.h>

DigitalOut sig(D13);
PS3 ps3(A0,A1);
sonMD injection(D9,D10,0.00015);
//sonMD name(PWMピン,PWMピン,周期)

void getdata(void);
void wait_ms(int t);
int ue,sita,maru,batu,start;

int main(){
    sig = 0;
    float dat = 0;
    bool state = 0;
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
        else if(ue == 1 && dat < 0.7){
            wait_ms(300);
            dat += 0.05;
            printf("dat=%lf\n",dat);
        }
        else if (sita == 1 && dat > 0){
            wait_ms(300);
            dat -= 0.05;
            printf("dat=%lf\n",dat);
        }

        if(state == 1){
            injection.move_p2(0.02, dat);
        }
        else{
            injection.stop();
        }
    }
}

void getdata(void){
    ue = ps3.getButtonState(PS3::ue);
    sita = ps3.getButtonState(PS3::sita);
    maru = ps3.getButtonState(PS3::maru);
    batu = ps3.getButtonState(PS3::batu);
}

void wait_ms(int t){
    wait_us(t*1000);
}