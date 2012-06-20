
#include "CActivityAquarium.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define SCREEN_WIDTH  320
#define SCREEN_HEIGHT 480
#define WIN_WIDTH 320
#define WIN_HEIGHT 480
#define WIN_LEFT 0
#define WIN_TOP 0
#define WIN_LEFT 0
#define BUBBLE_WIDTH 15
#define BUBBLE_HEIGHT 15

#define FISH_WIDTH 48
#define FISH_HEIGHT 30

typedef struct FishData FishData;
struct FishData{
    Int32 timer;
    Int32 left;
    Int32 top;
    Int32 offset_x;
    Int32 offset_y;
    Int32 flag;
    Int32 speed;
};

FishData g_fishData[2] = {
    { 0, 160, 240, 3, 2, 0, 12 },
    { 0, 80,  120, -4, 2, 0, 15 }
};

ECode CActivityAquarium::OnCreate()
{
    SetContentView("/res/layout/appForm.xml");

    m_pBgPanel = (CPictureBox *)GetControlById("bgPanel");
    m_pBgPanel->SetImageAlignment(ImageAlign_Stretch);
    m_pBgPanel->SetImageByPath("/res/drawable/Images/aquarium.png");
	CPictureBox::AddClickCallback(
	            m_pBgPanel, this, &CActivityAquarium::OnClick);
	CPictureBox::AddStylusHeldCallback(
	            m_pBgPanel, this, &CActivityAquarium::OnFinish);

//		m_pTouch = (CPictureBox *)GetControlById("touch");
//		m_pTouch->SetPosition(0,0);
//		CPictureBox::AddClickCallback(
//		            m_pTouch, this, &CActivityAquarium::OnClick);
//		CPictureBox::AddStylusHeldCallback(
//		            m_pTouch, this, &CActivityAquarium::OnActivityResult);

    String bubble[] = {"bubble1","bubble2","bubble3","bubble4","bubble5"};
    int i;

    time_t timer;
    time(&timer);
    srand(timer - 10000000);
    srand(rand());

    for (i=0; i<BUBBLE_TOTAL; i++){
	    m_pBubble[i] = (CPictureBox *)GetControlById(bubble[i]);
	    m_pBubble[i]->SetPosition(30+54*i, 480+div(rand(),1000).rem);
	    m_pBubble[i]->SetImageAlignment(ImageAlign_Stretch);
	    m_pBubble[i]->SetImageByPath(
	    		"/res/drawable/bubble.png");
	    CPictureBox::AddClickCallback(
	            m_pBubble[i], this, &CActivityAquarium::OnBubbleClick);
    }

	m_pFish1 = (CPictureBox *)GetControlById("fish1");
	m_pFish1->SetPosition(g_fishData[0].left,g_fishData[0].top);
	m_pFish1->SetImageAlignment(ImageAlign_Stretch);
	m_pFish1->SetImageByPath("/res/drawable/Images/fishR.png");
	CPictureBox::AddClickCallback(
	        m_pFish1, this, &CActivityAquarium::OnFishClick);

	m_pFish2 = (CPictureBox *)GetControlById("fish2");
	m_pFish2->SetPosition(g_fishData[1].left,g_fishData[1].top);
	m_pFish2->SetImageAlignment(ImageAlign_Stretch);
	m_pFish2->SetImageByPath("/res/drawable/Images/fishL.png");
	CPictureBox::AddClickCallback(
	        m_pFish2, this, &CActivityAquarium::OnFishClick);


    ECode ec = CGrafixTimer::New(550, &m_timer);
    CGrafixTimer::AddTimerCallback(m_timer, this, &CActivityAquarium::OnTimer);

    return ec;
}

ECode CActivityAquarium::OnStart()
{
    bubble_count_in_screen = 0;
    bubble_offset_left = 2;

    random_tweak = 3;

    m_timer->Start();

    return NOERROR;
}

ECode CActivityAquarium::OnResume()
{
    // TODO: Add your code here
    return NOERROR;
}

ECode CActivityAquarium::OnPause()
{
    // TODO: Add your code here
    return NOERROR;
}

ECode CActivityAquarium::OnStop()
{
    // TODO: Add your code here
    return NOERROR;
}

ECode CActivityAquarium::OnDestroy()
{
	CPictureBox::RemoveAllCallbacks(m_pBgPanel);

    for (int i=0; i<BUBBLE_TOTAL; i++){
		CPictureBox::RemoveAllCallbacks(m_pBubble[i]);
    }

    CPictureBox::RemoveAllCallbacks(m_pFish1);
    CPictureBox::RemoveAllCallbacks(m_pFish2);

    if (m_timer) {
        CGrafixTimer::RemoveAllCallbacks(m_timer);
        m_timer->Release();
        m_timer = NULL;
    }

    return NOERROR;
}

ECode CActivityAquarium::OnTimer(
    /* [in] */ PInterface pObject,
    /* [in] */ Int32 count)
{
    FishRelocate(0);
    FishRelocate(1);

    int i;
	for (i=0; i<BUBBLE_TOTAL; i++){

        if (bubble_UserData[i] != 0)
            BubbleRelocate(i);

        if (bubble_UserData[i] == 0 && bubble_count_in_screen < BUBBLE_TOTAL){
            bubble_UserData[i] = i + 1;       // bubble index
            bubble_count_in_screen = bubble_count_in_screen + 1;
        }
    }
    return NOERROR;
}

