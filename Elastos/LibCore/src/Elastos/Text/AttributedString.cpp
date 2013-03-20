#include "AttributedString.h"

AttributedString::Range::Range(
        /* [in] */ Int32 s,
        /* [in] */ Int32 e,
        /* [in] */ IInterface* v)
{
    start = s;
    end = e;
    value = v;
}

AttributedString::AttributedIterator::AttributedIterator(
        /* [in] */ AttributedString* attrString)
{
    this->attrString = attrString;
    begin = 0;
    end = (attrString->text).GetLength();
    offset = 0;
}

AttributedString::AttributedIterator::AttributedIterator(
        /* [in] */ AttributedString* attrString,
        /* [in] */ ArrayOf<IAttributedCharacterIterator_Attribute*>* attributes,
        /* [in] */ Int32 begin,
        /* [in] */ Int32 end)
{
    if (begin < 0 || end > (Int32)((attrString->text).GetLength()) || begin > end) {
        //throw new IllegalArgumentException();
        assert(1 == 0);
    }
    this->begin = begin;
    this->end = end;
    this->offset = begin;
    this->attrString = attrString;
/*
    if (attributes != NULL) {
        HashSet<IAttributedCharacterIterator_Attribute* >* set =
                new HashSet<IAttributedCharacterIterator_Attribute*>(
                    (attributes->GetLength() * 4 / 3) + 1 );
        for (Int32 i = attributes->GetLength(); --i >= 0;) {
            set->Insert((*attributes)[i]);
        }
        attributesAllowed = set;
    }*/
}

ECode AttributedString::AttributedIterator::Current(
        /* [out] */ Char32* value)
{
    VALIDATE_NOT_NULL(value);
    if (offset == end) {
        //return DONE;
        *value = '0xffff';
        return NULL;
    }
    *value = attrString->text.GetChar(offset);
    return NOERROR;
}

ECode AttributedString::AttributedIterator::First(
        /* [out] */ Char32* value)
{
    VALIDATE_NOT_NULL(value);
    if (begin == end) {
        //return DONE;
        *value = '0xffff';
        return NULL;
    }
    offset = begin;
    *value = attrString->text.GetChar(offset);
    return NOERROR;
}

ECode AttributedString::AttributedIterator::GetBeginIndex(
        /* [out] */ Int32* index)
{
    VALIDATE_NOT_NULL(index);
    *index = begin;
    return NOERROR;
}

ECode AttributedString::AttributedIterator::GetEndIndex(
        /* [out] */ Int32* index)
{
    VALIDATE_NOT_NULL(index);
    *index = end;
    return NOERROR;
}

ECode AttributedString::AttributedIterator::GetIndex(
        /* [out] */ Int32* index)
{
    VALIDATE_NOT_NULL(index);
    *index = offset;
    return NOERROR;
}

PInterface AttributedString::Range::Probe(
                /* [in]  */ REIID riid)
{
    return NULL;
}

ECode AttributedString::AttributedIterator::InRange(
    /* [in] */ Range* range,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    if ((range->value)->Probe(EIID_IAnnotation) == NULL) {
        *result = TRUE;
        return NOERROR;
    }
    *result = range->start >= begin && range->start < end
            && range->end > begin && range->end <= end;
    return NOERROR;
}

ECode AttributedString::AttributedIterator::InRange(
    /* [in] */ List<Range*>* ranges,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    List<Range*>::Iterator it1 = ranges->Begin();
    List<Range*>::Iterator it2 = ranges->End();
    while (it1 != it2) {
        Range* range = *it1;
        if (range->start >= begin && range->start < end) {
            *result = ((range->value)->Probe(EIID_IAnnotation) == NULL)
                    || (range->end > begin && range->end <= end);
            return NOERROR;
        } else if (range->end > begin && range->end <= end) {
            *result = ((range->value)->Probe(EIID_IAnnotation) == NULL)
                    || (range->start >= begin && range->start < end);
            return NOERROR;
        }
        ++it1;
    }
    *result = FALSE;
    return NOERROR;
}

