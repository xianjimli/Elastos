#include "NativeBreakIterator.h"

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

Int32 NativeBreakIterator::GetCharacterInstanceImpl(
    /* [in] */ String locale)
{
    return 0;
}

Int32 NativeBreakIterator::GetWordInstanceImpl(
    /* [in] */ String locale)
{
    return 0;
}

Int32 NativeBreakIterator::GetLineInstanceImpl(
    /* [in] */ String locale)
{
    return 0;
}

Int32 NativeBreakIterator::GetSentenceInstanceImpl(
    /* [in] */ String locale)
{
    return 0;
}

void NativeBreakIterator::CloseBreakIteratorImpl(
    /* [in] */ Int32 addr)
{
}

void NativeBreakIterator::SetTextImpl(
    /* [in] */ Int32 addr, 
    /* [in] */ String text)
{
}

Int32 NativeBreakIterator::CloneImpl(
    /* [in] */ Int32 addr)
{
    return 0;
}

Int32 NativeBreakIterator::PrecedingImpl(
    /* [in] */ Int32 addr, 
    /* [in] */ Int32 offset)
{
    return 0;
}

Boolean NativeBreakIterator::IsBoundaryImpl(
    /* [in] */ Int32 addr,
    /* [in] */ Int32 offset)
{
    return FALSE;
}

Int32 NativeBreakIterator::NextImpl(
    /* [in] */ Int32 addr,
    /* [in] */ Int32 n)
{
    return 0;
}

Int32 NativeBreakIterator::PreviousImpl(
    /* [in] */ Int32 addr)
{
    return 0;
}

Int32 NativeBreakIterator::CurrentImpl(
    /* [in] */ Int32 addr)
{
    return 0;
}

Int32 NativeBreakIterator::FirstImpl(
    /* [in] */ Int32 addr)
{
    return 0;
}

Int32 NativeBreakIterator::FollowingImpl(
    /* [in] */ Int32 addr,
    /* [in] */ Int32 offset)
{
    return 0;
}

Int32 NativeBreakIterator::LastImpl(
    /* [in] */ Int32 addr)
{
    return 0;
}