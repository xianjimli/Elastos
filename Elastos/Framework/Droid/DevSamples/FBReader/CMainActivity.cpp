
#include "FBReaderDef.h"
#include "CMainActivity.h"
#include "Uri.h"
#include <elastos/AutoPtr.h>
#include <stdio.h>
#include <vkey.h>
#include <elastos/Vector.h>

const char* g_books_path[4] = {
    "/data/data/com.elastos.runtime/elastos/FBReader/Books/MiniHelp.en.fb2",
    "/data/data/com.elastos.runtime/elastos/FBReader/Books/daodejing.epub",
    "/data/data/com.elastos.runtime/elastos/FBReader/Books/MiniHelp.zh.fb2",
    "/data/data/com.elastos.runtime/elastos/FBReader/Books/doyle-lost-world.epub"
};

CMainActivity::MyListener::MyListener(
    /* [in] */ CMainActivity* host) :
    mHost(host),
    mRef(0)
{
}

PInterface CMainActivity::MyListener::Probe(
    /* [in]  */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)(IViewOnTouchListener*)this;
    }
    else if (riid == EIID_IViewOnTouchListener) {
        return (IViewOnTouchListener*)this;
    }
    else if (riid == EIID_IViewOnKeyListener) {
        return (IViewOnKeyListener*)this;
    }

    return NULL;
}

UInt32 CMainActivity::MyListener::AddRef()
{
    // atomic_inc of android bionic C library will return
    // the old value of mRef before it is increased
    Int32 ref = atomic_inc(&mRef);
    // so we should increase ref before return
    return ++ref;
}

UInt32 CMainActivity::MyListener::Release()
{
    // atomic_inc of android bionic C library will return
    // the old value of mRef before it is decreased
    Int32 ref = atomic_dec(&mRef);
    // so we should decrease ref
    if (--ref == 0) {
        delete this;
    }
    assert(ref >= 0);
    return ref;
}

ECode CMainActivity::MyListener::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    if (pIID == NULL) {
        return E_INVALID_ARGUMENT;
    }

    if (pObject == (IInterface*)(IViewOnTouchListener*)this) {
        *pIID = EIID_IRunnable;
    }
    else {
        return E_INVALID_ARGUMENT;
    }
    return NOERROR;
}

ECode CMainActivity::MyListener::OnItemClick(
	/* [in] */ IAdapterView* parent,
	/* [in] */ IView* view,
	/* [in] */ Int32 position,
	/* [in] */ Int64 id)
{
	printf("OnItemClick position = %d, id = %d\n", position, id);

    Int32 listFlag = mHost->GetListFlag();
    switch(listFlag) {
        case CONTENTLIST: {
            mHost->ShowContentList(position);
            break;
        }
        case BOOKSLIST: {
            mHost->ShowBooksList(position);
            break;
        }
    }

	return NOERROR;
}

ECode CMainActivity::MyListener::OnTouch(
    /* [in] */ IView* v,
    /* [in] */ IMotionEvent* event,
    /* [out] */ Boolean* result)
{
    printf("OnTouch\n");
    AutoPtr<ICharSequence> csq;

    Int32 action;
    event->GetAction(&action);

    switch (action & MotionEvent_ACTION_MASK) {
    	case MotionEvent_ACTION_DOWN: {
			Float fx;
			event->GetX(&fx);

			Int32 x = (Int32)fx;
			Int32 width = 0;
			v->GetWidth(&width);

			if (x >= 0 && x <= width / 2) {
				printf("The previous page..................\n");

				if ( (IView*)mHost->FindViewById(0x7f050000) == v) {
					if (m_currentPageIndex > 0) {
						String str("");
						mHost->GetPageText(-- m_currentPageIndex, &str);
						CStringWrapper::New(str, (ICharSequence**)&csq);
						ITextView::Probe(v)->SetText(csq);
					}
				}
			}
			else if (x > width / 2 && x <= width) {
				printf("The next page..................1  pageCount = [%d]\n", mHost->GetPageCount());
				if (m_currentPageIndex + 1 < mHost->GetPageCount()) {
					printf("The next page..................2\n");
					String str("");
					mHost->GetPageText(++ m_currentPageIndex, &str);
					CStringWrapper::New(str, (ICharSequence**)&csq);
					ITextView::Probe(v)->SetText(csq);
				}
			}

			printf("==== File: %s, Line: %d ==== m_currentPageIndex = [%d] \n", __FILE__, __LINE__, m_currentPageIndex);
    	}

		default: {
			printf("OnTouch ----------- Not support now..................\n");
		}
    }

    if (result) {
        *result = TRUE;
    }

    return NOERROR;
}

