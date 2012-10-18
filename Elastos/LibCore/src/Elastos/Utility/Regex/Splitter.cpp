
#include "Splitter.h"
#include "cmdef.h"
#include "CMatcher.h"
#include <elastos/Math.h>
#include <elastos/AutoPtr.h>

using namespace Elastos::Core;

const CString Splitter::METACHARACTERS = "\\?*+[](){}^$.|";

Splitter::Splitter(
    /* [in] */ const String& text,
    /* [in] */ ArrayOf<Int32>* offsets)
    : mText(text)
    , mOffsets(offsets)
{
}

ArrayOf<String>* Splitter::FastSplit(
    /* [in] */ const String& re,
    /* [in] */ const String& input,
    /* [in] */ Int32 limit)
{
    // Can we do it cheaply?
    Int32 len = re.GetLength();
    if (len == 0) {
        return NULL;
    }
    Char8 ch = re[0];
    if (len == 1 && METACHARACTERS.IndexOf(ch) == -1) {
        // We're looking for a single non-metacharacter. Easy.
    }
    else if (len == 2 && ch == '\\') {
        // We're looking for a quoted character.
        // Quoted metacharacters are effectively single non-metacharacters.
        ch = re[1];
        if (METACHARACTERS.IndexOf(ch) == -1) {
            return NULL;
        }
    }
    else {
        return NULL;
    }

    // We can do this cheaply...

    // Unlike Perl, which considers the result of splitting the empty string to be the empty
    // array, Java returns an array containing the empty string.
    if (input.IsEmpty()) {
        return ArrayOf<String>::Alloc(1);
    }

    // Collect text preceding each occurrence of the separator, while there's enough space.
    List<String>* list = new List<String>();
    UInt32 maxSize = limit <= 0 ? Math::INT32_MAX_VALUE : limit;
    Int32 begin = 0;
    Int32 end;
    while ((end = input.IndexOf(ch, begin)) != -1 && list->GetSize() + 1 < maxSize) {
        list->PushBack(input.Substring(begin, end));
        begin = end + 1;
    }
    return FinishSplit(list, input, begin, maxSize, limit);
}

ArrayOf<String>* Splitter::Split(
    /* [in] */ IPattern* pattern,
    /* [in] */ const String& re,
    /* [in] */ const String& input,
    /* [in] */ Int32 limit)
{
    ArrayOf<String>* fastResult = FastSplit(re, input, limit);
    if (fastResult != NULL) {
        return fastResult;
    }

    // Unlike Perl, which considers the result of splitting the empty string to be the empty
    // array, Java returns an array containing the empty string.
    if (input.IsEmpty()) {
        return ArrayOf<String>::Alloc(1);
    }

    // Collect text preceding each occurrence of the separator, while there's enough space.
    List<String>* list = new List<String>();
    UInt32 maxSize = limit <= 0 ? Math::INT32_MAX_VALUE : limit;
    AutoPtr<CMatcher> matcher;
    AutoPtr<ICharSequence> charSeq;
    CStringWrapper::New(input, (ICharSequence**)&charSeq);
    CMatcher::NewByFriend((IPattern*)pattern, charSeq, (CMatcher**)&matcher);
    Int32 begin = 0;
    Boolean result;
    matcher->Find(&result);
    while (result && list->GetSize() + 1 < maxSize) {
        Int32 index;
        matcher->Start(&index);
        list->PushBack(input.Substring(begin, index));
        matcher->End(&begin);
    }
    return FinishSplit(list, input, begin, maxSize, limit);
}

ArrayOf<String>* Splitter::FinishSplit(
    /* [in] */ List<String>* list,
    /* [in] */ const String& input,
    /* [in] */ Int32 begin,
    /* [in] */ Int32 maxSize,
    /* [in] */ Int32 limit)
{
    // Add trailing text.
    if ((UInt32)begin < input.GetLength()) {
        list->PushBack(input.Substring(begin));
    }
    else if (limit != 0) { // No point adding the empty string if limit == 0, just to remove it below.
        list->PushBack(String(""));
    }
    // Remove all trailing empty matches in the limit == 0 case.
    if (limit == 0) {
        List<String>::ReverseIterator rit = list->RBegin();
        while (rit != list->REnd() && (*rit).IsEmpty()) {
            List<String>::Iterator it = --(rit.GetBase());
            ++rit;
            list->Erase(it);
        }
    }
    // Convert to an array.
    ArrayOf<String>* arrayOfStr = ArrayOf<String>::Alloc(list->GetSize());
    List<String>::Iterator it = list->Begin();
    for (Int32 i = 0; it != list->End(); ++i, ++it) {
        (*arrayOfStr)[i] = *it;
    }

    return arrayOfStr;
}
