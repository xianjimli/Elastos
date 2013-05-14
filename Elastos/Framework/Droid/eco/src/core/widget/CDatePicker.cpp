
#include "widget/CDatePicker.h"

IVIEW_METHODS_IMPL(CDatePicker, DatePicker, DatePicker);

IVIEWGROUP_METHODS_IMPL(CDatePicker, DatePicker, DatePicker);

IVIEWPARENT_METHODS_IMPL(CDatePicker, DatePicker, DatePicker);

IVIEWMANAGER_METHODS_IMPL(CDatePicker, DatePicker, DatePicker);

IDrawableCallback_METHODS_IMPL(CDatePicker, DatePicker, DatePicker);

IKeyEventCallback_METHODS_IMPL(CDatePicker, DatePicker, DatePicker);

IAccessibilityEventSource_METHODS_IMPL(CDatePicker, DatePicker, DatePicker);

PInterface CDatePicker::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_View) {
        return reinterpret_cast<PInterface>((View*)this);
    }
    return _CDatePicker::Probe(riid);
}

ECode CDatePicker::SetForegroundGravity(
    /* [in] */ Int32 foregroundGravity)
{
    return DatePicker::SetForegroundGravity(foregroundGravity);
}

ECode CDatePicker::SetForeground(
    /* [in] */ IDrawable* drawable)
{
    return DatePicker::SetForeground(drawable);
}

ECode CDatePicker::GetForeground(
    /* [out] */ IDrawable** foreground)
{
    VALIDATE_NOT_NULL(foreground);
    AutoPtr<IDrawable> d = DatePicker::GetForeground();
    *foreground = d.Get();
    if (*foreground) {
        (*foreground)->AddRef();
    }

    return NOERROR;
}

ECode CDatePicker::SetMeasureAllChildren(
    /* [in] */ Boolean measureAll)
{
    return DatePicker::SetMeasureAllChildren(measureAll);
}

ECode CDatePicker::GetConsiderGoneChildrenWhenMeasuring(
    /* [out] */ Boolean* measureAll)
{
    VALIDATE_NOT_NULL(measureAll);
    *measureAll = DatePicker::GetConsiderGoneChildrenWhenMeasuring();

    return NOERROR;
}

ECode CDatePicker::UpdateDate(
    /* [in] */ Int32 year,
    /* [in] */ Int32 monthOfYear,
    /* [in] */ Int32 dayOfMonth)
{
    return DatePicker::UpdateDate(year, monthOfYear, dayOfMonth);
}

ECode CDatePicker::Init(
    /* [in] */ Int32 year,
    /* [in] */ Int32 monthOfYear,
    /* [in] */ Int32 dayOfMonth,
    /* [in] */ IOnDateChangedListener* onDateChangedListener)
{
    // TODO: Add your code here
    return DatePicker::Init(year, monthOfYear, dayOfMonth, onDateChangedListener);
}

ECode CDatePicker::GetYear(
    /* [out] */ Int32* year)
{
    VALIDATE_NOT_NULL(year);

    *year = DatePicker::GetYear();
    return NOERROR;
}

ECode CDatePicker::GetMonth(
    /* [out] */ Int32* month)
{
    VALIDATE_NOT_NULL(month);

    *month = DatePicker::GetMonth();
    return NOERROR;
}

ECode CDatePicker::GetDayOfMonth(
    /* [out] */ Int32* day)
{
    VALIDATE_NOT_NULL(day);

    *day = DatePicker::GetDayOfMonth();
    return NOERROR;
}

ECode CDatePicker::constructor(
    /* [in] */ IContext* context)
{
    return constructor(context, NULL);
}

ECode CDatePicker::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    return constructor(context, attrs, 0);
}

ECode CDatePicker::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
{
    DatePicker::Init(context, attrs, defStyle);
    return NOERROR;
}

