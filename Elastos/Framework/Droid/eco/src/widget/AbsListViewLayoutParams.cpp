
#include "widget/AbsListViewLayoutParams.h"

AbsListViewLayoutParams::AbsListViewLayoutParams()
    : mViewType(0)
    , mRecycledHeaderFooter(FALSE)
    , mForceAdd(FALSE)
{}

AbsListViewLayoutParams::AbsListViewLayoutParams(
    /* [in] */ IContext* c,
    /* [in] */ IAttributeSet* attrs)
    : mViewType(0)
    , mRecycledHeaderFooter(FALSE)
    , mForceAdd(FALSE)
{
    ASSERT_SUCCEEDED(Init(c, attrs));
}

AbsListViewLayoutParams::AbsListViewLayoutParams(
    /* [in] */ Int32 width,
    /* [in] */ Int32 height)
    : ViewGroupLayoutParams(width, height)
    , mViewType(0)
    , mRecycledHeaderFooter(FALSE)
    , mForceAdd(FALSE)
{}

AbsListViewLayoutParams::AbsListViewLayoutParams(
    /* [in] */ Int32 width,
    /* [in] */ Int32 height,
    /* [in] */ Int32 viewType)
    : ViewGroupLayoutParams(width, height)
    , mViewType(viewType)
    , mRecycledHeaderFooter(FALSE)
    , mForceAdd(FALSE)
{}

AbsListViewLayoutParams::AbsListViewLayoutParams(
    /* [in] */ ViewGroupLayoutParams* source)
    : ViewGroupLayoutParams(source)
    , mViewType(0)
    , mRecycledHeaderFooter(FALSE)
    , mForceAdd(FALSE)
{}

ECode AbsListViewLayoutParams::Init(
    /* [in] */ IContext* c,
    /* [in] */ IAttributeSet* attrs)
{
    return ViewGroupLayoutParams::Init(c, attrs);
}

ECode AbsListViewLayoutParams::Init(
    /* [in] */ Int32 width,
    /* [in] */ Int32 height)
{
    return ViewGroupLayoutParams::Init(width, height);
}

ECode AbsListViewLayoutParams::Init(
    /* [in] */ Int32 width,
    /* [in] */ Int32 height,
    /* [in] */ Int32 viewType)
{
    ViewGroupLayoutParams::Init(width, height);
    mViewType = viewType;

    return NOERROR;
}

ECode AbsListViewLayoutParams::Init(
    /* [in] */ IViewGroupLayoutParams* source)
{
    return ViewGroupLayoutParams::Init(source);
}
