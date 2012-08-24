
#include "FBReaderDef.h"
#include "CMainActivity.h"
#include "CUri.h"
#include <elastos/AutoPtr.h>
#include <stdio.h>
#include <vkey.h>

typedef union {
	float f;
	int i;
}TESTTYPE;

//TESTTYPE g_testy = 100;


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

	return mHost->ShowContentList(position);
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

				if ( mHost->FindViewById(0x7f040000) == v) {
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

    Int32 action;
    event->GetAction(&action);
    Char16 label;
    event->GetDisplayLabel(&label);

    if (action == KeyEvent_ACTION_DOWN) {
        printf("Key '%c' is down on ImageView\n", (char)label);
    }
    else if (action == KeyEvent_ACTION_UP) {
        printf("Key '%c' is up on ImageView\n", (char)label);
		//menu key
		if (keyCode == 82) {
			mHost->ShowContentList();
		}
    }

    if (result) {
        *result = TRUE;
    }

    return NOERROR;
}

Int32 CMainActivity::m_currentPageIndex = 0;

ECode CMainActivity::OnCreate(
    /* [in] */ IBundle* savedInstanceState)
{
	m_pageCount = 0;
	m_currentPageIndex = 0;
printf("==== File: %s, Line: %d ==== \n", __FILE__, __LINE__);
    SetContentView(0x7f020001);
printf("==== File: %s, Line: %d ==== \n", __FILE__, __LINE__);
    AutoPtr<IView> view1 = FindViewById(0x7f040000);
    assert(view1 != NULL);
printf("==== File: %s, Line: %d ==== \n", __FILE__, __LINE__);
    m_textView1 = (ITextView*)view1->Probe(EIID_ITextView);

    m_textView1->SetFocusable(TRUE);
    m_textView1->SetFocusableInTouchMode(TRUE);
	m_textView1->SetGravity(Gravity_LEFT | Gravity_TOP);

    AutoPtr<MyListener> l = new MyListener(this);
    m_textView1->SetOnTouchListener(l.Get());
    m_textView1->SetOnKeyListener(l.Get());
	printf("==== File: %s, Line: %d ==== \n", __FILE__, __LINE__);

	m_textView1->SetTextSize(20);

	printf("==== File: %s, Line: %d ==== \n", __FILE__, __LINE__);
	Float textSize = 0;
	ITypeface* typeface = NULL;

	m_textView1->GetTextSize(&textSize);
	m_textView1->GetTypeface(&typeface);

	Int32 lineHeight = 0;
	m_textView1->GetLineHeight(&lineHeight);
	printf("==== File: %s, Line: %d ==== , m_textView1..................lineHeight	= [%d], textsize1 = [%d].textsize2 = [%f]\n", __FILE__, __LINE__, lineHeight, (unsigned int)textSize, (float)textSize);

	Int32 height = 0, width = 0;
	m_textView1->GetHeight(&height);
	m_textView1->GetWidth(&width);
	printf("==== File: %s, Line: %d ==1== height = [%d],  width = [%d]\n", __FILE__, __LINE__, height, width);

	AutoPtr<ITextPaint> 	mTextPaint;
	m_textView1->GetPaint((ITextPaint**)&mTextPaint);
	ECode ec = CBook::New(320, 455, lineHeight, mTextPaint, (IBook**)&mBook);
    printf("==== File: %s, Line: %d ==== , ec == [%0x].\n", __FILE__, __LINE__, ec);

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
	if (m_contentsListView == NULL) {
		return E_INVALID_ARGUMENT;
	}

	Int32 visibility;
	m_contentsListView->GetVisibility(&visibility);
	if (visibility == 0x00000000) {
//		m_contentsListView->SetVisibility(View::GONE);
//		m_textView1->SetVisibility(View::VISIBLE);
		m_contentsListView->SetVisibility(0x00000008);
		m_textView1->SetVisibility(0x00000000);

		if (pos != -1) {
			AutoPtr<ICharSequence> csq;
			String str("");
			mBook->GetTextNodeByChapterIndex(pos, &m_currentPageIndex, &str);

			CStringWrapper::New(str, (ICharSequence**)&csq);
			m_textView1->SetText(csq);
		}
	}
	else if (visibility == 0x00000008) {
//		m_contentsListView->SetVisibility(View::VISIBLE);
//		m_textView1->SetVisibility(View::GONE);
		m_contentsListView->SetVisibility(0x00000000);
		m_textView1->SetVisibility(0x00000008);
	}

	return NOERROR;
}

ECode CMainActivity::LoadContentsListView()
{
    AutoPtr<IView> view = FindViewById(0x7f040001);
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
	
    FAIL_RETURN(mBook->GetTextNodeCount(&m_pageCount));
	printf("==== File: %s, Line: %d ==== , m_pageCount == [%d].\n", __FILE__, __LINE__, m_pageCount);

	Int32 contentItemCount = 0;
	FAIL_RETURN(mBook->GetContentItemCount(&contentItemCount));
	printf("==== File: %s, Line: %d ==== , contentItemCount == [%d].\n", __FILE__, __LINE__, contentItemCount);

	AutoPtr<ICharSequence> cs;
	AutoPtr<IObjectContainer> strs;
    CParcelableObjectContainer::New((IObjectContainer**)&strs);

	for (Int32 j = 0; j < contentItemCount; j ++) {
		String itemText("");
		FAIL_RETURN(mBook->GetContentItemByIndex(j, &itemText));

		printf("Line: %d ==== , j == [%d], itemText = [%s].\n", __LINE__, j, (const char*)itemText);

		CStringWrapper::New(String(itemText), (ICharSequence**)&cs);
        strs->Add(cs);
        cs = NULL;
	} 

    AutoPtr<IArrayAdapter> adapter;
    CArrayAdapter::New(this, 0x7f020000, strs, (IArrayAdapter**)&adapter);
    assert(adapter != NULL);

    m_contentsListView->SetAdapter(adapter.Get());

	return NOERROR;
}

