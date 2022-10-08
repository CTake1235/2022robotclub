#include "mbed.h"
#include "sonMD.h"
#define shotpower 0.6

UnbufferedSerial raspi(PA_0,PA_1,9600);
sonMD            inside(PB_10,PA_8,0.00015);
sonMD            outside(PA_9,PC_7,0.00015);
sonMD            inside_reload(PB_6,PB_9,0.00015);
sonMD            outside_reload(PB_8,PC_9,0.00015);
sonMD            leg(PC_8,PC_6,0.00015);
sonMD            leg_reload(PA_11,PB_2,0.00015);

int main(){
    raspi.format(8,BufferedSerial::None,1);
    int res; 
    static char data;
    while(true){    
        res = raspi.read(&data,4);
        if(res == 1){
            switch(int(data)){
                case 9://maru,inject
                    inside.move_p1(0.02,shotpower);
                    outside.move_p1(0.02,shotpower);
                    leg.move_p1(0.02,shotpower);
                    break;
                case 10://sikaku,reload back
                    inside_reload.move_p2(0.02,0.2);
                    outside_reload.move_p2(0.02,0.2);
                    leg_reload.move_p2(0.02,0.2);
                    break;
                case 11://sankaku,reload go
                    inside_reload.move_p1(0.02,0.1);
                    outside_reload.move_p1(0.02,0.1);
                    leg_reload.move_p1(0.02,0.1);
                    break;
                default:
                    inside.stop();
                    outside.stop();
                    inside_reload.stop();
                    outside_reload.stop();
                    leg.stop();
                    leg_reload.stop();
                    break;
            }
        }   
    }
}