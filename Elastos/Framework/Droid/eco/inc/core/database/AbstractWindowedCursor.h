#ifndef __ABSTRACTWINDOWEDCURSOR_H__
#define __ABSTRACTWINDOWEDCURSOR_H__

#include "ext/frameworkext.h"
#include <elastos/AutoPtr.h>
#include "database/AbstractCursor.h"

/**
 * A base class for Cursors that store their data in {@link CursorWindow}s.
 */
class AbstractWindowedCursor : public AbstractCursor
{
public:
    CARAPI GetBlob(
        /* [in] */ Int32 columnIndex,
        /* [out] */ ArrayOf<Byte>** blob);

    CARAPI GetString(
        /* [in] */ Int32 columnIndex,
        /* [out] */ String* value);

    CARAPI CopyStringToBuffer(
        /* [in] */ Int32 columnIndex,
        /* [in] */ CharArrayBuffer* buffer);

    CARAPI GetShort(
        /* [in] */ Int32 columnIndex,
        /* [out] */ Int16* value);

    CARAPI GetInt(
        /* [in] */ Int32 columnIndex,
        /* [out] */ Int32* value);

    CARAPI GetLong(
        /* [in] */ Int32 columnIndex,
        /* [out] */ Int64* value);

    CARAPI GetFloat(
        /* [in] */ Int32 columnIndex,
        /* [out] */ Float* value);

    CARAPI GetDouble(
        /* [in] */ Int32 columnIndex,
        /* [out] */ Double* value);

    CARAPI IsNull(
        /* [in] */ Int32 columnIndex,
        /* [out] */ Boolean* value);

    virtual CARAPI IsBlob(
        /* [in] */ Int32 columnIndex,
        /* [out] */ Boolean* value);

    virtual CARAPI IsString(
        /* [in] */ Int32 columnIndex,
        /* [out] */ Boolean* value);

    virtual CARAPI IsInt64(
        /* [in] */ Int32 columnIndex,
        /* [out] */ Boolean* value);

    virtual CARAPI IsFloat(
        /* [in] */ Int32 columnIndex,
        /* [out] */ Boolean* value);

    CARAPI GetWindow(
        /* [out] */ ICursorWindow** window);

    /**
     * Set a new cursor window to cursor, usually set a remote cursor window
     * @param window cursor window
     */
    virtual CARAPI SetWindow(
        /* [in] */ ICursorWindow* window);

    virtual CARAPI HasWindow(
        /* [out] */ Boolean* rst);

protected:
    CARAPI CheckPosition();

protected:
    /**
     * This needs be updated in {@link #onMove} by subclasses, and
     * needs to be set to NULL when the contents of the cursor change.
     */
    AutoPtr<ICursorWindow> mWindow;
};
#endif //__ABSTRACTWINDOWEDCURSOR_H__
