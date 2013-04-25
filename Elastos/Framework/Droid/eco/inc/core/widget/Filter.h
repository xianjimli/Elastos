
#ifndef __FILTER_H__
#define __FILTER_H__

#include "ext/frameworkext.h"
#include <elastos/ElRefBase.h>
#include <elastos/AutoPtr.h>
#include <elastos/Vector.h>
#include <elastos/Mutex.h>

using namespace Elastos::Core::Threading;

class Filter : public ElRefBase, public IFilter
{
private:
    static const char* TAG;
    static const char* THREAD_NAME;
    static const Int32 FILTER_TOKEN = 0xD0D0F00D;
    static const Int32 FINISH_TOKEN = 0xDEADBEEF;

protected:
    /**
     * <p>Holds the results of a filtering operation. The results are the values
     * computed by the filtering operation and the number of these values.</p>
     */
    class FilterResults : public ElRefBase, public IFilterResults
    {
    public:
        CARAPI_(PInterface) Probe(
            /* [in]  */ REIID riid);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface *pObject,
            /* [out] */ InterfaceID *pIID);

    public:
        /**
         * <p>Contains all the values computed by the filtering operation.</p>
         */
        AutoPtr<IInterface> mValues;

        /**
         * <p>Contains the number of values computed by the filtering
         * operation.</p>
         */
        Int32 mCount;
    };

private:
    /**
     * <p>Holds the arguments of a filtering request as well as the results
     * of the request.</p>
     */
    class RequestArguments
    {
    public:
        /**
         * <p>The constraint used to filter the data.</p>
         */
        AutoPtr<ICharSequence> mConstraint;

        /**
         * <p>The listener to notify upon completion. Can be NULL.</p>
         */
        AutoPtr<IFilterListener> mListener;

        /**
         * <p>The results of the filtering operation.</p>
         */
        AutoPtr<IFilterResults> mResults;
    };

protected:
    Filter();

public:
    CARAPI_(PInterface) Probe(
        /* [in]  */ REIID riid);

    CARAPI_(UInt32) AddRef();

    CARAPI_(UInt32) Release();

    CARAPI GetInterfaceID(
        /* [in] */ IInterface *pObject,
        /* [out] */ InterfaceID *pIID);

    virtual CARAPI SetDelayer(
        /* [in] */ IFilterDelayer* delayer);

    virtual CARAPI DoFilter(
        /* [in] */ ICharSequence* constraint);

    virtual CARAPI DoFilterEx(
        /* [in] */ ICharSequence* constraint,
        /* [in] */ IFilterListener* listener);

    virtual CARAPI ConvertResultToString(
        /* [in] */ IInterface* resultValue,
        /* [out] */ ICharSequence** cs);

protected:
    /**
     * <p>Invoked in a worker thread to filter the data according to the
     * constraint. Subclasses must implement this method to perform the
     * filtering operation. Results computed by the filtering operation
     * must be returned as a {@link android.widget.Filter.FilterResults} that
     * will then be published in the UI thread through
     * {@link #publishResults(CharSequence,
     * android.widget.Filter.FilterResults)}.</p>
     *
     * <p><strong>Contract:</strong> When the constraint is NULL, the original
     * data must be restored.</p>
     *
     * @param constraint the constraint used to filter the data
     * @return the results of the filtering operation
     *
     * @see #filter(CharSequence, android.widget.Filter.FilterListener)
     * @see #publishResults(CharSequence, android.widget.Filter.FilterResults)
     * @see android.widget.Filter.FilterResults
     */
    virtual CARAPI PerformFiltering(
        /* [in] */ ICharSequence* constraint,
        /* [out] */ IFilterResults** filterResults) = 0;

    /**
     * <p>Invoked in the UI thread to publish the filtering results in the
     * user interface. Subclasses must implement this method to display the
     * results computed in {@link #performFiltering}.</p>
     *
     * @param constraint the constraint used to filter the data
     * @param results the results of the filtering operation
     *
     * @see #filter(CharSequence, android.widget.Filter.FilterListener)
     * @see #performFiltering(CharSequence)
     * @see android.widget.Filter.FilterResults
     */
    virtual CARAPI PublishResults(
        /* [in] */ ICharSequence* constraint,
        /* [in] */ IFilterResults* results) = 0;

private:
    CARAPI HandleFilterMessage(
        /* [in] */ RequestArguments* args);

    CARAPI HandleFinishMessage();

    CARAPI HandleResultsMessage(
        /* [in] */ RequestArguments* args);

private:
    AutoPtr<IFilterDelayer> mDelayer;

    Mutex mLock;

    AutoPtr<IApartment> mRequestApartment;
    AutoPtr<IApartment> mDefaultApartment;
};

#endif //__FILTER_H__
