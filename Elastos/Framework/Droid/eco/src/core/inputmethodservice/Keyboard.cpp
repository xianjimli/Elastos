
#ifdef _FRAMEWORK_CORE
#include "inputmethodservice/Keyboard.h"
#include "utils/Xml.h"
#include "utils/CTypedValue.h"
#include "text/TextUtils.h"
#else
#include "Keyboard.h"
#include "Xml.h"
// #include "TextUtils.h"

#define VALIDATE_NOT_NULL(x) if (!(x)) { return E_ILLEGAL_ARGUMENT_EXCEPTION; }
#define ASSERT_SUCCEEDED(expr) assert(SUCCEEDED(expr));
#define FAIL_RETURN(expr) \
    do { \
        ECode ec = expr; \
        if (FAILED(ec)) return ec; \
    } while(0);
#endif

#include <elastos/Math.h>
#include <elastos/Character.h>
#include <elastos/AutoFree.h>


using namespace Elastos::Core;

const Int32 Keyboard::EDGE_LEFT;
const Int32 Keyboard::EDGE_RIGHT;
const Int32 Keyboard::EDGE_TOP;
const Int32 Keyboard::EDGE_BOTTOM;

CString Keyboard::TAG = "Keyboard";
CString Keyboard::TAG_KEYBOARD = "Keyboard";
CString Keyboard::TAG_ROW = "Row";
CString Keyboard::TAG_KEY = "Key";

const Int32 Keyboard::GRID_WIDTH;
const Int32 Keyboard::GRID_HEIGHT;
const Int32 Keyboard::GRID_SIZE;
Float Keyboard::SEARCH_DISTANCE = 1.8f;

static Int32 R_Styleable_Keyboard[] = {
    0x0101023d, 0x0101023e, 0x0101023f, 0x01010240
};

static Int32 R_Styleable_Keyboard_Row[] = {
    0x01010241, 0x0101024d
};

Keyboard::Row::Row(
    /* [in] */ Keyboard* parent)
    : mParent(parent)
{}

Keyboard::Row::Row(
    /* [in] */ IResources* res,
    /* [in] */ Keyboard* parent,
    /* [in] */ IXmlResourceParser* parser)
    : mParent(parent)
{
    assert(res != NULL && parent != NULL && parser != NULL);

    AutoPtr<ITypedArray> a;
    res->ObtainAttributes(Xml::AsAttributeSet(parser),
            ArrayOf<Int32>(R_Styleable_Keyboard,
                    sizeof(R_Styleable_Keyboard) / sizeof(Int32)) /*com.android.internal.R.styleable.Keyboard*/,
            (ITypedArray**)&a);
    mDefaultWidth = Keyboard::GetDimensionOrFraction(a,
            0 /*com.android.internal.R.styleable.Keyboard_keyWidth*/,
            parent->mDisplayWidth, parent->mDefaultWidth);
    mDefaultHeight = Keyboard::GetDimensionOrFraction(a,
            1 /*com.android.internal.R.styleable.Keyboard_keyHeight*/,
            parent->mDisplayHeight, parent->mDefaultHeight);
    mDefaultHorizontalGap = Keyboard::GetDimensionOrFraction(a,
            2 /*com.android.internal.R.styleable.Keyboard_horizontalGap*/,
            parent->mDisplayWidth, parent->mDefaultHorizontalGap);
    mVerticalGap = Keyboard::GetDimensionOrFraction(a,
            3 /*com.android.internal.R.styleable.Keyboard_verticalGap*/,
            parent->mDisplayHeight, parent->mDefaultVerticalGap);
    a->Recycle();
    a = NULL;
    res->ObtainAttributes(Xml::AsAttributeSet(parser),
            ArrayOf<Int32>(R_Styleable_Keyboard_Row,
                    sizeof(R_Styleable_Keyboard_Row) / sizeof(Int32)) /*com.android.internal.R.styleable.Keyboard_Row*/,
            (ITypedArray**)&a);
    a->GetInt32(0 /*com.android.internal.R.styleable.Keyboard_Row_rowEdgeFlags*/,
                0, &mRowEdgeFlags);
    a->GetResourceId(1 /*com.android.internal.R.styleable.Keyboard_Row_keyboardMode*/,
            0, &mMode);
    a->Recycle();
}

PInterface Keyboard::Row::Probe(
    /* [in]  */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)(IKeyboardRow*)this;
    }
    else if (riid == EIID_IKeyboardRow) {
        return (IKeyboardRow*)this;
    }

    return NULL;
}

UInt32 Keyboard::Row::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 Keyboard::Row::Release()
{
    return ElRefBase::Release();
}

ECode Keyboard::Row::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    if (pIID == NULL) {
        return E_INVALID_ARGUMENT;
    }

    if (pObject == (IInterface*)(IKeyboardRow*)this) {
        *pIID = EIID_IKeyboardRow;
    }
    else {
        return E_INVALID_ARGUMENT;
    }

    return NOERROR;
}

ECode Keyboard::Row::GetDefaultWidth(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);
    *value = mDefaultWidth;
    return NOERROR;
}

ECode Keyboard::Row::SetDefaultWidth(
    /* [in] */ Int32 value)
{
    mDefaultWidth = value;
    return NOERROR;
}

ECode Keyboard::Row::GetDefaultHeight(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);
    *value = mDefaultHeight;
    return NOERROR;
}

