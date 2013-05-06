
#include "app/CLauncherActivity.h"
#include "content/CIntent.h"


CLauncherActivity::ActivityAdapter::ActivityAdapter(
    /* [in] */ ILauncherActivityIconResizer* resizer,
    /* [in] */ ILauncherActivity* lActivity)
{
    mLauncherActivity = (CLauncherActivity*)lActivity;
    mIconResizer = (CLauncherActivityIconResizer*)resizer;
    mLauncherActivity->GetSystemService(
            Context_LAYOUT_INFLATER_SERVICE, (IInterface**)&mInflater);
    AutoPtr<IObjectContainer> container;
    mLauncherActivity->MakeListItems((IObjectContainer**)&container);
    AutoPtr<IObjectEnumerator> enumerator;
    container->GetObjectEnumerator((IObjectEnumerator**)&enumerator);
    Boolean hasNext = FALSE;
    while (enumerator->MoveNext(&hasNext), hasNext) {
        CLauncherActivityListItem* listItem;
        enumerator->Current((IInterface**)&listItem);
        mActivitiesList->PushBack(listItem);
    }
}

AutoPtr<IIntent> CLauncherActivity::ActivityAdapter::IntentForPosition(
    /* [in] */ Int32 position)
{
    if (mActivitiesList == NULL) {
        return NULL;
    }

    AutoPtr<IIntent> newintent;
    CIntent::New(mLauncherActivity->mIntent, (IIntent**)&newintent);
    AutoPtr<ILauncherActivityListItem> lItem = (*mActivitiesList)[position];
    CLauncherActivityListItem* item = (CLauncherActivityListItem*)(ILauncherActivityListItem*)lItem;
    newintent->SetClassNameEx(item->mCapsuleName, item->mClassName);
    if (item->mExtras != NULL) {
        newintent->PutExtrasEx(item->mExtras);
    }
    return newintent;
}

AutoPtr<ILauncherActivityListItem> CLauncherActivity::ActivityAdapter::ItemForPosition(
    /* [in] */ Int32 position)
{
    if (mActivitiesList == NULL) {
        return NULL;
    }

    return (*mActivitiesList)[position];
}

Int32 CLauncherActivity::ActivityAdapter::GetCount()
{
    return mActivitiesList != NULL ? mActivitiesList->GetSize() : 0;
}

AutoPtr<IInterface> CLauncherActivity::ActivityAdapter::GetItem(
    /* [in] */ Int32 position)
{
    return (IInterface*)&position;
}

Int64 CLauncherActivity::ActivityAdapter::GetItemId(
    /* [in] */ Int32 position)
{
    return position;
}

AutoPtr<IView> CLauncherActivity::ActivityAdapter::GetView(
    /* [in] */ Int32 position,
    /* [in] */ IView* convertView,
    /* [in] */ IViewGroup* parent)
{
    AutoPtr<IView> view;
    if (convertView == NULL) {
        mInflater->InflateEx2(
                0/*com.android.internal.R.layout.activity_list_item_2*/,
                parent, FALSE, (IView**)&view);
    }
    else {
        view = convertView;
    }
    AutoPtr<ILauncherActivityListItem> item = (*mActivitiesList)[position];
    BindView(view, item);
    return view;
}

void CLauncherActivity::ActivityAdapter::BindView(
    /* [in] */ IView* view,
    /* [in] */ ILauncherActivityListItem* item)
{
    AutoPtr<ITextView> text = (ITextView*)view;
    text->SetText(((CLauncherActivityListItem*)item)->mLabel);
    if (((CLauncherActivityListItem*)item)->mIcon == NULL) {
        AutoPtr<ILocalCapsuleManager> cmanager;
        mLauncherActivity->GetCapsuleManager((ILocalCapsuleManager**)&cmanager);
        AutoPtr<IDrawable> icon;
        ((CLauncherActivityListItem*)item)->mResolveInfo->LoadIcon(cmanager, (IDrawable**)&icon);
        AutoPtr<IDrawable> cIcon;
        mIconResizer->CreateIconThumbnail(icon, (IDrawable**)&cIcon);
        ((CLauncherActivityListItem*)item)->mIcon = cIcon;
    }
    text->SetCompoundDrawablesWithIntrinsicBoundsEx(((CLauncherActivityListItem*)item)->mIcon, NULL, NULL, NULL);
}

ECode CLauncherActivity::ActivityAdapter::GetFilter(
    /* [out] */ IFilter** filter)
{
    if (mFilter == NULL) {
        mFilter = new ArrayFilter(this);
    }
    *filter = mFilter;
    return NOERROR;
}

