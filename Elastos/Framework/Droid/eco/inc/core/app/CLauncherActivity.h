
#ifndef __CLAUNCHERACTIVITY_H__
#define __CLAUNCHERACTIVITY_H__

#include "_CLauncherActivity.h"
#include "app/ListActivity.h"
#include "app/CLauncherActivityListItem.h"
#include "app/CLauncherActivityIconResizer.h"
#include "widget/BaseAdapter.h"
#include "widget/Filter.h"
#include "widget/AdapterMacro.h"
#include <elastos/List.h>
#include <elastos/Mutex.h>
#include <elastos/ElRefBase.h>

using namespace Elastos;
using namespace Elastos::Core::Threading;


CarClass(CLauncherActivity), public ListActivity
{
private:
    class ActivityAdapter : public BaseAdapter
    {
    private:
        /**
         * An array filters constrains the content of the array adapter with a prefix. Each
         * item that does not start with the supplied prefix is removed from the list.
         */
        class ArrayFilter : public Filter
        {
        public:
            ArrayFilter(ActivityAdapter* adapter);
        protected:
            //@Override
            virtual CARAPI PerformFiltering(
                /* [in] */ ICharSequence* prefix,
                /* [out] */ IFilterResults** filterResults);

            //@Override
            virtual CARAPI PublishResults(
                /* [in] */ ICharSequence* constraint,
                /* [in] */ IFilterResults* results);

        private:
            ActivityAdapter* mActivityAdapter;
        };

    public:
        ActivityAdapter(
            /* [in] */ ILauncherActivityIconResizer* resizer,
            /* [in] */ ILauncherActivity* lActivity);

        CARAPI_(AutoPtr<IIntent>) IntentForPosition(
            /* [in] */ Int32 position);

        CARAPI_(AutoPtr<ILauncherActivityListItem>) ItemForPosition(
            /* [in] */ Int32 position);

        CARAPI_(Int32) GetCount();

        CARAPI_(AutoPtr<IInterface>) GetItem(
            /* [in] */ Int32 position);

        CARAPI_(Int64) GetItemId(
            /* [in] */ Int32 position);

        CARAPI_(AutoPtr<IView>) GetView(
            /* [in] */ Int32 position,
            /* [in] */ IView* convertView,
            /* [in] */ IViewGroup* parent);

        CARAPI GetFilter(
            /* [out] */ IFilter** filter);

    private:
        CARAPI_(void) BindView(
            /* [in] */ IView* view,
            /* [in] */ ILauncherActivityListItem* item);

    protected:
        AutoPtr<ILauncherActivityIconResizer> mIconResizer;
        AutoPtr<ILayoutInflater> mInflater;

        List<AutoPtr<ILauncherActivityListItem> >* mActivitiesList;

    private:
        Mutex mLock;
        List<AutoPtr<ILauncherActivityListItem> >* mOriginalValues;
        AutoPtr<IFilter> mFilter;
        CLauncherActivity* mLauncherActivity;
    };

    /**
     * Adapter which shows the set of activities that can be performed for a given intent.
     */
    class _ActivityAdapter 
        : public ElRefBase
        , public ActivityAdapter
        , public IBaseAdapter
        , public IFilterable
    {
    public:
        _ActivityAdapter(
            /* [in] */ ILauncherActivityIconResizer* resizer,
            /* [in] */ ILauncherActivity* lActivity);

        ~_ActivityAdapter();

        IADAPTER_METHODS_DECL();

        ILISTADAPTER_METHODS_DECL();

        IBASEADAPTER_METHODS_DECL();

        CARAPI_(PInterface) Probe(
             /* [in]  */ REIID riid);

        CARAPI GetInterfaceID(
            /* [in] */ IInterface *pObject,
            /* [out] */ InterfaceID *pIID);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

         CARAPI GetFilter(
             /* [out] */ IFilter** filter);

    };

public:
    /**
     * Perform the query to determine which results to show and return a list of them.
     */
    CARAPI  MakeListItems(
        /* [out] */ IObjectContainer** listItems);

protected:
    //@Override
    virtual CARAPI OnCreate(
        /* [in] */ IBundle* icicle);

    /**
     * Override to call setContentView() with your own content view to
     * customize the list layout.
     */
    CARAPI OnSetContentView();

    //@Override
    virtual CARAPI OnListItemClick(
        /* [in] */ IListView* l,
        /* [in] */ IView* v,
        /* [in] */ Int32 position,
        /* [in] */ Int64 id);

    /**
     * Return the actual Intent for a specific position in our
     * {@link android.widget.ListView}.
     * @param position The item whose Intent to return
     */
    CARAPI_(AutoPtr<IIntent>) IntentForPosition(
        /* [in] */ Int32 position);

    /**
     * Return the {@link ListItem} for a specific position in our
     * {@link android.widget.ListView}.
     * @param position The item to return
     */
    CARAPI_(AutoPtr<ILauncherActivityListItem>) ItemForPosition(
        /* [in] */ Int32 position);

    /**
     * Get the base intent to use when running
     * {@link PackageManager#queryIntentActivities(Intent, int)}.
     */
    CARAPI_(AutoPtr<IIntent>) GetTargetIntent();

    /**
     * Perform query on package manager for list items.  The default
     * implementation queries for activities.
     */
    CARAPI_(AutoPtr<IObjectContainer>) OnQueryCapsuleManager(
        /* [in] */ IIntent* queryIntent);

public:
    AutoPtr<IIntent> mIntent;
    AutoPtr<ILocalCapsuleManager> mCapsuleManager;
    AutoPtr<ILauncherActivityIconResizer> mIconResizer;
};

#endif //__CLAUNCHERACTIVITY_H__