ECode Keyboard::Row::SetDefaultHeight(
    /* [in] */ Int32 value)
{
    mDefaultHeight = value;
    return NOERROR;
}

ECode Keyboard::Row::GetDefaultHorizontalGap(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);
    *value = mDefaultHorizontalGap;
    return NOERROR;
}

ECode Keyboard::Row::SetDefaultHorizontalGap(
    /* [in] */ Int32 value)
{
    mDefaultHorizontalGap = value;
    return NOERROR;
}

ECode Keyboard::Row::GetVerticalGap(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);
    *value = mVerticalGap;
    return NOERROR;
}

ECode Keyboard::Row::SetVerticalGap(
    /* [in] */ Int32 value)
{
    mVerticalGap = value;
    return NOERROR;
}

ECode Keyboard::Row::GetRowEdgeFlags(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);
    *value = mRowEdgeFlags;
    return NOERROR;
}

ECode Keyboard::Row::SetRowEdgeFlags(
    /* [in] */ Int32 value)
{
    mRowEdgeFlags = value;
    return NOERROR;
}

ECode Keyboard::Row::GetMode(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);
    *value = mMode;
    return NOERROR;
}

ECode Keyboard::Row::SetMode(
    /* [in] */ Int32 value)
{
    mMode = value;
    return NOERROR;
}


static Int32 R_Styleable_Keyboard_Key[] = {
    0x01010242, 0x01010243, 0x01010244, 0x01010245,
    0x01010246, 0x01010247, 0x01010248, 0x01010249,
    0x0101024a, 0x0101024b, 0x0101024c, 0x0101024d
};

Int32 Keyboard::Key::KEY_STATE_NORMAL_ON[] = {
    0x0101009f /*android.R.attr.state_checkable*/,
    0x010100a0 /*android.R.attr.state_checked*/
};

Int32 Keyboard::Key::KEY_STATE_PRESSED_ON[] = {
    0x010100a7 /*android.R.attr.state_pressed*/,
    0x0101009f /*android.R.attr.state_checkable*/,
    0x010100a0 /*android.R.attr.state_checked*/
};

Int32 Keyboard::Key::KEY_STATE_NORMAL_OFF[] = {
    0x0101009f /*android.R.attr.state_checkable*/
};

Int32 Keyboard::Key::KEY_STATE_PRESSED_OFF[] = {
    0x010100a7 /*android.R.attr.state_pressed*/,
    0x0101009f /*android.R.attr.state_checkable*/
};

Int32 Keyboard::Key::KEY_STATE_NORMAL[] = {
};

Int32 Keyboard::Key::KEY_STATE_PRESSED[] = {
    0x010100a7 /*android.R.attr.state_pressed*/,
};

Keyboard::Key::Key(
    /* [in] */ Row* parent)
    : mCodes(NULL)
    , mWidth(parent->mDefaultWidth)
    , mHeight(parent->mDefaultHeight)
    , mGap(parent->mDefaultHorizontalGap)
    , mSticky(FALSE)
    , mX(0)
    , mY(0)
    , mPressed(FALSE)
    , mOn(FALSE)
    , mEdgeFlags(parent->mRowEdgeFlags)
    , mModifier(FALSE)
    , mPopupResId(0)
    , mRepeatable(FALSE)
    , mKeyboard(parent->mParent)
{}

