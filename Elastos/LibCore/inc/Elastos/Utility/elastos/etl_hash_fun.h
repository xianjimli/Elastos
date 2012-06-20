
#ifndef __ETL_HASH_FUN_H__
#define __ETL_HASH_FUN_H__

#include <elastos/etl_def.h>

_ELASTOS_NAMESPACE_BEGIN

template <class Key> 
struct Hash { };

inline size_t HashString(const char* s)
{
    unsigned long h = 0; 
    for ( ; *s; ++s) {
        h = 5*h + *s;
    }
  
    return size_t(h);
}

template<> struct Hash<char*>
{
    size_t operator()(const char* s) const { return HashString(s); }
};

template<> struct Hash<const char*>
{
    size_t operator()(const char* s) const { return HashString(s); }
};

template<> struct Hash<String>
{
    size_t operator()(String s) const { return HashString((const char*)s); }
};

template<> struct Hash<char> 
{
    size_t operator()(char x) const { return x; }
};

template<> struct Hash<unsigned char> 
{
    size_t operator()(unsigned char x) const { return x; }
};

template<> struct Hash<signed char> 
{
    size_t operator()(unsigned char x) const { return x; }
};

template<> struct Hash<short> 
{
    size_t operator()(short x) const { return x; }
};

template<> struct Hash<unsigned short> 
{
    size_t operator()(unsigned short x) const { return x; }
};

template<> struct Hash<int> 
{
    size_t operator()(int x) const { return x; }
};

template<> struct Hash<unsigned int> 
{
    size_t operator()(unsigned int x) const { return x; }
};

template<> struct Hash<long> 
{
    size_t operator()(long x) const { return x; }
};

template<> struct Hash<unsigned long> 
{
    size_t operator()(unsigned long x) const { return x; }
};

_ELASTOS_NAMESPACE_END


#endif //__ETL_HASH_FUN_H__
