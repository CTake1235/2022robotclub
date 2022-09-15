#include "mbed.h"
PwmOut p1(D5),p2(D6);
DigitalOut sig(D13);


int main(){
    sig=0;
    p1.period_us(1000);
    p2.period_us(1000);
    while (true) {
        p1=0;
        p2=0.1;
    }
}
