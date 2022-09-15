/*#include "mbed.h"
#include "sonMD.h"

DigitalOut sig(D13);

sonMD injection(D9,D10,0.003);
//sonMD name(PWMピン,PWMピン,周期)

int main(){
    sig = 0;
    while (true){
        injection.move_p2(0.01, 0.4);
    }
}
*/