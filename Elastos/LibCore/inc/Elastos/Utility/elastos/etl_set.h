
#ifndef __ETL_SET_H__
#define __ETL_SET_H__

#include <elastos/etl_def.h>
#include <elastos/ConceptCheck.h>

_ELASTOS_NAMESPACE_BEGIN

// Forward declarations of operators < and ==, needed for friend declaration.

template <class Key, class Compare = Less<Key>,
          class Alloc = Allocator<Key> >
class Set;

template <class Key, class Compare, class Alloc>
inline Boolean operator==(const Set<Key, Compare, Alloc>& x, 
                          const Set<Key, Compare, Alloc>& y);

template <class Key, class Compare, class Alloc>
inline Boolean operator<(const Set<Key, Compare, Alloc>& x, 
                         const Set<Key, Compare, Alloc>& y);
                         

template <class Key, class Compare, class Alloc>
class Set
{
    // concept requirements
    Concept_ClassRequires(Key, SGIAssignableConcept)
    Concept_ClassRequires4(Compare, Boolean, Key, Key, BinaryFunctionConcept);

public:
    // typedefs:
    typedef Key     KeyType;
    typedef Key     ValueType;
    typedef Compare KeyCompare;
    typedef Compare ValueCompare;
private:
    typedef RBTree<KeyType, ValueType, 
                   Identity<ValueType>, KeyCompare, Alloc> RepType;
    RepType mTree;  // red-black tree representing Set
public:
    typedef typename RepType::ConstPointer Pointer;
    typedef typename RepType::ConstPointer ConstPointer;
    typedef typename RepType::ConstReference Reference;
    typedef typename RepType::ConstReference ConstReference;
    typedef typename RepType::ConstIterator Iterator;
    typedef typename RepType::ConstIterator ConstIterator;
    typedef typename RepType::ConstReverseIterator ReverseIterator;
    typedef typename RepType::ConstReverseIterator ConstReverseIterator;
    typedef typename RepType::SizeType SizeType;
    typedef typename RepType::DifferenceType DifferenceType;
    typedef typename RepType::AllocatorType AllocatorType;

    // allocation/deallocation
  
    Set() : mTree(Compare(), AllocatorType()) {}
    explicit Set(const Compare& comp,
                 const AllocatorType& a = AllocatorType())
      : mTree(comp, a) {}
  
    template <class InputIterator>
    Set(InputIterator first, InputIterator last)
      : mTree(Compare(), AllocatorType())
      { mTree.InsertUnique(first, last); }
  
    template <class InputIterator>
    Set(InputIterator first, InputIterator last, const Compare& comp,
        const AllocatorType& a = AllocatorType())
      : mTree(comp, a) { mTree.InsertUnique(first, last); }
  
    Set(const Set<Key,Compare,Alloc>& x) : mTree(x.mTree) {}
    Set<Key,Compare,Alloc>& operator=(const Set<Key, Compare, Alloc>& x)
    { 
      mTree = x.mTree; 
      return *this;
    }

    // accessors:
  
    KeyCompare KeyComp() const { return mTree.KeyComp(); }
    ValueCompare ValueComp() const { return mTree.KeyComp(); }
    AllocatorType GetAllocator() const { return mTree.GetAllocator(); }
  
    Iterator Begin() const { return mTree.Begin(); }
    Iterator End() const { return mTree.End(); }
    ReverseIterator RBegin() const { return mTree.RBegin(); } 
    ReverseIterator REnd() const { return mTree.REnd(); }
    Boolean IsEmpty() const { return mTree.IsEmpty(); }
    SizeType GetSize() const { return mTree.GetSize(); }
    SizeType GetMaxSize() const { return mTree.GetMaxSize(); }
    void Swap(Set<Key,Compare,Alloc>& x) { mTree.Swap(x.mTree); }

