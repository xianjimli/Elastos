
#ifndef __CMAP_H__
#define __CMAP_H__

#include<elastos.h>

using namespace Elastos;

#define SIZE 32

template<class T1>
class StringMap
{
public:
    StringMap();
    
    ~StringMap();
    
    void Put(String key, T1 *pValue);
    
    T1* Get(String key);
    
    T1* Remove(String key);

private:
    class CElement 
    {
    public:
        CElement(String key, T1 *pValue)
        {
            m_Key = String::Duplicate(key);
            m_Value = pValue;
            m_pNext = NULL;
        }
        
    public:
        String m_Key;
        T1 *m_Value;
        CElement *m_pNext;
    };
    
    UInt32 Hash(String key);
    
private:
    CElement *m_Slot[SIZE];    
};

template<class T1>
StringMap<T1>::StringMap()
{
    for(Int32 i = 0; i < SIZE; i++) {
        m_Slot[i] = NULL;
    }
}

template<class T1>
StringMap<T1>::~StringMap()
{
    for(Int32 i = 0; i < SIZE; i++) {
//
    }
}

template<class T1>
UInt32 StringMap<T1>::Hash(String key)
{
    UInt32 val = 0;
    UInt32 pos = 0;
    Int32 len = key.GetLength();
    
    while(pos < len) {
        val += ((const char*)key)[pos] * pos;
        pos++; 
    }
    
    return val;
}

template<class T1>
void StringMap<T1>::Put(String key, T1 *pValue)
{
    Int32 idx = Hash(key) % SIZE;
    
    CElement *pElement = new CElement(key, pValue);
    if (m_Slot[idx] == NULL) {
        m_Slot[idx] = pElement;
    }
    else {
        CElement *p1 = m_Slot[idx];
        while(p1 != NULL) {
            if (!p1->m_Key.Compare(key)) {
                T1 *p2 = p1->m_Value;
                p1->m_Value = pValue;
                delete p2;
                
                return; 
            }
            
            p1 = p1->m_pNext;
        }
        
        pElement->m_pNext = m_Slot[idx];
        m_Slot[idx] = pElement;
    }
}

template<class T1>
T1* StringMap<T1>::Get(String key)
{
    Int32 idx = Hash(key) % SIZE;
    
    if (m_Slot[idx] == NULL) return NULL;
        
    CElement *p1 = m_Slot[idx];
    while(p1 != NULL) {
        if (!p1->m_Key.Compare(key)) {
            return p1->m_Value;
        }
        
        p1 = p1->m_pNext;
    }
    
    return NULL;
}

template<class T1>
T1* StringMap<T1>::Remove(String key)
{
    Int32 idx = Hash(key) % SIZE;
    
    if (m_Slot[idx] == NULL) return NULL;
    else {
        CElement *p1 = m_Slot[idx];
        if (!p1->m_Key.Compare(key)) {
            T1 *p2 = p1->m_Value;
            p1->m_Value = NULL;
            m_Slot[idx] = p1->m_pNext;
            p1->m_pNext = NULL;
            delete p1;
            
            return p2;
        }

        while(p1->m_pNext != NULL) {
            if (!p1->m_pNext->m_Key.Compare(key)) {
                CElement *p3 = p1->m_pNext;
                p1->m_pNext = p3->m_pNext;
                p3->m_pNext = NULL;
                
                T1 *p2 = p3->m_Value;
                p3->m_Value = NULL;
                delete p3;
                
                return p2;
            }
            
            p1 = p1->m_pNext;
        }
        
        return NULL;
    }
}

#endif //__CMAP_H__