ECode AttributedString::AttributedIterator::GetAllAttributeKeys(
        /* [out] */ Set<IAttributedCharacterIterator_Attribute* >* allAttributedKeys)
{
    Map<IAttributedCharacterIterator_Attribute*, List<Range*>* >::Iterator map_it1
            = attrString->attributeMap->Begin();
    Map<IAttributedCharacterIterator_Attribute*, List<Range*>* >::Iterator map_it2
            = attrString->attributeMap->End();
    if (begin == 0 && end == (Int32)(attrString->text.GetLength())
                && attributesAllowed == NULL) {
        for(; map_it1 != map_it2; ++map_it1) {
            allAttributedKeys->Insert(map_it1->mFirst);
        }
        return NOERROR;
    }
/*
    Set<IAttributedCharacterIterator_Attribute* >* result = 
            new HashSet<IAttributedCharacterIterator_Attribute* >(
            ( (attrString->attributeMap)->GetSize() * 4 / 3) + 1);

    map_it1 = attrString->attributeMap->Begin();
    while (map_it1 != map_it2) {
        HashSet<IAttributedCharacterIterator_Attribute*>::Iterator set_it1 = attributesAllowed->Begin();
        HashSet<IAttributedCharacterIterator_Attribute*>::Iterator set_it2 = attributesAllowed->End();
        if (attributesAllowed == NULL
                || attributesAllowed->Find(map_it1->mFirst) != set_it2) {
            List<Range*>* ranges = map_it1->mSecond;
            Boolean isInRange;
            InRange(ranges, &isInRange);
            if (isInRange) {
                result->Insert(map_it1->mFirst);
            }
        }
        ++map_it1;
    }
    allAttributedKeys = result; */
    return NOERROR;
}

ECode AttributedString::AttributedIterator::CurrentValue(
            /* [in] */ List<Range*>* ranges,
            /* [out] */ IInterface** currentValue)
{
    List<Range*>::Iterator it1 = ranges->Begin();
    List<Range*>::Iterator it2 = ranges->End();
    while (it1 != it2) {
        Range* range = *it1;
        if (offset >= range->start && offset < range->end) {
            Boolean result;
            InRange(range, &result);
            *currentValue = result ? (IInterface*)(range->value) : NULL;
            return NOERROR;
        }
        ++it1;
    }
    *currentValue = NULL;
    return NOERROR;
}

ECode AttributedString::AttributedIterator::GetAttribute(
                /* [in] */ IAttributedCharacterIterator_Attribute* attribute,
                /* [out] */ IInterface** instance)
{
/*
    HashSet<IAttributedCharacterIterator_Attribute*>::Iterator iter = 
                    attributesAllowed->Find(attribute);
    Boolean result = iter != attributesAllowed->End();
    if (attributesAllowed != NULL && !result) {
        *instance = NULL;
        return NOERROR;
    }
    List<Range*>* ranges = (*(attrString->attributeMap))[attribute];
    if (ranges == NULL) {
        *instance = NULL;
        return NOERROR;
    }
    return CurrentValue(ranges, instance);
*/
    return E_NOT_IMPLEMENTED;
}

/* CARAPI Map<Attribute, Object> GetAttributes() {
Map<Attribute, Object> result = new HashMap<Attribute, Object>(
    (attrString.attributeMap.size() * 4 / 3) + 1);
Iterator<Map.Entry<Attribute, List<Range>>> it = attrString.attributeMap
    .entrySet().iterator();
while (it.hasNext()) {
Map.Entry<Attribute, List<Range>> entry = it.next();
if (attributesAllowed == null
        || attributesAllowed.contains(entry.getKey())) {
    Object value = currentValue(entry.getValue());
    if (value != null) {
        result.put(entry.getKey(), value);
    }
}
}
return result;
}*/

