#ifndef __CVM_H__
#define __CVM_H__

#include "Vm.h"
#include "_CVm.h"

CarClass(CVm), public Vm
{
public:
	CARAPI constructor();
	
    CARAPI Step(
        /** [in] **/ICallback* cb,
        /** [out] **/Boolean* result);

    CARAPI Compile(
        /** [out] **/Boolean* result);

    CARAPI Stop();

    CARAPI Finalize();
};
#endif //__CVM_H__