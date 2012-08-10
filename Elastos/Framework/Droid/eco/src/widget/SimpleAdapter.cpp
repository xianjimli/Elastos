
#include "widget/SimpleAdapter.h"
#include "os/ElDataInterface.h"

SimpleAdapter::SimpleFilter::SimpleFilter(
    /* [in] */ SimpleAdapter* host)
    : mHost(host)
{}

//@Override
ECode SimpleAdapter::SimpleFilter::PerformFiltering(
    /* [in] */ ICharSequence* prefix,
    /* [out] */ IFilterResults** filterResults)
{
    AutoPtr<FilterResults> results = new FilterResults();

    if (mHost->mUnfilteredData == NULL) {
        mHost->mUnfilteredData = new Vector<AutoPtr<IObjectStringMap> >(
            mHost->mData);
    }

    Int32 length = 0;
    if (prefix) {
        prefix->GetLength(&length);
    }
    if (length == 0) {
        results->mValues = (IInterface*)new ElDataInterface<
            Vector<AutoPtr<IObjectStringMap> > >(*mHost->mUnfilteredData);
        results->mCount = mHost->mUnfilteredData->GetSize();
    }
    else {
        String prefixString;
        prefix->ToString(&prefixString);

        Vector<AutoPtr<IObjectStringMap> > newValues;
        Vector<AutoPtr<IObjectStringMap> >::Iterator iter =
                mHost->mUnfilteredData->Begin();
        for (; iter != mHost->mUnfilteredData->End(); ++iter) {
            IObjectStringMap* h = (*iter).Get();
            if (h != NULL) {
                Int32 len = mHost->mTo->GetLength();
                for (Int32 i = 0; i < len; i++) {
                    //String str =  (String)h.get(mFrom[j]);
                    AutoPtr<ICharSequence> cs;
                    h->Get((*mHost->mFrom)[i], (IInterface**)&cs);
                    String str;
                    cs->ToString(&str);

                    Int32 start = 0;
                    Int32 index = str.IndexOf(' ');
                    String subStr = str;
                    while (index > 0 || subStr.GetLength() > 0) {
                        if (index < 0) {
                            index = subStr.GetLength();
                        }
                        String word = subStr.Substring(start, index - start);
                        subStr = subStr.Substring(index + 1);
                        start = index + 1;
                        if (word.StartWith(prefixString, StringCase_Insensitive)) {
                            newValues.PushBack(h);
                            break;
                        }
                        index = subStr.IndexOf(' ');
                    }
                }
            }
        }

        results->mValues =
            new ElDataInterface<Vector<AutoPtr<IObjectStringMap> > >(newValues);
        results->mCount = newValues.GetSize();
    }

    *filterResults = (IFilterResults*)results.Get();
    if (*filterResults) {
        (*filterResults)->AddRef();
    }

    return NOERROR;
}

//@Override
ECode SimpleAdapter::SimpleFilter::PublishResults(
    /* [in] */ ICharSequence* constraint,
    /* [in] */ IFilterResults* results)
{
    assert(results);
    FilterResults* filterResults = (FilterResults*)results;
    Vector<AutoPtr<IObjectStringMap> >& values =
        ((ElDataInterface<Vector<AutoPtr<IObjectStringMap> > >*)filterResults
        ->mValues.Get())->mData;
    //noinspection unchecked
    mHost->mData.Assign(values.Begin(), values.End());
    if (filterResults->mCount > 0) {
        mHost->NotifyDataSetChanged();
    }
    else {
        mHost->NotifyDataSetInvalidated();
    }

    return NOERROR;
}

SimpleAdapter::SimpleAdapter()
    : mTo(NULL)
    , mFrom(NULL)
    , mUnfilteredData(NULL)
{}

/**
 * Constructor
 *
 * @param context The context where the View associated with this SimpleAdapter is running
 * @param data A List of Maps. Each entry in the List corresponds to one row in the list. The
 *        Maps contain the data for each row, and should include all the entries specified in
 *        "from"
 * @param resource Resource identifier of a view layout that defines the views for this list
 *        item. The layout file should include at least those named views defined in "to"
 * @param from A list of column names that will be added to the Map associated with each
 *        item.
 * @param to The views that should display column in the "from" parameter. These should all be
 *        TextViews. The first N views in this list are given the values of the first N columns
 *        in the from parameter.
 */
