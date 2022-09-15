#include "mbed.h"
PwmOut p1(D9),p2(D10);
DigitalOut sig(D13);

int main(){
    p1.period_us(3000);
    p2.period_us(3000);
    sig=0;
    while (true) {
        p1=0;
        p2=0.2;
    }
}
