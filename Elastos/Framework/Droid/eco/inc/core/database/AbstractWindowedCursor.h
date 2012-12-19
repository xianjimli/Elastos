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
    //@Override
    CARAPI GetBlob(
        /* [in] */ Int32 columnIndex,
        /* [out] */ ArrayOf<Byte>** blob);

    //@Override
    CARAPI GetString(
        /* [in] */ Int32 columnIndex,
        /* [out] */ String* str);

    //@Override
    CARAPI CopyStringToBuffer(
        /* [in] */ Int32 columnIndex,
        /* [in] */ ICharArrayBuffer* buffer);

    //@Override
    CARAPI GetInt16(
        /* [in] */ Int32 columnIndex,
        /* [out] */ Int16* value);

    //@Override
    CARAPI GetInt32(
        /* [in] */ Int32 columnIndex,
        /* [out] */ Int32* value);

    //@Override
    CARAPI GetInt64(
        /* [in] */ Int32 columnIndex,
        /* [out] */ Int64* value);

    //@Override
    CARAPI GetFloat(
        /* [in] */ Int32 columnIndex,
        /* [out] */ Float* value);

    //@Override
    CARAPI GetDouble(
        /* [in] */ Int32 columnIndex,
        /* [out] */ Double* value);

    //@Override
    CARAPI IsNull(
        /* [in] */ Int32 columnIndex,
        /* [out] */ Boolean* isNull);

    virtual CARAPI IsBlob(
        /* [in] */ Int32 columnIndex,
        /* [out] */ Boolean* isBlob);

    virtual CARAPI IsString(
        /* [in] */ Int32 columnIndex,
        /* [out] */ Boolean* isString);

    virtual CARAPI IsInt64(
        /* [in] */ Int32 columnIndex,
        /* [out] */ Boolean* isInt64);

    virtual CARAPI IsFloat(
        /* [in] */ Int32 columnIndex,
        /* [out] */ Boolean* isFloat);

    //@Override
    CARAPI_(AutoPtr<ICursorWindow>) GetWindow();

    /**
     * Set a new cursor window to cursor, usually set a remote cursor window
     * @param window cursor window
     */
    virtual CARAPI SetWindow(
        /* [in] */ ICursorWindow* window);

    virtual CARAPI_(Boolean) HasWindow();

protected:
    //@Override
    CARAPI CheckPosition();

protected:
    /**
     * This needs be updated in {@link #onMove} by subclasses, and
     * needs to be set to NULL when the contents of the cursor change.
     */
    AutoPtr<ICursorWindow> mWindow;
};
#endif //__ABSTRACTWINDOWEDCURSOR_H__
