#include <iostream>
#include <memory>
#include <ostream>
#include <stdexcept>
#include <algorithm> // For std::copy
#include <format>
#include <vector>

// basically just RAII wrapper, but STL version has more features

template<typename T>
class UniquePtr {
public:
UniquePtr(T* ptr)
: d_ptr(ptr)
{}

UniquePtr(UniquePtr&) = delete;
UniquePtr operator=(UniquePtr&) = delete;
UniquePtr(UniquePtr&&) = delete;
UniquePtr operator=(UniquePtr&&) = delete;

~UniquePtr()
{
    std::default_delete<T>{}(d_ptr); // will know to call delete[] or delete depending on type
}

T* get() const
{
    return d_ptr;
}

void reset()
{
    delete d_ptr;
    d_ptr = nullptr;
}

private:
T* d_ptr;
};

template<typename T, typename Args>
UniquePtr<T> makeUnique(Args&& args)
{
    T* ptr = new T(args);
    return UniquePtr<T>(ptr);
}

template<typename T>
UniquePtr<T> makeUnique(T* ptr)
{
    return UniquePtr<T>(ptr);
}

// int main() {
//     std::cout << "hello" << std::endl;

//     {
//         int* a = new int(5);
//         UniquePtr<int> u_ptr(a);

//         auto u_ptr2 = makeUnique<int>(10);
        
//         std::cout << *u_ptr2.get() << std::endl;
        
//         auto u_ptr3 = makeUnique<int>(new int(11));
//         std::cout << *u_ptr3.get() << std::endl;
//     }

//     return 0;
// }