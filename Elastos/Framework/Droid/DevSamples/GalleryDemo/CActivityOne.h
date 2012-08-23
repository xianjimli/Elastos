
#ifndef __CACTIVITYONE_H__
#define __CACTIVITYONE_H__

#include "CActivity.h"
#include "_CActivityOne.h"

class CActivityOne : public CActivity
{
public:
    class MyListener : public IViewOnTouchListener, public IViewOnKeyListener
    {
    public:
        MyListener(
            /* [in] */ CActivityOne* host);

        CARAPI_(PInterface) Probe(
            /* [in]  */ REIID riid);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface *pObject,
            /* [out] */ InterfaceID *pIID);

        CARAPI OnTouch(
            /* [in] */ IView* v,
            /* [in] */ IMotionEvent* event,
            /* [out] */ Boolean* result);

        CARAPI OnKey(
            /* [in] */ IView * v,
            /* [in] */ Int32 keyCode,
            /* [in] */ IKeyEvent* event,
            /* [out] */ Boolean* result);

    private:
        AutoPtr<CActivityOne> mHost;
        Int32 mRef;
    };

    class ImageAdapter : public IBaseAdapter, public ISpinnerAdapter
    {
    private:
        AutoPtr<IContext> mContext;
        static Int32 mImage[];
        static Int32 mImageLength;

    public:
        ImageAdapter(
            /* [in] */ IContext* c);

        CARAPI_(PInterface) Probe(
            /* [in]  */ REIID riid);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface *pObject,
            /* [out] */ InterfaceID *pIID);

        CARAPI RegisterDataSetObserver(
            /* [in] */ IDataSetObserver * pObserver);

        CARAPI UnregisterDataSetObserver(
            /* [in] */ IDataSetObserver * pObserver);

        CARAPI GetCount(
            /* [out] */ Int32 * pCount);

        CARAPI GetItem(
            /* [in] */ Int32 position,
            /* [out] */ IInterface ** ppItem);

        CARAPI GetItemId(
            /* [in] */ Int32 position,
            /* [out] */ Int64 * pItemId);

        CARAPI HasStableIds(
            /* [out] */ Boolean * pHasStableIds);

        CARAPI GetView(
            /* [in] */ Int32 position,
            /* [in] */ IView * pConvertView,
            /* [in] */ IViewGroup * pParent,
            /* [out] */ IView ** ppView);

        CARAPI GetItemViewType(
            /* [in] */ Int32 position,
            /* [out] */ Int32 * pViewType);

        CARAPI GetViewTypeCount(
            /* [out] */ Int32 * pCount);

        CARAPI IsEmpty(
            /* [out] */ Boolean * pIsEmpty);

        CARAPI AreAllItemsEnabled(
            /* [out] */ Boolean * pEnabled);

        CARAPI IsEnabled(
            /* [in] */ Int32 position,
            /* [out] */ Boolean * pEnabled);

        CARAPI NotifyDataSetChanged();

        CARAPI NotifyDataSetInvalidated();

        CARAPI GetDropDownView(
            /* [in] */ Int32 position,
            /* [in] */ IView* convertView,
            /* [in] */ IViewGroup* parent,
            /* [out] */ IView** view);

    private:
        Int32 mRef;
    };

protected:
    CARAPI OnCreate(
        /* [in] */ IBundle* savedInstanceState);

    CARAPI OnStart();

    CARAPI OnResume();

    CARAPI OnPause();

    CARAPI OnStop();

    CARAPI OnDestroy();

private:
    CARAPI OnActivityResult(
        /* [in] */ Int32 requestCode,
        /* [in] */ Int32 resultCode,
        /* [in] */ IIntent *data);

private:
    AutoPtr<IDrawable> mDrawable1;
    AutoPtr<IDrawable> mDrawable2;
};

#endif // __CACTIVITYONE_H__
