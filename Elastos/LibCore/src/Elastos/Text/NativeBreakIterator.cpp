
#include <unicode/ubrk.h>
#include "NativeBreakIterator.h"
#include "ElStringByteSink.h"
#include <Elastos.Utility.h>
#include "CStringCharacterIterator.h"


NativeBreakIterator::NativeBreakIterator(
        /* [in] */ Int32 iterAddr,
        /* [in] */ Int32 type)
{
    this->addr = iterAddr;
    this->type = type;
    CStringCharacterIterator::New(String(""), (IStringCharacterIterator**)&charIter);
}

NativeBreakIterator::~NativeBreakIterator()
{
    CloseBreakIteratorImpl(addr);
}

Int32 NativeBreakIterator::Current()
{
    return CurrentImpl(addr);
}

Int32 NativeBreakIterator::First()
{
    return FirstImpl(addr);
}

Int32 NativeBreakIterator::Following(
    /* [in] */ Int32 offset)
{
    return FollowingImpl(addr, offset);
}

AutoPtr<ICharacterIterator> NativeBreakIterator::GetText()
{
    Int32 newLoc = CurrentImpl(addr);
    Char32 c;
    charIter->SetIndex(newLoc, &c);
    return charIter;
}

Int32 NativeBreakIterator::Last()
{
    return LastImpl(addr);
}

Int32 NativeBreakIterator::Next(
    /* [in] */ Int32 n) {
    return NextImpl(addr, n);
}

Int32 NativeBreakIterator::Next()
{
    return NextImpl(addr, 1);
}

Int32 NativeBreakIterator::Previous()
{
    return PreviousImpl(addr);
}

void NativeBreakIterator::SetText(
    /* [in] */ ICharacterIterator* newText)
{
    this->charIter = newText;
    StringBuffer* sb = new StringBuffer(NULL);
    Char32 c;
    newText->First(&c);
    while(c != '0xffff') {
        (*sb) += c;
        newText->Next(&c);
    }
    return SetTextImpl(addr, sb->Substring(0, sb->GetLength()));
}

void NativeBreakIterator::SetText(
    /* [in] */ String newText)
{
    AutoPtr<IStringCharacterIterator> pISCI;
    CStringCharacterIterator::New(newText, (IStringCharacterIterator**)&pISCI);
//    return SetText((ICharacterIterator*)pISCI);
}

Boolean NativeBreakIterator::IsBoundary(
    /* [in] */ Int32 offset)
{
    return IsBoundaryImpl(addr, offset);
}

Int32 NativeBreakIterator::Preceding(
    /* [in] */ Int32 offset) 
{
    return PrecedingImpl(addr, offset);
}

NativeBreakIterator* NativeBreakIterator::GetCharacterInstance(
    /* [in] */ ILocale* where)
{
    String* name = new String(NULL);
    where->GetDisplayName(name);
    return new NativeBreakIterator(GetCharacterInstanceImpl(*name), BI_CHAR_INSTANCE);
}

NativeBreakIterator* NativeBreakIterator::GetLineInstance(
    /* [in] */ ILocale* where)
{
    String* name = new String(NULL);
    where->GetDisplayName(name);
    return new NativeBreakIterator(GetLineInstanceImpl(*name), BI_LINE_INSTANCE);
}

NativeBreakIterator* NativeBreakIterator::GetSentenceInstance(
    /* [in] */ ILocale* where)
{
    String* name = new String(NULL);
    where->GetDisplayName(name);
    return new NativeBreakIterator(GetSentenceInstanceImpl(*name), BI_SENT_INSTANCE);
}

NativeBreakIterator* NativeBreakIterator::GetWordInstance(
    /* [in] */ ILocale* where)
{
    String* name = new String(NULL);
    where->GetDisplayName(name);
    return new NativeBreakIterator(GetWordInstanceImpl(*name), BI_WORD_INSTANCE);
}

/**
* Checks if an error has occurred, throwing a suitable exception if so.
* @param env JNI environment
* @param errorCode code to determine if it is an error
* @return 0 if errorCode is not an error, 1 if errorCode is an error, but the
*         creation of the exception to be thrown fails
 * @exception thrown if errorCode represents an error
*/
static ECode icu4_error(UErrorCode errorCode)
{
//    const char* message = u_errorName(errorCode);
    if (errorCode <= U_ZERO_ERROR || errorCode >= U_ERROR_LIMIT) {
        return NOERROR;
    }

    switch (errorCode) {
    case U_ILLEGAL_ARGUMENT_ERROR:
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    case U_INDEX_OUTOFBOUNDS_ERROR:
    case U_BUFFER_OVERFLOW_ERROR:
        return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    case U_UNSUPPORTED_ERROR:
        return E_UNSUPPORTED_OPERATION_EXCEPTION;
    default:
        return E_RUNTIME_EXCEPTION;
    }
}

