
#ifndef __DYNAMICLAYOUT_H__
#define __DYNAMICLAYOUT_H__

#include "ext/frameworkext.h"
#include "text/Layout.h"
#include "text/CStaticLayout.h"
#include "text/PackedIntVector.h"
#include "text/PackedObjectVector.h"
#include <elastos/Mutex.h>
#include <elastos/Vector.h>

using namespace Elastos::Core::Threading;

/**
 * DynamicLayout is a text layout that updates itself as the text is edited.
 * <p>This is used by widgets to control text layout. You should not need
 * to use this class directly unless you are implementing your own widget
 * or custom display object, or need to call
 * {@link android.graphics.Canvas#drawText(java.lang.CharSequence, Int32, Int32, Float, Float, android.graphics.Paint)
 *  Canvas.drawText()} directly.</p>
 */
class DynamicLayout : public Layout
{
public:

    DynamicLayout(){}

    DynamicLayout(
        /* [in] */ ICharSequence* base,
        /* [in] */ ITextPaint* paint,
        /* [in] */ Int32 width,
        /* [in] */ LayoutAlignment align,
        /* [in] */ Float spacingmult,
        /* [in] */ Float spacingadd,
        /* [in] */ Boolean includepad);

    DynamicLayout(
        /* [in] */ ICharSequence* base,
        /* [in] */ ICharSequence* display,
        /* [in] */ ITextPaint* paint,
        /* [in] */ Int32 width,
        /* [in] */ LayoutAlignment align,
        /* [in] */ Float spacingmult,
        /* [in] */ Float spacingadd,
        /* [in] */ Boolean includepad);

    DynamicLayout(
        /* [in] */ ICharSequence* base,
        /* [in] */ ICharSequence* display,
        /* [in] */ ITextPaint* paint,
        /* [in] */ Int32 width,
        /* [in] */ LayoutAlignment align,
        /* [in] */ Float spacingmult,
        /* [in] */ Float spacingadd,
        /* [in] */ Boolean includepad,
        /* [in] */ TextUtilsTruncateAt ellipsize,
        /* [in] */ Int32 ellipsizedWidth);

    virtual ~DynamicLayout();

    CARAPI_(Int32) GetLineCount();

    CARAPI_(Int32) GetLineTop(
        /* [in] */ Int32 line);

    CARAPI_(Int32) GetLineDescent(
        /* [in] */ Int32 line);

    CARAPI_(Int32) GetLineStart(
        /* [in] */ Int32 line);

    CARAPI_(Boolean) GetLineContainsTab(
        /* [in] */ Int32 line);

    CARAPI_(Int32) GetParagraphDirection(
        /* [in] */ Int32 line);

    CARAPI_(AutoPtr<IDirections>) GetLineDirections(
        /* [in] */ Int32 line);

    CARAPI_(Int32) GetTopPadding();

    CARAPI_(Int32) GetBottomPadding();

    CARAPI_(Int32) GetEllipsizedWidth();

    CARAPI_(Int32) GetEllipsisStart(
        /* [in] */ Int32 line);

