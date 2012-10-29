
#include "text/method/QwertyKeyListener.h"
#include "text/method/CQwertyKeyListener.h"
#include "text/method/TextKeyListener.h"
#include "text/SpannableStringInternal.h"
#include "text/TextUtils.h"
#include "text/Selection.h"
#include "view/ElKeyCharacterMap.h"
#include "view/CKeyEvent.h"
#include <elastos/Math.h>
#include <elastos/Character.h>
#include <stdio.h>

using namespace Elastos::Core;

const InterfaceID EIID_Replaced =
    {0xa40b81a, 0x803b, 0x4e5f, {0xb6, 0xde, 0x49, 0x87, 0x24, 0x49, 0xab, 0x50}};

QwertyKeyListener::Replaced::Replaced(
    /* [in] */ ArrayOf<Char8>* text)
    : mText(text)
{
}

QwertyKeyListener::Replaced::~Replaced()
{
    ArrayOf<Char8>::Free(mText);
}

PInterface QwertyKeyListener::Replaced::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (IInterface*)(INoCopySpan*)this;
    }
    else if (riid == EIID_INoCopySpan) {
        return (INoCopySpan*)this;
    }
    else if (riid == EIID_Replaced) {
        return reinterpret_cast<PInterface>(this);
    }

    return NULL;
}

UInt32 QwertyKeyListener::Replaced::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 QwertyKeyListener::Replaced::Release()
{
    return ElRefBase::Release();
}

ECode QwertyKeyListener::Replaced::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    if (pIID == NULL) {
        return E_INVALID_ARGUMENT;
    }

    if (pObject == reinterpret_cast<PInterface>(this)) {
        *pIID = EIID_Replaced;
    }
    else {
        return E_INVALID_ARGUMENT;
    }
    return NOERROR;
}

const Int32 QwertyKeyListener::CAPITALIZELENGTH;
AutoPtr<IQwertyKeyListener> QwertyKeyListener::sInstance[QwertyKeyListener::CAPITALIZELENGTH * 2];
HashMap<Char32, String> QwertyKeyListener::PICKER_SETS;

//static {
//    PICKER_SETS.put('A', "\u00C0\u00C1\u00C2\u00C4\u00C6\u00C3\u00C5\u0104\u0100");
//    PICKER_SETS.put('C', "\u00C7\u0106\u010C");
//    PICKER_SETS.put('D', "\u010E");
//    PICKER_SETS.put('E', "\u00C8\u00C9\u00CA\u00CB\u0118\u011A\u0112");
//    PICKER_SETS.put('G', "\u011E");
//    PICKER_SETS.put('L', "\u0141");
//    PICKER_SETS.put('I', "\u00CC\u00CD\u00CE\u00CF\u012A\u0130");
//    PICKER_SETS.put('N', "\u00D1\u0143\u0147");
//    PICKER_SETS.put('O', "\u00D8\u0152\u00D5\u00D2\u00D3\u00D4\u00D6\u014C");
//    PICKER_SETS.put('R', "\u0158");
//    PICKER_SETS.put('S', "\u015A\u0160\u015E");
//    PICKER_SETS.put('T', "\u0164");
//    PICKER_SETS.put('U', "\u00D9\u00DA\u00DB\u00DC\u016E\u016A");
//    PICKER_SETS.put('Y', "\u00DD\u0178");
//    PICKER_SETS.put('Z', "\u0179\u017B\u017D");
//    PICKER_SETS.put('a', "\u00E0\u00E1\u00E2\u00E4\u00E6\u00E3\u00E5\u0105\u0101");
//    PICKER_SETS.put('c', "\u00E7\u0107\u010D");
//    PICKER_SETS.put('d', "\u010F");
//    PICKER_SETS.put('e', "\u00E8\u00E9\u00EA\u00EB\u0119\u011B\u0113");
//    PICKER_SETS.put('g', "\u011F");
//    PICKER_SETS.put('i', "\u00EC\u00ED\u00EE\u00EF\u012B\u0131");
//    PICKER_SETS.put('l', "\u0142");
//    PICKER_SETS.put('n', "\u00F1\u0144\u0148");
//    PICKER_SETS.put('o', "\u00F8\u0153\u00F5\u00F2\u00F3\u00F4\u00F6\u014D");
//    PICKER_SETS.put('r', "\u0159");
//    PICKER_SETS.put('s', "\u00A7\u00DF\u015B\u0161\u015F");
//    PICKER_SETS.put('t', "\u0165");
//    PICKER_SETS.put('u', "\u00F9\u00FA\u00FB\u00FC\u016F\u016B");
//    PICKER_SETS.put('y', "\u00FD\u00FF");
//    PICKER_SETS.put('z', "\u017A\u017C\u017E");
//    PICKER_SETS.put(ElKeyCharacterMap::PICKER_DIALOG_INPUT,
//                         "\u2026\u00A5\u2022\u00AE\u00A9\u00B1[]{}\\|");
//    PICKER_SETS.put('/', "\\");

