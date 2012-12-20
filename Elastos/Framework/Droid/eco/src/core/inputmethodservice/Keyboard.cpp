
#include "inputmethodservice/Keyboard.h"
#include <elastos/Math.h>
#include <elastos/Character.h>
#include "utils/Xml.h"
#include "utils/CTypedValue.h"
#include "text/TextUtils.h"
#include "utils/CDisplayMetrics.h"

using namespace Elastos::Core;

const Int32 Keyboard::EDGE_LEFT;
const Int32 Keyboard::EDGE_RIGHT;
const Int32 Keyboard::EDGE_TOP;
const Int32 Keyboard::EDGE_BOTTOM;

const Int32 Keyboard::KEYCODE_SHIFT;
const Int32 Keyboard::KEYCODE_MODE_CHANGE;
const Int32 Keyboard::KEYCODE_CANCEL;
const Int32 Keyboard::KEYCODE_DONE;
const Int32 Keyboard::KEYCODE_DELETE;
const Int32 Keyboard::KEYCODE_ALT;
CString Keyboard::TAG = "Keyboard";
CString Keyboard::TAG_KEYBOARD = "Keyboard";
CString Keyboard::TAG_ROW = "Row";
CString Keyboard::TAG_KEY = "Key";

const Int32 Keyboard::GRID_WIDTH;
const Int32 Keyboard::GRID_HEIGHT;
const Int32 Keyboard::GRID_SIZE;
float Keyboard::SEARCH_DISTANCE = 1.8f;

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

ArrayOf<Int32>* Keyboard::Key::KEY_STATE_NORMAL = NULL;

Int32 Keyboard::Key::KEY_STATE_PRESSED[] = {
    0x010100a7 /*android.R.attr.state_pressed*/,
};

static Int32 R_Styleable_Keyboard[] = {
    0x0101023d, 0x0101023e, 0x0101023f, 0x01010240
};

static Int32 R_Styleable_Keyboard_Key[] = {
    0x01010242, 0x01010243, 0x01010244, 0x01010245,
    0x01010246, 0x01010247, 0x01010248, 0x01010249,
    0x0101024a, 0x0101024b, 0x0101024c, 0x0101024d
};

static Int32 R_Styleable_Keyboard_Row[] = {
    0x01010241, 0x0101024d
};

Keyboard::Row::Row(
    /* [in] */ Keyboard* parent)
{
    mParent = parent;
}

Keyboard::Row::Row(
    /* [in] */ IResources* res,
    /* [in] */ Keyboard* parent,
    /* [in] */ IXmlResourceParser* parser)
{
    assert(res != NULL && parent != NULL && parser != NULL);
    mParent = parent;
    AutoPtr<ITypedArray> a;
    res->ObtainAttributes(Xml::AsAttributeSet(parser),
            ArrayOf<Int32>(R_Styleable_Keyboard,
                    sizeof(R_Styleable_Keyboard) / sizeof(Int32)) /*com.android.internal.R.styleable.Keyboard*/,
            (ITypedArray**) &a);

    mDefaultWidth = GetDimensionOrFraction(a,
            0 /*com.android.internal.R.styleable.Keyboard_keyWidth*/,
            parent->mDisplayWidth, parent->mDefaultWidth);

    mDefaultHeight = GetDimensionOrFraction(a,
            1 /*com.android.internal.R.styleable.Keyboard_keyHeight*/,
            parent->mDisplayHeight, parent->mDefaultHeight);

    mDefaultHorizontalGap = GetDimensionOrFraction(a,
            2 /*com.android.internal.R.styleable.Keyboard_horizontalGap*/,
            parent->mDisplayWidth, parent->mDefaultHorizontalGap);

    mVerticalGap = GetDimensionOrFraction(a,
            3 /*com.android.internal.R.styleable.Keyboard_verticalGap*/,
            parent->mDisplayHeight, parent->mDefaultVerticalGap);

    a->Recycle();
    res->ObtainAttributes(Xml::AsAttributeSet(parser),
            ArrayOf<Int32>(R_Styleable_Keyboard_Row,
                    sizeof(R_Styleable_Keyboard_Row) / sizeof(Int32)) /*com.android.internal.R.styleable.Keyboard_Row*/,
            (ITypedArray**) &a);

    a->GetInt32(0 /*com.android.internal.R.styleable.Keyboard_Row_rowEdgeFlags*/,
                0, &mRowEdgeFlags);

    a->GetResourceId(1 /*com.android.internal.R.styleable.Keyboard_Row_keyboardMode*/,
            0, &mMode);
    a->Recycle();
}