SimpleAdapter::SimpleAdapter(
    /* [in] */ IContext* context,
    /* [in] */ IObjectContainer* data,
    /* [in] */ Int32 resource,
    /* [in] */ const ArrayOf<String>& from,
    /* [in] */ const ArrayOf<Int32>& to)
    : mTo(NULL)
    , mFrom(NULL)
    , mUnfilteredData(NULL)
{
    Init(context, data, resource, from, to);
}


/**
 * @see android.widget.Adapter#getCount()
 */
Int32 SimpleAdapter::GetCount()
{
    return mData.GetSize();
}

/**
 * @see android.widget.Adapter#getItem(Int32)
 */
AutoPtr<IInterface> SimpleAdapter::GetItem(
    /* [in] */ Int32 position)
{
    return mData[position];
}

/**
 * @see android.widget.Adapter#getItemId(Int32)
 */
Int64 SimpleAdapter::GetItemId(
    /* [in] */ Int32 position)
{
    return position;
}

/**
 * @see android.widget.Adapter#getView(Int32, View, ViewGroup)
 */
AutoPtr<IView> SimpleAdapter::GetView(
    /* [in] */ Int32 position,
    /* [in] */ IView* convertView,
    /* [in] */ IViewGroup* parent)
{
    return CreateViewFromResource(position, convertView, parent, mResource);
}

AutoPtr<IView> SimpleAdapter::CreateViewFromResource(
    /* [in] */ Int32 position,
    /* [in] */ IView* convertView,
    /* [in] */ IViewGroup* parent,
    /* [in] */ Int32 resource)
{
    AutoPtr<IView> view;

    if (convertView == NULL) {
        mInflater->InflateEx2(resource, parent, FALSE, (IView**)&view);
    }
    else {
        view = convertView;
    }

    BindView(position, view);

    return view;
}

/**
 * <p>Sets the layout resource to create the drop down views.</p>
 *
 * @param resource the layout resource defining the drop down views
 * @see #getDropDownView(Int32, android.view.View, android.view.ViewGroup)
 */
ECode SimpleAdapter::SetDropDownViewResource(
    /* [in] */ Int32 resource)
{
    mDropDownResource = resource;

    return NOERROR;
}

//@Override
AutoPtr<IView> SimpleAdapter::GetDropDownView(
    /* [in] */ Int32 position,
    /* [in] */ IView* convertView,
    /* [in] */ IViewGroup* parent)
{
    return CreateViewFromResource(
        position, convertView, parent, mDropDownResource);
}

ECode SimpleAdapter::BindView(
    /* [in] */ Int32 position,
    /* [in] */ IView* view)
{
    AutoPtr<IObjectStringMap> dataSet = mData[position];
    if (dataSet == NULL) {
        return NOERROR;
    }

    IViewBinder* binder = mViewBinder;
    Int32 count = mTo->GetLength();

    for (Int32 i = 0; i < count; i++) {
        AutoPtr<IView> v;
        view->FindViewById((*mTo)[i], (IView**)&v);
        if (v != NULL) {
            AutoPtr<IInterface> data;
            dataSet->Get((*mFrom)[i], (IInterface**)&data);

            String text;
            if (data != NULL) {
                //data->ToString(&text);
            }

            if (text.IsNull()) {
                text = "";
            }

            Boolean bound = FALSE;
            if (binder != NULL) {
                binder->SetViewValue(v, data, text, &bound);
            }

            if (!bound) {
                if (ICheckable::Probe(v)) {
                    /*if (IBoolean::Probe(data)) {
                        ICheckable::Probe(v)->SetChecked((Boolean)data);
                    }
                    else*/ if (ITextView::Probe(v)) {
                        // Note: keep the instanceof TextView check at the bottom of these
                        // ifs since a lot of views are TextViews (e.g. CheckBoxes).
                        SetViewText(ITextView::Probe(v), text);
                    }
                    else {
                        //throw new IllegalStateException(v.getClass().getName() +
                        //        " should be bound to a Boolean, not a " +
                        //        (data == NULL ? "<unknown type>" : data.getClass()));
                        return E_ILLEGAL_STATE_EXCEPTION;
                    }
                }
                else if (ITextView::Probe(v)) {
                    // Note: keep the instanceof TextView check at the bottom of these
                    // ifs since a lot of views are TextViews (e.g. CheckBoxes).
                    SetViewText(ITextView::Probe(v), text);
                }
                else if (IImageView::Probe(v)) {
                    //if (data instanceof Integer) {
                    //    SetViewImage(IImageView::Probe(v), (Integer) data);
                    //}
                    //else {
                        SetViewImage(IImageView::Probe(v), text);
                    //}
                }
                else {
                    //throw new IllegalStateException(v.getClass().getName() + " is not a " +
                    //        " view that can be bounds by this SimpleAdapter");

                    return E_ILLEGAL_STATE_EXCEPTION;
                }
            }
        }
    }

    return NOERROR;
}

