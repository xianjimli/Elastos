
#ifndef __CACTIVITYDIALOGBOX_H__
#define __CACTIVITYDIALOGBOX_H__

#include "CActivity.h"
#include "_CActivityDialogBox.h"

class CActivityDialogBox : public CActivity
{
public:

protected:
    CARAPI OnCreate();
    
    CARAPI OnStart();
    
    CARAPI OnResume();
    
    CARAPI OnPause();
    
    CARAPI OnStop();
    
    CARAPI OnDestroy();

private:
    CARAPI OnDoTaskClick(
    	/* [in] */ PInterface object,
      	/* [in] */ Int32 x,
      	/* [in] */ Int32 y,
      	/* [in] */ Timestamp64 timeStamp);
    
    CARAPI OnFinish(
        /* [in] */ PInterface object);
private:
		CForm* m_pDialogBox;
		CCanvasPanel* m_pDialogBoxPanel;
		CPictureBox* m_pIconBox;
		CPictureBox* m_pReturnButton;
		CLabel* m_pTitleLabel;
		CLabel* m_pDoShare;
		CLabel* m_pDoMove;
		CLabel* m_pDoSetting;
		CLabel* m_pDoInfo;
		CLabel* m_pDoUninstall;
};

#endif // __CACTIVITYDIALOGBOX_H__
