
#include "cmdef.h"
#include "AttributedString.h"
#include <StringBuffer.h>

namespace Elastos
{
    const RBTreeColorType S_RBTreeRed = FALSE;
    const RBTreeColorType S_RBTreeBlack = TRUE;
}

AttributedString::Range::Range(
    /* [in] */ Int32 s,
    /* [in] */ Int32 e,
    /* [in] */ IInterface* v)
    : mStart(s)
    , mEnd(e)
    , mValue(v)
{}

AttributedString::AttributedIterator::AttributedIterator(
    /* [in] */ AttributedString* attrString)
    : mBegin(0)
    , mOffset(0)
    , mAttrString(attrString)
{
    mEnd = attrString->mText.GetLength();
}

AttributedString::AttributedIterator::AttributedIterator(
    /* [in] */ AttributedString* attrString,
    /* [in] */ IObjectContainer* attributes,
    /* [in] */ Int32 begin,
    /* [in] */ Int32 end)
{
    if (begin < 0 || end > (Int32)mAttrString->mText.GetLength() || begin > end) {
        //throw new IllegalArgumentException();
        assert(0);
    }
    mBegin = begin;
    mEnd = end;
    mOffset = begin;
    mAttrString = attrString;

    if (attributes != NULL) {
        AutoPtr<IObjectEnumerator> enumerator;
        attributes->GetObjectEnumerator((IObjectEnumerator**)&enumerator);
        Boolean hasNext;
        while(enumerator->MoveNext(&hasNext), hasNext) {
            AutoPtr<IAttributedCharacterIteratorAttribute> attr;
            enumerator->Current((IInterface**)&attr);
            mAttributesAllowed->Insert(attr);
        }
    }
}

AttributedString::AttributedIterator::~AttributedIterator()
{
    mAttributesAllowed->Clear();
}

PInterface AttributedString::AttributedIterator::Probe(
    /* [in]  */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)this;
    }
    else if (riid == EIID_IAttributedCharacterIterator) {
        return (IAttributedCharacterIterator*)this;
    }

    return NULL;
}

UInt32 AttributedString::AttributedIterator::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 AttributedString::AttributedIterator::Release()
{
    return ElRefBase::Release();
}

AttributedString::AttributedIterator::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    return E_NOT_IMPLEMENTED;
}

ECode AttributedString::AttributedIterator::Clone(
    /* [out] */ IInterface** copy)
{
    return E_NOT_IMPLEMENTED;
}

ECode AttributedString::AttributedIterator::Current(
    /* [out] */ Char32* value)
{
    VALIDATE_NOT_NULL(value);
    if (mOffset == mEnd) {
        *value = (Char32)CharacterIterator_DONE;
        return NOERROR;
    }
    *value = mAttrString->mText.GetChar(mOffset);
    return NOERROR;
}

ECode AttributedString::AttributedIterator::First(
    /* [out] */ Char32* value)
{
    VALIDATE_NOT_NULL(value);
    if (mBegin == mEnd) {
        *value = (Char32)CharacterIterator_DONE;
        return NOERROR;
    }
    mOffset = mBegin;
    *value = mAttrString->mText.GetChar(mOffset);
    return NOERROR;
}

ECode AttributedString::AttributedIterator::GetBeginIndex(
    /* [out] */ Int32* index)
{
    VALIDATE_NOT_NULL(index);
    *index = mBegin;
    return NOERROR;
}

ECode AttributedString::AttributedIterator::GetEndIndex(
    /* [out] */ Int32* index)
{
    VALIDATE_NOT_NULL(index);
    *index = mEnd;
    return NOERROR;
}

ECode AttributedString::AttributedIterator::GetIndex(
    /* [out] */ Int32* index)
{
    VALIDATE_NOT_NULL(index);
    *index = mOffset;
    return NOERROR;
}