ECode AttributedString::AttributedIterator::GetRunLimit(
        /* [out] */ Int32* runLimit)
{
    //return getRunLimit(getAllAttributeKeys());
    return E_NOT_IMPLEMENTED;
}

ECode AttributedString::AttributedIterator::RunLimit(
        /* [in] */ List<Range*>* ranges,
        /* [out] */ Int32* limitValue)
{
    VALIDATE_NOT_NULL(limitValue);
    Int32 result = end;

    List<Range*>::ReverseIterator it1 = ranges->RBegin();
    List<Range*>::ReverseIterator it2 = ranges->REnd();
    while (it1 != it2) {
        Range* range = *it1;
        if (range->end <= begin) {
            break;
        }
        if (offset >= range->start && offset < range->end) {
            Boolean b;
            InRange(range, &b);
            *limitValue = b ? range->end : result;
            return NOERROR;
        } else if (offset >= range->end) {
            break;
        }
        result = range->start;
        ++it1;
    }
    *limitValue = result;
    return NOERROR;
}

ECode AttributedString::AttributedIterator::GetRunLimit(
        /* [in] */ IAttributedCharacterIterator_Attribute* attribute,
        /* [out] */ Int32* runLimit)
{
/*
    VALIDATE_NOT_NULL(runLimit);
    HashSet<IAttributedCharacterIterator_Attribute*>::Iterator iter = 
                    attributesAllowed->Find(attribute);
    Boolean result = iter != attributesAllowed->End();
    if (attributesAllowed != NULL && !result) {
        *runLimit = end;
        return NOERROR;
    }
    List<Range*>* ranges = (*(attrString->attributeMap))[attribute];
    if (ranges == NULL) {
        *runLimit = end;
        return NOERROR;
    }
    return RunLimit(ranges, runLimit);
*/
    return NOERROR;
}

/*
public int getRunLimit(Set<? extends Attribute> attributes) {
    int limit = end;
    Iterator<? extends Attribute> it = attributes.iterator();
    while (it.hasNext()) {
        AttributedCharacterIterator.Attribute attribute = it.next();
        int newLimit = getRunLimit(attribute);
        if (newLimit < limit) {
            limit = newLimit;
        }
    }
    return limit;
}*/

ECode AttributedString::AttributedIterator::GetRunStart(
        /* [out] */ Int32* runStart)
{
    //return getRunStart(getAllAttributeKeys());
    return E_NOT_IMPLEMENTED;
}

ECode AttributedString::AttributedIterator::RunStart(
        /* [in] */ List<Range*>* ranges,
        /* [out] */ Int32* runStart)
{
    VALIDATE_NOT_NULL(runStart);
    Int32 result = begin;

    List<Range*>::Iterator it1 = ranges->Begin();
    List<Range*>::Iterator it2 = ranges->End();    
    while (it1 != it2) {
        Range* range = *it1;
        if (range->start >= end) {
            break;
        }
        if (offset >= range->start && offset < range->end) {
            Boolean isInRange;
            InRange(range, &isInRange);
            *runStart = isInRange ? range->start : result;
            return NOERROR;
        } else if (offset < range->start) {
            break;
        }
        result = range->end;
    }
    *runStart = result;
    return NOERROR;
}

ECode AttributedString::AttributedIterator::GetRunStart(
        /* [in] */ IAttributedCharacterIterator_Attribute* attribute,
        /* [out] */ Int32* runStart) 
{
/*
    VALIDATE_NOT_NULL(runStart);
    HashSet<IAttributedCharacterIterator_Attribute*>::Iterator iter = 
                    attributesAllowed->Find(attribute);
    Boolean result = (iter != attributesAllowed->End()) ? TRUE : FALSE;
    if (attributesAllowed != NULL && !result) {
        *runStart = begin;
        return NOERROR;
    }
    List<Range*>* ranges = (*(attrString->attributeMap))[attribute];
    if (ranges == NULL) {
        *runStart = begin;
        return NOERROR;
    }
    return RunStart(ranges, runStart);
*/
    return E_NOT_IMPLEMENTED;
}

