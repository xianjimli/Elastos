
#ifndef __ETL_TYPE_TRAITS_H__
#define __ETL_TYPE_TRAITS_H__

#include <elastos/etl_def.h>

_ELASTOS_NAMESPACE_BEGIN

struct TrueType {};
struct FalseType {};

template <typename T>
struct TypeTraits 
{
    typedef TrueType    ThisDummyMemberMustBeFirst;
    typedef FalseType   HasTrivialDefaultConstructor;
    typedef FalseType   HasTrivialCopyConstructor;
    typedef FalseType   HasTrivialAssignmentOperator;
    typedef FalseType   HasTrivialDestructor;
    typedef FalseType   IsPOD;
};

template <>
struct TypeTraits<Boolean> 
{
    typedef TrueType    HasTrivialDefaultConstructor;
    typedef TrueType    HasTrivialCopyConstructor;
    typedef TrueType    HasTrivialAssignmentOperator;
    typedef TrueType    HasTrivialDestructor;
    typedef TrueType    IsPOD;    
};

template <>
struct TypeTraits<Char8> 
{
    typedef TrueType    HasTrivialDefaultConstructor;
    typedef TrueType    HasTrivialCopyConstructor;
    typedef TrueType    HasTrivialAssignmentOperator;
    typedef TrueType    HasTrivialDestructor;
    typedef TrueType    IsPOD;    
};

template <>
struct TypeTraits<Int32> 
{
    typedef TrueType    HasTrivialDefaultConstructor;
    typedef TrueType    HasTrivialCopyConstructor;
    typedef TrueType    HasTrivialAssignmentOperator;
    typedef TrueType    HasTrivialDestructor;
    typedef TrueType    IsPOD;    
};

template <>
struct TypeTraits<Float> 
{
    typedef TrueType    HasTrivialDefaultConstructor;
    typedef TrueType    HasTrivialCopyConstructor;
    typedef TrueType    HasTrivialAssignmentOperator;
    typedef TrueType    HasTrivialDestructor;
    typedef TrueType    IsPOD;    
};

template <>
struct TypeTraits<Double> 
{
    typedef TrueType    HasTrivialDefaultConstructor;
    typedef TrueType    HasTrivialCopyConstructor;
    typedef TrueType    HasTrivialAssignmentOperator;
    typedef TrueType    HasTrivialDestructor;
    typedef TrueType    IsPOD;    
};

template <typename T>
struct TypeTraits<T*> 
{
    typedef TrueType    HasTrivialDefaultConstructor;
    typedef TrueType    HasTrivialCopyConstructor;
    typedef TrueType    HasTrivialAssignmentOperator;
    typedef TrueType    HasTrivialDestructor;
    typedef TrueType    IsPOD;    
};

// The following could be written in terms of numeric_limits.
// We're doing it separately to reduce the number of dependencies.

template <typename T> 
struct IsInteger
{
    typedef FalseType Integral;
};

template<> 
struct IsInteger<bool> 
{
    typedef TrueType Integral;
};

template<>
struct IsInteger<char> 
{
    typedef TrueType Integral;
};

template<>
struct IsInteger<signed char>
{
    typedef TrueType Integral;
};

template<>
struct IsInteger<unsigned char>
{
    typedef TrueType Integral;
};

template<> 
struct IsInteger<short> 
{
    typedef TrueType Integral;
};

template<> 
struct IsInteger<unsigned short> 
{
    typedef TrueType Integral;
};

template<> 
struct IsInteger<int> 
{
    typedef TrueType Integral;
};

template<>
struct IsInteger<unsigned int> 
{
    typedef TrueType Integral;
};

template<> 
struct IsInteger<long> 
{
    typedef TrueType Integral;
};

template<> 
struct IsInteger<unsigned long> 
{
    typedef TrueType Integral;
};

template<> 
struct IsInteger<long long> 
{
    typedef TrueType Integral;
};

template<> 
struct IsInteger<unsigned long long> 
{
    typedef TrueType Integral;
};

template<typename T> 
struct IsNormalIterator 
{
    typedef FalseType Normal;
};

// Forward declaration hack, should really include this from somewhere.
template<typename Iterator, typename Container> class NormalIterator;

template<typename Iterator, typename Container>
struct IsNormalIterator<NormalIterator<Iterator, Container> > 
{
    typedef TrueType Normal;
};

_ELASTOS_NAMESPACE_END

#endif //__ETL_TYPE_TRAITS_H__
