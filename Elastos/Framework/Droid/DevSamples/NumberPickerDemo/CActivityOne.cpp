
#include "CActivityOne.h"
#include <elastos/AutoPtr.h>
#include <stdio.h>

ECode CActivityOne::OnCreate(
    /* [in] */ IBundle* savedInstanceState)
{
    SetContentView(0x7f030000);

    AutoPtr<IView> view = FindViewById(0x7f050000);
    AutoPtr<INumberPicker> numberPicker = INumberPicker::Probe(view);
    assert(numberPicker != NULL);
    ArrayOf<String>* rangs = ArrayOf<String>::Alloc(10);
    (*rangs)[0] = String("0");
    (*rangs)[1] = String("1");
    (*rangs)[2] = String("2");
    (*rangs)[3] = String("3");
    (*rangs)[4] = String("4");
    (*rangs)[5] = String("5");
    (*rangs)[6] = String("6");
    (*rangs)[7] = String("7");
    (*rangs)[8] = String("8");
    (*rangs)[9] = String("9");
    Int32 len = rangs->GetLength();
    numberPicker->SetRangeEx(0, 9, *rangs);
    return NOERROR;
}

ECode CActivityOne::OnStart()
{
    // TODO: Add your code here
    return NOERROR;
}

ECode CActivityOne::OnResume()
{
    // TODO: Add your code here
    return NOERROR;
}

ECode CActivityOne::OnPause()
{
    // TODO: Add your code here
    return NOERROR;
}

ECode CActivityOne::OnStop()
{
    // TODO: Add your code here
    return NOERROR;
}

ECode CActivityOne::OnDestroy()
{
    // TODO: Add your code here
    return NOERROR;
}

ECode CActivityOne::OnActivityResult(
    /* [in] */ Int32 requestCode,
    /* [in] */ Int32 resultCode,
    /* [in] */ IIntent *data)
{
    return NOERROR;
}