Boolean AttributedString::AttributedIterator::InRange(
    /* [in] */ Range* range)
{
    if ((range->mValue)->Probe(EIID_IAnnotation) == NULL) {
        return TRUE;
    }
    return range->mStart >= mBegin && range->mStart < mEnd
            && range->mEnd > mBegin && range->mEnd <= mEnd;
}

Boolean AttributedString::AttributedIterator::InRange(
    /* [in] */ List<Range*>* ranges)
{
    List<Range*>::Iterator it;
    for (it = ranges->Begin(); it != ranges->End(); ++it) {
        Range* range = *it;
        if (range->mStart >= mBegin && range->mStart < mEnd) {
            return (range->mValue->Probe(EIID_IAnnotation) == NULL)
                    || (range->mEnd > mBegin && range->mEnd <= mEnd);
        }
        else if (range->mEnd > mBegin && range->mEnd <= mEnd) {
            return (range->mValue->Probe(EIID_IAnnotation) == NULL)
                    || (range->mStart >= mBegin && range->mStart < mEnd);
        }
    }
    return FALSE;
}

ECode AttributedString::AttributedIterator::GetAllAttributeKeys(
    /* [out] */ IObjectContainer** allAttributedKeys)
{
    VALIDATE_NOT_NULL(allAttributedKeys);

    AutoPtr<IObjectContainer> container;
    CObjectContainer::New((IObjectContainer**)&container);
    Map<AutoPtr<IAttributedCharacterIteratorAttribute>, List<Range*>* >::Iterator map_it
            = mAttrString->mAttributeMap->Begin();
    if (mBegin == 0 && mEnd == (Int32)mAttrString->mText.GetLength()
            && mAttributesAllowed == NULL) {
        for(; map_it != mAttrString->mAttributeMap->End(); ++map_it) {
            container->Add(map_it->mFirst);
        }
    }
    else {
        for(; map_it != mAttrString->mAttributeMap->End(); ++map_it) {
            if (mAttributesAllowed == NULL
                    || mAttributesAllowed->Find(map_it->mFirst) != mAttributesAllowed->End()) {
                List<Range*>* ranges = map_it->mSecond;
                if (InRange(ranges)) {
                    container->Add(map_it->mFirst);
                }
            }
        }
    }

    *allAttributedKeys = container;
    if (*allAttributedKeys != NULL) {
        (*allAttributedKeys)->AddRef();
    }
    return NOERROR;
}

AutoPtr<IInterface> AttributedString::AttributedIterator::CurrentValue(
    /* [in] */ List<Range*>* ranges)
{
    List<Range*>::Iterator it = ranges->Begin();
    for (it = ranges->Begin(); it != ranges->End(); ++it) {
        Range* range = *it;
        if (mOffset >= range->mStart && mOffset < range->mEnd) {
            return InRange(range) ? range->mValue : NULL;
        }
    }

    return NULL;
}

ECode AttributedString::AttributedIterator::GetAttribute(
    /* [in] */ IAttributedCharacterIteratorAttribute* attribute,
    /* [out] */ IInterface** instance)
{
    VALIDATE_NOT_NULL(instance);

    if (mAttributesAllowed != NULL
            && mAttributesAllowed->Find(attribute) == mAttributesAllowed->End()) {
        *instance = NULL;
        return NOERROR;
    }
    List<Range*>* ranges;
    Map<AutoPtr<IAttributedCharacterIteratorAttribute>, List<Range*>* >::Iterator it
            = mAttrString->mAttributeMap->Find(attribute);
    if (it == mAttrString->mAttributeMap->End()) {
        *instance = NULL;
        return NOERROR;
    }
    ranges = it->mSecond;
    AutoPtr<IInterface> temp = CurrentValue(ranges);
    *instance = temp;
    if (*instance != NULL) {
        (*instance)->AddRef();
    }

    return NOERROR;
}