ECode CMainActivity::MyListener::OnKey(
    /* [in] */ IView * v,
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event,
    /* [out] */ Boolean* result)
{
	printf("==== File: %s, Line: %d ==== ------------------------------keyCode = [%d]\n", __FILE__, __LINE__, keyCode);
    *result = FALSE;
    if (*result == FALSE) {
        return NOERROR;
    }

    Int32 action;
    event->GetAction(&action);
    Char16 label;
    event->GetDisplayLabel(&label);

    if (action == KeyEvent_ACTION_DOWN) {
        printf("==== File: %s, Line: %d ====, FUNC : %s.\n", __FILE__, __LINE__, __FUNCTION__);
    }
    else if (action == KeyEvent_ACTION_UP) {
        printf("==== File: %s, Line: %d ====, FUNC : %s.\n", __FILE__, __LINE__, __FUNCTION__);
		//menu key
		if (keyCode == 82) {
            Int32 listFlag = mHost->GetListFlag();
            switch(listFlag) {
                case CONTENTLIST: {
                    mHost->ShowContentList();
                    break;
                }
                case BOOKSLIST: {
                    mHost->ShowBooksList();
                    break;
                }
            }

		}
    }

    if (result) {
        *result = TRUE;
    }
printf("==== File: %s, Line: %d ====, FUNC : %s.\n", __FILE__, __LINE__, __FUNCTION__);
    return NOERROR;
}

Int32 CMainActivity::m_currentPageIndex = 0;

ECode CMainActivity::OnCreate(
    /* [in] */ IBundle* savedInstanceState)
{
    m_listFlag = NONE;
	m_pageCount = 0;
	m_currentPageIndex = 0;
    m_contentsListView = NULL;
    m_booksListView = NULL;

    SetContentView(0x7f020001);

    AutoPtr<IView> view1 = FindViewById(0x7f050000);
    assert(view1 != NULL);
    m_textView1 = (ITextView*)view1->Probe(EIID_ITextView);

    m_textView1->SetFocusable(TRUE);
    m_textView1->SetFocusableInTouchMode(TRUE);
	m_textView1->SetGravity(Gravity_LEFT | Gravity_TOP);

    AutoPtr<MyListener> l = new MyListener(this);
    m_textView1->SetOnTouchListener(l.Get());
    m_textView1->SetOnKeyListener(l.Get());

	m_textView1->SetTextSize(20);

	Float textSize = 0;
	ITypeface* typeface = NULL;

	m_textView1->GetTextSize(&textSize);
	m_textView1->GetTypeface(&typeface);

	Int32 lineHeight = 0;
	m_textView1->GetLineHeight(&lineHeight);

	Int32 height = 0, width = 0;
	m_textView1->GetHeight(&height);
	m_textView1->GetWidth(&width);

	AutoPtr<ITextPaint> 	mTextPaint;
	m_textView1->GetPaint((ITextPaint**)&mTextPaint);
	CBook::New(320, 455, lineHeight, String(g_books_path[0]), mTextPaint, (IBook**)&mBook);

    String title;
    mBook->GetTitle(&title);
	printf("==== File: %s, Line: %d ==== , title == [%s].\n", __FILE__, __LINE__, (const char*)title);

    AutoPtr<ICharSequence> csq;
	String str("");
	GetPageText(m_currentPageIndex, &str);
	CStringWrapper::New(str, (ICharSequence**)&csq);

    m_textView1->SetText(csq);

	LoadContentsListView();

    return NOERROR;
}

ECode CMainActivity::OnStart()
{
    // TODO: Add your code here
    return NOERROR;
}

ECode CMainActivity::OnResume()
{
    // TODO: Add your code here
    return NOERROR;
}

ECode CMainActivity::OnPause()
{
    // TODO: Add your code here
    return NOERROR;
}

ECode CMainActivity::OnStop()
{
    // TODO: Add your code here
    return NOERROR;
}

ECode CMainActivity::OnDestroy()
{
    // TODO: Add your code here
    return NOERROR;
}

ECode CMainActivity::OnActivityResult(
    /* [in] */ Int32 requestCode,
    /* [in] */ Int32 resultCode,
    /* [in] */ IIntent *data)
{
    return NOERROR;
}

ECode CMainActivity::GetPageText(
	/* [in] */Int32 index,
	/* [out] */ String* text)
{
	return mBook->GetTextNodeByIndex(index, text);
}

Int32 CMainActivity::GetPageCount()
{
	return m_pageCount;
}

