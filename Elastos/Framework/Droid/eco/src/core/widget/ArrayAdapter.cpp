
#include "widget/ArrayAdapter.h"
#include "os/ElDataInterface.h"

ArrayAdapter::ArrayFilter::ArrayFilter(
    /* [in] */ ArrayAdapter* host)
    : mHost(host)
{}

//@Override
ECode ArrayAdapter::ArrayFilter::PerformFiltering(
    /* [in] */ ICharSequence* prefix,
    /* [out] */ IFilterResults** filterResults)
{
    AutoPtr<FilterResults> results = new FilterResults();

    if (mHost->mOriginalValues == NULL) {
        Mutex::Autolock lock(mHost->mLock);
        mHost->mOriginalValues = new Vector<AutoPtr<IInterface> >(
            mHost->mObjects.Begin(), mHost->mObjects.End());
    }

    Int32 length = 0;
    if (prefix) {
        prefix->GetLength(&length);
    }
    if (length == 0) {
        Mutex::Autolock lock(mHost->mLock);

        results->mValues = new ElDataInterface<Vector<AutoPtr<IInterface> > >(
            *mHost->mOriginalValues);
        results->mCount = mHost->mOriginalValues->GetSize();
    }
    else {
        String prefixString;
        prefix->ToString(&prefixString);

        Vector<AutoPtr<IInterface> > newValues;
        Vector<AutoPtr<IInterface> >::Iterator iter =
                        mHost->mOriginalValues->Begin();
        for (; iter!=mHost->mOriginalValues->End(); ++iter) {
            AutoPtr<IInterface> value = *iter;
            String valueText("");
            if (ICharSequence::Probe(value.Get()))
                ICharSequence::Probe(value.Get())->ToString(&valueText);

            // First match against the whole, non-splitted value
            if (valueText.StartWith(prefixString, StringCase_Insensitive)) {
                newValues.PushBack(value);
            }
            else {
                Int32 start = 0;
                Int32 index = valueText.IndexOf(' ');
                String subStr = valueText;
                while (index > 0 || subStr.GetLength() > 0) {
                    if (index < 0) {
                        index = subStr.GetLength();
                    }
                    String word = subStr.Substring(start, index - start);
                    subStr = subStr.Substring(index + 1);
                    start = index + 1;
                    if (word.StartWith(prefixString, StringCase_Insensitive)) {
                        newValues.PushBack(value);
                        break;
                    }
                    if (subStr.IsNull())
                        break;
                    index = subStr.IndexOf(' ');
                }
            }
        }

        results->mValues =
            new ElDataInterface<Vector<AutoPtr<IInterface> > >(newValues);
        results->mCount = newValues.GetSize();
    }

    *filterResults = (IFilterResults*)results.Get();
    if (*filterResults) {
        (*filterResults)->AddRef();
    }

    return NOERROR;
}

//@Override
ECode ArrayAdapter::ArrayFilter::PublishResults(
    /* [in] */ ICharSequence* constraint,
    /* [in] */ IFilterResults* results)
{
    assert(results);
    FilterResults* filterResults = (FilterResults*)results;
    Vector<AutoPtr<IInterface> >& values =
        ((ElDataInterface<Vector<AutoPtr<IInterface> > >*)filterResults
        ->mValues.Get())->mData;
    //noinspection unchecked
    mHost->mObjects.Assign(values.Begin(), values.End());
    if (filterResults->mCount > 0) {
        mHost->NotifyDataSetChanged();
    }
    else {
        mHost->NotifyDataSetInvalidated();
    }

    return NOERROR;
}

ArrayAdapter::ArrayAdapter()
    : mFieldId(0)
    , mNotifyOnChange(TRUE)
    , mOriginalValues(NULL)
{}

/**
 * Constructor
 *
 * @param context The current context.
 * @param textViewResourceId The resource ID for a layout file containing a TextView to use when
 *                 instantiating views.
 */
ArrayAdapter::ArrayAdapter(
    /* [in] */ IContext* context,
    /* [in] */ Int32 textViewResourceId)
    : mFieldId(0)
    , mNotifyOnChange(TRUE)
    , mOriginalValues(NULL)
{
    Init(context, textViewResourceId, 0);
}

/**
 * Constructor
 *
 * @param context The current context.
 * @param resource The resource ID for a layout file containing a layout to use when
 *                 instantiating views.
 * @param textViewResourceId The id of the TextView within the layout resource to be populated
 */
ArrayAdapter::ArrayAdapter(
    /* [in] */ IContext* context,
    /* [in] */ Int32 resource,
    /* [in] */ Int32 textViewResourceId)
    : mFieldId(0)
    , mNotifyOnChange(TRUE)
    , mOriginalValues(NULL)
{
    Init(context, resource, textViewResourceId);
}

/**
 * Constructor
 *
 * @param context The current context.
 * @param textViewResourceId The resource ID for a layout file containing a TextView to use when
 *                 instantiating views.
 * @param objects The objects to represent in the ListView.
 */
