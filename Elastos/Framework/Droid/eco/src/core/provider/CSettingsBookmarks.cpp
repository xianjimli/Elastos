#include "provider/CSettingsBookmarks.h"
#include <Slogger.h>
#include <elastos/AutoPtr.h>

using namespace Elastos::Core;

ECode CSettingsBookmarks::GetIntentForShortcut(
    /*[in]*/ IContentResolver* cr,
    /*[in]*/ Char8 shortcut,
    /*[out]*/ IIntent** outIntent)
{
    return E_NOT_IMPLEMENTED;
}

ECode CSettingsBookmarks::Add(
        /*[in]*/ IContentResolver* cr,
        /*[in]*/ IIntent* intent,
        /*[in]*/ const String& title,
        /*[in]*/ const String& folder,
        /*[in]*/ Char8 shortcut,
        /*[in]*/ Int32 ordering,
        /*[out]*/ IUri** outUri)
{
    return E_NOT_IMPLEMENTED;
}
