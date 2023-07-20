#include<iostream>
#include "avt.h"
class A{
public:
static void execute_0(){}
static void execute_1(){}
static void execute_2(){}
static void execute_3(){}
static void execute_4(){}
static void execute_5(){}
static void execute_6(){}
static void execute_7(){}
static void execute_b(){}
static void execute_B(){}
static void execute_R(){}
static void execute_r(){}
static void execute_s(){}
static void execute_t(){}
static void execute_u(){}
static void execute_h(){}
static void execute_move(int,int*){}
static void execute_scan(int,int*){}
static void execute_waitTicks(int,int*){}
static void execute_waitTime(int,int*){}
};
int main(){
    ProtocoleAvt <A> avtik;
    avtik.readByte('$');
}
