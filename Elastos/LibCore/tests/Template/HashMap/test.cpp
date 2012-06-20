
#define __USE_MALLOC

#include <elastos.h>
#include <elastos/HashMap.h>
#include <stdio.h>

using namespace Elastos;

struct Eqstr
{
    Boolean operator()(const char* s1, const char* s2) const
    {
        return strcmp(s1, s2) == 0;
    }
};

void testHashMap1()
{
    HashMap<const char*, Int32, Hash<const char*>, Eqstr> days;
    
    days["january"] = 31;
    days["february"] = 28;
    days["march"] = 31;
    days["april"] = 30;
    days["may"] = 31;
    days["june"] = 30;
    days["july"] = 31;
    days["august"] = 31;
    days["september"] = 30;
    days["october"] = 31;
    days["november"] = 30;
    days["december"] = 31;
    
    printf("september -> %d\n", days["september"]);
    printf("june -> %d\n", days["june"]);
    printf("february -> %d\n", days["february"]);
    printf("december -> %d\n", days["december"]);
    
    HashMap<const char*, Int32, Hash<const char*>, Eqstr>::Iterator it1 = days.Begin();
    HashMap<const char*, Int32, Hash<const char*>, Eqstr>::Iterator it2 = days.End();
        
    for(; it1 != it2; ++it1) printf("%s ", it1->mFirst);
    printf("\n");    
}

struct Eqstring
{
    Boolean operator()(String s1, String s2) const
    {
        return s1.Compare(s2) == 0;
    }
};

void testHashMap2()
{
    HashMap<String, Int32, Hash<String>, Eqstring> days;
    
    days["january"] = 31;
    days["february"] = 28;
    days["march"] = 31;
    days["april"] = 30;
    days["may"] = 31;
    days["june"] = 30;
    days["july"] = 31;
    days["august"] = 31;
    days["september"] = 30;
    days["october"] = 31;
    days["november"] = 30;
    days["december"] = 31;
    
    printf("september -> %d\n", days["september"]);
    printf("june -> %d\n", days["june"]);
    printf("february -> %d\n", days["february"]);
    printf("december -> %d\n", days["december"]);
    
    HashMap<String, Int32, Hash<String>, Eqstring>::Iterator it1 = days.Begin();
    HashMap<String, Int32, Hash<String>, Eqstring>::Iterator it2 = days.End();
        
    for(; it1 != it2; ++it1) printf("%s ", it1->mFirst);
    printf("\n");    
}

int main(int argc, char *argv[])
{
    testHashMap1();
    testHashMap2();
    
    return 0;
}