//    // From packages/inputmethods/LatinIME/res/xml/kbd_symbols.xml

//    PICKER_SETS.put('1', "\u00b9\u00bd\u2153\u00bc\u215b");
//    PICKER_SETS.put('2', "\u00b2\u2154");
//    PICKER_SETS.put('3', "\u00b3\u00be\u215c");
//    PICKER_SETS.put('4', "\u2074");
//    PICKER_SETS.put('5', "\u215d");
//    PICKER_SETS.put('7', "\u215e");
//    PICKER_SETS.put('0', "\u207f\u2205");
//    PICKER_SETS.put('$', "\u00a2\u00a3\u20ac\u00a5\u20a3\u20a4\u20b1");
//    PICKER_SETS.put('%', "\u2030");
//    PICKER_SETS.put('*', "\u2020\u2021");
//    PICKER_SETS.put('-', "\u2013\u2014");
//    PICKER_SETS.put('+', "\u00b1");
//    PICKER_SETS.put('(', "[{<");
//    PICKER_SETS.put(')', "]}>");
//    PICKER_SETS.put('!', "\u00a1");
//    PICKER_SETS.put('"', "\u201c\u201d\u00ab\u00bb\u02dd");
//    PICKER_SETS.put('?', "\u00bf");
//    PICKER_SETS.put(',', "\u201a\u201e");

//    // From packages/inputmethods/LatinIME/res/xml/kbd_symbols_shift.xml

//    PICKER_SETS.put('=', "\u2260\u2248\u221e");
//    PICKER_SETS.put('<', "\u2264\u00ab\u2039");
//    PICKER_SETS.put('>', "\u2265\u00bb\u203a");
//};

QwertyKeyListener::QwertyKeyListener()
{}

/**
 * Returns a new or existing instance with the specified capitalization
 * and correction properties.
 */
AutoPtr<IQwertyKeyListener> QwertyKeyListener::GetInstance(
    /* [in] */ Boolean autotext,
    /* [in] */ Capitalize cap)
{
    Int32 off = cap * 2 + (autotext ? 1 : 0);

    if (sInstance[off] == NULL) {
        AutoPtr<IQwertyKeyListener> listener;
        CQwertyKeyListener::New(cap, autotext, (IQwertyKeyListener**)&listener);
        sInstance[off] = listener;
    }

    return sInstance[off];
}

Int32 QwertyKeyListener::GetInputType()
{
    return MakeTextContentType(mAutoCap, mAutoText);
}