// ECode AttributedString::AttributedIterator::GetAttributes(
//     /* [out] */ Map<IAttributedCharacterIteratorAttribute*, IInterface*>* attributes)
// {
//     VALIDATE_NOT_NULL(attributes);

//     Map<AutoPtr<IAttributedCharacterIteratorAttribute>, AutoPtr<IInterface> > result;// = new HashMap<Attribute, Object>(
//         //(attrString.attributeMap.size() * 4 / 3) + 1);

//     Map<AutoPtr<IAttributedCharacterIteratorAttribute>, List<Range*>* >::Iterator it
//             = mAttrString->mAttributeMap->Begin();
//     for(; it != mAttrString->mAttributeMap->End(); ++it) {
//         if (mAttributesAllowed == NULL
//                 || mAttributesAllowed->Find(it->mFirst) != mAttributesAllowed->End()) {
//             AutoPtr<IInterface> value = CurrentValue(it->mSecond);
//             if (value != NULL) {
//                 result.put(entry.getKey(), value);
//             }
//         }
//     }
//     attributes = &result;
//     return NOERROR;
// }

ECode AttributedString::AttributedIterator::GetRunLimit(
    /* [out] */ Int32* runLimit)
{
    AutoPtr<IObjectContainer> allAttributedKeys;
    GetAllAttributeKeys((IObjectContainer**)&allAttributedKeys);
    return GetRunLimitEx2(allAttributedKeys, runLimit);
}

Int32 AttributedString::AttributedIterator::RunLimit(
        /* [in] */ List<Range*>* ranges)
{
    Int32 result = mEnd;

    List<Range*>::ReverseIterator rit = ranges->RBegin();
    for (rit = ranges->RBegin(); rit != ranges->REnd(); ++rit) {
        Range* range = *rit;
        if (range->mEnd <= mBegin) {
            break;
        }
        if (mOffset >= range->mStart && mOffset < range->mEnd) {
            return InRange(range) ? range->mEnd : result;
        }
        else if (mOffset >= range->mEnd) {
            break;
        }
        result = range->mStart;
    }

    return result;
}

ECode AttributedString::AttributedIterator::GetRunLimitEx(
    /* [in] */ IAttributedCharacterIteratorAttribute* attribute,
    /* [out] */ Int32* runLimit)
{
    VALIDATE_NOT_NULL(runLimit);

    if (mAttributesAllowed != NULL
            && mAttributesAllowed->Find(attribute) == mAttributesAllowed->End()) {
        *runLimit = mEnd;
        return NOERROR;
    }

    Map<AutoPtr<IAttributedCharacterIteratorAttribute>, List<Range*>* >::Iterator it
            = mAttrString->mAttributeMap->Find(attribute);
    if (it == mAttrString->mAttributeMap->End()) {
        *runLimit = mEnd;
        return NOERROR;
    }

    List<Range*>* ranges = it->mSecond;
    *runLimit = RunLimit(ranges);

    return NOERROR;

}

ECode AttributedString::AttributedIterator::GetRunLimitEx2(
    /* [in] */ IObjectContainer* attributes,
    /* [out] */ Int32* runLimit)
{
    VALIDATE_NOT_NULL(runLimit);

    Int32 limit = mEnd;
    AutoPtr<IObjectEnumerator> enumerator;
    attributes->GetObjectEnumerator((IObjectEnumerator**)&enumerator);
    Boolean hasNext;
    while(enumerator->MoveNext(&hasNext), hasNext) {
        AutoPtr<IAttributedCharacterIteratorAttribute> attr;
        enumerator->Current((IInterface**)&attr);
        Int32 newLimit;
        GetRunLimitEx(attr, &newLimit);
        if (newLimit < limit) {
            limit = newLimit;
        }
    }

    *runLimit = limit;
    return NOERROR;
}

ECode AttributedString::AttributedIterator::GetRunStart(
    /* [out] */ Int32* runStart)
{
    AutoPtr<IObjectContainer> allAttributedKeys;
    GetAllAttributeKeys((IObjectContainer**)&allAttributedKeys);
    return GetRunStartEx2(allAttributedKeys, runStart);
}

