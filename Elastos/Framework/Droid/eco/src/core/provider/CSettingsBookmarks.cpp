
#include "ext/frameworkdef.h"
#include "provider/CSettingsBookmarks.h"


ECode CSettingsBookmarks::GetIntentForShortcut(
    /* [in] */ IContentResolver* cr,
    /* [in] */ Char32 shortcut,
    /* [out] */ IIntent** intent)
{
    VALIDATE_NOT_NULL(intent);
    return Settings::Bookmarks::GetIntentForShortcut(cr, shortcut, intent);
}

ECode CSettingsBookmarks::Add(
    /* [in] */ IContentResolver* cr,
    /* [in] */ IIntent* intent,
    /* [in] */ const String& title,
    /* [in] */ const String& folder,
    /* [in] */ Char32 shortcut,
    /* [in] */ Int32 ordering,
    /* [out] */ IUri** uri)
{
    VALIDATE_NOT_NULL(uri);
    return Settings::Bookmarks::Add(cr,
        intent, title, folder, shortcut, ordering, uri);
}

ECode CSettingsBookmarks::GetLabelForFolder(
    /* [in] */ IResources* r,
    /* [in] */ const String& folder,
    /* [out] */ ICharSequence** fName)
{
    VALIDATE_NOT_NULL(fName);
    return Settings::Bookmarks::GetLabelForFolder(r, folder, fName);
}

ECode CSettingsBookmarks::GetTitle(
	/* [in] */ IContext* context,
	/* [in] */ ICursor* cursor,
	/* [out] */ ICharSequence** title)
{
    VALIDATE_NOT_NULL(title);
    return Settings::Bookmarks::GetTitle(context, cursor, title);
}
