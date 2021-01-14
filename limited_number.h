#ifndef __LIMITED_NUMBER_H__
#define __LIMITED_NUMBER_H__

#include <algorithm>
#include <cstdint>
#include <cassert>

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

template <typename T, typename S, S min, S max, typename limited_number_strategy = limited_number_strategies::clamp<T>>
class limited_number
{
static_assert(std::is_arithmetic<T>::value, "T should be an arithmetic type");
static_assert(std::is_integral<S>::value, "S should be an integral type");

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
        val_ = limited_number_strategy()(v, static_cast<T>(min), static_cast<T>(max));
    }
    
private:
    T val_ = min;
};

template <typename T, typename S, S min, S max, typename limited_number_strategy = limited_number_strategies::clamp<T>>
T operator+(limited_number<T, S, min, max, limited_number_strategy>& a, T b)
{
    limited_number<T, S, min, max, limited_number_strategy> c = *a;
    c += b;
    return c;
}

template <typename T, typename S, S min, S max, typename limited_number_strategy = limited_number_strategies::clamp<T>>
T operator-(limited_number<T, S, min, max, limited_number_strategy>& a, T b)
{
    limited_number<T, S, min, max, limited_number_strategy> c = *a;
    c -= b;
    return c;
}

template <typename T, typename S, S min, S max, typename limited_number_strategy = limited_number_strategies::clamp<T>>
T operator*(limited_number<T, S, min, max, limited_number_strategy>& a, T b)
{
    limited_number<T, S, min, max, limited_number_strategy> c = *a;
    c *= b;
    return c;
}

template <typename T, typename S, S min, S max, typename limited_number_strategy = limited_number_strategies::clamp<T>>
T operator/(limited_number<T, S, min, max, limited_number_strategy>& a, T b)
{
    limited_number<T, S, min, max, limited_number_strategy> c = *a;
    c /= b;
    return c;
}

#define LIMIT_INTS(name, type)  \
    template <type min, type max, typename limited_number_strategy = limited_number_strategies::clamp<type>>   \
    using name = limited_number<type, type, min, max, limited_number_strategy>;    

LIMIT_INTS(limited_int, int)
LIMIT_INTS(limited_int8, int8_t)
LIMIT_INTS(limited_int16, int16_t)
LIMIT_INTS(limited_int32, int32_t)
LIMIT_INTS(limited_int64, int64_t)
LIMIT_INTS(limited_uint8, uint8_t)
LIMIT_INTS(limited_uint16, uint16_t)
LIMIT_INTS(limited_uint32, uint32_t)
LIMIT_INTS(limited_uint64, uint64_t)

#define LIMIT_FLOATS(name, type, cont_type)  \
    template <cont_type min, cont_type max, typename limited_number_strategy = limited_number_strategies::clamp<type>>     \
    using name = limited_number<type, cont_type, min, max, limited_number_strategy>;

LIMIT_FLOATS(limited_float, float, int64_t)
LIMIT_FLOATS(limited_double, double, int64_t)

#undef LIMIT_INTS
#undef LIMIT_FLOATS

} // namespace estd

#endif