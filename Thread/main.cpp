#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>
#include <condition_variable>
#include <vector>

std::mutex mtx_1;
std::condition_variable cond_1, cond_2, cond_3, cond_4;
const int LoopNum = 10;

void fun1(int n)//初始化构造函数
{
    int i = 1;
    while(1){
        //
        {
            //std::lock_guard<std::mutex> lk1(mtx_1);
            std::unique_lock<std::mutex> lk(mtx_1);
            std::cout << "Thread No. " << n << std::endl;
            cond_2.notify_one();
            if (i++ >= LoopNum){
                break;
            }
            cond_1.wait(lk);
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
}

void fun2(int n)//初始化构造函数
{
    int i = 1;
    while(1){
        //std::unique_lock<std::mutex> lock(t_mtx);
        {
            //std::lock_guard<std::mutex>
            std::unique_lock<std::mutex> lk(mtx_1);
            std::cout << "Thread No. " << n << std::endl;
            cond_3.notify_one();
            if (i++ >= LoopNum){
                break;
            }
            cond_2.wait(lk);
        }
        //this_thread::sleep_for(chrono::milliseconds(1000));
    }
}
void fun3(int n)//初始化构造函数
{
    int i = 1;
    while(1){
        //std::unique_lock<std::mutex> lock(t_mtx);
        {
            std::unique_lock<std::mutex> lk(mtx_1);
            std::cout << "Thread No. " << n << std::endl;
            cond_4.notify_one();
            if (i++ >= LoopNum){
                break;
            }
            cond_3.wait(lk);
        }
        //this_thread::sleep_for(chrono::milliseconds(500));
    }
}
void fun4(int n)//初始化构造函数
{
    int i = 1;
    while(1){
        //std::unique_lock<std::mutex> lock(t_mtx);
        {
            std::unique_lock<std::mutex> lk(mtx_1);
            std::cout << "Thread No. " << n << std::endl;
            cond_1.notify_one();
            if (i++ >= LoopNum){
                break;
            }
            cond_4.wait(lk);
        }
        //this_thread::sleep_for(chrono::milliseconds(1000));
    }
}

bool g_bQuit = false;
std::condition_variable gCv;
std::mutex gMtx;
void Fun_producer(std::vector<int>* datalist, int num){
    while(!g_bQuit){
        {
            std::lock_guard<std::mutex> lk(gMtx);
            std::cout<<"Fun_producer producing"<<std::endl;
            datalist->push_back(num);
            gCv.notify_all();
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
}

void Fun_consumer(std::vector<int>* datalist, int num){
    while(!g_bQuit){
        {
            std::unique_lock<std::mutex> lk(gMtx);
            while (datalist->empty())
            {
                gCv.wait_for(lk, std::chrono::milliseconds(100));
            }

            std::cout<<"Consumer datalist count: "<<datalist->size()<<std::endl;
            std::vector<int>::iterator front = datalist->begin();
            std::cout<<"Consumer: "<<num<<", data form Producer : "<<*front<<std::endl;
            datalist->erase(front);
        }
    }
}
int main()
{
    /*
    {
        std::cout<<"===================== 1234 线程顺序输出 =================="<<std::endl;
        std::thread t1(fun1, 1);
        std::thread t2(fun2, 2);
        std::thread t3(fun3, 3);
        std::thread t4(fun4, 4);
        t1.join();
        t2.join();
        t3.join();
        t4.join();
    }
    */
    {
        std::vector<int> datalist;
        std::vector<std::shared_ptr<std::thread>> prothreadlist;
        std::vector<std::shared_ptr<std::thread>> cumthreadlist;
        std::cout<<"===================== 消费生产模型 ==========================="<<std::endl;
        for (int i = 0; i < 2; i++)
        {
            std::shared_ptr<std::thread> pthread = std::make_shared<std::thread>(Fun_producer, &datalist, i+1);
            prothreadlist.push_back(std::move(pthread));
        }

        for (int i = 0; i < 5; i++)
        {
            cumthreadlist.push_back(std::move(std::make_shared<std::thread>(Fun_consumer, &datalist, i+1)));
        }

        for (int i = 0; i < 2; i++)
        {
            if (prothreadlist[i]->joinable()){
                prothreadlist[i]->join();
            }
        }

        for (int i = 0; i < 5; i++)
        {
            if (cumthreadlist[i]->joinable()){
                cumthreadlist[i]->join();
            }
        }

        prothreadlist.clear();
    }

    //sleep();
    return 0;
}