/**
 * Returns the {@link ViewBinder} used to bind data to views.
 *
 * @return a ViewBinder or NULL if the binder does not exist
 *
 * @see #setViewBinder(android.widget.SimpleAdapter.ViewBinder)
 */
AutoPtr<IViewBinder> SimpleAdapter::GetViewBinder()
{
    return mViewBinder;
}

/**
 * Sets the binder used to bind data to views.
 *
 * @param viewBinder the binder used to bind data to views, can be NULL to
 *        remove the existing binder
 *
 * @see #getViewBinder()
 */
ECode SimpleAdapter::SetViewBinder(
    /* [in] */ IViewBinder* viewBinder)
{
    mViewBinder = viewBinder;

    return NOERROR;
}

/**
 * Called by bindView() to set the image for an ImageView but only if
 * there is no existing ViewBinder or if the existing ViewBinder cannot
 * handle binding to an ImageView.
 *
 * This method is called instead of {@link #setViewImage(ImageView, String)}
 * if the supplied data is an Int32 or Integer.
 *
 * @param v ImageView to receive an image
 * @param value the value retrieved from the data set
 *
 * @see #setViewImage(ImageView, String)
 */
ECode SimpleAdapter::SetViewImage(
    /* [in] */ IImageView* v,
    /* [in] */ Int32 value)
{
    return v->SetImageResource(value);
}

/**
 * Called by bindView() to set the image for an ImageView but only if
 * there is no existing ViewBinder or if the existing ViewBinder cannot
 * handle binding to an ImageView.
 *
 * By default, the value will be treated as an image resource. If the
 * value cannot be used as an image resource, the value is used as an
 * image Uri.
 *
 * This method is called instead of {@link #setViewImage(ImageView, Int32)}
 * if the supplied data is not an Int32 or Integer.
 *
 * @param v ImageView to receive an image
 * @param value the value retrieved from the data set
 *
 * @see #setViewImage(ImageView, Int32)
 */
ECode SimpleAdapter::SetViewImage(
    /* [in] */ IImageView* v,
    /* [in] */ const String& value)
{
    Int32 intValue = value.ToInt32();
    if (!value.Equals("0") || intValue != 0) {
        v->SetImageResource(intValue);
    }
    else {
        AutoPtr<IUri> uri;
        //uri = Uri.parse(value);
        v->SetImageURI(uri);
    }

    return NOERROR;
}

/**
 * Called by bindView() to set the text for a TextView but only if
 * there is no existing ViewBinder or if the existing ViewBinder cannot
 * handle binding to an TextView.
 *
 * @param v TextView to receive text
 * @param text the text to be set for the TextView
 */
ECode SimpleAdapter::SetViewText(
    /* [in] */ ITextView* v,
    /* [in] */ const String& text)
{
    AutoPtr<ICharSequence> cs;
    CStringWrapper::New(text, (ICharSequence**)&cs);

    return v->SetText(cs);
}

AutoPtr<IFilter> SimpleAdapter::GetFilter()
{
    if (mFilter == NULL) {
        mFilter = new SimpleFilter(this);
    }

    return mFilter;
}

ECode SimpleAdapter::Init(
    /* [in] */ IContext* context,
    /* [in] */ IObjectContainer* data,
    /* [in] */ Int32 resource,
    /* [in] */ const ArrayOf<String>& from,
    /* [in] */ const ArrayOf<Int32>& to)
{
    AutoPtr<IObjectEnumerator> it;
    data->GetObjectEnumerator((IObjectEnumerator**)&it);
    Boolean hasNext;
    while (it->MoveNext(&hasNext), hasNext) {
        AutoPtr<IInterface> obj;
        it->Current((IInterface**)&obj);
        IObjectStringMap* item = IObjectStringMap::Probe(obj.Get());
        mData.PushBack(item);
    }

    mResource = mDropDownResource = resource;
    mFrom = from.Clone();
    mTo = to.Clone();

    context->GetSystemService(
        Context_LAYOUT_INFLATER_SERVICE, (IInterface**)&mInflater);

    return NOERROR;
}