Boolean QwertyKeyListener::OnKeyDown(
    /* [in] */ IView* view,
    /* [in] */ IEditable* content,
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event)
{
    Int32 selStart, selEnd;
    Int32 pref = 0;

    if (view != NULL) {
        AutoPtr<IContext> context;
        view->GetContext((IContext**)&context);
        TextKeyListener::GetInstance()->GetPrefs(context, &pref);
    }

    {
        Int32 a = Selection::GetSelectionStart(content);
        Int32 b = Selection::GetSelectionEnd(content);

        selStart = Math::Min(a, b);
        selEnd = Math::Max(a, b);

        if (selStart < 0 || selEnd < 0) {
            selStart = selEnd = 0;
            Selection::SetSelection(content, 0, 0);
        }
    }
    Int32 activeStart;
    ISpannable::Probe(content)->GetSpanStart(
        TextKeyListener::ACTIVE, &activeStart);
    Int32 activeEnd;
    ISpannable::Probe(content)->GetSpanEnd(
        TextKeyListener::ACTIVE, &activeEnd);

    // QWERTY keyboard normal case

    Int32 i;
    event->GetUnicodeCharEx(GetMetaState(content), &i);

    Int32 count;
    event->GetRepeatCount(&count);
    if (count > 0 && selStart == selEnd && selStart > 0) {
        Char32 c;
        content->GetCharAt(selStart - 1, &c);

        if (c == i || c == Character::ToUpperCase(i) && view != NULL) {
            if (ShowCharacterPicker(view, content, c, FALSE, count)) {
                ResetMetaState(content);
                return TRUE;
            }
        }
    }
    if (i == ElKeyCharacterMap::PICKER_DIALOG_INPUT) {
        if (view != NULL) {
            ShowCharacterPicker(view, content,
                ElKeyCharacterMap::PICKER_DIALOG_INPUT, TRUE, 1);
        }
        ResetMetaState(content);
        return TRUE;
    }
    if (i == ElKeyCharacterMap::HEX_INPUT) {
        Int32 start;

        if (selStart == selEnd) {
            start = selEnd;

            Char32 ch;
            while (start > 0 && selEnd - start < 4 &&
                Character::ToDigit((content->GetCharAt(start - 1, &ch), ch), 16) >= 0) {
                start--;
            }
        }
        else {
            start = selStart;
        }

        Int32 ch = -1;

        String hex;
        TextUtils::Substring(content, start, selEnd, &hex);
        ch = hex.ToInt32(16);

        if (ch >= 0) {
            selStart = start;
            Selection::SetSelection(content, selStart, selEnd);
            i = ch;
        }
        else {
            i = 0;
        }
    }
    if (i != 0) {
        Boolean dead = FALSE;

        if ((i & ElKeyCharacterMap::COMBINING_ACCENT) != 0) {
            dead = TRUE;
            i = i & ElKeyCharacterMap::COMBINING_ACCENT_MASK;
        }

        if (activeStart == selStart && activeEnd == selEnd) {
            Boolean replace = FALSE;

            if (selEnd - selStart - 1 == 0) {
                Char32 accent;
                content->GetCharAt(selStart, &accent);
                Int32 composed = CKeyEvent::GetDeadChar(accent, i);

                if (composed != 0) {
                    i = composed;
                    replace = TRUE;
                }
            }

            if (!replace) {
                Selection::SetSelection(content, selEnd);
                content->RemoveSpan(TextKeyListener::ACTIVE);
                selStart = selEnd;
            }
        }
        if ((pref & TextKeyListener::AUTO_CAP) != 0 &&
            Character::IsLowerCase(i) &&
            TextKeyListener::ShouldCap(mAutoCap, content, selStart)) {
            Int32 where;
            content->GetSpanEnd(TextKeyListener::CAPPED, &where);
            Int32 flags;
            content->GetSpanFlags(TextKeyListener::CAPPED, &flags);

            if (where == selStart && (((flags >> 16) & 0xFFFF) == i)) {
                content->RemoveSpan(TextKeyListener::CAPPED);
            }
            else {
                flags = i << 16;
                i = Character::ToUpperCase(i);

                if (selStart == 0)
                    content->SetSpan(TextKeyListener::CAPPED, 0, 0,
                                    Spanned_SPAN_MARK_MARK | flags);
                else
                    content->SetSpan(TextKeyListener::CAPPED,
                                    selStart - 1, selStart,
                                    Spanned_SPAN_EXCLUSIVE_EXCLUSIVE |
                                    flags);
            }
        }
        if (selStart != selEnd) {
            Selection::SetSelection(content, selEnd);
        }
        content->SetSpan(OLD_SEL_START, selStart, selStart,
                        Spanned_SPAN_MARK_MARK);


        ArrayOf_<Char8, 5> chs;
        Int32 num;
        Character::ToChars(i, chs, 0, &num);
        chs[num] = 0;
        AutoPtr<ICharSequence> cs;
        CStringWrapper::New(String(chs.GetPayload()), (ICharSequence**)&cs);
        AutoPtr<IEditable> editable;
        content->ReplaceEx(selStart, selEnd, cs, (IEditable**)&editable);

        Int32 oldStart;
        content->GetSpanStart(OLD_SEL_START, &oldStart);
        selEnd = Selection::GetSelectionEnd(content);
        if (oldStart < selEnd) {
            content->SetSpan(TextKeyListener::LAST_TYPED,
                            oldStart, selEnd,
                            Spanned_SPAN_EXCLUSIVE_EXCLUSIVE);
            if (dead) {
                Selection::SetSelection(content, oldStart, selEnd);
                content->SetSpan(TextKeyListener::ACTIVE, oldStart, selEnd,
                    Spanned_SPAN_EXCLUSIVE_EXCLUSIVE);
            }
        }

        AdjustMetaAfterKeypress(content);

        // potentially do autotext replacement if the character
        // that was typed was an autotext terminator

        Int32 end;
        if ((pref & TextKeyListener::AUTO_TEXT) != 0 && mAutoText &&
            (i == ' ' || i == '\t' || i == '\n' ||
             i == ',' || i == '.' || i == '!' || i == '?' ||
             i == '"' || Character::GetType(i) == Character::END_PUNCTUATION) &&
             (content->GetSpanEnd(TextKeyListener::INHIBIT_REPLACEMENT, &end), end)
             != oldStart) {
            Int32 x;

            for (x = oldStart; x > 0; x--) {
                Char32 c;
                content->GetCharAt(x - 1, &c);
                if (c != '\'' && !Character::IsLetter(c)) {
                    break;
                }
            }

            String rep = GetReplacement(content, x, oldStart, view);

            if (rep != NULL) {
                ArrayOf<IInterface*>* repl;
                Int32 len;
                content->GetLength(&len);
                content->GetSpans(0, len, EIID_Replaced, &repl);
                for (Int32 a = 0; a < repl->GetLength(); a++)
                    content->RemoveSpan((*repl)[a]);

                FreeArray(repl);

                ArrayOf<Char8>* orig = ArrayOf<Char8>::Alloc((oldStart - x) * 4);
                TextUtils::GetChars(content, x, oldStart, orig, 0);

                content->SetSpan(new Replaced(orig), x, oldStart,
                                Spanned_SPAN_EXCLUSIVE_EXCLUSIVE);
                AutoPtr<ICharSequence> cs;
                CStringWrapper::New(rep, (ICharSequence**)&cs);
                AutoPtr<IEditable> editable;
                content->ReplaceEx(x, oldStart, cs, (IEditable**)&editable);
            }
        }

        // Replace two spaces by a period and a space.

        if ((pref & TextKeyListener::AUTO_PERIOD) != 0 && mAutoText) {
            selEnd = Selection::GetSelectionEnd(content);
            if (selEnd - 3 >= 0) {
                Char32 ch;
                if ((content->GetCharAt(selEnd - 1, &ch), ch) == ' ' &&
                    (content->GetCharAt(selEnd - 2, &ch), ch) == ' ') {
                    Char32 c;
                    content->GetCharAt(selEnd - 3, &c);

                    for (Int32 j = selEnd - 3; j > 0; j--) {
                        if (c == '"' ||
                            Character::GetType(c) == Character::END_PUNCTUATION) {
                            content->GetCharAt(j - 1, &c);
                        }
                        else {
                            break;
                        }
                    }

                    if (Character::IsLetter(c) || Character::IsDigit(c)) {
                        AutoPtr<ICharSequence> cs;
                        CStringWrapper::New(String("."), (ICharSequence**)&cs);
                        AutoPtr<IEditable> temp;
                        content->ReplaceEx(selEnd - 2, selEnd - 1, cs, (IEditable**)&temp);
                    }
                }
            }
        }

        return TRUE;
    }
    else if (keyCode == KeyEvent_KEYCODE_DEL && selStart == selEnd) {
        // special backspace case for undoing autotext

        Int32 consider = 1;

        // if backspacing over the last typed character,
        // it undoes the autotext prior to that character
        // (unless the character typed was newline, in which
        // case this behavior would be confusing)

        Int32 end;
        content->GetSpanEnd(TextKeyListener::LAST_TYPED, &end);
        if (end == selStart) {
            Char32 c;
            if ((content->GetCharAt(selStart - 1, &c), c) != '\n')
                consider = 2;
        }

        ArrayOf<IInterface*>* repl = NULL;
        content->GetSpans(
            selStart - consider, selStart, EIID_Replaced, &repl);

        if (repl->GetLength() > 0) {
            Int32 st;
            content->GetSpanStart((*repl)[0], &st);
            Int32 en;
            content->GetSpanEnd((*repl)[0], &en);
            String old(((Replaced*)(*repl)[0])->mText->GetPayload());

            content->RemoveSpan((*repl)[0]);

            // only cancel the autocomplete if the cursor is at the end of
            // the replaced span (or after it, because the user is
            // backspacing over the space after the word, not the word
            // itself).
            if (selStart >= en) {
                content->SetSpan(TextKeyListener::INHIBIT_REPLACEMENT,
                                en, en, Spanned_SPAN_POINT_POINT);
                AutoPtr<ICharSequence> oldCs;
                CStringWrapper::New(old, (ICharSequence**)&oldCs);
                AutoPtr<IEditable> temp;
                content->ReplaceEx(st, en, oldCs, (IEditable**)&temp);

                content->GetSpanStart(TextKeyListener::INHIBIT_REPLACEMENT, &en);
                if (en - 1 >= 0) {
                    content->SetSpan(
                        TextKeyListener::INHIBIT_REPLACEMENT,
                        en - 1, en, Spanned_SPAN_EXCLUSIVE_EXCLUSIVE);
                }
                else {
                    content->RemoveSpan(TextKeyListener::INHIBIT_REPLACEMENT);
                }
                AdjustMetaAfterKeypress(content);
            }
            else {
                AdjustMetaAfterKeypress(content);
                return BaseKeyListener::OnKeyDown(view, content, keyCode, event);
            }

            FreeArray(repl);

            return TRUE;
        }
    }
    return BaseKeyListener::OnKeyDown(view, content, keyCode, event);
}

