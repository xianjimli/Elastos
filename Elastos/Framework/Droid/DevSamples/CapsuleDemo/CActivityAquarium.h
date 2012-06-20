
#ifndef __CACTIVITYAQUARIUM_H__
#define __CACTIVITYAQUARIUM_H__

#define BUBBLE_TOTAL 5
#include "CActivity.h"
#include "_CActivityAquarium.h"

#include <math.h>

class CActivityAquarium : public CActivity
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
    CARAPI OnClick(
    	/* [in] */ PInterface object);
    	
    CARAPI OnBubbleClick(
    	/* [in] */ PInterface object);
    	
    CARAPI OnFishClick(
    	/* [in] */ PInterface object);
    	
    CARAPI OnTimer(
        /* [in] */ PInterface pObject,
        /* [in] */ Int32 count);
    
    CARAPI BubbleRelocate(
        /* [in] */ Int32 idx);
    
    CARAPI FishRelocate(
        /* [in] */ Int32 idx);
    
    CARAPI OnFinish(
    	/* [in] */ PInterface pObject,
        /* [in] */ Int32 x,
        /* [in] */ Int32 y,
        /* [in] */ Timestamp64 timeStamp);
        
private:

//		int m_bubble_count_in_screen;
		CPictureBox* m_pBgPanel;
//		CPictureBox* m_pTouch;
		CPictureBox* m_pBubble[BUBBLE_TOTAL];
		CPictureBox* m_pFish1;
		CPictureBox* m_pFish2;
		IGrafixTimer* m_timer;

private:
        int bubble_count_in_screen;
        int bubble_offset_left;
        int random_tweak;
        int bubble_UserData[BUBBLE_TOTAL];
};

#endif // __CACTIVITYAQUARIUM_H__