CLauncherActivity::ActivityAdapter::ArrayFilter::ArrayFilter(ActivityAdapter* adapter)
    : mActivityAdapter(adapter)
{}

ECode CLauncherActivity::ActivityAdapter::ArrayFilter::PerformFiltering(
    /* [in] */ ICharSequence* prefix,
    /* [out] */ IFilterResults** filterResults)
{
    AutoPtr<FilterResults> results = new FilterResults();

    if (mActivityAdapter->mOriginalValues == NULL) {
        Mutex::Autolock lock(mActivityAdapter->mLock);
        mActivityAdapter->mOriginalValues = mActivityAdapter->mActivitiesList;
    }

    Int32 length;
    prefix->GetLength(&length);
    if (prefix == NULL || length == 0) {
        Mutex::Autolock lock(mActivityAdapter->mLock);
        List<AutoPtr<ILauncherActivityListItem> >* list = mActivityAdapter->mOriginalValues;
        AutoPtr<IObjectContainer> listcontainer;
        for (Int32 i = 0; i < list->GetSize(); i++) {
            AutoPtr<ILauncherActivityListItem> item = (*list)[i];
            listcontainer->Add(item);
        }
        results->mValues = (IInterface*)listcontainer;
        results->mCount = list->GetSize();
    }
    else {
        String prefixString;
        prefix->ToString(&prefixString);
        prefixString.ToLowerCase();

        List<AutoPtr<ILauncherActivityListItem> >* values = mActivityAdapter->mOriginalValues;
        Int32 count = values->GetSize();

        List<AutoPtr<ILauncherActivityListItem> >* newValues = new List<AutoPtr<ILauncherActivityListItem> >(count);

        for (Int32 i = 0; i < count; i++) {
            AutoPtr<ILauncherActivityListItem> item = (*values)[i];

            Vector<String> words; //= item.label.toString().toLowerCase().split(" ");
            Int32 wordCount = words.GetSize();

            for (Int32 k = 0; k < wordCount; k++) {
                const String word = words[k];

                if (word.StartWith(prefixString)) {
                    newValues->PushBack(item);
                    break;
                }
            }
        }

        AutoPtr<IObjectContainer> valuecontainer;
        for (Int32 i = 0; i < newValues->GetSize(); i++) {
            AutoPtr<ILauncherActivityListItem> item = (*newValues)[i];
            valuecontainer->Add(item);
        }
        results->mValues = (IInterface*)valuecontainer;
        results->mCount = newValues->GetSize();

        delete newValues;
    }

    *filterResults = results;
    return NOERROR;
}

ECode CLauncherActivity::ActivityAdapter::ArrayFilter::PublishResults(
    /* [in] */ ICharSequence* constraint,
    /* [in] */ IFilterResults* results)
{
    //noinspection unchecked
    AutoPtr<IObjectContainer> listcontainer;
    listcontainer = IObjectContainer::Probe(((FilterResults*)results)->mValues);
    AutoPtr<IObjectEnumerator> enumerator;
    listcontainer->GetObjectEnumerator((IObjectEnumerator**)&enumerator);
    Boolean hasNext = FALSE;
    while (enumerator->MoveNext(&hasNext), hasNext) {
        AutoPtr<ILauncherActivityListItem> item;
        enumerator->Current((IInterface**)&item);
        mActivityAdapter->mActivitiesList->PushBack(item);
    }
    if (((FilterResults*)results)->mCount > 0) {
        mActivityAdapter->NotifyDataSetChanged();
    }
    else {
        mActivityAdapter->NotifyDataSetInvalidated();
    }
    return NOERROR;
}

CLauncherActivity::_ActivityAdapter::_ActivityAdapter(
    /* [in] */ ILauncherActivityIconResizer* resizer,
    /* [in] */ ILauncherActivity* lActivity)
    : ActivityAdapter(resizer, lActivity)
{}

CLauncherActivity::_ActivityAdapter::~_ActivityAdapter()
{}

PInterface CLauncherActivity::_ActivityAdapter::Probe(
    /* [in]  */ REIID riid)
{
    if (riid == EIID_IListAdapter) {
        return (PInterface)(IListAdapter*)this;
    }
    else if (riid == EIID_IFilterable) {
        return (IFilterable*)this;
    }

    return NULL;
}

ECode CLauncherActivity::_ActivityAdapter::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    if (pIID == NULL) {
        return E_INVALID_ARGUMENT;
    }

    if (pObject == (IInterface*)(IListAdapter*)this) {
        *pIID = EIID_IListAdapter;
    }
    else if (pObject == (IInterface*)(IFilterable*)this) {
        *pIID = EIID_IFilterable;
    }

    return NOERROR;
}