UInt32 Keyboard::Row::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 Keyboard::Row::Release()
{
    return ElRefBase::Release();
}

Keyboard::Key::Key(
    /* [in] */ Row* parent)
{
    Init(parent);
}

Keyboard::Key::Key(
    /* [in] */ IResources* res,
    /* [in] */ Row* parent,
    /* [in] */ Int32 x,
    /* [in] */ Int32 y,
    /* [in] */ IXmlResourceParser* parser)
{
    // this(parent);
    Init(parent);

    mX = x;
    mY = y;

    assert(res != NULL && parser != NULL);
    AutoPtr<ITypedArray> a;
    res->ObtainAttributes(Xml::AsAttributeSet(parser),
            ArrayOf<Int32>(R_Styleable_Keyboard,
                    sizeof(R_Styleable_Keyboard) / sizeof(Int32)) /*com.android.internal.R.styleable.Keyboard*/,
            (ITypedArray**) &a);

    mWidth = GetDimensionOrFraction(a,
            0 /*com.android.internal.R.styleable.Keyboard_keyWidth*/,
            mKeyboard->mDisplayWidth, parent->mDefaultWidth);

    mHeight = GetDimensionOrFraction(a,
            1 /*com.android.internal.R.styleable.Keyboard_keyHeight*/,
            mKeyboard->mDisplayHeight, parent->mDefaultHeight);

    mGap = GetDimensionOrFraction(a,
            2 /*com.android.internal.R.styleable.Keyboard_horizontalGap*/,
            mKeyboard->mDisplayWidth, parent->mDefaultHorizontalGap);
    a->Recycle();

    res->ObtainAttributes(Xml::AsAttributeSet(parser),
            ArrayOf<Int32>(R_Styleable_Keyboard_Key,
                    sizeof(R_Styleable_Keyboard_Key) / sizeof(Int32)) /*com.android.internal.R.styleable.Keyboard_Key*/,
            (ITypedArray**) &a);

    mX += mGap;
    AutoPtr<ITypedValue> codesValue;
    CTypedValue::New((ITypedValue**) &codesValue);

    Boolean ret = FALSE;
    a->GetValue(0 /*com.android.internal.R.styleable.Keyboard_Key_codes*/,
            codesValue, &ret);

    if (((CTypedValue*)codesValue.Get())->mType == TypedValue_TYPE_INT_DEC
            || ((CTypedValue*)codesValue.Get())->mType == TypedValue_TYPE_INT_HEX)
    {
        mCodes = ArrayOf<Int32>::Alloc(1);
        (*mCodes)[0] = ((CTypedValue*)codesValue.Get())->mData;
    } else if (((CTypedValue*)codesValue.Get())->mType == TypedValue_TYPE_STRING)
    {
        String str;
        ((CTypedValue*)codesValue.Get())->mString->ToString(&str);
        mCodes = ParseCSV(str);
    }

    a->GetDrawable(7 /*com.android.internal.R.styleable.Keyboard_Key_iconPreview*/,
            (IDrawable**) &mIconPreview);

    if (mIconPreview != NULL) {
        Int32 intrinsicWidth = 0, intrinsicHeight = 0;
        mIconPreview->GetIntrinsicWidth(&intrinsicWidth);
        mIconPreview->GetIntrinsicHeight(&intrinsicHeight);
        mIconPreview->SetBounds(0, 0, intrinsicWidth,
                intrinsicHeight);
    }

    a->GetText(
            2 /*com.android.internal.R.styleable.Keyboard_Key_popupCharacters*/,
            (ICharSequence**) &mPopupCharacters);

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
            10 /*com.android.internal.R.styleable.Keyboard_Key_keyIcon*/, (IDrawable**) &mIcon);

    if (mIcon != NULL) {
        Int32 intrinsicWidth = 0, intrinsicHeight = 0;
        mIcon->GetIntrinsicWidth(&intrinsicWidth);
        mIcon->GetIntrinsicHeight(&intrinsicHeight);
        mIcon->SetBounds(0, 0, intrinsicWidth,
                intrinsicHeight);
    }

    a->GetText(9 /*com.android.internal.R.styleable.Keyboard_Key_keyLabel*/,
            (ICharSequence**) &mLabel);

    a->GetText(8 /*com.android.internal.R.styleable.Keyboard_Key_keyOutputText*/,
            (ICharSequence**) &mText);

    if (mCodes == NULL && !TextUtils::IsEmpty(mLabel)) {
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
        mCodes = NULL;
    }
}