ArrayAdapter::ArrayAdapter(
    /* [in] */ IContext* context,
    /* [in] */ Int32 textViewResourceId,
    /* [in] */ IObjectContainer* objects)
    : mFieldId(0)
    , mNotifyOnChange(TRUE)
    , mOriginalValues(NULL)
{
    Init(context, textViewResourceId, 0, objects);
}

/**
 * Constructor
 *
 * @param context The current context.
 * @param resource The resource ID for a layout file containing a layout to use when
 *                 instantiating views.
 * @param textViewResourceId The id of the TextView within the layout resource to be populated
 * @param objects The objects to represent in the ListView.
 */
ArrayAdapter::ArrayAdapter(
    /* [in] */ IContext* context,
    /* [in] */ Int32 resource,
    /* [in] */ Int32 textViewResourceId,
    /* [in] */ IObjectContainer* objects)
    : mFieldId(0)
    , mNotifyOnChange(TRUE)
    , mOriginalValues(NULL)
{
    Init(context, resource, textViewResourceId, objects);
}

ArrayAdapter::~ArrayAdapter()
{
    delete mOriginalValues;
}

/**
 * Adds the specified object at the end of the array.
 *
 * @param object The object to add at the end of the array.
 */
ECode ArrayAdapter::Add(
    /* [in] */ IInterface* object)
{
    if (mOriginalValues != NULL) {
        Mutex::Autolock lock(mLock);
        mOriginalValues->PushBack(object);

        if (mNotifyOnChange) {
            NotifyDataSetChanged();
        }
    }
    else {
        mObjects.PushBack(object);

        if (mNotifyOnChange) {
            NotifyDataSetChanged();
        }
    }

    return NOERROR;
}

/**
 * Inserts the specified object at the specified index in the array.
 *
 * @param object The object to insert into the array.
 * @param index The index at which the object must be inserted.
 */
ECode ArrayAdapter::Insert(
    /* [in] */ IInterface* object,
    /* [in] */ Int32 index)
{
    if (mOriginalValues != NULL) {
        Mutex::Autolock lock(mLock);
        mOriginalValues->Insert(mOriginalValues->Begin() + index, object);

        if (mNotifyOnChange) {
            NotifyDataSetChanged();
        }
    }
    else {
        mObjects.Insert(index, object);

        if (mNotifyOnChange) {
            NotifyDataSetChanged();
        }
    }

    return NOERROR;
}

/**
 * Removes the specified object from the array.
 *
 * @param object The object to remove.
 */
ECode ArrayAdapter::Remove(
    /* [in] */ IInterface* object)
{
    if (mOriginalValues != NULL) {
        Mutex::Autolock lock(mLock);
        Vector<AutoPtr<IInterface> >::Iterator iter = mOriginalValues->Begin();
        for (; iter != mOriginalValues->End(); ++iter) {
            if ((*iter).Get() == object) {
                mOriginalValues->Erase(iter);
                break;
            }
        }
    }
    else {
        List<AutoPtr<IInterface> >::Iterator iter = mObjects.Begin();
        for (; iter != mObjects.End(); ++iter) {
            if ((*iter).Get() == object) {
                mObjects.Erase(iter);
                break;
            }
        }
    }

    if (mNotifyOnChange) {
        NotifyDataSetChanged();
    }

    return NOERROR;
}

/**
 * Remove all elements from the list.
 */
ECode ArrayAdapter::Clear()
{
    if (mOriginalValues != NULL) {
        Mutex::Autolock lock(mLock);
        mOriginalValues->Clear();
    }
    else {
        mObjects.Clear();
    }

    if (mNotifyOnChange) {
        NotifyDataSetChanged();
    }

    return NOERROR;
}

/**
 * Sorts the content of this adapter using the specified comparator.
 *
 * @param comparator The comparator used to sort the objects contained
 *        in this adapter.
 */
ECode ArrayAdapter::Sort(
    /* [in] */ IComparator* comparator)
{
    //typedef ECode (STDCALL *Comp)(IInterface*, IInterface*);
    //Comp comp = comparator->Compare;
    //mObjects.Sort(comp);
    if (mNotifyOnChange) {
        NotifyDataSetChanged();
    }

    return NOERROR;
}

/**
 * {@inheritDoc}
 */
//@Override
ECode ArrayAdapter::NotifyDataSetChanged()
{
    BaseAdapter::NotifyDataSetChanged();
    mNotifyOnChange = TRUE;

    return NOERROR;
}

/**
 * Control whether methods that change the list ({@link #add},
 * {@link #insert}, {@link #remove}, {@link #clear}) automatically call
 * {@link #notifyDataSetChanged}.  If set to false, caller must
 * manually call notifyDataSetChanged() to have the changes
 * reflected in the attached view.
 *
 * The default is TRUE, and calling notifyDataSetChanged()
 * resets the flag to TRUE.
 *
 * @param notifyOnChange if TRUE, modifications to the list will
 *                       automatically call {@link
 *                       #notifyDataSetChanged}
 */
