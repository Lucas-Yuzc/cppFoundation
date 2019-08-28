#include <iostream>
#include <string.h>
#include <thread>
#include <mutex>

class Singleton_Ref{
public:
    Singleton_Ref(const Singleton_Ref& instance)=delete;
    Singleton_Ref& operator = (const Singleton_Ref& instance)=delete;
    static Singleton_Ref& GetInstance(){
        static Singleton_Ref instance;
        return instance;
    }

private:
    Singleton_Ref(){};
};

void Func_GetInstance_Ref(int ID)
{
    for (int i=0; i<10; i++){
        Singleton_Ref& instance = Singleton_Ref::GetInstance();
        printf("Thread id %d, index:%d instace:%p\n", ID, i+1, &instance);
    }
}

class Singleton_Ptr{
public:
    ~Singleton_Ptr(){
        printf("this is ~Singleton_Ptr\n");
    }

    Singleton_Ptr(const Singleton_Ptr& instance)=delete;
    Singleton_Ptr& operator = (const Singleton_Ptr& instance)=delete;
    static Singleton_Ptr* GetInstance(){
        if ( m_Instance == nullptr){
            // m_mtx.lock();
            std::lock_guard<std::mutex> lk(m_mtx);
            if (m_Instance == nullptr){
                m_Instance = new Singleton_Ptr();
            }
            // m_mtx.unlock();
        }

        return m_Instance;
    }

    void Show(){
        printf("this is show out\n");
    }
private:
    Singleton_Ptr(){};
    static Singleton_Ptr* m_Instance;
    static std::mutex m_mtx;
};

Singleton_Ptr* Singleton_Ptr::m_Instance = nullptr;
std::mutex Singleton_Ptr::m_mtx;


void Func_GetInstance_Ptr(int ID)
{
    for (int i=0; i<10; i++){
        Singleton_Ptr* instance = Singleton_Ptr::GetInstance();
        // Singleton_Ptr instancePtr(*instance);
        // instancePtr.Show();
        printf("Thread id %d, index: %d, instace:%p\n", ID, i+1, instance);
    }
}

int main(){
    //Singleton
    printf("=========================================Singleton======================================\n");
    printf("\t1、一个类全局仅有一个实例\n");
    printf("\t2、构造函数private对外接口隐藏\n");
    printf("\t3、禁止拷贝构造和复制构造，防止编译器自动生成默认构造，多次实例化\n");
    printf("\t4、指针情况，多线程判定\n");
    printf("\t5、局部静态成员变量，作用于在作用域内，生命周期直至程序结束\n");
    printf("=========================================================================================\n");
    std::thread fun1(Func_GetInstance_Ref, 1);
    std::thread fun2(Func_GetInstance_Ref, 2);

    std::thread fun3(Func_GetInstance_Ptr, 3);
    std::thread fun4(Func_GetInstance_Ptr, 4);

    fun1.join();
    fun2.join();

    fun3.join();
    fun4.join();

    return 0;
}