UInt32 Keyboard::Key::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 Keyboard::Key::Release()
{
    return ElRefBase::Release();
}

void Keyboard::Key::OnPressed()
{
    mPressed = !mPressed;
}

void Keyboard::Key::OnReleased(
    /* [in] */ Boolean inside)
{
    mPressed = !mPressed;
    if (mSticky) {
        mOn = !mOn;
    }
}

ArrayOf<Int32>* Keyboard::Key::ParseCSV(
    /* [in] */ const String& value)
{
    Int32 count = 0;
    Int32 lastIndex = 0;
    //if (value.length() > 0) {
    if (value.GetCharCount() > 0) {
        count++;
        while ((lastIndex = value.IndexOf(',', lastIndex + 1)) > 0) {
            count++;
        }
    }

    assert(count != 0);
    ArrayOf<Int32>* values = ArrayOf<Int32>::Alloc(count);
    count = 0;
    StringTokenizer* st = new StringTokenizer(value, ",");
    while (st->HasMoreTokens()) {
        // try {
        (*values)[count++] = st->NextToken().ToInt32();
        // } catch (NumberFormatException nfe) {
        //     Log.e(TAG, "Error parsing keycodes " + value);
        // }
    }

    delete st;
    st = NULL;

    return values;
}

Boolean Keyboard::Key::IsInside(
    /* [in] */ Int32 x,
    /* [in] */ Int32 y)
{
    Boolean leftEdge = (mEdgeFlags & EDGE_LEFT) > 0;
    Boolean rightEdge = (mEdgeFlags & EDGE_RIGHT) > 0;
    Boolean topEdge = (mEdgeFlags & EDGE_TOP) > 0;
    Boolean bottomEdge = (mEdgeFlags & EDGE_BOTTOM) > 0;

    if ((x >= mX || (leftEdge && x <= mX + mWidth))
            && (x < mX + mWidth || (rightEdge && x >= mX))
            && (y >= mY || (topEdge && y <= mY + mHeight))
            && (y < mY + mHeight || (bottomEdge && y >= mY))) {
        return TRUE;
    } else {
        return FALSE;
    }
}

Int32 Keyboard::Key::SquaredDistanceFrom(
    /* [in] */ Int32 x,
    /* [in] */ Int32 y)
{
    Int32 xDist = mX + mWidth / 2 - x;
    Int32 yDist = mY + mHeight / 2 - y;
    return xDist * xDist + yDist * yDist;
}

ArrayOf<Int32>* Keyboard::Key::GetCurrentDrawableState()
{
    if (KEY_STATE_NORMAL != NULL) {
        ArrayOf<Int32>::Free(KEY_STATE_NORMAL);
        KEY_STATE_NORMAL = NULL;
    }

    if (mOn) {
        if (mPressed) {
            KEY_STATE_NORMAL = ArrayOf<Int32>::Alloc(3);
            (*KEY_STATE_NORMAL)[0] = KEY_STATE_PRESSED_ON[0];
            (*KEY_STATE_NORMAL)[1] = KEY_STATE_PRESSED_ON[1];
            (*KEY_STATE_NORMAL)[2] = KEY_STATE_PRESSED_ON[2];
        } else {
            KEY_STATE_NORMAL = ArrayOf<Int32>::Alloc(2);
            (*KEY_STATE_NORMAL)[0] = KEY_STATE_NORMAL_ON[0];
            (*KEY_STATE_NORMAL)[1] = KEY_STATE_NORMAL_ON[1];
        }
    } else {
        if (mSticky) {
            if (mPressed) {
                KEY_STATE_NORMAL = ArrayOf<Int32>::Alloc(2);
                (*KEY_STATE_NORMAL)[0] = KEY_STATE_PRESSED_OFF[0];
                (*KEY_STATE_NORMAL)[1] = KEY_STATE_PRESSED_OFF[1];
            } else {
                KEY_STATE_NORMAL = ArrayOf<Int32>::Alloc(1);
                (*KEY_STATE_NORMAL)[0] = KEY_STATE_NORMAL_OFF[0];
            }
        } else {
            if (mPressed) {
                KEY_STATE_NORMAL = ArrayOf<Int32>::Alloc(1);
                (*KEY_STATE_NORMAL)[0] = KEY_STATE_PRESSED[0];
            }
        }
    }

    return KEY_STATE_NORMAL;
}