ECode CMainActivity::ShowContentList(
	/* [in] */ Int32 pos)
{
    printf("==== File: %s, Line: %d ====, FUNC : %s.\n", __FILE__, __LINE__, __FUNCTION__);
	if (m_contentsListView == NULL) {
		return E_INVALID_ARGUMENT;
	}

    Int32 contentItemCount = 0;
    FAIL_RETURN(mBook->GetContentItemCount(&contentItemCount));
    printf("==== File: %s, Line: %d ====, FUNC : %s, contentItemCount=[%d].\n", __FILE__, __LINE__, __FUNCTION__, contentItemCount);
    if (contentItemCount == 0) {
        return NOERROR;
    }

	Int32 visibility;
	m_contentsListView->GetVisibility(&visibility);
	if (visibility == 0x00000000) {
		m_contentsListView->SetVisibility(View_GONE);
		m_textView1->SetVisibility(View_VISIBLE);

		if (pos != -1) {
			AutoPtr<ICharSequence> csq;
			String str("");
			mBook->GetTextNodeByChapterIndex(pos, &m_currentPageIndex, &str);

			CStringWrapper::New(str, (ICharSequence**)&csq);
			m_textView1->SetText(csq);
		}
	}
	else if (visibility == 0x00000008) {
		m_contentsListView->SetVisibility(View_VISIBLE);
		m_textView1->SetVisibility(View_GONE);
	}

	return NOERROR;
}

ECode CMainActivity::ShowBooksList(
    /* [in] */ Int32 pos)
{
    printf("==== File: %s, Line: %d ====, FUNC : %s.\n", __FILE__, __LINE__, __FUNCTION__);
    if (m_booksListView == NULL) {
        return E_INVALID_ARGUMENT;
    }

    Int32 visibility;
    m_booksListView->GetVisibility(&visibility);
    if (visibility == 0x00000000) {
        m_booksListView->SetVisibility(View_GONE);
        m_textView1->SetVisibility(View_VISIBLE);
    }
    else if (visibility == 0x00000008) {
        m_booksListView->SetVisibility(View_VISIBLE);
        m_textView1->SetVisibility(View_GONE);
    }

    if (pos != -1 && pos >= 0 && pos < 4 && mBook != NULL) {
        m_pageCount = 0;
        m_currentPageIndex = 0;

        //g_books_path
        mBook->LoadBook(String(g_books_path[pos]));
        String title;
        mBook->GetTitle(&title);
        printf("==== File: %s, Line: %d ==== , title == [%s].\n", __FILE__, __LINE__, (const char*)title);

        AutoPtr<ICharSequence> csq;
        String str("");
        GetPageText(m_currentPageIndex, &str);
        CStringWrapper::New(str, (ICharSequence**)&csq);

        m_textView1->SetText(csq);

        LoadContentsListView();

    }

    return NOERROR;
}

ECode CMainActivity::OnOptionsItemSelected(
    /* [in] */ IMenuItem* item,
    /* [out] */ Boolean* res)
{
    printf("==== File: %s, Line: %d ====, FUNC : %s.\n", __FILE__, __LINE__, __FUNCTION__);
    Int32 itemID = -1;
    item->GetItemId(&itemID);
    printf("==== File: %s, Line: %d ====, FUNC : %s, itemID=[0x%08x].\n", __FILE__, __LINE__, __FUNCTION__, itemID);

    // String title;
    // AutoPtr<ICharSequence> temp = NULL;
    // item->GetTitle((ICharSequence**) &temp);
    // assert(temp != NULL);

    // temp->ToString(&title);

    switch(itemID) {
        case 0x7f050003 /*showContent*/: {
            m_listFlag = CONTENTLIST;
            ShowContentList();

            break;
        }
        case 0x7f050004 /*loadBooks*/: {
            m_listFlag = BOOKSLIST;
            LoadBooksList();
            ShowBooksList();

            break;
        }
        case 0x7f050005 /*bookmark*/: {
            m_listFlag = BOOKMARK;

            break;
        }
    }

    return Activity::OnOptionsItemSelected(item, res);
}

ECode CMainActivity::OnCreateOptionsMenu(
    /* [in] */ IMenu* menu,
    /* [out] */ Boolean* allowToShow) {
    if (menu != NULL) {
        //1.
        // AutoPtr<IMenuItem> item = NULL;
        // AutoPtr<ICharSequence> csq;
        // printf("==== File: %s, Line: %d ====, FUNC : %s.\n", __FILE__, __LINE__, __FUNCTION__);
        // CStringWrapper::New(String("item1"), (ICharSequence**)&csq);
        // menu->AddEx2(0, 1, 1, csq, (IMenuItem**) &item);

        // if (item == NULL) {
        //     printf("==== File: %s, Line: %d ====, FUNC : %s.\n", __FILE__, __LINE__, __FUNCTION__);
        // }
        // else {
        //     //printf("==== File: %s, Line: %d ====, FUNC : %s, item=[0x%08x].\n", __FILE__, __LINE__, __FUNCTION__, item.Get());
        // }

        // CStringWrapper::New(String("item2"), (ICharSequence**)&csq);
        // menu->AddEx2(0, 2, 2, csq, (IMenuItem**) &item);


        //2. Created by xml
        AutoPtr<IMenuInflater> menuInflater = NULL;
        GetMenuInflater((IMenuInflater**) & menuInflater);
        if (menuInflater == NULL) {
            assert(0);
        }

        //R.menu.menu  --->0x7f040000
        menuInflater->Inflate(0x7f040000, menu);
    }

    return Activity::OnCreateOptionsMenu(menu, allowToShow);
}

