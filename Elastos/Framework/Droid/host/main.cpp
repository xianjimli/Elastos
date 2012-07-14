
#include <stdio.h>
#include <elastos.h>
#include <elastos/AutoPtr.h>

using namespace Elastos;

void Usage()
{
    printf("Host.ecx MODULE_NAME CLASS_NAME \n");
}

int main(int argc, char* argv[])
{
    AutoPtr<IModuleInfo> moduleInfo;
    AutoPtr<IEnumInfo> enumInfo;
    AutoPtr<IEnumItemInfo> emumItemInfo;
    AutoPtr<IClassInfo> classInfo;
    AutoPtr<IInterface> object;
    AutoPtr<IMethodInfo> methodInfo;
    AutoPtr<IArgumentList> argumentList;
    Int32 value;
    char* moduleName;
    char* klassName;

    if (argc < 3) {
        Usage();
        return 0;
    }

    moduleName = argv[1];
    klassName = argv[2];

    char path[260];
    sprintf(path, "/data/data/com.elastos.runtime/elastos/%s", moduleName);
    ECode ec = _CReflector_AcquireModuleInfo(
            path, (IModuleInfo**)&moduleInfo);
    if (FAILED(ec)) {
        printf("Acquire \"%s\" module info failed!\n", moduleName);
        return 1;
    }

    ec = moduleInfo->GetEnumInfo("ApartmentAttr", (IEnumInfo**)&enumInfo);
    if (FAILED(ec)) {
        printf("Acquire enum \"ApartmentAttr\" failed!\n");
        return 1;
    }

    ec = enumInfo->GetItemInfo("ApartmentAttr_Current",
            (IEnumItemInfo**)&emumItemInfo);
    if (FAILED(ec)) {
        printf("Acquire enum item\"ApartmentAttr_Current\" failed!\n");
        return 1;
    }

    ec = moduleInfo->GetClassInfo(
            klassName, (IClassInfo**)&classInfo);
    if (FAILED(ec)) {
        printf("Acquire \"%s\" class info failed!\n", klassName);
        return 1;
    }

    ec = classInfo->CreateObject((IInterface**)&object);
    if (FAILED(ec)) {
        printf("Create object failed!\n");
        return 1;
    }

    ec = classInfo->GetMethodInfo(
            "Startup", (IMethodInfo**)&methodInfo);
    if (FAILED(ec)) {
        printf("Acquire \"Startup\" method info failed!\n");
        return 1;
    }

    ec = methodInfo->CreateArgumentList((IArgumentList**)&argumentList);
    if (FAILED(ec)) {
        printf("Acquire \"Startup\" method info failed!\n");
        return 1;
    }

    emumItemInfo->GetValue(&value);
    argumentList->SetInputArgumentOfEnum(0, value);

    ec = methodInfo->Invoke(object, argumentList);
    if (FAILED(ec)) {
        printf("Invoke \"Startup\" method failed!\n");
        return 1;
    }

    return 0;
}