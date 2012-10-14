
#include "widget/Filter.h"
#include "os/CApartment.h"
#include "os/SystemClock.h"

const char* Filter::TAG = "Filter";
const char* Filter::THREAD_NAME = "Filter";
const Int32 Filter::FILTER_TOKEN;
const Int32 Filter::FINISH_TOKEN;

PInterface Filter::FilterResults::Probe(
    /* [in]  */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)this;
    }
    else if (riid == EIID_IFilterResults) {
        return (IFilterResults*)this;
    }

    return NULL;
}

UInt32 Filter::FilterResults::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 Filter::FilterResults::Release()
{
    return ElRefBase::Release();
}

ECode Filter::FilterResults::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    VALIDATE_NOT_NULL(pIID);

    if (pObject == (IInterface*)(IFilterResults*)this) {
        *pIID = EIID_IFilterResults;
    }
    else {
        return E_INVALID_ARGUMENT;
    }
    return NOERROR;
}

/**
 * <p>Creates a new asynchronous filter.</p>
 */
Filter::Filter()
{
    ASSERT_SUCCEEDED(CApartment::GetDefaultApartment(
        (IApartment**)&mDefaultApartment));
}

PInterface Filter::Probe(
    /* [in]  */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)this;
    }
    else if (riid == EIID_IFilter) {
        return (IFilter*)this;
    }

    return NULL;
}

UInt32 Filter::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 Filter::Release()
{
    return ElRefBase::Release();
}

ECode Filter::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    VALIDATE_NOT_NULL(pIID);

    if (pObject == (IInterface*)(IFilter*)this) {
        *pIID = EIID_IFilter;
    }
    else {
        return E_INVALID_ARGUMENT;
    }
    return NOERROR;
}

/**
 * Provide an interface that decides how Int64 to delay the message for a given query.  Useful
 * for heuristics such as posting a delay for the delete key to avoid doing any work while the
 * user holds down the delete key.
 *
 * @param delayer The delayer.
 * @hide
 */
ECode Filter::SetDelayer(
    /* [in] */ IFilterDelayer* delayer)
{
    Mutex::Autolock lock(mLock);
    mDelayer = delayer;

    return NOERROR;
}

/**
 * <p>Starts an asynchronous filtering operation. Calling this method
 * cancels all previous non-executed filtering requests and posts a new
 * filtering request that will be executed later.</p>
 *
 * @param constraint the constraint used to filter the data
 *
 * @see #filter(CharSequence, android.widget.Filter.FilterListener)
 */
ECode Filter::DoFilter(
    /* [in] */ ICharSequence* constraint)
{
    return DoFilterEx(constraint, NULL);
}

/**
 * <p>Starts an asynchronous filtering operation. Calling this method
 * cancels all previous non-executed filtering requests and posts a new
 * filtering request that will be executed later.</p>
 *
 * <p>Upon completion, the listener is notified.</p>
 *
 * @param constraint the constraint used to filter the data
 * @param listener a listener notified upon completion of the operation
 *
 * @see #filter(CharSequence)
 * @see #performFiltering(CharSequence)
 * @see #publishResults(CharSequence, android.widget.Filter.FilterResults)
 */
ECode Filter::DoFilterEx(
    /* [in] */ ICharSequence* constraint,
    /* [in] */ IFilterListener* listener)
{
    Mutex::Autolock lock(mLock);

    if (mRequestApartment == NULL) {
        ASSERT_SUCCEEDED(CApartment::New(FALSE, (IApartment**)&mRequestApartment));
        mRequestApartment->Start(ApartmentAttr_New);
    }

    Int64 delay = 0;
    if (mDelayer != NULL) {
        mDelayer->GetPostingDelay(constraint, &delay);
    }

    RequestArguments* args = new RequestArguments();
    // make sure we use an immutable copy of the constraint, so that
    // it doesn't change while the filter operation is in progress
    //
    String str;
    if (constraint) {
        constraint->ToString(&str);
    }
    CStringWrapper::New(str, (ICharSequence**)&args->mConstraint);
    args->mListener = listener;

    ECode (STDCALL Filter::*pHandlerFunc)(RequestArguments* args);

    pHandlerFunc = &Filter::HandleFilterMessage;

    AutoPtr<IParcel> params;
    CCallbackParcel::New((IParcel**)&params);
    params->WriteInt32((Handle32)args);

    //mThreadHandler.removeMessages(FILTER_TOKEN);
    //mThreadHandler.removeMessages(FINISH_TOKEN);

    return mRequestApartment->PostCppCallbackDelayed(
        (Handle32)this, *(Handle32*)&pHandlerFunc, params, 0, delay);
}

/**
 * <p>Converts a value from the filtered set into a CharSequence. Subclasses
 * should override this method to convert their results. The default
 * implementation returns an empty String for NULL values or the default
 * String representation of the value.</p>
 *
 * @param resultValue the value to convert to a CharSequence
 * @return a CharSequence representing the value
 */
ECode Filter::ConvertResultToString(
    /* [in] */ IInterface* resultValue,
    /* [out] */ ICharSequence** cs)
{
    VALIDATE_NOT_NULL(cs);
    String str;
    if (resultValue) {
        //resultValue->ToString(&str);
    }

    return CStringWrapper::New(str, cs);
}

ECode Filter::HandleFilterMessage(
    /* [in] */ RequestArguments* args)
{
    if (FAILED(PerformFiltering(args->mConstraint,
        (IFilterResults**)&args->mResults))) {
        args->mResults = new FilterResults();
    }

    ECode (STDCALL Filter::*pHandlerFunc)(RequestArguments* args);

    pHandlerFunc = &Filter::HandleResultsMessage;

    AutoPtr<IParcel> params;
    CCallbackParcel::New((IParcel**)&params);
    params->WriteInt32((Handle32)args);

    mDefaultApartment->PostCppCallback(
        (Handle32)this, *(Handle32*)&pHandlerFunc, params, 0);

    Mutex::Autolock lock(mLock);
    if (mRequestApartment != NULL) {
        ECode (STDCALL Filter::*pHandlerFunc)();

        pHandlerFunc = &Filter::HandleFinishMessage;

        mRequestApartment->PostCppCallbackDelayed(
            (Handle32)this, *(Handle32*)&pHandlerFunc, params, 0, 3000);
    }

    return NOERROR;
}

ECode Filter::HandleFinishMessage()
{
    Mutex::Autolock lock(mLock);
    if (mRequestApartment != NULL) {
//        mThreadHandler.getLooper().quit();
        mRequestApartment = NULL;
    }

    return NOERROR;
}

ECode Filter::HandleResultsMessage(
    /* [in] */ RequestArguments* args)
{
    PublishResults(args->mConstraint, args->mResults);
    if (args->mListener != NULL) {
        Int32 count = -1;
        if (args->mResults != NULL) {
            count = ((FilterResults*)args->mResults.Get())->mCount;
        }
        args->mListener->OnFilterComplete(count);
    }

    return NOERROR;
}