Int32 AttributedString::AttributedIterator::RunStart(
    /* [in] */ List<Range*>* ranges)
{
    Int32 result = mBegin;

    List<Range*>::Iterator it;
    for (it = ranges->Begin(); it != ranges->End(); ++it) {
        Range* range = *it;
        if (range->mStart >= mEnd) {
            break;
        }
        if (mOffset >= range->mStart && mOffset < range->mEnd) {
            return InRange(range) ? range->mStart : result;
        }
        else if (mOffset < range->mStart) {
            break;
        }
        result = range->mEnd;
    }

    return result;
}

ECode AttributedString::AttributedIterator::GetRunStartEx(
    /* [in] */ IAttributedCharacterIteratorAttribute* attribute,
    /* [out] */ Int32* runStart)
{
    VALIDATE_NOT_NULL(runStart);

    if (mAttributesAllowed != NULL
            && mAttributesAllowed->Find(attribute) == mAttributesAllowed->End()) {
        *runStart = mBegin;
        return NOERROR;
    }
    Map<AutoPtr<IAttributedCharacterIteratorAttribute>, List<Range*>* >::Iterator it
            = mAttrString->mAttributeMap->Find(attribute);
    if (it == mAttrString->mAttributeMap->End()) {
        *runStart = mBegin;
        return NOERROR;
    }

    List<Range*>* ranges = it->mSecond;
    *runStart = RunStart(ranges);
    return NOERROR;
}

ECode AttributedString::AttributedIterator::GetRunStartEx2(
    /* [in] */ IObjectContainer* attributes,
    /* [out] */ Int32* runStart)
{
    VALIDATE_NOT_NULL(runStart);

    Int32 start = mBegin;
    AutoPtr<IObjectEnumerator> enumerator;
    attributes->GetObjectEnumerator((IObjectEnumerator**)&enumerator);
    Boolean hasNext;
    while(enumerator->MoveNext(&hasNext), hasNext) {
        AutoPtr<IAttributedCharacterIteratorAttribute> attr;
        enumerator->Current((IInterface**)&attr);
        Int32 newStart;
        GetRunStartEx(attr, &newStart);
        if (newStart > start) {
            start = newStart;
        }
    }

    *runStart = start;
    return NOERROR;
}

ECode AttributedString::AttributedIterator::Last(
        /* [out] */ Char32* lastValue)
{
    VALIDATE_NOT_NULL(lastValue);
    if (mBegin == mEnd) {
        *lastValue = (Char32)CharacterIterator_DONE;
        return NOERROR;
    }
    mOffset = mEnd - 1;
    *lastValue = mAttrString->mText.GetChar(mOffset);
    return NOERROR;
}

ECode AttributedString::AttributedIterator::Next(
    /* [out] */ Char32* nextValue)
{
    VALIDATE_NOT_NULL(nextValue);
    if (mOffset >= (mEnd - 1)) {
        mOffset = mEnd;
        *nextValue = (Char32)CharacterIterator_DONE;
        return NOERROR;
    }
    *nextValue = mAttrString->mText.GetChar(++mOffset);
    return NOERROR;
}

ECode AttributedString::AttributedIterator::Previous(
    /* [out] */ Char32* previousValue)
{
    VALIDATE_NOT_NULL(previousValue);
    if (mOffset == mBegin) {
        *previousValue = (Char32)CharacterIterator_DONE;
        return NOERROR;
    }

    *previousValue = mAttrString->mText.GetChar(--mOffset);
    return NOERROR;
}