Keyboard::Key::Key(
    /* [in] */ IResources* res,
    /* [in] */ Row* parent,
    /* [in] */ Int32 x,
    /* [in] */ Int32 y,
    /* [in] */ IXmlResourceParser* parser)
    : mCodes(NULL)
    , mWidth(parent->mDefaultWidth)
    , mHeight(parent->mDefaultHeight)
    , mGap(parent->mDefaultHorizontalGap)
    , mSticky(FALSE)
    , mX(x)
    , mY(y)
    , mPressed(FALSE)
    , mOn(FALSE)
    , mEdgeFlags(parent->mRowEdgeFlags)
    , mModifier(FALSE)
    , mPopupResId(0)
    , mRepeatable(FALSE)
    , mKeyboard(parent->mParent)
{
    assert(res != NULL && parser != NULL);

    AutoPtr<ITypedArray> a;
    res->ObtainAttributes(Xml::AsAttributeSet(parser),
            ArrayOf<Int32>(R_Styleable_Keyboard,
                    sizeof(R_Styleable_Keyboard) / sizeof(Int32)) /*com.android.internal.R.styleable.Keyboard*/,
            (ITypedArray**)&a);

    mWidth = Keyboard::GetDimensionOrFraction(a,
            0 /*com.android.internal.R.styleable.Keyboard_keyWidth*/,
            mKeyboard->mDisplayWidth, parent->mDefaultWidth);
    mHeight = Keyboard::GetDimensionOrFraction(a,
            1 /*com.android.internal.R.styleable.Keyboard_keyHeight*/,
            mKeyboard->mDisplayHeight, parent->mDefaultHeight);
    mGap = Keyboard::GetDimensionOrFraction(a,
            2 /*com.android.internal.R.styleable.Keyboard_horizontalGap*/,
            mKeyboard->mDisplayWidth, parent->mDefaultHorizontalGap);
    a->Recycle();
    a = NULL;
    res->ObtainAttributes(Xml::AsAttributeSet(parser),
            ArrayOf<Int32>(R_Styleable_Keyboard_Key,
                    sizeof(R_Styleable_Keyboard_Key) / sizeof(Int32)) /*com.android.internal.R.styleable.Keyboard_Key*/,
            (ITypedArray**)&a);
    mX += mGap;
    AutoPtr<ITypedValue> codesValue;
    CTypedValue::New((ITypedValue**)&codesValue);
    Boolean ret = FALSE;
    a->GetValue(0 /*com.android.internal.R.styleable.Keyboard_Key_codes*/,
            codesValue, &ret);
    Int32 type;
    codesValue->GetType(&type);
    if (type == TypedValue_TYPE_INT_DEC
            || type == TypedValue_TYPE_INT_HEX) {
        mCodes = ArrayOf<Int32>::Alloc(1);
        Int32 data;
        codesValue->GetData(&data);
        (*mCodes)[0] = data;
    }
    else if (type == TypedValue_TYPE_STRING) {
        AutoPtr<ICharSequence> string;
        codesValue->GetString((ICharSequence**)&string);
        String str;
        string->ToString(&str);
        mCodes = ParseCSV(str);
    }

    a->GetDrawable(7 /*com.android.internal.R.styleable.Keyboard_Key_iconPreview*/,
            (IDrawable**)&mIconPreview);
    if (mIconPreview != NULL) {
        Int32 intrinsicWidth = 0, intrinsicHeight = 0;
        mIconPreview->GetIntrinsicWidth(&intrinsicWidth);
        mIconPreview->GetIntrinsicHeight(&intrinsicHeight);
        mIconPreview->SetBounds(0, 0, intrinsicWidth, intrinsicHeight);
    }
    a->GetText(
            2 /*com.android.internal.R.styleable.Keyboard_Key_popupCharacters*/,
            (ICharSequence**)&mPopupCharacters);
    a->GetResourceId(
            1 /*com.android.internal.R.styleable.Keyboard_Key_popupKeyboard*/, 0, &mPopupResId);
    a->GetBoolean(
            6 /*com.android.internal.R.styleable.Keyboard_Key_isRepeatable*/, FALSE, &mRepeatable);
    a->GetBoolean(
            4 /*com.android.internal.R.styleable.Keyboard_Key_isModifier*/, FALSE, &mModifier);
    a->GetBoolean(
            5 /*com.android.internal.R.styleable.Keyboard_Key_isSticky*/, FALSE, &mSticky);
    a->GetInt32(3 /*com.android.internal.R.styleable.Keyboard_Key_keyEdgeFlags*/, 0, &mEdgeFlags);
    mEdgeFlags |= parent->mRowEdgeFlags;
    a->GetDrawable(
            10 /*com.android.internal.R.styleable.Keyboard_Key_keyIcon*/, (IDrawable**)&mIcon);

    if (mIcon != NULL) {
        Int32 intrinsicWidth = 0, intrinsicHeight = 0;
        mIcon->GetIntrinsicWidth(&intrinsicWidth);
        mIcon->GetIntrinsicHeight(&intrinsicHeight);
        mIcon->SetBounds(0, 0, intrinsicWidth, intrinsicHeight);
    }
    a->GetText(9 /*com.android.internal.R.styleable.Keyboard_Key_keyLabel*/,
            (ICharSequence**)&mLabel);
    a->GetText(8 /*com.android.internal.R.styleable.Keyboard_Key_keyOutputText*/,
            (ICharSequence**)&mText);

    Int32 len;
    if (mCodes == NULL && mLabel != NULL && (mLabel->GetLength(&len), len != 0)/* !TextUtils::IsEmpty(mLabel)*/) {
        // mCodes = new Int32[] { mLabel.charAt(0) };
        mCodes = ArrayOf<Int32>::Alloc(1);
        Char32 c = 0;
        mLabel->GetCharAt(0, &c);
        (*mCodes)[0] = c;
    }
    a->Recycle();
}

Keyboard::Key::~Key()
{
    if (mCodes != NULL) {
        ArrayOf<Int32>::Free(mCodes);
    }
}

PInterface Keyboard::Key::Probe(
    /* [in]  */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)(IKeyboardKey*)this;
    }
    else if (riid == EIID_IKeyboardKey) {
        return (IKeyboardKey*)this;
    }

    return NULL;
}

UInt32 Keyboard::Key::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 Keyboard::Key::Release()
{
    return ElRefBase::Release();
}

ECode Keyboard::Key::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    if (pIID == NULL) {
        return E_INVALID_ARGUMENT;
    }

    if (pObject == (IInterface*)(IKeyboardKey*)this) {
        *pIID = EIID_IKeyboardKey;
    }
    else {
        return E_INVALID_ARGUMENT;
    }

    return NOERROR;
}

/**
 * Informs the key that it has been pressed, in case it needs to change its appearance or
 * state.
 * @see #onReleased(Boolean)
 */
ECode Keyboard::Key::OnPressed()
{
    mPressed = !mPressed;
    return NOERROR;
}

/**
 * Changes the pressed state of the key. If it is a sticky key, it will also change the
 * toggled state of the key if the finger was release inside.
 * @param inside whether the finger was released inside the key
 * @see #onPressed()
 */
ECode Keyboard::Key::OnReleased(
    /* [in] */ Boolean inside)
{
    mPressed = !mPressed;
    if (mSticky) {
        mOn = !mOn;
    }

    return NOERROR;
}