/*
public int getRunStart(Set<? extends Attribute> attributes) {
    int start = begin;
    Iterator<? extends Attribute> it = attributes.iterator();
    while (it.hasNext()) {
        AttributedCharacterIterator.Attribute attribute = it.next();
        int newStart = getRunStart(attribute);
        if (newStart > start) {
            start = newStart;
        }
    }
    return start;
}
*/

ECode AttributedString::AttributedIterator::Last(
        /* [out] */ Char32* lastValue) 
{
    VALIDATE_NOT_NULL(lastValue);
    if (begin == end) {
        *lastValue = '0xffff';
        return NOERROR;
    }
    offset = end - 1;
    *lastValue = (attrString->text).GetChar(offset);
    return NOERROR;
}

ECode AttributedString::AttributedIterator::Next(
    /* [out] */ Char32* nextValue) 
{
    VALIDATE_NOT_NULL(nextValue);
    if (offset >= (end - 1)) {
        offset = end;
        *nextValue = '0xffff';
        return NOERROR;
    }
    *nextValue = (attrString->text).GetChar(++offset);
    return NOERROR;
}

ECode AttributedString::AttributedIterator::Previous(
    /* [out] */ Char32* previousValue) 
{
    VALIDATE_NOT_NULL(previousValue);
    if (offset == begin) {
        *previousValue = '0xffff';
        return NOERROR;
    }

    *previousValue = (attrString->text).GetChar(--offset);
    return NOERROR;
}

