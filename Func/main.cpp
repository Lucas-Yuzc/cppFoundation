#include <iostream>
#include <functional>
#include <algorithm>
#include <vector>

int func_1(int a){
    std::cout<<"this is func_1."<<a<<std::endl;
    return a;
}

auto func_lambda = [](int a)->int{
    std::cout<<"this is lambda."<<a<<std::endl;
    return a;
};

class Functor_1{
public:
    int operator()(int a){
        std::cout<<"this is operator()."<<a<<std::endl;
        return a;
    }
};

class Functor_2{
public:
    int func_class_1(int a){
        std::cout<<"this is func_class_1."<<a<<std::endl;
        return a;
    }
    static int func_class_static(int a){
        std::cout<<"this is func_class_static."<<a<<std::endl;
        return a;
    }
};

class Functor_3{
public:
    int _varout = 1;
    void output(int x, int y){
        std::cout<<"x:"<<x<<" y:"<<y<<std::endl;
    }
};

int main(){
    {
        std::cout<<"====================== std::function ========================"<<std::endl;
        std::cout<<"function 是一种通用、多态的类封装，对C++中可调用的实体（普通函数、lambda表达式、函数指针及函数对象）的封装，形成可调用的std::function对象。"<<std::endl;
        std::function<int(int)> Functional;
        Functional = func_1;
        Functional(1);

        Functional = func_lambda;
        Functional(2);

        Functor_1 test_class_1;
        Functional = test_class_1;
        Functional(3);

        Functor_2 test_class_2;
        Functional = std::bind(&Functor_2::func_class_1, test_class_2, std::placeholders::_1);
        Functional(4);

        Functional = Functor_2::func_class_static;
        Functional(5);

        //类对象，可赋值
        std::function<int(int)>Functional_1;
        Functional_1 = Functional;
        Functional_1(100);
    }

    {
        std::cout<<"================================ std::bind ======================================="<<std::endl;
        Functor_3 test_c;
        std::function<void(int,int)> Functional = std::bind(&Functor_3::output, &test_c, std::placeholders::_1, std::placeholders::_2);
        Functional(1,2);

        std::function<int&(void)> Functional_1 = std::bind(&Functor_3::_varout, &test_c);
        Functional_1() = 300;

        std::cout<<"test_c varout:"<<test_c._varout<<std::endl;
    }

    {
        std::cout<<"================================ lambda ======================================="<<std::endl;
        std::vector<int> numlist;
        numlist.push_back(10);
        numlist.push_back(20);
        int a = 10;
        //无参数
        for_each(numlist.begin(), numlist.end(), [](int v){std::cout<<"v: "<< v <<std::endl;});

        //有参数
        int b = 10;
        for_each(numlist.begin(), numlist.end(), [=,&b](int v){
            std::cout<<"v+a+b: "<< v+a+b <<std::endl;
            b++;
        });

        {
            std::cout<<"默认情况下，对于一个值被拷贝的变量，lambda不会改变其值。（默认为右值）"<<std::endl;
            std::cout<<"mutable,则会使得该“值捕获变量”的值，可以在被捕获的值的基础上进行变化。"<<std::endl;
            int a = 10;
            for_each(numlist.begin(), numlist.end(), [a](int v)mutable{ std::cout << v+a << std::endl; a++; });
            std::cout << a << std::endl;
        }
    }


    return 0;
}