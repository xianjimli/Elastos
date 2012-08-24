
#include "text/DynamicLayout.h"
#include "text/TextUtils.h"
#include "text/CStaticLayout.h"
#include "graphics/CPaintFontMetricsInt.h"

#include <stdio.h>

// {6A565FF4-9481-4C51-B29D-53E8E20C8D3A}
static const InterfaceID EIID_ChangeWatcher =
    { 0x6a565ff4, 0x9481, 0x4c51, { 0xb2, 0x9d, 0x53, 0xe8, 0xe2, 0xc, 0x8d, 0x3a } };

AutoPtr<CStaticLayout> InitsStaticLayout()
{
    AutoPtr<CStaticLayout> sl;
    CStaticLayout::NewByFriend(TRUE, (CStaticLayout**)&sl);
    return sl;
}

const Int32 DynamicLayout::PRIORITY;
AutoPtr<CStaticLayout> DynamicLayout::sStaticLayout = InitsStaticLayout();
Mutex DynamicLayout::sLock;

const Int32 DynamicLayout::START;
const Int32 DynamicLayout::DIR;
const Int32 DynamicLayout::TAB;
const Int32 DynamicLayout::TOP;
const Int32 DynamicLayout::DESCENT;
const Int32 DynamicLayout::COLUMNS_NORMAL;

const Int32 DynamicLayout::ELLIPSIS_START;
const Int32 DynamicLayout::ELLIPSIS_COUNT;
const Int32 DynamicLayout::COLUMNS_ELLIPSIZE;

const Int32 DynamicLayout::START_MASK;
const Int32 DynamicLayout::DIR_MASK;
const Int32 DynamicLayout::DIR_SHIFT;
const Int32 DynamicLayout::TAB_MASK;

const Int32 DynamicLayout::ELLIPSIS_UNDEFINED;


PInterface DynamicLayout::ChangeWatcher::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (IInterface*)(ISpanWatcher*)this;
    }
    else if (riid == EIID_ChangeWatcher) {
        return (IInterface*)(ISpanWatcher*)this;
    }
    else if (riid == EIID_ITextWatcher) {
        return (ITextWatcher*)this;
    }
    else if (riid == EIID_ISpanWatcher) {
        return (ISpanWatcher*)this;
    }

    return NULL;
}

ECode DynamicLayout::ChangeWatcher::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    return E_NOT_IMPLEMENTED;
}

void DynamicLayout::ChangeWatcher::Reflow(
    /* [in] */ ICharSequence* s,
    /* [in] */ Int32 where,
    /* [in] */ Int32 before,
    /* [in] */ Int32 after)
{
    if (mLayout != NULL) {
        mLayout->Reflow(s, where, before, after);
    }
    else if (ISpannable::Probe(s)) {
       ISpannable::Probe(s)->RemoveSpan((IInterface*)this->Probe(EIID_IInterface));
    }
}

ECode DynamicLayout::ChangeWatcher::OnSpanAdded(
    /* [in] */ ISpannable* s,
    /* [in] */ IInterface* o,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    if (IUpdateLayout::Probe(o)) {
        Reflow(s, start, end - start, end - start);
    }
    return NOERROR;
}

ECode DynamicLayout::ChangeWatcher::OnSpanRemoved(
    /* [in] */ ISpannable* s,
    /* [in] */ IInterface* o,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    if (IUpdateLayout::Probe(o)) {
        Reflow(s, start, end - start, end - start);
    }
    return NOERROR;
}

ECode DynamicLayout::ChangeWatcher::OnSpanChanged(
    /* [in] */ ISpannable* s,
    /* [in] */ IInterface* o,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ Int32 nstart,
    /* [in] */ Int32 nend)
{
    if (IUpdateLayout::Probe(o)) {
        Reflow(s, start, end - start, end - start);
        Reflow(s, nstart, nend - nstart, nend - nstart);
    }
    return NOERROR;
}

