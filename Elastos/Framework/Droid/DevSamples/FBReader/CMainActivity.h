
#ifndef __CMAINACTIVITY_H__
#define __CMAINACTIVITY_H__

#include "Activity.h"
#include "_CMainActivity.h"
#include "CBook.h"

enum LISTFLAG {
    NONE,
    CONTENTLIST,
    BOOKSLIST,
    BOOKMARK
};

class CMainActivity : public Activity
{
public:
    class MyListener : public IViewOnTouchListener, public IViewOnKeyListener, public IOnItemClickListener
    {
    public:
        MyListener(
            /* [in] */ CMainActivity* host);

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

		CARAPI OnItemClick(
			/* [in] */ IAdapterView* parent,
			/* [in] */ IView* view,
			/* [in] */ Int32 position,
			/* [in] */ Int64 id);

    private:
        AutoPtr<CMainActivity> mHost;
        Int32 mRef;
    };

public:
    virtual CARAPI OnCreateOptionsMenu(
        /* [in] */ IMenu* menu,
        /* [out] */ Boolean* allowToShow);

    virtual CARAPI OnOptionsItemSelected(
        /* [in] */ IMenuItem* item,
        /* [out] */ Boolean* res);

protected:
    CARAPI OnCreate(
        /* [in] */ IBundle* savedInstanceState);

    CARAPI OnStart();

    CARAPI OnResume();

    CARAPI OnPause();

    CARAPI OnStop();

    CARAPI OnDestroy();

	Int32 GetPageCount();

private:
    CARAPI OnActivityResult(
        /* [in] */ Int32 requestCode,
        /* [in] */ Int32 resultCode,
        /* [in] */ IIntent *data);

	ECode GetPageText(
		 /* [in] */ Int32 index,
		 /* [out] */ String* text);

	ECode LoadContentsListView();

    ECode LoadBooksList();

	ECode ShowContentList(
		 /* [in] */ Int32 pos = -1);

    ECode ShowBooksList(
        /* [in] */ Int32 pos = -1);

    Int32 GetListFlag() {
        return m_listFlag;
    }

private:
    AutoPtr<IBook>     mBook;
    AutoPtr<ITextView> m_textView1;
	AutoPtr<IListView> m_contentsListView;
    AutoPtr<IListView> m_booksListView;

    LISTFLAG           m_listFlag;

	Int32 			   m_pageCount;
	static Int32 	   m_currentPageIndex;
};


#endif // __CMAINACTIVITY_H__