ArrayOf<Int32>* Keyboard::Key::ParseCSV(
    /* [in] */ const String& value)
{
    Int32 count = 0;
    Int32 lastIndex = 0;
    if (value.GetCharCount() > 0) {
        count++;
        while ((lastIndex = value.IndexOf(',', lastIndex + 1)) > 0) {
            count++;
        }
    }
    assert(count != 0);
    ArrayOf<Int32>* values = ArrayOf<Int32>::Alloc(count);
    count = 0;
    StringTokenizer st(value, ",");
    while (st.HasMoreTokens()) {
        // try {
        (*values)[count++] = st.NextToken().ToInt32();
        // } catch (NumberFormatException nfe) {
        //     Log.e(TAG, "Error parsing keycodes " + value);
        // }
    }
    return values;
}

ECode Keyboard::Key::IsInside(
    /* [in] */ Int32 x,
    /* [in] */ Int32 y,
    /* [out] */ Boolean* isInside)
{
    VALIDATE_NOT_NULL(isInside);
    Boolean leftEdge = (mEdgeFlags & Keyboard::EDGE_LEFT) > 0;
    Boolean rightEdge = (mEdgeFlags & Keyboard::EDGE_RIGHT) > 0;
    Boolean topEdge = (mEdgeFlags & Keyboard::EDGE_TOP) > 0;
    Boolean bottomEdge = (mEdgeFlags & Keyboard::EDGE_BOTTOM) > 0;

    if ((x >= mX || (leftEdge && x <= mX + mWidth))
            && (x < mX + mWidth || (rightEdge && x >= mX))
            && (y >= mY || (topEdge && y <= mY + mHeight))
            && (y < mY + mHeight || (bottomEdge && y >= mY))) {
        *isInside = TRUE;
        return NOERROR;
    }
    else {
        *isInside = FALSE;
        return NOERROR;
    }
}

ECode Keyboard::Key::SquaredDistanceFrom(
    /* [in] */ Int32 x,
    /* [in] */ Int32 y,
    /* [out] */ Int32* dis)
{
    VALIDATE_NOT_NULL(dis);
    Int32 xDist = mX + mWidth / 2 - x;
    Int32 yDist = mY + mHeight / 2 - y;
    *dis = xDist * xDist + yDist * yDist;
    return NOERROR;
}

ECode Keyboard::Key::GetCurrentDrawableState(
    /* [out, callee] */ ArrayOf<Int32>** drawableStates)
{
    VALIDATE_NOT_NULL(drawableStates != NULL);

    if (mOn) {
        if (mPressed) {
            *drawableStates = ArrayOf<Int32>::Alloc(3);
            (**drawableStates)[0] = KEY_STATE_PRESSED_ON[0];
            (**drawableStates)[1] = KEY_STATE_PRESSED_ON[1];
            (**drawableStates)[2] = KEY_STATE_PRESSED_ON[2];
        }
        else {
            *drawableStates = ArrayOf<Int32>::Alloc(2);
            (**drawableStates)[0] = KEY_STATE_NORMAL_ON[0];
            (**drawableStates)[1] = KEY_STATE_NORMAL_ON[1];
        }
    }
    else {
        if (mSticky) {
            if (mPressed) {
                *drawableStates = ArrayOf<Int32>::Alloc(2);
                (**drawableStates)[0] = KEY_STATE_PRESSED_OFF[0];
                (**drawableStates)[1] = KEY_STATE_PRESSED_OFF[1];
            }
            else {
                *drawableStates = ArrayOf<Int32>::Alloc(1);
                (**drawableStates)[0] = KEY_STATE_NORMAL_OFF[0];
            }
        }
        else {
            if (mPressed) {
                *drawableStates = ArrayOf<Int32>::Alloc(1);
                (**drawableStates)[0] = KEY_STATE_PRESSED[0];
            }
        }
    }
    *drawableStates = ArrayOf<Int32>::Alloc(0);
    return NOERROR;
}

ECode Keyboard::Key::GetCodes(
    /* [out, callee] */ ArrayOf<Int32>** codes)
{
    VALIDATE_NOT_NULL(codes);
    *codes = mCodes != NULL ? mCodes->Clone() : NULL;
    return NOERROR;
}

ECode Keyboard::Key::SetCodes(
    /* [in] */ ArrayOf<Int32>* codes)
{
    if (mCodes != NULL) ArrayOf<Int32>::Free(mCodes);
    mCodes = codes;
    return NOERROR;
}

ECode Keyboard::Key::GetLabel(
    /* [in] */ ICharSequence** label)
{
    VALIDATE_NOT_NULL(label);
    *label = mLabel;
    if (*label != NULL) (*label)->AddRef();
    return NOERROR;
}

ECode Keyboard::Key::SetLabel(
    /* [in] */ ICharSequence* label)
{
    mLabel = label;
    return NOERROR;
}

ECode Keyboard::Key::GetIcon(
    /* [out] */ IDrawable** icon)
{
    VALIDATE_NOT_NULL(icon);
    *icon = mIcon;
    if (*icon != NULL) (*icon)->AddRef();
    return NOERROR;
}

ECode Keyboard::Key::SetIcon(
    /* [in] */ IDrawable* icon)
{
    mIcon = icon;
    return NOERROR;
}

