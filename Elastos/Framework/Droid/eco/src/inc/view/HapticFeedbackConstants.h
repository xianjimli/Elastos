
#ifndef __HAPTICFEEDBACKCONSTANTS_H__
#define __HAPTICFEEDBACKCONSTANTS_H__

class HapticFeedbackConstants
{
private:
    HapticFeedbackConstants() {}

public:
    /**
     * The user has performed a long press on an object that is resulting
     * in an action being performed.
     */
    static const Int32 LONG_PRESS = 0;

    /**
     * The user has pressed on a virtual on-screen key.
     */
    static const Int32 VIRTUAL_KEY = 1;

    /**
     * The user has pressed a soft keyboard key.
     */
    static const Int32 KEYBOARD_TAP = 3;

    /**
     * This is a private constant.  Feel free to renumber as desired.
     * @hide
     */
    static const Int32 SAFE_MODE_DISABLED = 10000;

    /**
     * This is a private constant.  Feel free to renumber as desired.
     * @hide
     */
    static const Int32 SAFE_MODE_ENABLED = 10001;

    /**
     * Flag for {@link View#performHapticFeedback(int, int)
     * View.performHapticFeedback(int, int)}: Ignore the setting in the
     * view for whether to perform haptic feedback, do it always.
     */
    static const Int32 FLAG_IGNORE_VIEW_SETTING = 0x0001;

    /**
     * Flag for {@link View#performHapticFeedback(int, int)
     * View.performHapticFeedback(int, int)}: Ignore the global setting
     * for whether to perform haptic feedback, do it always.
     */
    static const Int32 FLAG_IGNORE_GLOBAL_SETTING = 0x0002;
};
#endif//__HAPTICFEEDBACKCONSTANTS_H__
