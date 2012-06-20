
#ifndef __EMOJIFACTORY_H__
#define __EMOJIFACTORY_H__

#include "ext/frameworkext.h"
#include <elastos/AutoPtr.h>

/**
 * A class for the factories which produce Emoji (pictgram) images.
 * This is intended to be used by IME, Email app, etc.
 * There's no plan to make this public for now.
 * @hide
 */
class EmojiFactory
{
public:
    /**
     * Returns Bitmap object corresponding to the AndroidPua.
     *
     * Note that each Bitmap is cached by this class, which means that, if you modify a
     * Bitmap object (using setPos() method), all same emoji Bitmap will be modified.
     * If it is unacceptable, please copy the object before modifying it.
     *
     * @param pua A unicode codepoint.
     * @return Bitmap object when this factory knows the Bitmap relevant to the codepoint.
     * Otherwise null is returned.
     */
    AutoPtr<IBitmap> GetBitmapFromAndroidPua(
        /* [in] */ Int32 pua);

    /**
     * Constructs an instance of available EmojiFactory.
     *
     * @return A concrete EmojiFactory instance. If there are several available
     * EmojiFactory class, preferred one is chosen by the system. If there isn't, null
     * is returned.
     */
    static CARAPI_(EmojiFactory*) NewAvailableInstance();

    /**
     * Returns the lowest code point corresponding to an Android
     * emoji character.
     */
    CARAPI_(Int32) GetMinimumAndroidPua();

    /**
     * Returns the highest code point corresponding to an Android
     * emoji character.
     */
    CARAPI_(Int32) GetMaximumAndroidPua();
};

#endif //__EMOJIFACTORY_H__