void Keyboard::Key::Init(
    /* [in] */ Row* parent)
{
    assert(parent != NULL);
    mCodes = NULL;
    mWidth = parent->mDefaultWidth;
    mHeight = parent->mDefaultHeight;
    mGap = parent->mDefaultHorizontalGap;
    mSticky = FALSE;
    mX = 0;
    mY = 0;
    mPressed = FALSE;
    mOn = FALSE;
    mEdgeFlags = parent->mRowEdgeFlags;
    mModifier = FALSE;
    mPopupResId = 0;
    mRepeatable = FALSE;
    mKeyboard = parent->mParent;
}

Keyboard::Keyboard(
    /* [in] */ IContext* context,
    /* [in] */ Int32 xmlLayoutResId)
{
    Init(context, xmlLayoutResId);
}

Keyboard::Keyboard(
    /* [in] */ IContext* context,
    /* [in] */ Int32 xmlLayoutResId,
    /* [in] */ Int32 modeId)
{
    Init(context, xmlLayoutResId, modeId);
}

Keyboard::Keyboard(
    /* [in] */ IContext* context,
    /* [in] */ Int32 layoutTemplateResId,
    /* [in] */ ICharSequence* characters,
    /* [in] */ Int32 columns,
    /* [in] */ Int32 horizontalPadding)
{
    Init(context, layoutTemplateResId);

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
    assert(characters != NULL);
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

        //TODO
        // key->mLabel = String.ValueOf(c);
        ArrayOf_<Char8, 5> chs;
        Int32 num = 0;
        Character::ToChars(c, chs, 0, &num);
        chs[num] = 0;
        CStringWrapper::New(String(chs.GetPayload()), (ICharSequence**)&key->mLabel);

        assert(key->mCodes == NULL);
        key->mCodes = ArrayOf<Int32>::Alloc(1);
        (*key->mCodes)[0] = c;
        //key->mCodes = new Int32[] { c };

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

UInt32 Keyboard::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 Keyboard::Release()
{
    return ElRefBase::Release();
}

List<AutoPtr<Keyboard::Key> > Keyboard::GetKeys()
{
    return mKeys;
}

List<AutoPtr<Keyboard::Key> > Keyboard::GetModifierKeys()
{
    return mModifierKeys;
}

Int32 Keyboard::GetHeight()
{
    return mTotalHeight;
}

Int32 Keyboard::GetMinWidth()
{
    return mTotalWidth;
}

Boolean Keyboard::SetShifted(
    /* [in] */ Boolean shiftState)
{
    if (mShiftKey != NULL) {
        mShiftKey->mOn = shiftState;
    }
    if (mShifted != shiftState) {
        mShifted = shiftState;
        return TRUE;
    }

    return FALSE;
}

Boolean Keyboard::IsShifted()
{
    return mShifted;
}

Int32 Keyboard::GetShiftKeyIndex()
{
    return mShiftKeyIndex;
}

ArrayOf<Int32>* Keyboard::GetNearestKeys(
    /* [in] */ Int32 x,
    /* [in] */ Int32 y)
{
    if (mGridNeighbors == NULL) {
        ComputeNearestNeighbors();
    }

    if (x >= 0 && x < GetMinWidth() && y >= 0 && y < GetHeight()) {
        Int32 index = (y / mCellHeight) * GRID_WIDTH + (x / mCellWidth);
        if (index < GRID_SIZE) {
            return (*mGridNeighbors)[index];
        }
    }

    //TODO
    //return new Int32[0];
    return NULL;
}

Int32 Keyboard::GetDimensionOrFraction(
    /* [in] */ ITypedArray* a,
    /* [in] */ Int32 index,
    /* [in] */ Int32 base,
    /* [in] */ Int32 defValue)
{
    assert(a != NULL);
    AutoPtr<ITypedValue> value;
    a->PeekValue(index, (ITypedValue**) &value);

    if (value == NULL) return defValue;

    if (((CTypedValue*)value.Get())->mType == TypedValue_TYPE_DIMENSION) {
        Int32 retValue = 0;
        a->GetDimensionPixelOffset(index, defValue, &retValue);
        return retValue;
    } else if (((CTypedValue*)value.Get())->mType == TypedValue_TYPE_FRACTION) {
        Float tmpValue = 0.0;
        a->GetFraction(index, base, base, defValue, &tmpValue);
        // Round it to avoid values like 47.9999 from getting truncated
        return Math::Round(tmpValue);
    }

    return defValue;
}

Int32 Keyboard::GetHorizontalGap()
{
    return mDefaultHorizontalGap;
}

void Keyboard::SetHorizontalGap(
    /* [in] */ Int32 gap)
{
    mDefaultHorizontalGap = gap;
}

Int32 Keyboard::GetVerticalGap()
{
    return mDefaultVerticalGap;
}

void Keyboard::SetVerticalGap(
    /* [in] */ Int32 gap)
{
    mDefaultVerticalGap = gap;
}

Int32 Keyboard::GetKeyHeight()
{
    return mDefaultHeight;
}

void Keyboard::SetKeyHeight(
    /* [in] */ Int32 height)
{
    mDefaultHeight = height;
}

Int32 Keyboard::GetKeyWidth()
{
    return mDefaultWidth;
}

void Keyboard::SetKeyWidth(
    /* [in] */ Int32 width)
{
    mDefaultWidth = width;
}

AutoPtr<Keyboard::Row> Keyboard::CreateRowFromXml(
    /* [in] */ IResources* res,
    /* [in] */ IXmlResourceParser* parser)
{
    return new Row(res, this, parser);
}

AutoPtr<Keyboard::Key> Keyboard::CreateKeyFromXml(
    /* [in] */ IResources* res,
    /* [in] */ Row* parent,
    /* [in] */ Int32 x,
    /* [in] */ Int32 y,
    /* [in] */ IXmlResourceParser* parser)
{
    return new Key(res, parent, x, y, parser);
}

void Keyboard::ComputeNearestNeighbors()
{
    // Round-up so we don't have any pixels outside the grid
    mCellWidth = (GetMinWidth() + GRID_WIDTH - 1) / GRID_WIDTH;
    mCellHeight = (GetHeight() + GRID_HEIGHT - 1) / GRID_HEIGHT;
    //mGridNeighbors = new Int32[GRID_SIZE][];
    mGridNeighbors = ArrayOf<ArrayOf<Int32>*>::Alloc(GRID_SIZE);

    Int32 keysSize = mKeys.GetSize();
    assert(keysSize != 0);

    ArrayOf<Int32>* indices = ArrayOf<Int32>::Alloc(keysSize);
    Int32 gridWidth = GRID_WIDTH * mCellWidth;
    Int32 gridHeight = GRID_HEIGHT * mCellHeight;
    for (Int32 x = 0; x < gridWidth; x += mCellWidth) {
        for (Int32 y = 0; y < gridHeight; y += mCellHeight) {
            Int32 count = 0, i = 0;

            List< AutoPtr<Key> >::Iterator it;
            for (it = mKeys.Begin(); it != mKeys.End(); it++, i++) {
                AutoPtr<Key> key = *it;
                if (key->SquaredDistanceFrom(x, y) < mProximityThreshold ||
                        key->SquaredDistanceFrom(x + mCellWidth - 1, y) < mProximityThreshold ||
                        key->SquaredDistanceFrom(x + mCellWidth - 1, y + mCellHeight - 1)
                            < mProximityThreshold ||
                        key->SquaredDistanceFrom(x, y + mCellHeight - 1) < mProximityThreshold) {
                    (*indices)[count++] = i;
                }
            }

            ArrayOf<Int32>* cell = ArrayOf<Int32>::Alloc(count);
            //System.arraycopy(indices, 0, cell, 0, count);
            memcpy(cell->GetPayload(), indices->GetPayload(), count * sizeof(Int32));
            (*mGridNeighbors)[(y / mCellHeight) * GRID_WIDTH + (x / mCellWidth)] = cell;
        }
    }

    ArrayOf<Int32>::Free(indices);
}

void Keyboard::LoadKeyboard(
    /* [in] */ IContext* context,
    /* [in] */ IXmlResourceParser* parser)
{
    assert(context != NULL && parser != NULL);

    Boolean inKey = FALSE;
    Boolean inRow = FALSE;
    // Boolean leftMostKey = FALSE;
    Int32 row = 0;
    Int32 x = 0;
    Int32 y = 0;
    AutoPtr<Key> key;
    AutoPtr<Row> currentRow;
    AutoPtr<IResources> res;
    context->GetResources((IResources**) &res);
    Boolean skipRow = FALSE;

    Int32 event = 0;
    parser->Next(&event);
    while (event != IXmlPullParser_END_DOCUMENT) {
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
           } else if (TAG_KEY.Equals(tag)) {
                inKey = TRUE;
                key = CreateKeyFromXml(res, currentRow, x, y, parser);
                mKeys.PushBack(key);
                if ((*key->mCodes)[0] == KEYCODE_SHIFT) {
                    mShiftKey = key;
                    mShiftKeyIndex = mKeys.GetSize()-1;
                    mModifierKeys.PushBack(key);
                } else if ((*key->mCodes)[0] == KEYCODE_ALT) {
                    mModifierKeys.PushBack(key);
                }
            } else if (TAG_KEYBOARD.Equals(tag)) {
                ParseKeyboardAttributes(res, parser);
            }
        } else if (event == IXmlPullParser_END_TAG) {
            if (inKey) {
                inKey = FALSE;
                x += key->mGap + key->mWidth;
                if (x > mTotalWidth) {
                    mTotalWidth = x;
                }
            } else if (inRow) {
                inRow = FALSE;
                y += currentRow->mVerticalGap;
                y += currentRow->mDefaultHeight;
                row++;
            } else {
                // TODO: error or extend?
                assert(0);
            }
        }

        parser->Next(&event);
    }

    mTotalHeight = y - mDefaultVerticalGap;
}

