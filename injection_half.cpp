#include "mbed.h"
#include "sonMD.h"

sonMD inside(D3,NC,0.00015);
sonMD outside(D4,NC,0.00015);
I2C i2c(D14,D15);

int send(char add, char dat);

int main(){
    while (true) {
       switch (/*受け取った変数*/)
        case 1:
    }
}

int send(char add, char dat){
    i2c.start();
    i2c.write(add);
    i2c.write(dat);
    i2c.stop();
    wait_us(1500);
}