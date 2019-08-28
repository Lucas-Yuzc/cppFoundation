#include<iostream>
#include <string.h>

void process_value(int& i)
{
    std::cout<<"LValue processed:"<<i<<std::endl;
}

void process_value(int&& i)
{
    std::cout<<"RValue processed:"<<i<<std::endl;
}

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
    int _a;
public:
    C_Test_2(/* args */){};
    ~C_Test_2(){};
    void Print(){};
    int Print1(){ return 1;};
};

class C_Test_3
{
public:

    C_Test_3(int A, char C, char* in):a(A),c(C){
        std::cout<<"C_Test_3 1"<<std::endl;
        this->p = new char[strlen(in)];
        memcpy(this->p, in, strlen(in));
    };

    C_Test_3(C_Test_3&& tmp){
        std::cout<<"C_Test_3 2"<<std::endl;
        this->a = tmp.a;
        this->c = tmp.c;
        this->p = tmp.p;
        tmp.p = NULL;
    };

    C_Test_3(C_Test_3& tmp){
        std::cout<<"C_Test_3 3"<<std::endl;
        this->a = tmp.a;
        this->c = tmp.c;
        this->p = new char[strlen(tmp.p)];
        memcpy(this->p, tmp.p, strlen(tmp.p));
    };
    ~C_Test_3(){
        if(p!=NULL){
            delete []p;
            p = NULL;
            std::cout<<"~C_Test_3"<<std::endl;
        }

    };
    int a;

    char c;

    char* p;

};

int main(){
    //l value
    int i = 1;
    process_value(i);
    //r value
    process_value(2);

    // Things that are declared as rvalue reference can be lvalues or rvalues.
    // The distinguishing criterion is: if it has a name, then it is an lvalue. Otherwise, it is an rvalue.
    int&& value = 3;
    process_value(value);
    
    char* p = "hello";
    std::cout<< p <<std::endl;
    printf("p:%p\n", p);

    std::string q = static_cast<std::string>(p);
    std::cout << q<<std::endl;
    printf("q:%p\n", q.c_str());

    // *p move
    char* move_p = std::move(p);
    std::cout<< "move_p:"<<move_p <<std::endl;
    std::cout<< "p after move:"<<p <<std::endl;
    std::string str = std::move(std::string(p));
    std::cout<<"str:" <<str <<std::endl;

    int Num = 10;
    int nmove = std::move(Num);

    //string move
    std::string move_str(std::move(std::string("test for std::string move")));
    std::cout<< "move_str:"<<move_str <<std::endl;

    //*p->data move
    char* pData = new char[10];
    memset(pData, 0x00, sizeof(10));
    memcpy(pData, "pData", 10);
    std::cout<< "pData before move: "<<pData <<std::endl;
    char* move_pData = std::move(pData);
    std::cout<< "move_pData:"<<move_pData <<std::endl;
    std::cout<< "pData after move: "<<pData <<std::endl;

    std::cout<<"sizeof pData: "<<sizeof(pData)<<std::endl;
    if (pData != nullptr){
        delete []pData;
        pData = nullptr;
    }

    char* pMalloc = (char*)malloc(10);
    memset(pMalloc, 0x00, sizeof(10));
    memcpy(pMalloc, "test2", 10);
    std::cout<< "pMalloc : "<<pMalloc <<std::endl;

    std::cout<<"sizeof int: "<<sizeof(int(1))<<std::endl;
    std::cout<<"sizeof pMalloc: "<<sizeof(pMalloc)<<std::endl;
    if (pMalloc != NULL){
        free(pMalloc);
        pMalloc = nullptr;
    }

    C_Test_3 tmp = C_Test_3(1,2, "abc");
    C_Test_3 test_class(tmp);

    C_Test_3 test_class_move(std::move(C_Test_3(1,2, "abc")));

    return 0;
}