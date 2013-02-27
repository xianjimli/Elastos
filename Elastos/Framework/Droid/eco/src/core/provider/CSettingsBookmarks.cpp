#include "provider/CSettingsBookmarks.h"
#include <Slogger.h>
#include <elastos/AutoPtr.h>

using namespace Elastos::Core;

ECode CSettingsBookmarks::GetIntentForShortcut(
    /*[in]*/ IContentResolver* cr,
    /*[in]*/ Char32 shortcut,
    /*[out]*/ IIntent** intent)
{
    return E_NOT_IMPLEMENTED;
}

ECode CSettingsBookmarks::Add(
    /*[in]*/ IContentResolver* cr,
    /*[in]*/ IIntent* intent,
    /*[in]*/ const String& title,
    /*[in]*/ const String& folder,
    /*[in]*/ Char32 shortcut,
    /*[in]*/ Int32 ordering,
    /*[out]*/ IUri** uri)
{
    return E_NOT_IMPLEMENTED;
}

ECode CSettingsBookmarks::GetLabelForFolder(
    /*[in]*/ IResources* r,
    /*[in]*/ const String& folder,
    /*[out]*/ ICharSequence** fName)
{
    return E_NOT_IMPLEMENTED;
}

ECode CSettingsBookmarks::GetTitle(
	/*[in]*/ IContext* context,
	/*[in]*/ ICursor* cursor,
	/*[out]*/ ICharSequence** title)
{
    return E_NOT_IMPLEMENTED;
}