ECode AttributedString::AttributedIterator::SetIndex(
    /* [in] */ Int32 location,
    /* [out] */ Char32* newIndex) 
{
    VALIDATE_NOT_NULL(newIndex);
    if (location < begin || location > end) {
        //throw new IllegalArgumentException();
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    offset = location;
    if (offset == end) {
        *newIndex = '0xffff';
        return NOERROR;
    }

    *newIndex = (attrString->text).GetChar(offset);
    return NOERROR;
}


AttributedString::AttributedString(){
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
    StringBuffer* buffer = new StringBuffer("");
    for (Int32 i = bi; i < ei; i++) {
        Char32 cv, nv;
        iterator->Current(&cv);
        (*buffer) += cv;
        iterator->Next(&nv);
    }
    this->text = buffer->Substring(0, buffer->GetLength() );

    Set<IAttributedCharacterIterator_Attribute* >* attributes;
    //iterator->GetAllAttributeKeys(attributes);
    if (attributes == NULL) {
        return E_NULL_POINTER_EXCEPTION;
    }
/*
    attributeMap = new HashMap<IAttributedCharacterIterator_Attribute*, List<Range*>* >(
            (attributes->GetSize() * 4 / 3) + 1);

    Set<IAttributedCharacterIterator_Attribute* >::Iterator it1 = attributes->Begin();
    Set<IAttributedCharacterIterator_Attribute* >::Iterator it2 = attributes->End();
    while (it1 != it2) {
        AutoPtr<IAttributedCharacterIterator_Attribute> attribute = *it1;
        Char32 ch;
        iterator->SetIndex(0, &ch);
        Char32 cv;
        iterator->Current(&cv);
        while (cv != '0xffff') {
            Int32 start;
            iterator->GetRunStartEx(attribute, &start);
            Int32 limit;
            iterator->GetRunLimitEx(attribute, &limit);
            AutoPtr<IInterface> value;
            iterator->GetAttribute(attribute, (IInterface**)&value);
            if (value != NULL) {
                AddAttributeEx(attribute, value, start, limit);
            }
            iterator->SetIndex(limit, &ch);
        }
    }  */
    return NOERROR;
}

ECode AttributedString::Init(
        /* [in] */ IAttributedCharacterIterator* iterator,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end) 
{
//    this(iterator, start, end, iterator.getAllAttributeKeys());
    return E_NOT_IMPLEMENTED;
}

ECode AttributedString::Init(
        /* [in] */ IAttributedCharacterIterator* iterator,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [in] */ ArrayOf<IAttributedCharacterIterator_Attribute* >* attributes)
{
    // BEGIN android-removed
    // this(iterator, start, end, new HashSet<Attribute>(Arrays
    //         .asList(attributes)));
    // END android-removed
    // BEGIN android-added
/*    this(iterator, start, end, (attributes == null
            ? new HashSet<Attribute>()
            : new HashSet<Attribute>(Arrays.asList(attributes))));
*/
    // END android-added
    return E_NOT_IMPLEMENTED;
}

ECode AttributedString::Init(
        /* [in] */ String value) 
{
    if (value.IsNull()) {
        //throw new NullPointerException();
        return E_NULL_POINTER_EXCEPTION;
    }
    text = String(value);
//    attributeMap = new HashMap<IAttributedCharacterIterator_Attribute*, List<Range*>* >(11);
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
        /* [in] */ IAttributedCharacterIterator_Attribute* attribute,
        /* [in] */ IInterface* value) 
{
    if (NULL == attribute) {
        //throw new NullPointerException();
        return E_NULL_POINTER_EXCEPTION;
    }
    if (text.GetLength() == 0) {
        //throw new IllegalArgumentException();
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    List<Range*>* ranges = (*attributeMap)[attribute];
    if (ranges == NULL) {
        ranges = new List<Range*>(1);
        Pair<IAttributedCharacterIterator_Attribute*, List<Range*>* > pairValue(attribute, ranges);
        attributeMap->Insert(pairValue);
    } else {
        ranges->Clear();
    }
    Range* rangeInstance = new Range(0, text.GetLength(), value);
    ranges->PushBack(rangeInstance);
    return NOERROR;
}

ECode AttributedString::AddAttributeEx(
        /* [in] */ IAttributedCharacterIterator_Attribute* attribute,
        /* [in] */ IInterface* value,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end) 
{
    if (NULL == attribute) {
        //throw new NullPointerException();
        return E_NULL_POINTER_EXCEPTION;
    }
    if (start < 0 || end > (Int32)(text.GetLength()) || start >= end) {
        //throw new IllegalArgumentException();
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    if (value == NULL) {
        return E_NULL_POINTER_EXCEPTION;
    }

    List<Range*>* ranges = (*attributeMap)[attribute];
    if (ranges == NULL) {
        ranges = new List<Range*>(1);
        Range* rangeInstance = new Range(start, end, value);
        ranges->PushBack(rangeInstance);
        Pair<IAttributedCharacterIterator_Attribute*, List<Range*>* > pairValue(attribute, ranges);
        attributeMap->Insert(pairValue);
        return E_NULL_POINTER_EXCEPTION;
    }
    List<Range*>::Iterator it = ranges->Begin();
    List<Range*>::Iterator itEnd = ranges->End();
    while (it != itEnd) {
        Range* range = *it;
        if (end <= range->start) {
            --it;
            break;
        } else if (start < range->end
                || (start == range->end && value == range->value)) {
            Range *r1 = NULL, *r3;
            ranges->Erase(it);
            r1 = new Range(range->start, start, range->value);
            r3 = new Range(end, range->end, range->value);

            while (end > range->end && it != itEnd) {
                range = *it;
                if (end <= range->end) {
                    if (end > range->start
                            || (end == range->start && value == range->value)) {
                        ranges->Erase(it);
                        r3 = new Range(end, range->end, range->value);
                        break;
                    }
                } else {
                    ranges->Erase(it);
                }
                ++it;
            }

            if (value == r1->value) {
                if (value == r3->value) {
                    ranges->PushBack(new Range(r1->start < start ? r1->start : start,
                            r3->end > end ? r3->end : end, r1->value));
                } else {
                    ranges->PushBack(new Range(r1->start < start ? r1->start : start,
                            end, r1->value));
                    if (r3->start < r3->end) {
                        ranges->PushBack(r3);
                    }
                }
            } else {
                if (value == r3->value) {
                    if (r1->start < r1->end) {
                        ranges->PushBack(r1);
                    }
                    ranges->PushBack(new Range(start, r3->end > end ? r3->end : end,
                            r3->value));
                } else {
                    if (r1->start < r1->end) {
                        ranges->PushBack(r1);
                    }
                    ranges->PushBack(new Range(start, end, value));
                    if (r3->start < r3->end) {
                        ranges->PushBack(r3);
                    }
                }
            }
            return E_NULL_POINTER_EXCEPTION;
        }
        ++it;
    }
    ranges->PushBack(new Range(start, end, value));
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
    //return new AttributedIterator(this);
    return NOERROR;
}

ECode AttributedString::GetIteratorEx(
        /* [in] */ ArrayOf<IAttributedCharacterIterator_Attribute*>* attributes,
        /* [out] */ IAttributedCharacterIterator** iterator) 
{
    //return new AttributedIterator(this, attributes, 0, text.length());
    return NOERROR;
}

ECode AttributedString::GetIteratorEx2(
        /* [in] */ ArrayOf<IAttributedCharacterIterator_Attribute*>* attributes,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [out] */ IAttributedCharacterIterator** iterator)
{
    //*iterator = (IAttributedCharacterIterator*)(new AttributedIterator(this, attributes, start, end) );
    return NOERROR;
}

AttributedString::AttributedString(
        /* [in] */ IAttributedCharacterIterator* iterator, 
        /* [in] */ Int32 start,
        /* [in] */ Int32 end, 
        /* [in] */ Set<IAttributedCharacterIterator_Attribute* >* attributes) 
{
    Int32 bi, ei;
    iterator->GetBeginIndex(&bi);
    iterator->GetEndIndex(&ei);
    if (start < bi || end > ei
            || start > end) {
        //throw new IllegalArgumentException();
        assert(1 == 0);
    }

    if (attributes == NULL) {
        assert(1 == 0);
    }

    StringBuffer* buffer = new StringBuffer();
    Char32 ch;
    iterator->SetIndex(start, &ch);
    Int32 i;
    iterator->GetIndex(&i);
    while (i < end) {
        iterator->Current(&ch);
        (*buffer) += ch;
        iterator->Next(&ch);
    }
    text = buffer->Substring(0, buffer->GetLength());
//    attributeMap = new HashMap<IAttributedCharacterIterator_Attribute*, List<Range*>*>(
//            (attributes->GetSize() * 4 / 3) + 1);

    Set<IAttributedCharacterIterator_Attribute* >::Iterator it = attributes->Begin();
    Set<IAttributedCharacterIterator_Attribute* >::Iterator itEnd = attributes->End();
    while (it != itEnd) {
        IAttributedCharacterIterator_Attribute* attribute = *(++it);
        iterator->SetIndex(start, &ch);
        iterator->GetIndex(&i);
        while (i < end) {
            AutoPtr<IInterface> value;
            iterator->GetAttribute(attribute, (IInterface**)&value);
            Int32 runStart;
            iterator->GetRunStartEx(attribute, &runStart);
            Int32 limit;
            iterator->GetRunLimitEx(attribute, &limit);

            if ((value->Probe(EIID_IAnnotation) != NULL && runStart >= start && limit <= end)
                    || (value != NULL && value->Probe(EIID_IAnnotation) == NULL)) {
                AddAttributeEx(attribute, value, 
                    (runStart < start ? start : runStart) - start, 
                    (limit > end ? end : limit) - start);
            }
            iterator->SetIndex(limit, &ch);
        }
    }
}
