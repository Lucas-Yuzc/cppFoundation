#include <iostream>
#include <string.h>
#include <memory>

class Node{
public:
    Node(){
        std::cout<<"Node Constructor"<<std::endl;
    }

    ~Node(){
        std::cout<<"~Node Destructor"<<std::endl;
    }

    void SetParent(std::shared_ptr<Node> par){
        this->_Parent = par;
    }

    void SetSon(std::shared_ptr<Node> ch){
        this->_Child = ch;
    }
private:
    std::shared_ptr<Node> _Parent;
    std::shared_ptr<Node> _Child;
};

class WeakNode{
public:
    WeakNode(){
        std::cout<<"WeakNode Constructor"<<std::endl;
    }

    ~WeakNode(){
        std::cout<<"~WeakNode Destructor"<<std::endl;
    }

    void SetParent(std::weak_ptr<WeakNode> par){
        this->_Parent = par;
    }

   void SetSon(std::shared_ptr<WeakNode> ch){
        this->_Child = ch;
    }
private:
    std::weak_ptr<WeakNode> _Parent;
    std::weak_ptr<WeakNode> _Child;
};

int main(){
    std::cout<< "====================== std::move ==================="<<std::endl;
    //std::move
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

    //string move
    std::string move_str = std::move(str);
    std::cout<< "move_str:"<<move_str <<std::endl;
    std::cout<< "str after move: "<<str <<std::endl;

    //*p->data move
    char* pData = new char[10];
    memset(pData, 0x00, sizeof(10));
    memcpy(pData, "pData", 10);
    std::cout<< "pData before move: "<<pData <<std::endl;
    char* move_pData = std::move(pData);
    std::cout<< "move_pData:"<<move_pData <<std::endl;
    std::cout<< "pData after move: "<<pData <<std::endl;

    if (pData != nullptr){
        delete []pData;
        pData = nullptr;
    }

    char* pMalloc = (char*)malloc(10);
    memset(pMalloc, 0x00, sizeof(10));
    memcpy(pMalloc, "test2", 10);
    std::cout<< "pMalloc : "<<pMalloc <<std::endl;

    if (pMalloc != NULL){
        free(pMalloc);
        pMalloc = nullptr;
    }

    std::cout<< "====================== 智能指针 -> class ==================="<<std::endl;
    std::cout<< "------- shared_ptr -------"<<std::endl;
    //智能指针
    int sptr_TestValue = 10;
    std::cout << "sptr_TestValue : "<<&sptr_TestValue << " "<<sptr_TestValue<<std::endl;
    //shared_ptr
    std::shared_ptr<int> sptr_1 = std::make_shared<int>(sptr_TestValue);
    std::cout << "sptr_1 get : "<<sptr_1.get()<<std::endl;
    std::cout << "sptr_1 use_count : "<<sptr_1.use_count()<<std::endl;

    std::shared_ptr<int> sptr_copy(sptr_1);
    std::cout << "sptr_copy get : "<< sptr_copy.get()<<std::endl;
    std::cout << "sptr_1 use_count : "<<sptr_1.use_count()<<std::endl;

    std::shared_ptr<int> sptr_2 = std::make_shared<int>(sptr_TestValue);
    sptr_copy = sptr_2;     //可共享资源，允许赋值拷贝，多个指向同一地址资源

    std::cout << "sptr_1 value : "<<sptr_1 << " "<<*sptr_1.get()<<std::endl;
    std::cout << "sptr_1 use_count : "<<sptr_1.use_count()<<std::endl;
    std::cout << "sptr_2 use_count : "<<sptr_2.use_count()<<std::endl;

    int* p_sptr = &sptr_TestValue;
    p_sptr = sptr_1.get();
    *p_sptr = 30;
    std::cout << "sptr_TestValue : "<<&sptr_TestValue << " "<<sptr_TestValue<<std::endl;
    std::cout << "sptr_1 value : "<<sptr_1 << " "<<*sptr_1.get()<<std::endl;
    std::cout << "sptr_1 use_count : "<<sptr_1.use_count()<<std::endl;
    std::cout << "sptr_2 use_count : "<<sptr_2.use_count()<<std::endl;

    //shared_ptr循环引用造成的资源无法回收
    std::cout<<"--- shared_ptr循环引用造成的资源无法回收 ---"<<std::endl;
    std::shared_ptr<Node> pfNode = std::make_shared<Node>();
    std::shared_ptr<Node> psNode = std::make_shared<Node>();
    pfNode->SetSon(psNode);
    psNode->SetParent(pfNode);
    //weak_ptr循环引用造成的资源无法回收
    std::cout<<"--- weak_ptr解决share_ptr循环引用造成的资源无法回收 ---"<<std::endl;
    std::shared_ptr<WeakNode> pfWeakNode(new WeakNode());
    std::shared_ptr<WeakNode> psWeakNode(new WeakNode());
    pfWeakNode->SetSon(psWeakNode);
    psWeakNode->SetParent(pfWeakNode);
    std::weak_ptr<WeakNode>wptr_1;// = pfWeakNode;
    {
        wptr_1 = pfWeakNode;
    }

    std::cout<< "wptr_1 use count :"<<wptr_1.use_count()<<std::endl;

    std::cout<< "------- weak_ptr -------"<<std::endl;
    std::shared_ptr<int> sptr_3 = std::make_shared<int>(10);
    std::cout<< "sptr_3 use count: "<<sptr_3.use_count()<<std::endl;
    std::weak_ptr<int> wptr_2(sptr_3);
    std::cout<< "after weak, sptr_3 use count: "<<sptr_3.use_count() <<std::endl;
    std::cout<< "wptr_1 use count: "<<wptr_2.use_count() <<std::endl;

    if(!wptr_2.expired()){
        std::cout<< "after expired, wptr_2 use count: "<<wptr_2.use_count() <<std::endl;
        std::shared_ptr<int> sptr_4 = wptr_2.lock();
        std::cout<< "sptr_4 use count: "<<sptr_4.use_count() <<" sptr_4 value:"<<*sptr_4 <<std::endl;
        *sptr_4 = 100;
        std::cout<< "sptr_4 use count: "<<sptr_4.use_count()<<" sptr_4 value:"<<*sptr_4 <<std::endl;
    }

    std::cout<< "------- unique_ptr -------"<<std::endl;
    std::unique_ptr<int> uptr_1(new int(10));
    std::cout<< "uptr_1 get :"<<uptr_1.get()<<std::endl;
    //std::unique_ptr<int> uptr_2 = uptr_1; //不共享资源，不能赋值拷贝，仅允许一个指向资源
    std::unique_ptr<int> uptr_2 = std::move(uptr_1);
    std::cout<< "uptr_2 get :"<<uptr_2.get()<<std::endl;
    std::cout<< "after move uptr_1 get :"<<uptr_1.get()<<std::endl;

    return 0;
}