ECode Keyboard::Key::GetIconPreview(
    /* [out] */ IDrawable** preview)
{
    VALIDATE_NOT_NULL(preview);
    *preview = mIconPreview;
    if (*preview != NULL) (*preview)->AddRef();
    return NOERROR;
}

ECode Keyboard::Key::SetIconPreview(
    /* [in] */ IDrawable* preview)
{
    mIconPreview = preview;
    return NOERROR;
}

ECode Keyboard::Key::GetWidth(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);
    *value = mWidth;
    return NOERROR;
}

ECode Keyboard::Key::SetWidth(
    /* [in] */ Int32 value)
{
    mWidth = value;
    return NOERROR;
}

ECode Keyboard::Key::GetHeight(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);
    *value = mHeight;
    return NOERROR;
}

ECode Keyboard::Key::SetHeight(
    /* [in] */ Int32 value)
{
    mHeight = value;
    return NOERROR;
}

ECode Keyboard::Key::GetGap(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);
    *value = mGap;
    return NOERROR;
}

ECode Keyboard::Key::SetGap(
    /* [in] */ Int32 value)
{
    mGap = value;
    return NOERROR;
}

ECode Keyboard::Key::IsSticky(
    /* [out] */ Boolean* value)
{
    VALIDATE_NOT_NULL(value);
    *value = mOn;
    return NOERROR;
}

ECode Keyboard::Key::SetSticky(
    /* [in] */ Boolean value)
{
    mOn = value;
    return NOERROR;
}

ECode Keyboard::Key::GetX(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);
    *value = mX;
    return NOERROR;
}

ECode Keyboard::Key::SetX(
    /* [in] */ Int32 value)
{
    mX = value;
    return NOERROR;
}

ECode Keyboard::Key::GetY(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);
    *value = mY;
    return NOERROR;
}

ECode Keyboard::Key::SetY(
    /* [in] */ Int32 value)
{
    mY = value;
    return NOERROR;
}

ECode Keyboard::Key::GetText(
    /* [out] */ ICharSequence** text)
{
    VALIDATE_NOT_NULL(text);
    *text = mText;
    if (*text != NULL) (*text)->AddRef();
    return NOERROR;
}

ECode Keyboard::Key::GetPopupCharacters(
    /* [out] */ ICharSequence** popupCharacters)
{
    VALIDATE_NOT_NULL(popupCharacters);
    *popupCharacters = mPopupCharacters;
    if (*popupCharacters != NULL) (*popupCharacters)->AddRef();
    return NOERROR;
}

ECode Keyboard::Key::GetPopupResId(
    /* [out] */ Int32* popupResId)
{
    VALIDATE_NOT_NULL(popupResId);
    *popupResId = mPopupResId;
    return NOERROR;
}

ECode Keyboard::Key::IsRepeatable(
    /* [out] */ Boolean* repeatable)
{
    VALIDATE_NOT_NULL(repeatable);
    *repeatable = mRepeatable;
    return NOERROR;
}


Keyboard::Keyboard(
    /* [in] */ IContext* context,
    /* [in] */ Int32 xmlLayoutResId)
    : mShifted(FALSE)
    , mShiftKeyIndex(-1)
    , mKeyWidth(0)
    , mKeyHeight(0)
    , mTotalHeight(0)
    , mTotalWidth(0)
    , mKeyboardMode(0)
    , mCellWidth(0)
    , mCellHeight(0)
    , mGridNeighbors(NULL)
{
    ASSERT_SUCCEEDED(Init(context, xmlLayoutResId));
}

Keyboard::Keyboard(
    /* [in] */ IContext* context,
    /* [in] */ Int32 xmlLayoutResId,
    /* [in] */ Int32 modeId)
    : mShifted(FALSE)
    , mShiftKeyIndex(-1)
    , mKeyWidth(0)
    , mKeyHeight(0)
    , mTotalHeight(0)
    , mTotalWidth(0)
    , mKeyboardMode(0)
    , mCellWidth(0)
    , mCellHeight(0)
    , mGridNeighbors(NULL)
{
    ASSERT_SUCCEEDED(Init(context, xmlLayoutResId, modeId));
}

Keyboard::Keyboard(
    /* [in] */ IContext* context,
    /* [in] */ Int32 layoutTemplateResId,
    /* [in] */ ICharSequence* characters,
    /* [in] */ Int32 columns,
    /* [in] */ Int32 horizontalPadding)
    : mShifted(FALSE)
    , mShiftKeyIndex(-1)
    , mKeyWidth(0)
    , mKeyHeight(0)
    , mTotalHeight(0)
    , mTotalWidth(0)
    , mKeyboardMode(0)
    , mCellWidth(0)
    , mCellHeight(0)
    , mGridNeighbors(NULL)
{
    // Init(context, layoutTemplateResId, characters, columns, horizontalPadding);
    ASSERT_SUCCEEDED(Init(context, layoutTemplateResId));
    Int32 x = 0;
    Int32 y = 0;
    Int32 column = 0;
    mTotalWidth = 0;

    AutoPtr<Row> row = new Row(this);
    row->mDefaultHeight = mDefaultHeight;
    row->mDefaultWidth = mDefaultWidth;
    row->mDefaultHorizontalGap = mDefaultHorizontalGap;
    row->mVerticalGap = mDefaultVerticalGap;
    row->mRowEdgeFlags = EDGE_TOP | EDGE_BOTTOM;
    Int32 maxColumns = columns == -1 ? Math::INT32_MAX_VALUE : columns;
    Int32 len = 0;
    characters->GetLength(&len);
    for (Int32 i = 0; i < len; i++) {
        Char32 c = 0;
        characters->GetCharAt(i, &c);
        if (column >= maxColumns
                || x + mDefaultWidth + horizontalPadding > mDisplayWidth) {
            x = 0;
            y += mDefaultVerticalGap + mDefaultHeight;
            column = 0;
        }

        AutoPtr<Key> key = new Key(row);
        key->mX = x;
        key->mY = y;
        // key->mLabel = String.ValueOf(c);
        ArrayOf_<Char8, 5> chs;
        Int32 num = 0;
        Character::ToChars(c, chs, 0, &num);
        chs[num] = 0;
        AutoPtr<ICharSequence> label;
        CStringWrapper::New(String(chs.GetPayload()), (ICharSequence**)&label);
        key->mLabel = label;
        key->mCodes = ArrayOf<Int32>::Alloc(1);
        (*key->mCodes)[0] = c;
        column++;
        x += key->mWidth + key->mGap;
        mKeys.PushBack(key);
        if (x > mTotalWidth) {
            mTotalWidth = x;
        }
    }
    mTotalHeight = y + mDefaultHeight;
}