    CARAPI_(Int32) GetEllipsisCount(
        /* [in] */ Int32 line);

private:
    class ChangeWatcher
        : public ElRefBase
        , public ITextWatcher
        , public ISpanWatcher
    {
    public:
        ChangeWatcher(
            /* [in] */ DynamicLayout* layout)
            : mLayout(layout)
        {}

        CARAPI_(PInterface) Probe(
            /* [in] */ REIID riid);

        CARAPI_(UInt32) AddRef()
        {
            return ElRefBase::AddRef();
        }

        CARAPI_(UInt32) Release()
        {
            return ElRefBase::Release();
        }

        CARAPI GetInterfaceID(
            /* [in] */ IInterface *pObject,
            /* [out] */ InterfaceID *pIID);

        CARAPI BeforeTextChanged(
            /* [in] */ ICharSequence* s,
            /* [in] */ Int32 where,
            /* [in] */ Int32 before,
            /* [in] */ Int32 after)
        {
            return NOERROR;
        }

        CARAPI OnTextChanged(
            /* [in] */ ICharSequence* s,
            /* [in] */ Int32 where,
            /* [in] */ Int32 before,
            /* [in] */ Int32 after)
        {
            Reflow(s, where, before, after);
            return NOERROR;
        }

        CARAPI AfterTextChanged(
            /* [in] */ IEditable* s)
        {
            return NOERROR;
        }

        CARAPI OnSpanAdded(
            /* [in] */ ISpannable* s,
            /* [in] */ IInterface* o,
            /* [in] */ Int32 start,
            /* [in] */ Int32 end);

        CARAPI OnSpanRemoved(
            /* [in] */ ISpannable* s,
            /* [in] */ IInterface* o,
            /* [in] */ Int32 start,
            /* [in] */ Int32 end);

        CARAPI OnSpanChanged(
            /* [in] */ ISpannable* s,
            /* [in] */ IInterface* o,
            /* [in] */ Int32 start,
            /* [in] */ Int32 end,
            /* [in] */ Int32 nstart,
            /* [in] */ Int32 nend);

    private:
        CARAPI_(void) Reflow(
            /* [in] */ ICharSequence* s,
            /* [in] */ Int32 where,
            /* [in] */ Int32 before,
            /* [in] */ Int32 after);

    private:
        DynamicLayout* mLayout;
    };

protected:
    CARAPI Init(
        /* [in] */ ICharSequence* base,
        /* [in] */ ICharSequence* display,
        /* [in] */ ITextPaint* paint,
        /* [in] */ Int32 width,
        /* [in] */ LayoutAlignment align,
        /* [in] */ Float spacingmult,
        /* [in] */ Float spacingadd,
        /* [in] */ Boolean includepad);

    CARAPI Init(
        /* [in] */ ICharSequence* base,
        /* [in] */ ICharSequence* display,
        /* [in] */ ITextPaint* paint,
        /* [in] */ Int32 width,
        /* [in] */ LayoutAlignment align,
        /* [in] */ Float spacingmult,
        /* [in] */ Float spacingadd,
        /* [in] */ Boolean includepad,
        /* [in] */ TextUtilsTruncateAt ellipsize,
        /* [in] */ Int32 ellipsizedWidth);

private:
    CARAPI_(void) Reflow(
        /* [in] */ ICharSequence* s,
        /* [in] */ Int32 where,
        /* [in] */ Int32 before,
        /* [in] */ Int32 after);

    CARAPI_(void) Dump(
        /* [in] */ Boolean show);

private:
    static const Int32 PRIORITY = 128;

    AutoPtr<ICharSequence> mBase;
    AutoPtr<ICharSequence> mDisplay;
    AutoPtr<ChangeWatcher> mWatcher;
    Boolean mIncludePad;
    Boolean mEllipsize;
    Int32 mEllipsizedWidth;
    TextUtilsTruncateAt mEllipsizeAt;

    PackedIntVector* mInts;
    PackedObjectVector* mObjects;

    Int32 mTopPadding, mBottomPadding;

    static AutoPtr<CStaticLayout> sStaticLayout;
    static Mutex sLock;

    static const Int32 START = 0;
    static const Int32 DIR = START;
    static const Int32 TAB = START;
    static const Int32 TOP = 1;
    static const Int32 DESCENT = 2;
    static const Int32 COLUMNS_NORMAL = 3;

    static const Int32 ELLIPSIS_START = 3;
    static const Int32 ELLIPSIS_COUNT = 4;
    static const Int32 COLUMNS_ELLIPSIZE = 5;

    static const Int32 START_MASK = 0x1FFFFFFF;
    static const Int32 DIR_MASK   = 0xC0000000;
    static const Int32 DIR_SHIFT  = 30;
    static const Int32 TAB_MASK   = 0x20000000;

    static const Int32 ELLIPSIS_UNDEFINED = 0x80000000;

};

#endif //__DYNAMICLAYOUT_H__