ECode CMainActivity::LoadContentsListView()
{
    printf("==== File: %s, Line: %d ====, FUNC : %s.\n", __FILE__, __LINE__, __FUNCTION__);
    if (m_contentsListView == NULL) {
        AutoPtr<IView> view = FindViewById(0x7f050001);
        assert(view != NULL);
    	m_contentsListView = IListView::Probe(view);
    	AutoPtr<MyListener> l = new MyListener(this);
        m_contentsListView->SetOnKeyListener(l.Get());
    	m_contentsListView->SetOnItemClickListener((IOnItemClickListener*)l.Get());

        AutoPtr<IColorDrawable> drawable;
        CColorDrawable::New(0xFF0000FF, (IColorDrawable**)&drawable);
    	assert(drawable != NULL);

        m_contentsListView->SetDivider(drawable);
    	m_contentsListView->SetDividerHeight(1);
    }

    FAIL_RETURN(mBook->GetTextNodeCount(&m_pageCount));
	printf("==== File: %s, Line: %d ==== , m_pageCount == [%d].\n", __FILE__, __LINE__, m_pageCount);

	Int32 contentItemCount = 0;
	FAIL_RETURN(mBook->GetContentItemCount(&contentItemCount));

	AutoPtr<ICharSequence> cs;
	AutoPtr<IObjectContainer> strs = NULL;

    if (contentItemCount > 0) {
        CParcelableObjectContainer::New((IObjectContainer**)&strs);
    }

	for (Int32 j = 0; j < contentItemCount; j ++) {
		String itemText("");
		FAIL_RETURN(mBook->GetContentItemByIndex(j, &itemText));

		//printf("Line: %d ==== , j == [%d], itemText = [%s].\n", __LINE__, j, (const char*)itemText);

		CStringWrapper::New(String(itemText), (ICharSequence**)&cs);
        strs->Add(cs);
        cs = NULL;
	}

    if (strs != NULL) {
        AutoPtr<IArrayAdapter> adapter = NULL;
        CArrayAdapter::New(this, 0x7f020000, strs, (IArrayAdapter**)&adapter);
        assert(adapter != NULL);

        m_contentsListView->SetAdapter(adapter.Get());
    }

	return NOERROR;
}

ECode CMainActivity::LoadBooksList() {
    printf("==== File: %s, Line: %d ====, FUNC : %s.\n", __FILE__, __LINE__, __FUNCTION__);
    if (m_booksListView == NULL) {
        AutoPtr<IView> view = FindViewById(0x7f050002);
        assert(view != NULL);
        m_booksListView = IListView::Probe(view);
        AutoPtr<MyListener> l = new MyListener(this);
        m_booksListView->SetOnKeyListener(l.Get());
        m_booksListView->SetOnItemClickListener((IOnItemClickListener*)l.Get());

        AutoPtr<IColorDrawable> drawable;
        CColorDrawable::New(0xFF0000FF, (IColorDrawable**)&drawable);
        assert(drawable != NULL);

        m_booksListView->SetDivider(drawable);
        m_booksListView->SetDividerHeight(1);
    }

    AutoPtr<ICharSequence> cs;
    AutoPtr<IObjectContainer> strs = NULL;

    Int32 counts = 4;
    if (counts > 0) {
        CParcelableObjectContainer::New((IObjectContainer**)&strs);
    }

    for (Int32 j = 0; j < counts; j ++) {
        printf("Line: %d ==== , j == [%d], itemText = [%s].\n", __LINE__, j, g_books_path[j]);
        CStringWrapper::New(String(g_books_path[j]), (ICharSequence**)&cs);
        strs->Add(cs);
        cs = NULL;
    }

    if (strs != NULL) {
        AutoPtr<IArrayAdapter> adapter = NULL;
        CArrayAdapter::New(this, 0x7f020000, strs, (IArrayAdapter**)&adapter);
        assert(adapter != NULL);

        m_booksListView->SetAdapter(adapter.Get());
    }

    return NOERROR;
}