Keyboard::~Keyboard()
{
    if (mGridNeighbors != NULL) {
        Int32 childSize = mGridNeighbors->GetLength();
        for (Int32 i = 0; i < childSize; i++) {
            if ((*mGridNeighbors)[i] != NULL) {
                ArrayOf<Int32>::Free((*mGridNeighbors)[i]);
            }
        }

        ArrayOf<ArrayOf<Int32>*>::Free(mGridNeighbors);
        mGridNeighbors = NULL;
    }
}

PInterface Keyboard::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)(IKeyboard*)this;
    }
    else if (riid == EIID_IKeyboard) {
        return (IKeyboard*)this;
    }

    return NULL;
}

UInt32 Keyboard::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 Keyboard::Release()
{
    return ElRefBase::Release();
}

ECode Keyboard::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    if (NULL == pIID) return E_INVALID_ARGUMENT;

    if (pObject == (IInterface *)(IKeyboard *)this) {
        *pIID = EIID_IKeyboard;
        return NOERROR;
    }

    return E_INVALID_ARGUMENT;
}

ECode Keyboard::GetKeys(
    /* [out] */ IObjectContainer** keys)
{
    assert(keys != NULL);
    FAIL_RETURN(CObjectContainer::New(keys));
    List< AutoPtr<Key> >::Iterator it;
    for (it = mKeys.Begin(); it != mKeys.End(); it++) {
        (*keys)->Add((IKeyboardKey*)(*it)->Probe(EIID_IKeyboardKey));
    }

    return NOERROR;
}

ECode Keyboard::GetModifierKeys(
    /* [out] */ IObjectContainer** keys)
{
    List< AutoPtr<Key> >::Iterator it;
    for (it = mModifierKeys.Begin(); it != mModifierKeys.End(); it++) {
        (*keys)->Add((IKeyboardKey*)(*it)->Probe(EIID_IKeyboardKey));
    }

    return NOERROR;
}

ECode Keyboard::GetHorizontalGap(
    /* [out] */ Int32* hGap)
{
    assert(hGap != NULL);
    *hGap = mDefaultHorizontalGap;
    return NOERROR;
}

ECode Keyboard::SetHorizontalGap(
    /* [in] */ Int32 gap)
{
    mDefaultHorizontalGap = gap;
    return NOERROR;
}

ECode Keyboard::GetVerticalGap(
    /* [out] */ Int32* vGap)
{
    assert(vGap != NULL);
    *vGap = mDefaultVerticalGap;
    return NOERROR;
}

ECode Keyboard::SetVerticalGap(
    /* [in] */ Int32 gap)
{
    mDefaultVerticalGap = gap;
    return NOERROR;
}

ECode Keyboard::GetKeyHeight(
    /* [out] */ Int32* h)
{
    assert(h != NULL);
    *h = mDefaultHeight;
    return NOERROR;
}

ECode Keyboard::SetKeyHeight(
    /* [in] */ Int32 height)
{
    mDefaultHeight = height;
    return NOERROR;
}

ECode Keyboard::GetKeyWidth(
    /* [out] */ Int32* w)
{
    assert(w != NULL);
    *w = mDefaultWidth;
    return NOERROR;
}

ECode Keyboard::SetKeyWidth(
    /* [in] */ Int32 width)
{
    mDefaultWidth = width;
    return NOERROR;
}

ECode Keyboard::GetHeight(
    /* [out] */ Int32* h)
{
    assert(h != NULL);
    *h = mTotalHeight;
    return NOERROR;
}

ECode Keyboard::GetMinWidth(
    /* [out] */ Int32* minW)
{
    assert(minW != NULL);
    *minW = mTotalWidth;
    return NOERROR;
}

ECode Keyboard::SetShifted(
    /* [in] */ Boolean shiftState,
    /* [out] */ Boolean* res)
{
    assert(res != NULL);
    if (mShiftKey != NULL) {
        mShiftKey->SetSticky(shiftState);
    }
    if (mShifted != shiftState) {
        mShifted = shiftState;
        *res = TRUE;
        return NOERROR;
    }
    *res = FALSE;
    return NOERROR;
}