ECode CActivityAquarium::FishRelocate(
        /* [in] */ Int32 idx)
{
    CPictureBox* currentFish;
    time_t timer;
    time(&timer);
    srand(timer - 10000000);
    srand(rand());
    div_t div_result;

	if (idx == 0) {
	    currentFish = m_pFish1;
	    if (g_fishData[idx].timer>15) {
            g_fishData[idx].flag = 0;
            g_fishData[idx].timer = 0;
            g_fishData[idx].offset_x = 3;

            random_tweak = rand()*20/100000 + random_tweak;
            if (random_tweak>20000) random_tweak = 10;

            div_result = div(rand(),100);
            if (div_result.rem >= 55) g_fishData[idx].offset_x = -g_fishData[idx].offset_x;

            if (div_result.rem<65) {
                g_fishData[idx].offset_y = -g_fishData[idx].offset_y;
            }
            else{
                if (div_result.rem<40) g_fishData[idx].flag = 1;       //move horizontal
            }
        }
    }

    if (idx == 1) {
	    currentFish = m_pFish2;
	    if (g_fishData[idx].timer>12) {
            g_fishData[idx].flag = 0;
            g_fishData[idx].timer = 0;
            g_fishData[idx].offset_x = -4;

            random_tweak = rand()*15/100000 + random_tweak;
            if (random_tweak>1500) random_tweak = 12;

            div_result = div(rand(),100);
            if (div_result.rem>=60) g_fishData[idx].offset_x = -g_fishData[idx].offset_x;

            if (div_result.rem>66) {
                g_fishData[idx].offset_y = -g_fishData[idx].offset_y;
            }
            else{
                if (div_result.rem<40) g_fishData[idx].flag = 1;       //move horizontal
            }
        }
    }

    g_fishData[idx].timer = g_fishData[idx].timer+1;
    currentFish->GetX(&g_fishData[idx].left);
    currentFish->GetY(&g_fishData[idx].top);

    g_fishData[idx].top = g_fishData[idx].top + g_fishData[idx].offset_y;

    g_fishData[idx].left = g_fishData[idx].left + g_fishData[idx].offset_x;

    if (g_fishData[idx].top < 0 || g_fishData[idx].top > (WIN_HEIGHT - FISH_HEIGHT - 5)) {
        g_fishData[idx].top = g_fishData[idx].top - g_fishData[idx].offset_y;
        g_fishData[idx].offset_y = -g_fishData[idx].offset_y;
    }

    if (g_fishData[idx].left < 0 || g_fishData[idx].left > (WIN_WIDTH - FISH_WIDTH)){
        g_fishData[idx].left = g_fishData[idx].left - g_fishData[idx].offset_x;
        g_fishData[idx].offset_x = -g_fishData[idx].offset_x;
    }

    currentFish->SetPosition(g_fishData[idx].left, g_fishData[idx].top);

    if (g_fishData[idx].offset_x >= 0)
        currentFish->SetImageByPath("/res/drawable/Images/fishR.png");
    else
        currentFish->SetImageByPath("/res/drawable/Images/fishL.png");      // reverse image

    return NOERROR;
}

ECode CActivityAquarium::BubbleRelocate(
        /* [in] */ Int32 idx)
{
    int offset_top = 10;

    int bubble_left;
    int bubble_top;

    m_pBubble[idx]->GetX(&bubble_left);
    m_pBubble[idx]->GetY(&bubble_top);
    div_t div_result;
    time_t timer;
    time(&timer);
    srand((unsigned)timer);
    srand(rand());

    div_result = div(rand(),2);
    if (div_result.rem == 0)
        bubble_left = bubble_left + bubble_offset_left;
    else
        bubble_left = bubble_left - bubble_offset_left;

    if (bubble_left<0)
        bubble_left = bubble_left + bubble_offset_left;

    if (bubble_left>(WIN_WIDTH - BUBBLE_WIDTH))
           bubble_left = bubble_left - bubble_offset_left;


//    bubble_offset_left = -bubble_offset_left;       // shivering effect

    bubble_top = bubble_top - offset_top + div(rand(),10).rem;
    if (bubble_top <= 0) {
        bubble_UserData[idx] = 0;
        bubble_count_in_screen = bubble_count_in_screen - 1;
        bubble_top = bubble_top + WIN_HEIGHT + BUBBLE_WIDTH + div(rand(),1000).rem;
    }
    m_pBubble[idx]->SetPosition(bubble_left, bubble_top);

    return NOERROR;
}

ECode CActivityAquarium::OnClick(
    /* [in] */ PInterface object)
{

    int i;
	for (i=0; i<BUBBLE_TOTAL; i++){
		m_pBubble[i]->SetVisible(true);
    }

//    m_pFish1->SetPosition(160,240);
//    m_pFish2->SetPosition(80,120);

    return NOERROR;
}

ECode CActivityAquarium::OnBubbleClick(
    /* [in] */ PInterface object)
{
    for (int i=0; i<BUBBLE_TOTAL; i++){
		if (CObject::Compare(object, m_pBubble[i])) {
	        m_pBubble[i]->SetVisible(FALSE);
	    }
  	}

    return NOERROR;
}

ECode CActivityAquarium::OnFishClick(
    /* [in] */ PInterface object)
{
    CPictureBox* currentFish;
    int idx = 0;


	if (CObject::Compare(object, m_pFish1)) {
	    currentFish = m_pFish1;
	    idx = 0;
    }
    if (CObject::Compare(object, m_pFish2)) {
	    currentFish = m_pFish2;
	    idx = 1;
    }

    g_fishData[idx].timer = 0;              //fish runaway
    g_fishData[idx].offset_x = g_fishData[idx].speed;

    return NOERROR;
}

ECode CActivityAquarium::OnFinish(
    /* [in] */ PInterface pObject,
    /* [in] */ Int32 x,
    /* [in] */ Int32 y,
    /* [in] */ Timestamp64 timeStamp)
{
    ECode ec;

	SetResult(23, NULL);
	ec = Finish();

    return ec;
}