String QwertyKeyListener::GetReplacement(
    /* [in] */ ICharSequence* src,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ IView* view)
{
    Int32 len = end - start;
    Boolean changecase = FALSE;

    String replacement;// = AutoText.get(src, start, end, view);

    if (replacement.IsNull()) {
        String key;
        TextUtils::Substring(src, start, end, &key);
        key.ToLowerCase();
        //replacement = AutoText.get(key, 0, end - start, view);
        changecase = TRUE;

        if (replacement.IsNull())
            return String(NULL);
    }

    Int32 caps = 0;

    if (changecase) {
        for (Int32 j = start; j < end; j++) {
            Char32 c;
            src->GetCharAt(j, &c);
            if (Character::IsUpperCase(c))
                caps++;
        }
    }

    String out;

    if (caps == 0)
        out = replacement;
    else if (caps == 1)
        out = ToTitleCase(replacement);
    else if (caps == len)
        out = (replacement.ToUpperCase(), replacement);
    else
        out = ToTitleCase(replacement);

    AutoPtr<ICharSequence> cs;
    CStringWrapper::New(out, (ICharSequence**)&cs);
    if (out.GetLength() == len &&
        TextUtils::RegionMatches(src, start, cs, 0, len))
        return String(NULL);

    return out;
}

/**
 * Marks the specified region of <code>content</code> as having
 * contained <code>original</code> prior to AutoText replacement.
 * Call this method when you have done or are about to do an
 * AutoText-style replacement on a region of text and want to let
 * the same mechanism (the user pressing DEL immediately after the
 * change) undo the replacement.
 *
 * @param content the Editable text where the replacement was made
 * @param start the start of the replaced region
 * @param end the end of the replaced region; the location of the cursor
 * @param original the text to be restored if the user presses DEL
 */
