
#ifndef __CIESTUNIT_H__
#define __CTESTUNIT_H__

#include<elastos.h>

using namespace Elastos;

class CTestUnit
{
public:
    CTestUnit();
    
    virtual ~CTestUnit();
    
    virtual CARAPI Run(BufferOf<String> &args) { return NOERROR; };
    
    ECode Start(BufferOf<String> &args);
    
    CARAPI Finish(Int32 flag);
    
private:
    CARAPI EnterEventLoop();

protected:
    IInterface *m_pCallbackContext;
};

#endif // __CTESTUNIT_H__
