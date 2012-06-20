
#ifndef __CDECELERATEINTERPOLATOR_H__
#define __CDECELERATEINTERPOLATOR_H__

#include "_CDecelerateInterpolator.h"

/**
* Use with {@link KeyEvent#dispatch(Callback, DispatcherState, Object)}
* for more advanced key dispatching, such as Int64 presses.
*/
CarClass(CDecelerateInterpolator)
{
public:
    CDecelerateInterpolator();

    CARAPI constructor();

    /**
     * Constructor
     * 
     * @param factor Degree to which the animation should be eased. Setting factor to 1.0f produces
     *        an upside-down y=x^2 parabola. Increasing factor above 1.0f makes exaggerates the
     *        ease-out effect (i.e., it starts even faster and ends evens slower)
     */
    CARAPI constructor(
        /* [in] */ Float factor);
    
    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);
    
    CARAPI GetInterpolation(
        /* [in] */ Float input,
        /* [out] */ Float* output);
    
private:
    Float mFactor;
};
#endif //__CDECELERATEINTERPOLATOR_H__