void Keyboard::SkipToEndOfRow(
    /* [in] */ IXmlResourceParser* parser)
        //throws XmlPullParserException, IOException {
{
    Int32 event = 0;
    parser->Next(&event);
    while (event != IXmlPullParser_END_DOCUMENT) {
        String tag;
        parser->GetName(&tag);
        if (event == IXmlPullParser_END_TAG && TAG_ROW.Equals(tag)) {
            break;
        }

        parser->Next(&event);
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
            (ITypedArray**) &a);

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

void Keyboard::Init(
    /* [in] */ IContext* context,
    /* [in] */ Int32 xmlLayoutResId,
    /* [in] */ Int32 modeId)
{
    mShiftKeyIndex = -1;
    assert(context != NULL);
    AutoPtr<IResources> res;
    context->GetResources((IResources**) &res);
    assert(res != NULL);

    AutoPtr<IDisplayMetrics> dm;
    res->GetDisplayMetrics((IDisplayMetrics**) &dm);

    mDisplayWidth = ((CDisplayMetrics*)dm.Get())->mWidthPixels;
    mDisplayHeight = ((CDisplayMetrics*)dm.Get())->mHeightPixels;
    //Log.v(TAG, "keyboard's display metrics:" + dm);

    mDefaultHorizontalGap = 0;
    mDefaultWidth = mDisplayWidth / 10;
    mDefaultVerticalGap = 0;
    mDefaultHeight = mDefaultWidth;
    // mKeys = new ArrayList<Key>();
    // mModifierKeys = new ArrayList<Key>();
    mKeyboardMode = modeId;

    AutoPtr<IXmlResourceParser> parser;
    res->GetXml(xmlLayoutResId, (IXmlResourceParser**) &parser);
    LoadKeyboard(context, parser);
}
