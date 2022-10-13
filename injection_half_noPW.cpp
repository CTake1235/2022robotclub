#include "mbed.h"
#include "sonMD.h"
#include "time.h"

UnbufferedSerial raspi         (PA_0,PA_1,9600);

sonMD            inside        (PA_8,PB_10,0.00015);
sonMD            outside       (PA_9,PC_7,0.00015);
sonMD            inside_reload (PB_6,PB_9,0.00015);
sonMD            outside_reload(PC_9,PB_8,0.00015);

int main(){
    raspi.format(8,BufferedSerial::None,1);
    int res,time;
    clock_t start,end; 
    static char data;
    double shotpower = 0.9;
    double reloadpower = 0.5;
    bool state = false;
    while(true){    
        printf("shot:%lf,reload:%lf\n",shotpower,reloadpower);
        res = raspi.read(&data,4);
        if(res == 1){
            switch(int(data)){
                case 1://increase shotpower
                    start = clock();
                    shotpower += 0.1;
                    printf("shotpower:%lf\n",shotpower);
                    while(true){
                        end = clock();
                        time = end - start;
                        if(double(time)*60/CLOCKS_PER_SEC >= 0.3){
                            break;
                        }
                    }
                    break;
                case 2://decrease shotpower
                    start = clock();
                    shotpower -= 0.1;
                    printf("shotpower:%lf\n",shotpower);
                    while(true){
                        end = clock();
                        time = end - start;
                        if(double(time)*60/CLOCKS_PER_SEC >= 0.3){
                            break;
                        }
                    }
                    break;
                case 3://increase reloadpower
                    start = clock();
                    reloadpower += 0.1;
                    printf("reload:%lf\n",reloadpower);
                    while(true){
                        end = clock();
                        time = end - start;
                        if(double(time)*60/CLOCKS_PER_SEC >= 0.3){
                            break;
                        }
                    }
                    break;
                case 4://decrease reloadpower
                    start = clock();
                    reloadpower -= 0.1;
                    printf("reload:%lf",reloadpower);
                    while(true){
                        end = clock();
                        time = end - start;
                        if(double(time)*60/CLOCKS_PER_SEC >= 0.3){
                            break;
                        }
                    }
                    break;
                case 9://maru,inject
                    state = true;
                    break;
                case 10://sikaku,reload back
                    inside_reload.move_p2(0.02,reloadpower);
                    outside_reload.move_p2(0.02,reloadpower);
                    break;
                case 11://sankaku,reload go
                    inside_reload.move_p1(0.02,reloadpower);
                    outside_reload.move_p1(0.02,reloadpower);
                    break;
                case 12://batu,stop
                    state = false;
                    break;
                default:
                    inside.stop();
                    outside.stop();
                    inside_reload.stop();
                    outside_reload.stop();
            }
            if(state == true){//for injection
                inside.move_p1(0.02,shotpower);
                outside.move_p1(0.02,shotpower);
            }
        }
        else{
            inside.stop();
            outside.stop();
            inside_reload.stop();
            outside_reload.stop();
        }   
    }
}