ECode Keyboard::IsShifted(
    /* [out] */ Boolean* res)
{
    assert(res != NULL);
    *res = mShifted;
    return NOERROR;
}

ECode Keyboard::GetShiftKeyIndex(
    /* [out] */ Int32* index)
{
    assert(index != NULL);
    *index = mShiftKeyIndex;
    return NOERROR;
}

void Keyboard::ComputeNearestNeighbors()
{
    Int32 w = 0, h = 0;
    GetMinWidth(&w);
    GetHeight(&h);
    // Round-up so we don't have any pixels outside the grid
    mCellWidth = (w + GRID_WIDTH - 1) / GRID_WIDTH;
    mCellHeight = (h + GRID_HEIGHT - 1) / GRID_HEIGHT;
    //mGridNeighbors = new Int32[GRID_SIZE][];
    mGridNeighbors = ArrayOf<ArrayOf<Int32>*>::Alloc(GRID_SIZE);
    AutoFree< ArrayOf<Int32> > indices = ArrayOf<Int32>::Alloc(mKeys.GetSize());
    Int32 gridWidth = GRID_WIDTH * mCellWidth;
    Int32 gridHeight = GRID_HEIGHT * mCellHeight;
    for (Int32 x = 0; x < gridWidth; x += mCellWidth) {
        for (Int32 y = 0; y < gridHeight; y += mCellHeight) {
            Int32 count = 0, i = 0;
            List< AutoPtr<Key> >::Iterator it;
            for (it = mKeys.Begin(); it != mKeys.End(); ++it, ++i) {
                AutoPtr<Key> key = *it;
                Int32 dis1 = 0, dis2 = 0, dis3 = 0, dis4 = 0;
                key->SquaredDistanceFrom(x, y, &dis1);
                key->SquaredDistanceFrom(x + mCellWidth - 1, y, &dis2);
                key->SquaredDistanceFrom(x + mCellWidth - 1, y + mCellHeight - 1, &dis3);
                key->SquaredDistanceFrom(x, y + mCellHeight - 1, &dis4);
                if (dis1 < mProximityThreshold ||
                    dis2 < mProximityThreshold ||
                    dis3 < mProximityThreshold ||
                    dis4 < mProximityThreshold) {
                    (*indices)[count++] = i;
                }
            }
            ArrayOf<Int32>* cell = ArrayOf<Int32>::Alloc(count);
            memcpy(cell->GetPayload(), indices->GetPayload(), count * sizeof(Int32));
            (*mGridNeighbors)[(y / mCellHeight) * GRID_WIDTH + (x / mCellWidth)] = cell;
        }
    }
}

ECode Keyboard::GetNearestKeys(
    /* [in] */ Int32 x,
    /* [in] */ Int32 y,
    /* [out, callee] */ ArrayOf<Int32>** nearestKeys,
    /* [out] */ Int32* count)
{
    if (mGridNeighbors == NULL) {
        ComputeNearestNeighbors();
    }
    Int32 w = 0, h = 0;
    GetMinWidth(&w);
    GetHeight(&h);
    if (x >= 0 && x < w && y >= 0 && y < h) {
        Int32 index = (y / mCellHeight) * GRID_WIDTH + (x / mCellWidth);
        if (index < GRID_SIZE) {
            ArrayOf<Int32>* keys = (*mGridNeighbors)[index];
            *nearestKeys = keys != NULL ? keys->Clone() : NULL;
            return NOERROR;
        }
    }
    *nearestKeys = ArrayOf<Int32>::Alloc(0);
    return NOERROR;
}

AutoPtr<Keyboard::Row> Keyboard::CreateRowFromXml(
    /* [in] */ IResources* res,
    /* [in] */ IXmlResourceParser* parser)
{
    AutoPtr<Row> row = new Row(res, this, parser);
    return row;
}

AutoPtr<Keyboard::Key> Keyboard::CreateKeyFromXml(
    /* [in] */ IResources* res,
    /* [in] */ Row* parent,
    /* [in] */ Int32 x,
    /* [in] */ Int32 y,
    /* [in] */ IXmlResourceParser* parser)
{
    AutoPtr<Key> key = new Key(res, parent, x, y, parser);
    return key;
}