/**
 * Make a layout for the specified text that will be updated as
 * the text is changed.
 */
DynamicLayout::DynamicLayout(
    /* [in] */ ICharSequence* base,
    /* [in] */ ITextPaint* paint,
    /* [in] */ Int32 width,
    /* [in] */ LayoutAlignment align,
    /* [in] */ Float spacingmult,
    /* [in] */ Float spacingadd,
    /* [in] */ Boolean includepad)
    : Layout(base, paint, width, align, spacingmult, spacingadd)
{
    Init(base, base, paint, width, align, spacingmult, spacingadd,
        includepad);
}

/**
 * Make a layout for the transformed text (password transformation
 * being the primary example of a transformation)
 * that will be updated as the base text is changed.
 */
DynamicLayout::DynamicLayout(
    /* [in] */ ICharSequence* base,
    /* [in] */ ICharSequence* display,
    /* [in] */ ITextPaint* paint,
    /* [in] */ Int32 width,
    /* [in] */ LayoutAlignment align,
    /* [in] */ Float spacingmult,
    /* [in] */ Float spacingadd,
    /* [in] */ Boolean includepad)
    : Layout(display, paint, width, align, spacingmult, spacingadd)
{
    Init(base, display, paint, width, align, spacingmult, spacingadd,
         includepad, -1, 0);
}

/**
 * Make a layout for the transformed text (password transformation
 * being the primary example of a transformation)
 * that will be updated as the base text is changed.
 * If ellipsize is non-NULL, the Layout will ellipsize the text
 * down to ellipsizedWidth.
 */
DynamicLayout::DynamicLayout(
    /* [in] */ ICharSequence* base,
    /* [in] */ ICharSequence* display,
    /* [in] */ ITextPaint* paint,
    /* [in] */ Int32 width,
    /* [in] */ LayoutAlignment align,
    /* [in] */ Float spacingmult,
    /* [in] */ Float spacingadd,
    /* [in] */ Boolean includepad,
    /* [in] */ TextUtilsTruncateAt ellipsize,
    /* [in] */ Int32 ellipsizedWidth)
    : Layout((ellipsize == -1) ? display
            : (display->Probe(EIID_ISpanned))
                ? new SpannedEllipsizer(display)
                : new Ellipsizer(display),
          paint, width, align, spacingmult, spacingadd)
{
    Init(base, display, paint, width, align, spacingmult, spacingadd,
        includepad, ellipsize, ellipsizedWidth);
}

DynamicLayout::~DynamicLayout()
{
    if (mInts != NULL) delete mInts;
    if (mObjects != NULL) delete mObjects;
}

