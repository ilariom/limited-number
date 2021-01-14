#ifndef __LIMITED_NUMBER_H__
#define __LIMITED_NUMBER_H__

#include <algorithm>

namespace estd
{

namespace limited_number_strategies
{

template <typename T>
class clamp
{
public:
    T operator()(T x, T m, T M) { return std::min(M, std::max(x, m)); }
};

template <typename T>
class assertion
{
public:
    T operator()(T x, T m, T M) { assert(m <= x && x <= M); return x; }
};

template <typename T>
class terminate
{
public:
    T operator()(T x, T m, T M)
    {
        if (!(m <= x && x <= M))
        {
            std::terminate();
        }
        
        return x;
    }
};

} // namespace limited_number_strategies

template <typename T, T min, T max, typename limited_number_strategy = limited_number_strategies::clamp<T>>
class limited_number
{
static_assert(std::is_arithmetic<T>::value, "T should be an arithmetic type");

public:
    limited_number() = default;
    limited_number(T val) { fix_and_set(val); }
    
    limited_number& operator=(T val)
    {
        fix_and_set(val);
        return *this;
    }
    
public:
    operator T() const { return val_; }
    T operator*() const { return val_; }

    T& operator+=(T v) { fix_and_set(val_ + v); }
    T& operator-=(T v) { fix_and_set(val_ - v); }
    T& operator*=(T v) { fix_and_set(val_ * v); }
    T& operator/=(T v) { fix_and_set(val_ / v); }
    
private:
    void fix_and_set(T v)
    {
        val_ = limited_number_strategy()(v, min, max);
    }
    
private:
    T val_ = min;
};

template <typename T, T min, T max, typename limited_number_strategy = limited_number_strategies::clamp<T>>
T operator+(limited_number<T, min, max, limited_number_strategy>& a, T b)
{
    limited_number<T, min, max, limited_number_strategy> c = *a;
    c += b;
    return c;
}

template <typename T, T min, T max, typename limited_number_strategy = limited_number_strategies::clamp<T>>
T operator-(limited_number<T, min, max, limited_number_strategy>& a, T b)
{
    limited_number<T, min, max, limited_number_strategy> c = *a;
    c -= b;
    return c;
}

template <typename T, T min, T max, typename limited_number_strategy = limited_number_strategies::clamp<T>>
T operator*(limited_number<T, min, max, limited_number_strategy>& a, T b)
{
    limited_number<T, min, max, limited_number_strategy> c = *a;
    c *= b;
    return c;
}

template <typename T, T min, T max, typename limited_number_strategy = limited_number_strategies::clamp<T>>
T operator/(limited_number<T, min, max, limited_number_strategy>& a, T b)
{
    limited_number<T, min, max, limited_number_strategy> c = *a;
    c /= b;
    return c;
}

template <int min, int max, typename limited_number_strategy = limited_number_strategies::clamp<int>>
using limited_int = limited_number<int, min, max, limited_number_strategy>;

} // namespace estd

#endif