void Keyboard::LoadKeyboard(
    /* [in] */ IContext* context,
    /* [in] */ IXmlResourceParser* parser)
{
    assert(context != NULL && parser != NULL);

    Boolean inKey = FALSE;
    Boolean inRow = FALSE;
    Boolean leftMostKey = FALSE;
    Int32 row = 0;
    Int32 x = 0;
    Int32 y = 0;
    AutoPtr<Key> key;
    AutoPtr<Row> currentRow;
    AutoPtr<IResources> res;
    context->GetResources((IResources**)&res);
    Boolean skipRow = FALSE;
    // try{
    Int32 event = 0;
    while (parser->Next(&event), event != IXmlPullParser_END_DOCUMENT) {
        if (event == IXmlPullParser_START_TAG) {
            String tag;
            parser->GetName(&tag);
            if (TAG_ROW.Equals(tag)) {
                inRow = TRUE;
                x = 0;
                currentRow = CreateRowFromXml(res, parser);
                skipRow = currentRow->mMode != 0 && currentRow->mMode != mKeyboardMode;
                if (skipRow) {
                    SkipToEndOfRow(parser);
                    inRow = FALSE;
                }
           }
           else if (TAG_KEY.Equals(tag)) {
                inKey = TRUE;
                key = CreateKeyFromXml(res, currentRow, x, y, parser);
                mKeys.PushBack(key);
                ArrayOf<Int32>* codes = key->mCodes;
                if ((*codes)[0] == Keyboard_KEYCODE_SHIFT) {
                    mShiftKey = key;
                    mShiftKeyIndex = mKeys.GetSize() - 1;
                    mModifierKeys.PushBack(key);
                }
                else if ((*codes)[0] == Keyboard_KEYCODE_ALT) {
                    mModifierKeys.PushBack(key);
                }
            }
            else if (TAG_KEYBOARD.Equals(tag)) {
                ParseKeyboardAttributes(res, parser);
            }
        }
        else if (event == IXmlPullParser_END_TAG) {
            if (inKey) {
                inKey = FALSE;
                x += key->mGap + key->mWidth;
                if (x > mTotalWidth) {
                    mTotalWidth = x;
                }
            }
            else if (inRow) {
                inRow = FALSE;
                y += currentRow->mVerticalGap;
                y += currentRow->mDefaultHeight;
                row++;
            }
            else {
                // TODO: error or extend?
            }
        }
    }
    // } catch (Exception e) {
    //     Log.e(TAG, "Parse error:" + e);
    //     e.printStackTrace();
    // }
    mTotalHeight = y - mDefaultVerticalGap;
}

void Keyboard::SkipToEndOfRow(
    /* [in] */ IXmlResourceParser* parser)
{
    Int32 event = 0;
    while (parser->Next(&event), event != IXmlPullParser_END_DOCUMENT) {
        String tag;
        parser->GetName(&tag);
        if (event == IXmlPullParser_END_TAG && TAG_ROW.Equals(tag)) {
            break;
        }
    }
}

void Keyboard::ParseKeyboardAttributes(
    /* [in] */ IResources* res,
    /* [in] */ IXmlResourceParser* parser)
{
    AutoPtr<ITypedArray> a;
    res->ObtainAttributes(Xml::AsAttributeSet(parser),
            ArrayOf<Int32>(R_Styleable_Keyboard,
                    sizeof(R_Styleable_Keyboard) / sizeof(Int32)) /*com.android.internal.R.styleable.Keyboard*/,
            (ITypedArray**)&a);

    mDefaultWidth = GetDimensionOrFraction(a,
            0 /*com.android.internal.R.styleable.Keyboard_keyWidth*/,
            mDisplayWidth, mDisplayWidth / 10);
    mDefaultHeight = GetDimensionOrFraction(a,
            1 /*com.android.internal.R.styleable.Keyboard_keyHeight*/,
            mDisplayHeight, 50);
    mDefaultHorizontalGap = GetDimensionOrFraction(a,
            2 /*com.android.internal.R.styleable.Keyboard_horizontalGap*/,
            mDisplayWidth, 0);
    mDefaultVerticalGap = GetDimensionOrFraction(a,
            3 /*com.android.internal.R.styleable.Keyboard_verticalGap*/,
            mDisplayHeight, 0);

    mProximityThreshold = (Int32) (mDefaultWidth * SEARCH_DISTANCE);
    mProximityThreshold = mProximityThreshold * mProximityThreshold; // Square it for comparison

    a->Recycle();
}

Int32 Keyboard::GetDimensionOrFraction(
    /* [in] */ ITypedArray* a,
    /* [in] */ Int32 index,
    /* [in] */ Int32 base,
    /* [in] */ Int32 defValue)
{
    assert(a != NULL);
    AutoPtr<ITypedValue> value;
    a->PeekValue(index, (ITypedValue**)&value);
    if (value == NULL) return defValue;
    Int32 type = 0;
    value->GetType(&type);
    if (type == TypedValue_TYPE_DIMENSION) {
        Int32 retValue = 0;
        a->GetDimensionPixelOffset(index, defValue, &retValue);
        return retValue;
    }
    else if (type == TypedValue_TYPE_FRACTION) {
        Float tmpValue = 0.0;
        a->GetFraction(index, base, base, defValue, &tmpValue);
        // Round it to avoid values like 47.9999 from getting truncated
        return Math::Round(tmpValue);
    }
    return defValue;
}

ECode Keyboard::Init(
    /* [in] */ IContext* context,
    /* [in] */ Int32 xmlLayoutResId,
    /* [in] */ Int32 modeId)
{
    assert(context != NULL);
    AutoPtr<IResources> res;
    FAIL_RETURN(context->GetResources((IResources**)&res));
    AutoPtr<IDisplayMetrics> dm;
    FAIL_RETURN(res->GetDisplayMetrics((IDisplayMetrics**)&dm));
    dm->GetWidthPixels(&mDisplayWidth);
    dm->GetHeightPixels(&mDisplayHeight);
    //Log.v(TAG, "keyboard's display metrics:" + dm);

    mDefaultHorizontalGap = 0;
    mDefaultWidth = mDisplayWidth / 10;
    mDefaultVerticalGap = 0;
    mDefaultHeight = mDefaultWidth;
    mKeyboardMode = modeId;

    AutoPtr<IXmlResourceParser> parser;
    FAIL_RETURN(res->GetXml(xmlLayoutResId, (IXmlResourceParser**)&parser));
    LoadKeyboard(context, parser);
    return NOERROR;
}
