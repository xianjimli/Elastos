
#ifndef __CCOLORSTATELIST_H__
#define __CCOLORSTATELIST_H__

#include "_CColorStateList.h"
#include "ext/frameworkext.h"
#include <elastos/Map.h>
#include <elastos/Mutex.h>
#include <elastos/AutoPtr.h>

using namespace Elastos::Core::Threading;

CarClass(CColorStateList)
{
public:
    CColorStateList();

    ~CColorStateList();

    /**
     * Creates or retrieves a ColorStateList that always returns a single color.
     */
    static CARAPI ValueOf(
        /* [in] */ Int32 color,
        /* [out] */ IColorStateList** csl);

    /**
     * Create a ColorStateList from an XML document, given a set of {@link Resources}.
     */
    static CARAPI CreateFromXml(
        /* [in] */ IResources* r,
        /* [in] */ IXmlPullParser* parser,
        /* [out] */ IColorStateList** csl);

    /**
     * Creates a new ColorStateList that has the same states and
     * colors as this one but where each color has the specified alpha value
     * (0-255).
     */
    CARAPI WithAlpha(
        /* [in] */ Int32 alpha,
        /* [out] */ IColorStateList** colorState);

    CARAPI IsStateful(
        /* [out] */ Boolean* isStateful);

    /**
     * Return the color associated with the given set of {@link android.view.View} states.
     *
     * @param stateSet an array of {@link android.view.View} states
     * @param defaultColor the color to return if there's not state spec in this
     * {@link ColorStateList} that matches the stateSet.
     *
     * @return the color associated with that set of states in this {@link ColorStateList}.
     */
    CARAPI GetColorForState(
        /* [in] */ ArrayOf<Int32>* stateSet,
        /* [in] */ Int32 defaultColor,
        /* [out] */ Int32* color);

    CARAPI GetDefaultColor(
        /* [out] */ Int32 * pColor);

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ const ArrayOf<Handle32> & states,
        /* [in] */ const ArrayOf<Int32> & colors);

private:
    /* Create from inside an XML document.  Called on a parser positioned at
     * a tag in an XML document, tries to create a ColorStateList from that tag.
     * Returns null if the tag is not a valid ColorStateList.
     */
    static CARAPI CreateFromXmlInner(
        /* [in] */ IResources* r,
        /* [in] */ IXmlPullParser* parser,
        /* [in] */ IAttributeSet* attrs,
        /* [out] */ IColorStateList** csl);

    /**
     * Fill in this object based on the contents of an XML "selector" element.
     */
    CARAPI Inflate(
        /* [in] */ IResources* r,
        /* [in] */ IXmlPullParser* parser,
        /* [in] */ IAttributeSet* attrs);

private:
    static ArrayOf<Handle32>* EMPTY;
    static Map<Int32, AutoPtr<IColorStateList> > sCache;
    static Mutex sCacheLock;

    ArrayOf<Handle32>* mStateSpecs; // must be parallel to mColors
    ArrayOf<Int32>* mColors;      // must be parallel to mStateSpecs
    Int32 mDefaultColor;
};

#endif // __CCOLORSTATELIST_H__