UInt32 CLauncherActivity::_ActivityAdapter::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CLauncherActivity::_ActivityAdapter::Release()
{
    return ElRefBase::Release();
}

ECode CLauncherActivity::_ActivityAdapter::GetFilter(
    /* [out] */ IFilter** filter)
{
    VALIDATE_NOT_NULL(filter);
    return ActivityAdapter::GetFilter(filter);
}

IADAPTER_METHODS_IMPL(CLauncherActivity::_ActivityAdapter, ActivityAdapter, ActivityAdapter);

IListADAPTER_METHODS_IMPL(CLauncherActivity::_ActivityAdapter, ActivityAdapter, ActivityAdapter);

IBASEADAPTER_METHODS_IMPL(CLauncherActivity::_ActivityAdapter, ActivityAdapter, ActivityAdapter);

ECode CLauncherActivity::OnCreate(
    /* [in] */ IBundle* icicle)
{
    Activity::OnCreate(icicle);

    GetCapsuleManager((ILocalCapsuleManager**)&mCapsuleManager);

    Boolean result;
    RequestWindowFeature(Window_FEATURE_INDETERMINATE_PROGRESS, &result);
    SetProgressBarIndeterminateVisibility(TRUE);
    OnSetContentView();

    CLauncherActivityIconResizer::New(this, (ILauncherActivityIconResizer**)&mIconResizer);

    CIntent::New(GetTargetIntent(), (IIntent**)&mIntent);
    mIntent->SetComponent(NULL);
    mAdapter = new _ActivityAdapter(mIconResizer, this);

    SetListAdapter(mAdapter);
    AutoPtr<IListView> listView;
    GetListView((IListView**)&listView);
    listView->SetTextFilterEnabled(TRUE);

    SetProgressBarIndeterminateVisibility(FALSE);
    return NOERROR;
}

ECode CLauncherActivity::OnSetContentView()
{
    SetContentView(0/*com.android.internal.R.layout.activity_list*/);
    return NOERROR;
}

ECode CLauncherActivity::OnListItemClick(
        /* [in] */ IListView* l,
        /* [in] */ IView* v,
        /* [in] */ Int32 position,
        /* [in] */ Int64 id)
{
    AutoPtr<IIntent> intent = IntentForPosition(position);
    StartActivity(intent);
    return NOERROR;
}

AutoPtr<IIntent> CLauncherActivity::IntentForPosition(
    /* [in] */ Int32 position)
{
    ActivityAdapter* adapter = (ActivityAdapter*)(_ActivityAdapter*)(IBaseAdapter*)(IListAdapter*) mAdapter;
    return adapter->IntentForPosition(position);
}

AutoPtr<ILauncherActivityListItem> CLauncherActivity::ItemForPosition(
    /* [in] */ Int32 position)
{
    ActivityAdapter* adapter = (ActivityAdapter*)(_ActivityAdapter*)(IBaseAdapter*)(IListAdapter*) mAdapter;
    return adapter->ItemForPosition(position);
}

AutoPtr<IIntent> CLauncherActivity::GetTargetIntent()
{
    AutoPtr<IIntent> intent;
    CIntent::New((IIntent**)&intent);
    return intent;
}

AutoPtr<IObjectContainer> CLauncherActivity::OnQueryCapsuleManager(
    /* [in] */ IIntent* queryIntent)
{
    AutoPtr<IObjectContainer> container;
    mCapsuleManager->QueryIntentActivities(queryIntent,
            /* no flags */ 0, (IObjectContainer**)&container);
    return container;
}

ECode CLauncherActivity::MakeListItems(
    /* [out] */ IObjectContainer** listItems)
{
    // Load all matching activities and sort correctly
    AutoPtr<IObjectContainer> list = OnQueryCapsuleManager(mIntent);
//    Collections.sort(list, new ResolveInfo.DisplayNameComparator(mPackageManager));

    AutoPtr<IObjectEnumerator> enumerator;
    list->GetObjectEnumerator((IObjectEnumerator**)&enumerator);

    AutoPtr<IObjectContainer> result;
    CObjectContainer::New((IObjectContainer**)&result);
    Boolean hasNext = FALSE;
    while (enumerator->MoveNext(&hasNext), hasNext) {
        AutoPtr<IResolveInfo> resolveInfo;
        enumerator->Current((IInterface**)&resolveInfo);
        AutoPtr<ILauncherActivityListItem> listItem;
        CLauncherActivityListItem::New(mCapsuleManager, resolveInfo, NULL,
                (ILauncherActivityListItem**)&listItem);
        result->Add(listItem);
    }

    *listItems = result;
    return NOERROR;
}