void DynamicLayout::Reflow(
    /* [in] */ ICharSequence* s,
    /* [in] */ Int32 where,
    /* [in] */ Int32 before,
    /* [in] */ Int32 after)
{
    if (s != mBase) {
        return;
    }

    AutoPtr<ICharSequence> text = mDisplay;
    Int32 len;
    text->GetLength(&len);

    // seek back to the start of the paragraph

    Int32 find = TextUtils::LastIndexOf(text, '\n', where - 1);
    if (find < 0) {
        find = 0;
    }
    else {
        find = find + 1;
    }

    {
        Int32 diff = where - find;
        before += diff;
        after += diff;
        where -= diff;
    }

    // seek forward to the end of the paragraph

    Int32 look = TextUtils::IndexOf(text, '\n', where + after);
    if (look < 0) {
        look = len;
    }
    else {
        look++; // we want the index after the \n
    }

    Int32 change = look - (where + after);
    before += change;
    after += change;

    // seek further out to cover anything that is forced to wrap together

    if (ISpanned::Probe(text) != NULL)
    {
        AutoPtr<ISpanned> sp = ISpanned::Probe(text);
        Boolean again;

        do {
            again = FALSE;

            ArrayOf<IInterface*>* force = NULL;
            sp->GetSpans(where, where + after,
                    EIID_IWrapTogetherSpan, &force);

            for (Int32 i = 0; i < force->GetLength(); i++) {
                Int32 st;
                sp->GetSpanStart((*force)[i], &st);

                Int32 en;
                sp->GetSpanEnd((*force)[i], &en);

                if (st < where) {
                    again = TRUE;

                    Int32 diff = where - st;
                    before += diff;
                    after += diff;
                    where -= diff;
                }

                if (en > where + after) {
                    again = TRUE;

                    Int32 diff = en - (where + after);
                    before += diff;
                    after += diff;
                }

                //free
                (*force)[i]->Release();
            }

            //free
            ArrayOf<IInterface*>::Free(force);

        } while (again);
    }

    // find affected region of old layout

    Int32 startline = GetLineForOffset(where);
    Int32 startv = GetLineTop(startline);

    Int32 endline = GetLineForOffset(where + before);
    if (where + after == len) {
        endline = GetLineCount();
    }
    Int32 endv = GetLineTop(endline);
    Boolean islast = (endline == GetLineCount());

    // generate new layout for affected text

    AutoPtr<CStaticLayout> reflowed;

    {
        Mutex::Autolock lock(&sLock);
        reflowed = sStaticLayout;
        sStaticLayout = NULL;
    }

    if (reflowed == NULL) {
        CStaticLayout::NewByFriend(TRUE, (CStaticLayout**)&reflowed);
    }

    reflowed->Generate(text, where, where + after,
                       GetPaint(), GetWidth(), GetAlignment(),
                       GetSpacingMultiplier(), GetSpacingAdd(),
                       FALSE, TRUE, mEllipsize,
                       mEllipsizedWidth, mEllipsizeAt);
    Int32 n;
    reflowed->GetLineCount(&n);

    // If the new layout has a blank line at the end, but it is not
    // the very end of the buffer, then we already have a line that
    // starts there, so disregard the blank line.
    Int32 offset;
    reflowed->GetLineStart(n - 1, &offset);
    if (where + after != len && offset == where + after) {
        n--;
    }

    // remove affected lines from old layout

    mInts->DeleteAt(startline, endline - startline);
    mObjects->DeleteAt(startline, endline - startline);

    // adjust offsets in layout for new height and offsets

    Int32 ht;
    reflowed->GetLineTop(n, &ht);

    Int32 toppad = 0, botpad = 0;

    if (mIncludePad && startline == 0) {
        reflowed->GetTopPadding(&toppad);
        mTopPadding = toppad;
        ht -= toppad;
    }
    if (mIncludePad && islast) {
        reflowed->GetBottomPadding(&botpad);
        mBottomPadding = botpad;
        ht += botpad;
    }

    mInts->AdjustValuesBelow(startline, START, after - before);
    mInts->AdjustValuesBelow(startline, TOP, startv - endv + ht);

    // insert new layout

    ArrayOf<Int32>* ints;

    if (mEllipsize) {
        ints = ArrayOf<Int32>::Alloc(COLUMNS_ELLIPSIZE);
        (*ints)[ELLIPSIS_START] = ELLIPSIS_UNDEFINED;
    }
    else {
        ints = ArrayOf<Int32>::Alloc(COLUMNS_NORMAL);
    }

    ArrayOf<IInterface*>* objects = ArrayOf<IInterface*>::Alloc(1);

    for (Int32 i = 0; i < n; i++) {
        Int32 off, dir;
        Boolean res;
        reflowed->GetLineStart(i, &off);
        reflowed->GetParagraphDirection(i, &dir);
        reflowed->GetLineContainsTab(i, &res);

        (*ints)[START] = off | (dir << DIR_SHIFT) | (res ? TAB_MASK : 0);

        Int32 top;
        reflowed->GetLineTop(i, &top);
        top += startv;
        if (i > 0) {
            top -= toppad;
        }
        (*ints)[TOP] = top;

        Int32 desc;
        reflowed->GetLineDescent(i, &desc);
        if (i == n - 1) {
            desc += botpad;
        }

        (*ints)[DESCENT] = desc;

        AutoPtr<IDirections> directions;
        reflowed->GetLineDirections(i, (IDirections**)&directions);
        (*objects)[0] = directions.Get();
        if ((*objects)[0] != NULL) (*objects)[0]->AddRef();

        if (mEllipsize) {
            reflowed->GetEllipsisStart(i, ints->GetPayload() + ELLIPSIS_START);
            reflowed->GetEllipsisCount(i, ints->GetPayload() + ELLIPSIS_COUNT);
        }

        mInts->InsertAt(startline + i, ints);
        mObjects->InsertAt(startline + i, objects);
    }

    {
        Mutex::Autolock lock(&sLock);
        sStaticLayout = reflowed;
    }
}

