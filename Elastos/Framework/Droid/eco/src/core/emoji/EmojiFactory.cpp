
#include "emoji/EmojiFactory.h"

AutoPtr<IBitmap> EmojiFactory::GetBitmapFromAndroidPua(
    /* [in] */ Int32 pua)
{
    return NULL;
}

EmojiFactory* EmojiFactory::NewAvailableInstance()
{
    return NULL;
}

Int32 EmojiFactory::GetMinimumAndroidPua()
{
    return -1;
}

Int32 EmojiFactory::GetMaximumAndroidPua()
{
    return -1;
}