static Int32 GetIterator(
    /* [in] */ const String& locale,
    /* [in] */ UBreakIteratorType type) {
    UErrorCode status = U_ZERO_ERROR;
    if (locale.string() == NULL) {
        return 0;
    }
    UBreakIterator* it = ubrk_open(type, locale.string(), NULL, 0, &status);
    icu4_error(status);
    return reinterpret_cast<uintptr_t>(it);
}

Int32 NativeBreakIterator::GetCharacterInstanceImpl(
    /* [in] */ const String& locale)
{
    return GetIterator(locale, UBRK_CHARACTER);
}

Int32 NativeBreakIterator::GetWordInstanceImpl(
    /* [in] */ String locale)
{
    return GetIterator(locale, UBRK_WORD);
}

Int32 NativeBreakIterator::GetLineInstanceImpl(
    /* [in] */ String locale)
{
    return GetIterator(locale, UBRK_LINE);
}

Int32 NativeBreakIterator::GetSentenceInstanceImpl(
    /* [in] */ String locale)
{
    return GetIterator(locale, UBRK_SENTENCE);
}

static UBreakIterator* breakIterator(
    /* [in] */ Int32 address) {
    return reinterpret_cast<UBreakIterator*>(static_cast<uintptr_t>(address));
}

void NativeBreakIterator::CloseBreakIteratorImpl(
    /* [in] */ Int32 addr)
{
    ubrk_close(breakIterator(addr));
}

void NativeBreakIterator::SetTextImpl(
    /* [in] */ Int32 addr, 
    /* [in] */ String text)
{
    UnicodeString _s = UnicodeString::fromUTF8(text.string());
    UnicodeString& s(_s);
    UErrorCode status = U_ZERO_ERROR;
    ubrk_setText(breakIterator(addr), s.getBuffer(), s.length(), &status);
    icu4_error(status);
}

Int32 NativeBreakIterator::CloneImpl(
    /* [in] */ Int32 addr)
{
    UErrorCode status = U_ZERO_ERROR;
    Int32 bufferSize = U_BRK_SAFECLONE_BUFFERSIZE;
    UBreakIterator* it = ubrk_safeClone(breakIterator(addr), NULL, &bufferSize, &status);
    icu4_error(status);
    return reinterpret_cast<uintptr_t>(it);
}

Int32 NativeBreakIterator::PrecedingImpl(
    /* [in] */ Int32 addr, 
    /* [in] */ Int32 offset)
{
    return ubrk_preceding(breakIterator(addr), offset);
}

Boolean NativeBreakIterator::IsBoundaryImpl(
    /* [in] */ Int32 addr,
    /* [in] */ Int32 offset)
{
    return ubrk_isBoundary(breakIterator(addr), offset);
}

Int32 NativeBreakIterator::NextImpl(
    /* [in] */ Int32 addr,
    /* [in] */ Int32 n)
{
    UBreakIterator* bi = breakIterator(addr);
    if (n < 0) {
        while (n++ < -1) {
            ubrk_previous(bi);
        }
        return ubrk_previous(bi);
    } else if (n == 0) {
        return ubrk_current(bi);
    } else {
        while (n-- > 1) {
            ubrk_next(bi);
        }
        return ubrk_next(bi);
    }
    return -1;
}

Int32 NativeBreakIterator::PreviousImpl(
    /* [in] */ Int32 addr)
{
    return ubrk_previous(breakIterator(addr));
}

Int32 NativeBreakIterator::CurrentImpl(
    /* [in] */ Int32 addr)
{
    return ubrk_current(breakIterator(addr));
}

Int32 NativeBreakIterator::FirstImpl(
    /* [in] */ Int32 addr)
{
    return ubrk_first(breakIterator(addr));
}

Int32 NativeBreakIterator::FollowingImpl(
    /* [in] */ Int32 addr,
    /* [in] */ Int32 offset)
{
    return ubrk_following(breakIterator(addr), offset);
}

Int32 NativeBreakIterator::LastImpl(
    /* [in] */ Int32 addr)
{
    return ubrk_last(breakIterator(addr));
}