void DynamicLayout::Dump(
    /* [in] */ Boolean show)
{
    Int32 n = GetLineCount();

    /*for (Int32 i = 0; i < n; i++) {
        System.out.print("line " + i + ": " + getLineStart(i) + " to " + getLineEnd(i) + " ");

        if (show) {
            System.out.print(getText().subSequence(getLineStart(i),
                                                   getLineEnd(i)));
        }

        System.out.println("");
    }

    System.out.println("");*/
}

Int32 DynamicLayout::GetLineCount()
{
    return mInts->Size() - 1;
}

Int32 DynamicLayout::GetLineTop(
    /* [in] */ Int32 line)
{
    return mInts->GetValue(line, TOP);
}

Int32 DynamicLayout::GetLineDescent(Int32 line) {
    return mInts->GetValue(line, DESCENT);
}

Int32 DynamicLayout::GetLineStart(
    /* [in] */ Int32 line)
{
    return mInts->GetValue(line, START) & START_MASK;
}

Boolean DynamicLayout::GetLineContainsTab(
    /* [in] */ Int32 line)
{
    return (mInts->GetValue(line, TAB) & TAB_MASK) != 0;
}

Int32 DynamicLayout::GetParagraphDirection(
    /* [in] */ Int32 line)
{
    return mInts->GetValue(line, DIR) >> DIR_SHIFT;
}

AutoPtr<IDirections> DynamicLayout::GetLineDirections(
    /* [in] */ Int32 line)
{
    AutoPtr<IDirections> dirs = (IDirections*)mObjects->GetValue(line, 0).Get();
    return dirs;
}

Int32 DynamicLayout::GetTopPadding()
{
    return mTopPadding;
}

Int32 DynamicLayout::GetBottomPadding()
{
    return mBottomPadding;
}

Int32 DynamicLayout::GetEllipsizedWidth()
{
    return mEllipsizedWidth;
}

Int32 DynamicLayout::GetEllipsisStart(
    /* [in] */ Int32 line)
{
    if (mEllipsizeAt == -1) {
        return 0;
    }

    return mInts->GetValue(line, ELLIPSIS_START);
}

Int32 DynamicLayout::GetEllipsisCount(
    /* [in] */ Int32 line)
{
    if (mEllipsizeAt == -1) {
        return 0;
    }

    return mInts->GetValue(line, ELLIPSIS_COUNT);
}

ECode DynamicLayout::Init(
     /* [in] */ ICharSequence* base,
     /* [in] */ ICharSequence* display,
     /* [in] */ ITextPaint* paint,
     /* [in] */ Int32 width,
     /* [in] */ LayoutAlignment align,
     /* [in] */ Float spacingmult,
     /* [in] */ Float spacingadd,
     /* [in] */ Boolean includepad)
{
    return Init(base, base, paint, width, align, spacingmult, spacingadd,
        includepad, -1, 0);
}