void QwertyKeyListener::MarkAsReplaced(
    /* [in] */ ISpannable* content,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ String original)
{
    ArrayOf<IInterface*>* repl;
    Int32 len;
    content->GetLength(&len);
    content->GetSpans(0, len, EIID_Replaced, &repl);
    for (Int32 a = 0; a < repl->GetLength(); a++) {
        content->RemoveSpan((*repl)[a]);
    }
    FreeArray(repl);

    len = original.GetLength();
    ArrayOf<Char8>* orig = ArrayOf<Char8>::Alloc(len);
    memcpy(orig->GetPayload(), original.string(), len);

    content->SetSpan(new Replaced(orig), start, end,
                    Spanned_SPAN_EXCLUSIVE_EXCLUSIVE);
}

Boolean QwertyKeyListener::ShowCharacterPicker(
    /* [in] */ IView* view,
    /* [in] */ IEditable* content,
    /* [in] */ Char32 c,
    /* [in] */ Boolean insert,
    /* [in] */ Int32 count)
{
    //String set = PICKER_SETS.get(c);
    //if (set == NULL) {
    //    return FALSE;
    //}

    //if (count == 1) {
    //    new CharacterPickerDialog(view.getContext(),
    //                              view, content, set, insert).show();
    //}

    //return TRUE;
    return FALSE;
}

String QwertyKeyListener::ToTitleCase(
    /* [in] */ String src)
{
    ArrayOf_<Char8, 5> chs;
    Int32 num;
    Character::ToChars(Character::ToUpperCase(src.GetChar(0)), chs, 0, &num);
    chs[num] = 0;

    return String(chs.GetPayload()) + src.Substring(num);
}

ECode QwertyKeyListener::Init(
    /* [in] */ Capitalize cap,
    /* [in] */ Boolean autotext)
{
    mAutoCap = cap;
    mAutoText = autotext;

    return NOERROR;
}
