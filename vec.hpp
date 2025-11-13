#include <iostream>
#include <memory>
#include <ostream>
#include <stdexcept>
#include <algorithm> // For std::copy
#include <format>

template<typename T>
class vec {
public:

    vec()
    : d_size(0)
    , d_capacity(4)
    {
        d_buf = std::make_unique<T[]>(d_capacity);
    }

    vec(std::initializer_list<T> vals)
    : d_size(0)
    , d_capacity(vals.size())
    {
        d_buf = std::make_unique<T[]>(d_capacity);
        for(const auto& val : vals)
        {
            push_back(val);
        }
    }

    vec(vec&) = delete;
    vec operator=(vec&) = delete;
    vec(vec&&) = delete;
    vec operator=(vec&&) = delete;

    void push_back(const T& value)
    {
        if (d_size >= d_capacity) {
            reAllocateAndGrow();
        }
        d_buf[d_size++] = value;
    }

    T& at(size_t index) const
    {
        if (index >= d_size)
        {
            throw std::runtime_error(std::format("index {} out of range {}.", index, d_size-1));
        }
        return d_buf[index];
    }

    size_t size() const
    {
        return d_size;
    }

private:

    void reAllocateAndGrow() {
        d_capacity *= 2;
        std::unique_ptr<T[]> newBuf = std::make_unique<T[]>(d_capacity);
        
        std::copy(d_buf.get(), d_buf.get()+d_size, newBuf.get());
        
        d_buf = std::move(d_buf);
    }

    size_t d_size;
    size_t d_capacity;
    std::unique_ptr<T[]> d_buf;
};

template<typename T>
std::ostream& operator<<(std::ostream& os, const vec<T>& vec)
{
    for (size_t i=0; i < vec.size(); i++)
    {
        std::cout << "i=" << i << ", val=" << vec.at(i) << std::endl;
    }
    return os;
}

/*
int main() {
    std::cout << "hello" << std::endl;
    
    vec<int> v;

    v.push_back(10);
    v.push_back(10);
    v.push_back(10);
    v.push_back(10);
    v.push_back(10);
    v.push_back(1);

    std::cout << v;

    vec<int> v2 = {1,2,3,4,5};

    std::cout << v2;

    std::vector<int> v3;

    return 0;
}
*/


