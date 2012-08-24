
#include "CActivityDialogBox.h"
#include "accord.h"

ECode CActivityDialogBox::OnCreate()
{
    SetContentView("/res/layout/dialogBox.xml");

    m_pDialogBox = (CForm *)GetControlById("dialogBox");

    m_pDialogBoxPanel = (CCanvasPanel *)GetControlById("dialogBoxPanel");

    m_pDialogBoxPanel->SetPosition(20,80);

    m_pDialogBoxPanel->SetElementImage(
    				"Panel.Background",
    				"/res/drawable/Images/dialogBox_bg.png");

	m_pIconBox = (CPictureBox *)GetControlById("iconBox");
    m_pIconBox->SetMargin(6, 40, 0, 0);
	m_pIconBox->SetImageAlignment(ImageAlign_Stretch);
	m_pIconBox->SetImageByPath("/res/drawable/Images/icon.png");

	m_pTitleLabel = (CLabel *)GetControlById(
					"titleLabel");
	m_pTitleLabel->SetMargin(6, 10, 0, 0);

	m_pDoShare = (CLabel *)GetControlById("doShare");
	m_pDoShare->SetMargin(0, 40, 0, 0);

	CLabel::AddStylusLiftedCallback(
					m_pDoShare, this, &CActivityDialogBox::OnDoTaskClick);

	m_pDoMove = (CLabel *)GetControlById("doMove");

	CLabel::AddStylusLiftedCallback(
					m_pDoMove, this, &CActivityDialogBox::OnDoTaskClick);

	m_pDoSetting = (CLabel *)GetControlById("doSetting");

	CLabel::AddStylusLiftedCallback(
					m_pDoSetting, this, &CActivityDialogBox::OnDoTaskClick);

  	m_pDoUninstall = (CLabel *)GetControlById("doUninstall");

	CLabel::AddStylusLiftedCallback(
					m_pDoUninstall, this, &CActivityDialogBox::OnDoTaskClick);

	m_pDoInfo = (CLabel *)GetControlById("doInfo");

	CLabel::AddStylusLiftedCallback(
					m_pDoInfo, this, &CActivityDialogBox::OnDoTaskClick);

	m_pReturnButton = (CPictureBox *)GetControlById(
					"returnButton");

	m_pReturnButton->SetImageAlignment(ImageAlign_Stretch);
	m_pReturnButton->SetImageByPath(
			"/res/drawable/Images/menu_button.png");
	CPictureBox::AddClickCallback(
	        m_pReturnButton, this, &CActivityDialogBox::OnFinish);

    return NOERROR;
}

ECode CActivityDialogBox::OnStart()
{
    // TODO: Add your code here
    return NOERROR;
}

ECode CActivityDialogBox::OnResume()
{
    // TODO: Add your code here
    return NOERROR;
}

ECode CActivityDialogBox::OnPause()
{
    // TODO: Add your code here
    return NOERROR;
}

ECode CActivityDialogBox::OnStop()
{
    // TODO: Add your code here
    return NOERROR;
}

ECode CActivityDialogBox::OnDestroy()
{

    CPictureBox::RemoveAllCallbacks(m_pReturnButton);

    CLabel::RemoveStylusLiftedCallback(m_pDoShare, this, &CActivityDialogBox::OnDoTaskClick);
    CLabel::RemoveStylusLiftedCallback(m_pDoInfo, this, &CActivityDialogBox::OnDoTaskClick);
    CLabel::RemoveStylusLiftedCallback(m_pDoUninstall, this, &CActivityDialogBox::OnDoTaskClick);
    CLabel::RemoveStylusLiftedCallback(m_pDoMove, this, &CActivityDialogBox::OnDoTaskClick);
    CLabel::RemoveStylusLiftedCallback(m_pDoSetting, this, &CActivityDialogBox::OnDoTaskClick);

    return NOERROR;
}

ECode CActivityDialogBox::OnDoTaskClick(
    /* [in] */ PInterface object,
    /* [in] */ Int32 x,
    /* [in] */ Int32 y,
    /* [in] */ Timestamp64 timeStamp)
{
    ECode ec = NOERROR;

    ICapsuleManager *pCapsuleManager = NULL;

    ec = Elastos::GetServiceManager()->GetService("CapsuleManager", (IInterface**)&pCapsuleManager);

	if (CObject::Compare(object, m_pDoShare))
    m_pTitleLabel->SetText("Share...");

    if (CObject::Compare(object, m_pDoInfo)){
        StringBuf_<20> infoString;
        String retString;

        infoString.Append("Author: ");
        pCapsuleManager->GetCapsuleInfo("CapsuleDemo","author.name", &retString);
        infoString.Append(retString);
        m_pTitleLabel->SetText(infoString);

    }

    if (CObject::Compare(object, m_pDoUninstall))
        m_pTitleLabel->SetText("Uninstall...");

    if (CObject::Compare(object, m_pDoSetting))
        m_pTitleLabel->SetText("Settings...");

    if (CObject::Compare(object, m_pDoMove)) {
//        IIntent *pIntent = NULL;
//        CIntent::New(&pIntent);
//        pIntent->SetComponentName("package", "activity");
//        pIntent->SetAction("move");
//        pIntent->PutInt32Extra("key", 10);
//        StartActivity(pIntent);
        IUri *pUri;
        IIntent *pIntent;
        CUri::Parse("CapsuleDemo", &pUri);
        CIntent::New(&pIntent);
        pIntent->SetData(pUri);
        pIntent->SetAction("Move");
        pIntent->SetComponentName("CapsuleDemo", "CActivityDashboard");

    	return StartActivity(pIntent);
    }

    return NOERROR;
}

ECode CActivityDialogBox::OnFinish(
    /* [in] */ PInterface object)
{
    ECode ec;

	SetResult(46, NULL);
	ec = Finish();

    return ec;
}