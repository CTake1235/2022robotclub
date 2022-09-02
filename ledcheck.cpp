#include "mbed.h"
#include "math.h"
PwmOut green(D3),red(D5),blue(D6);
double PI=3.141592;
int main(){
    while (true) {
        for(double i=0;;i++){
            green=0.5*sin(i*PI/180)+0.5;
            red=0.5*sin((i+120)*PI/180)+0.5;
            blue=0.5*sin((i+240)*PI/180)+0.5;
            wait_us(8000);
        }
    }
}

