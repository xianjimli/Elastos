
#include "CActivityDashboard.h"

ECode CActivityDashboard::OnCreate()
{
    ECode ec;

    SetContentView("/res/drawable/mainForm.xml");

    m_isMovable = false;
	m_isFocused = false;
	m_ticker = 0;

	m_pWindow = (CForm *)GetControlById(
    				"Window");
    m_pWindow->SetElementImage("Form.Background",
    				"/res/drawable/Images/dashboard_bg.png");

	m_pClosetPanel = (CGridPanel *)GetControlById(
    				"closetPanel");

    m_pCapsuleTitle = (CLabel *)GetControlById("capsuleTitle");
	CLabel::AddStylusLiftedCallback(
			m_pCapsuleTitle, this, &CActivityDashboard::OnTitleClick);
	CLabel::AddStylusHeldCallback(
			m_pCapsuleTitle, this, &CActivityDashboard::OnStylusHeld);

    m_pCapsuleBox = (CPictureBox *)GetControlById("capsuleBox");
	m_pCapsuleBox->SetImageAlignment(ImageAlign_Stretch);
	m_pCapsuleBox->SetImageByPath("/res/drawable/Images/icon.png");
	CPictureBox::AddClickCallback(
			m_pCapsuleBox, this, &CActivityDashboard::OnClick);
	CPictureBox::AddStylusHeldCallback(
			m_pCapsuleBox, this, &CActivityDashboard::OnStylusHeld);

	m_pPageBar = (CPictureBox *)GetControlById("pageBar");
    m_pPageBar->SetImageAlignment(ImageAlign_HCenter);

	m_pPageBar->SetImageByPath("/res/drawable/Images/page_bar.png");

    m_pExitButton = (CPictureBox *)GetControlById("exitButton");
	m_pExitButton->SetImageAlignment(ImageAlign_Stretch);

	m_pExitButton->SetImageByPath("/res/drawable/Images/core_button.png");
	CPictureBox::AddClickCallback(
		    m_pExitButton, this, &CActivityDashboard::OnExitClick);

    ec = CGrafixTimer::New(500, &m_pClock);
//    ERROR_RETURN(ec);

    CGrafixTimer::AddTimerCallback(
		    m_pClock, this, &CActivityDashboard::OnTimer);

    return ec;
}

ECode CActivityDashboard::OnStart()
{
    // TODO: Add your code here
//    pIntent = GetIntent();
//    pIntent->GetAction();
//    pIntent->GetInt32Extra("key", &i);

    return NOERROR;
}

ECode CActivityDashboard::OnResume()
{
    // TODO: Add your code here
    return NOERROR;
}

ECode CActivityDashboard::OnPause()
{
    // TODO: Add your code here
    return NOERROR;
}

ECode CActivityDashboard::OnStop()
{
    // TODO: Add your code here
    return NOERROR;
}

ECode CActivityDashboard::OnDestroy()
{
    CPictureBox::RemoveAllCallbacks(m_pCapsuleBox);
    CPictureBox::RemoveAllCallbacks(m_pExitButton);

    CLabel::RemoveStylusLiftedCallback(m_pCapsuleTitle, this, &CActivityDashboard::OnTitleClick);
    CLabel::RemoveStylusLiftedCallback(m_pCapsuleTitle, this, &CActivityDashboard::OnStylusHeld);

    CGrafixTimer::RemoveTimerCallback(m_pClock, this, &CActivityDashboard::OnTimer);

    return NOERROR;
}

ECode CActivityDashboard::OnClick(
    /* [in] */ PInterface pObject)
{
 	ECode ec;

 	IIntent *pIntent;
	CIntent::New(&pIntent);

	if (!m_isMovable){
		pIntent->SetComponentName("CapsuleDemo", "CActivityAquarium");
		ec = StartActivityForResult(pIntent, 23);
    }

    return ec;
}

ECode CActivityDashboard::OnTitleClick(
    /* [in] */ PInterface pObject,
    /* [in] */ Int32 x,
    /* [in] */ Int32 y,
    /* [in] */ Timestamp64 timeStamp)
{
 	ECode ec;

 	IIntent *pIntent;
	CIntent::New(&pIntent);

	if (!m_isMovable){
		pIntent->SetComponentName("CapsuleDemo", "CActivityAquarium");
		ec = StartActivityForResult(pIntent, 23);
    }

    return ec;
}

ECode CActivityDashboard::OnExitClick(
    /* [in] */ PInterface object)
{
    ECode ec;

	ec = Finish();

    return ec;
}

ECode CActivityDashboard::OnTimer(
    /* [in] */ PInterface pObject,
    /* [in] */ Int32 count)
{
    if (m_isMovable){
       if (!m_isFocused){
           m_pCapsuleBox->SetImageByPath("/res/drawable/Images/icon.png");
           m_isFocused = true;
           m_pClosetPanel->ShowGridLines(false);
       }
       else{
           m_pCapsuleBox->SetImageByPath("/res/drawable/Images/icon_focused.png");
           m_pClosetPanel->ShowGridLines(true);
           m_isFocused = false;
       }

        m_ticker = m_ticker + 1;
        if (m_ticker == 20){
            m_ticker = 0;
            m_pClock->Stop();
            m_pCapsuleBox->SetImageByPath("Images/icon.png");
            m_isMovable = false;
            m_isFocused = false;
            m_pClosetPanel->ShowGridLines(false);
        }
    }

    return NOERROR;
}

ECode CActivityDashboard::OnStylusHeld(
    /* [in] */ PInterface pObject,
    /* [in] */ Int32 x,
    /* [in] */ Int32 y,
    /* [in] */ Timestamp64 timeStamp)
{
    ECode ec;
    if (!m_isMovable){
        IIntent *pIntent;
	    CIntent::New(&pIntent);
		pIntent->SetComponentName("CapsuleDemo", "CActivityDialogBox");
        ec = StartActivityForResult(pIntent, 46);
    }
    else{
        m_pCapsuleBox->SetImageByPath("Images/icon.png");
        m_pClosetPanel->ShowGridLines(false);
        m_pClock->Stop();
        m_isMovable = false;
        m_ticker = 0;
        m_isFocused = false;
    }

    return NOERROR;
}

ECode CActivityDashboard::OnNewIntent(
    /* [in] */ IIntent *intent)
{
    String action;
    IUri *pUri;
    String uriString;

    intent->GetAction(&action);
    intent->GetData(&pUri);
    pUri->ToString(&uriString);

    if (!action.Compare("Move")) {
        m_isMovable = true;
        if (!uriString.Compare("CapsuleDemo")) {
            m_pClock->Start();
            m_ticker = 0;
        }
    }

    return NOERROR;
}

ECode CActivityDashboard::OnActivityResult(
    /* [in] */ Int32 requestCode,
    /* [in] */ Int32 resultCode,
    /* [in] */ IIntent *pData)
{
    // TODO: Add your code here
    return NOERROR;
}