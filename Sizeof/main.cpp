#include<iostream>
#include <string.h>

struct S_Test_1
{
    /* data */
};

struct S_Test_2
{
    /* data */
    int a;
};

class C_Test_1
{
private:
    /* data */
public:
    C_Test_1(/* args */){};
    ~C_Test_1();
};

class C_Test_2
{
private:
    /* data */
    // int _a;
public:
    C_Test_2(/* args */){};
    ~C_Test_2(){};
    void Print(){};
    virtual int Print1();
};

class C_Test_3
{
public:

    //C_Test_3():d(10){};
    C_Test_3(){};
    int a;
    // const int d;

    static int b;

};

int main(){
    //struct
    std::cout<<"size of empty struct: "<<sizeof(S_Test_1)<<std::endl;
    std::cout<<"size of struct with a char : "<<sizeof(S_Test_2)<<std::endl;
    //class
    std::cout<<"size of empty class C_Test_1 : "<<sizeof(C_Test_1)<<std::endl;
    std::cout<<"size of class C_Test_2 with a func : "<<sizeof(C_Test_2)<<std::endl;
    std::cout<<"size of class C_Test_3 with a static : "<<sizeof(C_Test_3)<<std::endl;

    void* p_void;
    std::cout<<"size of void* : "<<sizeof(p_void)<<std::endl;
    return 0;
}