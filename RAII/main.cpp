#include <iostream>
#include <functional>

#define SCOPEGUARD_LINENAME_CAT(name, line) name##line
#define SCOPEGUARD_LINENAME(name, line) SCOPEGUARD_LINENAME_CAT(name, line)
#define ON_SCOPE_EXIT(callback) ScopeGuard SCOPEGUARD_LINENAME(EXIT, __LINE__)(callback)

class ScopeGuard
{
public:
    explicit ScopeGuard(std::function<void()> f) : handle_exit_scope_(f){};

    ~ScopeGuard(){ handle_exit_scope_(); }
private:
    std::function<void()> handle_exit_scope_;
};

class A{};

int main()
{
    {
        A *a = new A();
        ON_SCOPE_EXIT([&] {delete a; });
    }

    {
        // std::ofstream f("test.txt");
        // ON_SCOPE_EXIT([&] {f.close(); });
    }

    system("pause");
    return 0;
}