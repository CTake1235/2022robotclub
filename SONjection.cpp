#include "mbed.h"
DigitalOut sig(D13);
PwmOut p1(D9),p2(D10);

int main(){
    p1.period_us(1500);
    p2.period_us(1500);
    sig = 0;
    bool state = 0;
    while (true) {
        p1.write(0);
        p2.write(0.3);
    }
}