    // Insert/Erase
    Pair<Iterator,Boolean> Insert(const ValueType& x) 
    { 
        Pair<typename RepType::Iterator, Boolean> p = mTree.InsertUnique(x); 
        return Pair<Iterator, Boolean>(p.mFirst, p.mSecond);
    }
    Iterator Insert(Iterator position, const ValueType& x) 
    {
        typedef typename RepType::Iterator RepIterator;
        return mTree.InsertUnique((RepIterator&)position, x);
    }
    template <class InputIterator>
    void Insert(InputIterator first, InputIterator last) 
    {
        mTree.InsertUnique(first, last);
    }
    void Erase(Iterator position) 
    { 
        typedef typename RepType::Iterator RepIterator;
        mTree.Erase((RepIterator&)position); 
    }
    SizeType Erase(const KeyType& x) 
    { 
        return mTree.Erase(x); 
    }
    void Erase(Iterator first, Iterator last) 
    { 
        typedef typename RepType::Iterator RepIterator;
        mTree.Erase((RepIterator&)first, (RepIterator&)last); 
    }
    void Clear() { mTree.Clear(); }

  // Set operations:

    SizeType GetCount(const KeyType& x) const 
    {
        return mTree.Find(x) == mTree.End() ? 0 : 1;
    }

//#ifdef _GLIBCPP_RESOLVE_LIB_DEFECTS
//214.  Set::Find() missing const overload
    Iterator Find(const KeyType& x) { return mTree.Find(x); }
    
    ConstIterator Find(const KeyType& x) const { return mTree.Find(x); }
    
    Iterator GetLowerBound(const KeyType& x) 
    {
        return mTree.GetLowerBound(x);
    }
    
    ConstIterator GetLowerBound(const KeyType& x) const 
    {
        return mTree.GetLowerBound(x);
    }
    
    Iterator GetUpperBound(const KeyType& x) 
    {
        return mTree.GetUpperBound(x); 
    }
    
    ConstIterator GetUpperBound(const KeyType& x) const 
    {
        return mTree.GetUpperBound(x); 
    }
    
    Pair<Iterator,Iterator> GetEqualRange(const KeyType& x) 
    {
        return mTree.GetEqualRange(x);
    }
    
    Pair<ConstIterator,ConstIterator> GetEqualRange(const KeyType& x) const 
    {
        return mTree.GetEqualRange(x);
    }
//#else
//    Iterator Find(const KeyType& x) const { return mTree.Find(x); }
//    Iterator GetLowerBound(const KeyType& x) const {
//      return mTree.GetLowerBound(x);
//    }
//    Iterator GetUpperBound(const KeyType& x) const {
//      return mTree.GetUpperBound(x); 
//    }
//    Pair<Iterator,Iterator> GetEqualRange(const KeyType& x) const {
//      return mTree.GetEqualRange(x);
//    }
//#endif

    template <class K1, class C1, class A1>
    friend Boolean operator== (const Set<K1,C1,A1>&, const Set<K1,C1,A1>&);
    template <class K1, class C1, class A1>
    friend Boolean operator< (const Set<K1,C1,A1>&, const Set<K1,C1,A1>&);
};

template <class Key, class Compare, class Alloc>
inline Boolean operator==(const Set<Key,Compare,Alloc>& x, 
                          const Set<Key,Compare,Alloc>& y) 
{
    return x.mTree == y.mTree;
}

template <class Key, class Compare, class Alloc>
inline Boolean operator<(const Set<Key,Compare,Alloc>& x, 
                         const Set<Key,Compare,Alloc>& y) 
{
    return x.mTree < y.mTree;
}

template <class Key, class Compare, class Alloc>
inline Boolean operator!=(const Set<Key,Compare,Alloc>& x, 
                          const Set<Key,Compare,Alloc>& y)
{
    return !(x == y);
}

template <class Key, class Compare, class Alloc>
inline Boolean operator>(const Set<Key,Compare,Alloc>& x, 
                         const Set<Key,Compare,Alloc>& y) 
{
    return y < x;
}

template <class Key, class Compare, class Alloc>
inline Boolean operator<=(const Set<Key,Compare,Alloc>& x, 
                          const Set<Key,Compare,Alloc>& y) 
{
    return !(y < x);
}

template <class Key, class Compare, class Alloc>
inline Boolean operator>=(const Set<Key,Compare,Alloc>& x, 
                          const Set<Key,Compare,Alloc>& y) 
{
    return !(x < y);
}

template <class Key, class Compare, class Alloc>
inline void Swap(Set<Key,Compare,Alloc>& x, 
                 Set<Key,Compare,Alloc>& y) 
{
    x.Swap(y);
}                      

_ELASTOS_NAMESPACE_END

#endif //__ETL_SET_H__
