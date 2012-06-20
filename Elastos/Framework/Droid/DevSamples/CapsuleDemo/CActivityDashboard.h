
#ifndef __CACTIVITYDASHBOARD_H__
#define __CACTIVITYDASHBOARD_H__
#include "CActivity.h"
#include "_CActivityDashboard.h"

class CActivityDashboard : public CActivity
{
public:

protected:
    CARAPI OnCreate();
    
    CARAPI OnStart();
    
    CARAPI OnResume();
    
    CARAPI OnPause();
    
    CARAPI OnStop();
    
    CARAPI OnDestroy();
    
    CARAPI OnNewIntent(
        /* [in] */ IIntent *intent);

private:
    CARAPI OnTimer(
	    /* [in] */ PInterface pObject,
	    /* [in] */ Int32 count);
    
    CARAPI OnClick(
    	/* [in] */ PInterface object);

    CARAPI OnTitleClick(
    	/* [in] */ PInterface object,
    	/* [in] */ Int32 x,
      	/* [in] */ Int32 y,
      	/* [in] */ Timestamp64 timeStamp);
    
    CARAPI OnStylusHeld(
    	/* [in] */ PInterface pObject,
      	/* [in] */ Int32 x,
      	/* [in] */ Int32 y,
      	/* [in] */ Timestamp64 timeStamp);
    
    CARAPI OnExitClick(
        /* [in] */ PInterface object);
        
    CARAPI OnActivityResult(
        /* [in] */ Int32 requestCode,
        /* [in] */ Int32 resultCode,
        /* [in] */ IIntent *pData);

private:
	CForm* 			m_pWindow;
	CGridPanel* 	m_pClosetPanel;
	CStackPanel* 	m_pCapsulePanel;
	CPictureBox* 	m_pCapsuleBox;
	CPictureBox* 	m_pTopBar;
	CLabel* 		m_pCapsuleTitle;
	CPictureBox* 	m_pPageBar;
	CPictureBox* 	m_pExitButton;
	IGrafixTimer*	m_pClock;
	Boolean 		m_isMovable;
	Boolean 		m_isFocused;
	Int32 			m_ticker;
};

#endif // __CACTIVITYDASHBOARD_H__
