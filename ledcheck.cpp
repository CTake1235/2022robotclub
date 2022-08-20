#include "mbed.h"
#include "math.h"
PwmOut green(D3),red(D5),blue(D6);
double PI=3.141592;
int main(){
    while (true) {
        for(double i=0;;i+=0.1){
            green=sin(i)*10;
            red=sin(2);
            blue=sin(i)*10;
            wait_us(50000);
        }
    }
}