ECode AttributedString::AttributedIterator::SetIndex(
    /* [in] */ Int32 location,
    /* [out] */ Char32* newChar)
{
    VALIDATE_NOT_NULL(newChar);

    if (location < mBegin || location > mEnd) {
        //throw new IllegalArgumentException();
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    mOffset = location;
    if (mOffset == mEnd) {
        *newChar = (Char32)CharacterIterator_DONE;
        return NOERROR;
    }

    *newChar = mAttrString->mText.GetChar(mOffset);
    return NOERROR;
}

AttributedString::~AttributedString()
{
    mAttributeMap->Clear();
}

ECode AttributedString::Init(
    /* [in] */ IAttributedCharacterIterator* iterator)
{
    Int32 bi, ei;
    iterator->GetBeginIndex(&bi);
    iterator->GetEndIndex(&ei);
    if (bi > ei) {
        //throw new IllegalArgumentException("Invalid substring range");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    StringBuffer buffer;
    for (Int32 i = bi; i < ei; i++) {
        Char32 cv, nv;
        iterator->Current(&cv);
        buffer += cv;
        iterator->Next(&nv);
    }
    mText = String(buffer);

    AutoPtr<IObjectContainer> attributes;
    iterator->GetAllAttributeKeys((IObjectContainer**)&attributes);
    if (attributes == NULL) {
        return NOERROR;
    }

    mAttributeMap =
            new Map<AutoPtr<IAttributedCharacterIteratorAttribute>, List<Range*>* >();

    AutoPtr<IObjectEnumerator> enumerator;
    attributes->GetObjectEnumerator((IObjectEnumerator**)&enumerator);
    Boolean hasNext;
    while(enumerator->MoveNext(&hasNext), hasNext) {
        AutoPtr<IAttributedCharacterIteratorAttribute> attr;
        enumerator->Current((IInterface**)&attr);
        Char32 ch;
        iterator->SetIndex(0, &ch);
        Char32 cv;
        while (iterator->Current(&cv), cv != (Char32)CharacterIterator_DONE) {
            Int32 start;
            iterator->GetRunStartEx(attr, &start);
            Int32 limit;
            iterator->GetRunLimitEx(attr, &limit);
            AutoPtr<IInterface> value;
            iterator->GetAttribute(attr, (IInterface**)&value);
            if (value != NULL) {
                AddAttributeEx(attr, value, start, limit);
            }
            iterator->SetIndex(limit, &ch);
        }
    }

    return NOERROR;
}

ECode AttributedString::Init(
    /* [in] */ IAttributedCharacterIterator* iterator,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ IObjectContainer* attributes)
{
    Int32 beginIndex, endIndex;
    iterator->GetBeginIndex(&beginIndex);
    iterator->GetEndIndex(&endIndex);
    if (start < beginIndex || end > endIndex || start > end) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
//        throw new IllegalArgumentException();
    }

    if (attributes == NULL) {
        return NOERROR;
    }

    StringBuffer buffer;
    Char32 newIndex;
    iterator->SetIndex(start, &newIndex);
    Int32 index;
    while (iterator->GetIndex(&index) ,index < end) {
        Char32 c;
        iterator->Current(&c);
        buffer += c;
        Char32 nextC;
        iterator->Next(&nextC);
    }
    mText = String(buffer);
    mAttributeMap =
            new Map<AutoPtr<IAttributedCharacterIteratorAttribute>, List<Range*>* >();

    AutoPtr<IObjectEnumerator> enumerator;
    attributes->GetObjectEnumerator((IObjectEnumerator**)&enumerator);
    Boolean hasNext;
    while(enumerator->MoveNext(&hasNext), hasNext) {
        AutoPtr<IAttributedCharacterIteratorAttribute> attr;
        enumerator->Current((IInterface**)&attr);
        Char32 c;
        iterator->SetIndex(start, &c);
        Int32 id;
        while (iterator->GetIndex(&id) ,index < end) {
            AutoPtr<IInterface> value;
            iterator->GetAttribute(attr, (IInterface**)&value);
            Int32 runStart;
            iterator->GetRunStartEx(attr, &runStart);
            Int32 limit;
            iterator->GetRunLimitEx(attr, &limit);
            if ((value->Probe(EIID_IAnnotation) != NULL
                    && runStart >= start && limit <= end)
                        || (value != NULL && (value->Probe(EIID_IAnnotation) == NULL))) {
                AddAttributeEx(attr, value, (runStart < start ? start
                        : runStart)
                        - start, (limit > end ? end : limit) - start);
            }
            Char32 newChar;
            iterator->SetIndex(limit, &newChar);
        }
    }

    return NOERROR;
}

ECode AttributedString::Init(
    /* [in] */ IAttributedCharacterIterator* iterator,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    AutoPtr<IObjectContainer> container;
    iterator->GetAllAttributeKeys((IObjectContainer**)&container);
    return Init(iterator, start, end, container);
}

ECode AttributedString::Init(
    /* [in] */ String value)
{
    if (value.IsNull()) {
        //throw new NullPointerException();
        return E_NULL_POINTER_EXCEPTION;
    }
    mText = value;
    mAttributeMap =
            new Map<AutoPtr<IAttributedCharacterIteratorAttribute>, List<Range*>* >();
    return NOERROR;
}

//public AttributedString(String value,
//      Map<? extends AttributedCharacterIterator.Attribute, ?> attributes) {
/*  if (value == null) {
        throw new NullPointerException();
    }
    if (value.length() == 0 && !attributes.isEmpty()) {
        throw new IllegalArgumentException("Cannot add attributes to empty string");
    }
    text = value;
    attributeMap = new HashMap<Attribute, List<Range>>(
            (attributes.size() * 4 / 3) + 1);
    Iterator<?> it = attributes.entrySet().iterator();
    while (it.hasNext()) {
        Map.Entry<?, ?> entry = (Map.Entry<?, ?>) it.next();
        ArrayList<Range> ranges = new ArrayList<Range>(1);
        ranges.add(new Range(0, text.length(), entry.getValue()));
        attributeMap.put((AttributedCharacterIterator.Attribute) entry
                .getKey(), ranges);
    }
}*/

ECode AttributedString::AddAttribute(
    /* [in] */ IAttributedCharacterIteratorAttribute* attribute,
    /* [in] */ IInterface* value)
{
    if (NULL == attribute) {
        //throw new NullPointerException();
        return E_NULL_POINTER_EXCEPTION;
    }
    if (mText.GetLength() == 0) {
        //throw new IllegalArgumentException();
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    List<Range*>* ranges;
    Map<AutoPtr<IAttributedCharacterIteratorAttribute>, List<Range*>* >::Iterator it
            = mAttributeMap->Find(attribute);
    if (it == mAttributeMap->End()) {
        ranges = new List<Range*>(1);
        (*mAttributeMap)[attribute] = ranges;
    }
    else {
        ranges = it->mSecond;
        ranges->Clear();
    }
    ranges->PushBack(new Range(0, mText.GetLength(), value));
    return NOERROR;
}

ECode AttributedString::AddAttributeEx(
    /* [in] */ IAttributedCharacterIteratorAttribute* attribute,
    /* [in] */ IInterface* value,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    if (NULL == attribute) {
        //throw new NullPointerException();
        return E_NULL_POINTER_EXCEPTION;
    }
    if (start < 0 || end > (Int32)mText.GetLength() || start >= end) {
        //throw new IllegalArgumentException();
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    if (value == NULL) {
        return NOERROR;
    }

    List<Range*>* ranges;
    Map<AutoPtr<IAttributedCharacterIteratorAttribute>, List<Range*>* >::Iterator mapIt
            = mAttributeMap->Find(attribute);
    if (mapIt == mAttributeMap->End()) {
        ranges = new List<Range*>(1);
        ranges->PushBack(new Range(start, end, value));
        (*mAttributeMap)[attribute] = ranges;
        return NOERROR;
    }
    else {
        ranges = mapIt->mSecond;
    }

    List<Range*>::Iterator it;
    for (it = ranges->Begin(); it != ranges->End(); ++it) {
        Range& range = **it;
        if (end <= range.mStart && it != ranges->Begin()) {
            --it;
            break;
        }
        else if (start < range.mEnd
                || (start == range.mEnd && value == range.mValue)) {
            it = ranges->Erase(it);
            Range* r1 = new Range(range.mStart, start, range.mValue);
            Range* r3 = new Range(end, range.mEnd, range.mValue);

            while (end > range.mEnd && (++it) != ranges->End()) {
                range = **it;
                if (end <= range.mEnd) {
                    if (end > range.mStart
                            || (end == range.mStart && value == range.mValue)) {
                        it = ranges->Erase(it);
                        r3 = new Range(end, range.mEnd, range.mValue);
                        break;
                    }
                }
                else {
                    it = ranges->Erase(it);
                }
            }

            if (value == r1->mValue) {
                if (value == r3->mValue) {
                    ranges->Insert(it,
                            new Range(r1->mStart < start ? r1->mStart : start,
                                r3->mEnd > end ? r3->mEnd : end, r1->mValue));
                }
                else {
                    it = ranges->Insert(it,
                            new Range(r1->mStart < start ? r1->mStart : start,
                                end, r1->mValue));
                    if (r3->mStart < r3->mEnd) {
                        ranges->Insert(it, r3);
                    }
                }
            }
            else {
                if (value == r3->mValue) {
                    if (r1->mStart < r1->mEnd) {
                        it = ranges->Insert(it, r1);
                    }
                    ranges->Insert(it,
                            new Range(start, r3->mEnd > end ? r3->mEnd : end,
                                r3->mValue));
                }
                else {
                    if (r1->mStart < r1->mEnd) {
                        ranges->Insert(it, r1);
                    }
                    it = ranges->Insert(it,
                            new Range(start, end, value));
                    if (r3->mStart < r3->mEnd) {
                        ranges->Insert(it, r3);
                    }
                }
            }
            return NOERROR;
        }
    }

    ranges->Insert(it, new Range(start, end, value));
    return NOERROR;
}

/*    public void addAttributes(
            Map<? extends AttributedCharacterIterator.Attribute, ?> attributes,
            int start, int end) {
        Iterator<?> it = attributes.entrySet().iterator();
        while (it.hasNext()) {
            Map.Entry<?, ?> entry = (Map.Entry<?, ?>) it.next();
            addAttribute(
                    (AttributedCharacterIterator.Attribute) entry.getKey(),
                    entry.getValue(), start, end);
        }
    }*/

ECode AttributedString::GetIterator(
    /* [out] */ IAttributedCharacterIterator** iterator)
{
    VALIDATE_NOT_NULL(iterator);
    *iterator = (IAttributedCharacterIterator*)new AttributedIterator(this);
    if (*iterator != NULL) {
        (*iterator)->AddRef();
    }
    return NOERROR;
}

ECode AttributedString::GetIteratorEx(
    /* [in] */ IObjectContainer* attributes,
    /* [out] */ IAttributedCharacterIterator** iterator)
{
    VALIDATE_NOT_NULL(iterator);
    *iterator = (IAttributedCharacterIterator*)new AttributedIterator(
            this, attributes, 0, mText.GetLength());
    if (*iterator != NULL) {
        (*iterator)->AddRef();
    }
    return NOERROR;
}

ECode AttributedString::GetIteratorEx2(
    /* [in] */ IObjectContainer* attributes,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [out] */ IAttributedCharacterIterator** iterator)
{
    VALIDATE_NOT_NULL(iterator);
    *iterator = (IAttributedCharacterIterator*)new AttributedIterator(
            this, attributes, start, end);
    if (*iterator != NULL) {
        (*iterator)->AddRef();
    }
    return NOERROR;
}
