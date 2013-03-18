#ifndef __CSETTINGSBOOKMARKS_H__
#define __CSETTINGSBOOKMARKS_H__

#include "_CSettingsBookmarks.h"
#include "Settings.h"


CarClass(CSettingsBookmarks), public Settings::Bookmarks
{
public:
    /**
     * Convenience function to retrieve the bookmarked Intent for a
     * particular shortcut key.
     *
     * @param cr The ContentResolver to query.
     * @param shortcut The shortcut key.
     *
     * @return Intent The bookmarked URL, or null if there is no bookmark
     *         matching the given shortcut.
     */
    CARAPI GetIntentForShortcut(
        /* [in] */ IContentResolver* cr,
        /* [in] */ Char32 shortcut,
        /* [out] */ IIntent** intent);

    /**
     * Add a new bookmark to the system.
     *
     * @param cr The ContentResolver to query.
     * @param intent The desired target of the bookmark.
     * @param title Bookmark title that is shown to the user; null if none
     *            or it should be resolved to the intent's title.
     * @param folder Folder in which to place the bookmark; null if none.
     * @param shortcut Shortcut that will invoke the bookmark; 0 if none. If
     *            this is non-zero and there is an existing bookmark entry
     *            with this same shortcut, then that existing shortcut is
     *            cleared (the bookmark is not removed).
     * @return The unique content URL for the new bookmark entry.
     */
    CARAPI Add(
        /* [in] */ IContentResolver* cr,
        /* [in] */ IIntent* intent,
        /* [in] */ const String& title,
        /* [in] */ const String& folder,
        /* [in] */ Char32 shortcut,
        /* [in] */ Int32 ordering,
        /* [out] */ IUri** uri);

    /**
     * Return the folder name as it should be displayed to the user.  This
     * takes care of localizing special folders.
     *
     * @param r Resources object for current locale; only need access to
     *          system resources.
     * @param folder The value found in the {@link #FOLDER} column.
     *
     * @return CharSequence The label for this folder that should be shown
     *         to the user.
     */
    CARAPI GetLabelForFolder(
        /* [in] */ IResources* r,
        /* [in] */ const String& folder,
        /* [out] */ ICharSequence** fName);

    /**
     * Return the title as it should be displayed to the user. This takes
     * care of localizing bookmarks that point to activities.
     *
     * @param context A context.
     * @param cursor A cursor pointing to the row whose title should be
     *        returned. The cursor must contain at least the {@link #TITLE}
     *        and {@link #INTENT} columns.
     * @return A title that is localized and can be displayed to the user,
     *         or the empty string if one could not be found.
     */
    CARAPI GetTitle(
        /* [in] */ IContext* context,
        /* [in] */ ICursor* cursor,
        /* [out] */ ICharSequence** title);
};

#endif //__CSETTINGSBOOKMARKS_H__