ECode ArrayAdapter::SetNotifyOnChange(
    /* [in] */ Boolean notifyOnChange)
{
    mNotifyOnChange = notifyOnChange;

    return NOERROR;
}

ECode ArrayAdapter::Init(
    /* [in] */ IContext* context,
    /* [in] */ Int32 resource,
    /* [in] */ Int32 textViewResourceId,
    /* [in] */ IObjectContainer* objects/* = NULL*/)
{
    mContext = context;
    context->GetSystemService(
        Context_LAYOUT_INFLATER_SERVICE, (IInterface**)&mInflater);
    mResource = mDropDownResource = resource;
    mFieldId = textViewResourceId;

    if (objects) {
        AutoPtr<IObjectEnumerator> it;
        objects->GetObjectEnumerator((IObjectEnumerator**)&it);
        Boolean hasNext;
        while (it->MoveNext(&hasNext), hasNext) {
            AutoPtr<IInterface> obj;
            it->Current((IInterface**)&obj);
            mObjects.PushBack(obj);
        }
    }

    return NOERROR;
}

/**
 * Returns the context associated with this array adapter. The context is used
 * to create views from the resource passed to the constructor.
 *
 * @return The Context associated with this adapter.
 */
AutoPtr<IContext> ArrayAdapter::GetContext()
{
    return mContext;
}

/**
 * {@inheritDoc}
 */
Int32 ArrayAdapter::GetCount()
{
    return mObjects.GetSize();
}

/**
 * {@inheritDoc}
 */
AutoPtr<IInterface> ArrayAdapter::GetItem(
    /* [in] */ Int32 position)
{
    return mObjects[position];
}

/**
 * Returns the position of the specified item in the array.
 *
 * @param item The item to retrieve the position of.
 *
 * @return The position of the specified item.
 */
Int32 ArrayAdapter::GetPosition(
    /* [in] */ IInterface* item)
{
    List<AutoPtr<IInterface> >::Iterator iter = mObjects.Begin();
    for (Int32 i = 0; iter != mObjects.End(); ++iter, ++i) {
        if ((*iter).Get() == item) {
            return i;
        }
    }

    return -1;
}

/**
 * {@inheritDoc}
 */
Int64 ArrayAdapter::GetItemId(
    /* [in] */ Int32 position)
{
    return position;
}

/**
 * {@inheritDoc}
 */
AutoPtr<IView> ArrayAdapter::GetView(
    /* [in] */ Int32 position,
    /* [in] */ IView* convertView,
    /* [in] */ IViewGroup* parent)
{
    return CreateViewFromResource(position, convertView, parent, mResource);
}

AutoPtr<IView> ArrayAdapter::CreateViewFromResource(
    /* [in] */ Int32 position,
    /* [in] */ IView* convertView,
    /* [in] */ IViewGroup* parent,
    /* [in] */ Int32 resource)
{
    AutoPtr<IView> view;
    AutoPtr<ITextView> text;

    if (convertView == NULL) {
        mInflater->InflateEx2(resource, parent, FALSE, (IView**)&view);
    }
    else {
        view = convertView;
    }

    //try {
        if (mFieldId == 0) {
            //  If no custom field is assigned, assume the whole resource is a TextView
            text = ITextView::Probe(view);
        }
        else {
            //  Otherwise, find the TextView field within the layout
            AutoPtr<IView> temp;
            view->FindViewById(mFieldId, (IView**)&temp);
            text = ITextView::Probe(temp);
        }
    //} catch (ClassCastException e) {
    //    Log.e("ArrayAdapter", "You must supply a resource ID for a TextView");
    //    throw new IllegalStateException(
    //            "ArrayAdapter requires the resource ID to be a TextView", e);
    //}

    AutoPtr<IInterface> item = GetItem(position);
    ICharSequence* cs = ICharSequence::Probe(item);
    if (cs) {
        text->SetText(cs);
    }
    else {
        //TODO:
        //text->SetText(item.toString());
    }

    return view;
}

/**
 * <p>Sets the layout resource to create the drop down views.</p>
 *
 * @param resource the layout resource defining the drop down views
 * @see #getDropDownView(Int32, android.view.View, android.view.ViewGroup)
 */
ECode ArrayAdapter::SetDropDownViewResource(
    /* [in] */ Int32 resource)
{
    mDropDownResource = resource;

    return NOERROR;
}

/**
 * {@inheritDoc}
 */
//@Override
AutoPtr<IView> ArrayAdapter::GetDropDownView(
    /* [in] */ Int32 position,
    /* [in] */ IView* convertView,
    /* [in] */ IViewGroup* parent)
{
    return CreateViewFromResource(
        position, convertView, parent, mDropDownResource);
}

/**
 * {@inheritDoc}
 */
AutoPtr<IFilter> ArrayAdapter::GetFilter()
{
    if (mFilter == NULL) {
        mFilter = new ArrayFilter(this);
    }

    return mFilter;
}