ECode DynamicLayout::Init(
    /* [in] */ ICharSequence* base,
    /* [in] */ ICharSequence* display,
    /* [in] */ ITextPaint* paint,
    /* [in] */ Int32 width,
    /* [in] */ LayoutAlignment align,
    /* [in] */ Float spacingmult,
    /* [in] */ Float spacingadd,
    /* [in] */ Boolean includepad,
    /* [in] */ TextUtilsTruncateAt ellipsize,
    /* [in] */ Int32 ellipsizedWidth)
{
    mBase = base;
    mDisplay = display;

    if (ellipsize != -1) {
        mInts = new PackedIntVector(COLUMNS_ELLIPSIZE);
        mEllipsizedWidth = ellipsizedWidth;
        mEllipsizeAt = ellipsize;
    }
    else {
        mInts = new PackedIntVector(COLUMNS_NORMAL);
        mEllipsizedWidth = width;
        mEllipsizeAt = ellipsize;
    }

    mObjects = new PackedObjectVector(1);

    mIncludePad = includepad;

    /*
     * This is annoying, but we can't refer to the layout until
     * superclass construction is finished, and the superclass
     * constructor wants the reference to the display text.
     *
     * This will break if the superclass constructor ever actually
     * cares about the content instead of just holding the reference.
     */
    if (ellipsize != -1) {
        Ellipsizer* e = (Ellipsizer*)GetText().Get();

        e->mLayout = this;
        e->mWidth = ellipsizedWidth;
        e->mMethod = ellipsize;
        mEllipsize = TRUE;
    }

    // Initial state is a single line with 0 characters (0 to 0),
    // with top at 0 and bottom at whatever is natural, and
    // undefined ellipsis.

    ArrayOf<Int32>* start;

    if (ellipsize != -1) {
        start = ArrayOf<Int32>::Alloc(COLUMNS_ELLIPSIZE);
        (*start)[ELLIPSIS_START] = ELLIPSIS_UNDEFINED;
    }
    else {
        start = ArrayOf<Int32>::Alloc(COLUMNS_NORMAL);
    }

    ArrayOf<IInterface*>* dirs = ArrayOf<IInterface*>::Alloc(1);
    (*dirs)[0]= DIRS_ALL_LEFT_TO_RIGHT;
    if ((*dirs)[0] != NULL) (*dirs)[0]->AddRef();

    AutoPtr<IPaintFontMetricsInt> fm;
    paint->GetFontMetricsIntEx((IPaintFontMetricsInt**)&fm);
    Int32 asc;
    fm->GetAscent(&asc);

    Int32 desc;
    fm->GetDescent(&desc);

    (*start)[DIR] = DIR_LEFT_TO_RIGHT << DIR_SHIFT;
    (*start)[TOP] = 0;
    (*start)[DESCENT] = desc;
    mInts->InsertAt(0, start);

    (*start)[TOP] = desc - asc;
    mInts->InsertAt(1, start);

    mObjects->InsertAt(0, dirs);

    // Update from 0 characters to whatever the real text is
    Int32 len;
    base->GetLength(&len);
    Reflow(base, 0, 0, len);

    if (ISpannable::Probe(base) != NULL) {
        if (mWatcher == NULL) {
            mWatcher = new ChangeWatcher(this);
        }

        // Strip out any watchers for other DynamicLayouts.
        ISpannable* sp = ISpannable::Probe(base);

        sp->GetLength(&len);

        ArrayOf<IInterface*>* spans;
        sp->GetSpans(0, len, EIID_ChangeWatcher, &spans);
        for (Int32 i = 0; i < spans->GetLength(); i++) {
            sp->RemoveSpan((*spans)[i]);
        }

        base->GetLength(&len);
        sp->SetSpan(mWatcher->Probe(EIID_IInterface), 0, len,
                   Spanned_SPAN_INCLUSIVE_INCLUSIVE |
                   (PRIORITY << Spanned_SPAN_PRIORITY_SHIFT));

        //free
        for (Int32 i = 0; i < spans->GetLength(); i++) {
            (*spans)[i]->Release();
        }
        ArrayOf<IInterface*>::Free(spans);
    }

    return NOERROR;
}
