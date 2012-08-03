
#include "view/animation/GridLayoutAnimationController.h"
#include "view/animation/Animation.h"
#include "view/animation/CLinearInterpolator.h"
#include "view/CViewGroupLayoutParams.h"

static Int32 R_Styleable_GridLayoutAnimation[] = {
    0x010101cf, 0x010101d0, 0x010101d1, 0x010101d2
};

GridLayoutAnimationController::GridLayoutAnimationController()
{}

/**
 * Creates a new grid layout animation controller from external resources.
 *
 * @param context the Context the view  group is running in, through which
 *        it can access the resources
 * @param attrs the attributes of the XML tag that is inflating the
 *        layout animation controller
 */
GridLayoutAnimationController::GridLayoutAnimationController(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    Init(context, attrs);
}

/**
 * Creates a new layout animation controller with the specified delays
 * and the specified animation.
 *
 * @param animation the animation to use on each child of the view group
 * @param columnDelay the delay by which each column animation must be offset
 * @param rowDelay the delay by which each row animation must be offset
 */
GridLayoutAnimationController::GridLayoutAnimationController(
    /* [in] */ IAnimation* animation,
    /* [in] */ Float columnDelay,
    /* [in] */ Float rowDelay)
{
    Init(animation, columnDelay, rowDelay);
}

/**
 * Returns the delay by which the children's animation are offset from one
 * column to the other. The delay is expressed as a fraction of the
 * animation duration.
 *
 * @return a fraction of the animation duration
 *
 * @see #setColumnDelay(Float)
 * @see #getRowDelay()
 * @see #setRowDelay(Float)
 */
Float GridLayoutAnimationController::GetColumnDelay()
{
    return mColumnDelay;
}

/**
 * Sets the delay, as a fraction of the animation duration, by which the
 * children's animations are offset from one column to the other.
 *
 * @param columnDelay a fraction of the animation duration
 *
 * @see #getColumnDelay()
 * @see #getRowDelay()
 * @see #setRowDelay(Float)
 */
ECode GridLayoutAnimationController::SetColumnDelay(
    /* [in] */ Float columnDelay)
{
    mColumnDelay = columnDelay;

    return NOERROR;
}

/**
 * Returns the delay by which the children's animation are offset from one
 * row to the other. The delay is expressed as a fraction of the
 * animation duration.
 *
 * @return a fraction of the animation duration
 *
 * @see #setRowDelay(Float)
 * @see #getColumnDelay()
 * @see #setColumnDelay(Float)
 */
Float GridLayoutAnimationController::GetRowDelay()
{
    return mRowDelay;
}

/**
 * Sets the delay, as a fraction of the animation duration, by which the
 * children's animations are offset from one row to the other.
 *
 * @param rowDelay a fraction of the animation duration
 *
 * @see #getRowDelay()
 * @see #getColumnDelay()
 * @see #setColumnDelay(Float) 
 */
ECode GridLayoutAnimationController::SetRowDelay(
    /* [in] */ Float rowDelay)
{
    mRowDelay = rowDelay;
    return NOERROR;
}

/**
 * Returns the direction of the animation. {@link #GridLayoutAnimationController_DIRECTION_HORIZONTAL_MASK}
 * and {@link #DIRECTION_VERTICAL_MASK} can be used to retrieve the
 * horizontal and vertical components of the direction.
 *
 * @return the direction of the animation
 *
 * @see #setDirection(Int32)
 * @see #DIRECTION_BOTTOM_TO_TOP
 * @see #DIRECTION_TOP_TO_BOTTOM
 * @see #DIRECTION_LEFT_TO_RIGHT
 * @see #GridLayoutAnimationController_DIRECTION_RIGHT_TO_LEFT
 * @see #GridLayoutAnimationController_DIRECTION_HORIZONTAL_MASK
 * @see #DIRECTION_VERTICAL_MASK
 */
Int32 GridLayoutAnimationController::GetDirection()
{
    return mDirection;
}

/**
 * Sets the direction of the animation. The direction is expressed as an
 * integer containing a horizontal and vertical component. For instance,
 * <code>DIRECTION_BOTTOM_TO_TOP | GridLayoutAnimationController_DIRECTION_RIGHT_TO_LEFT</code>.
 *
 * @param direction the direction of the animation
 *
 * @see #getDirection()
 * @see #DIRECTION_BOTTOM_TO_TOP
 * @see #DIRECTION_TOP_TO_BOTTOM
 * @see #DIRECTION_LEFT_TO_RIGHT
 * @see #GridLayoutAnimationController_DIRECTION_RIGHT_TO_LEFT
 * @see #GridLayoutAnimationController_DIRECTION_HORIZONTAL_MASK
 * @see #DIRECTION_VERTICAL_MASK
 */
ECode GridLayoutAnimationController::SetDirection(
    /* [in] */ Int32 direction)
{
    mDirection = direction;
    return NOERROR;
}

/**
 * Returns the direction priority for the animation. The priority can
 * be either {@link #GridLayoutAnimationController_PRIORITY_NONE}, {@link #GridLayoutAnimationController_PRIORITY_COLUMN} or
 * {@link #GridLayoutAnimationController_PRIORITY_ROW}.
 *
 * @return the priority of the animation direction
 *
 * @see #setDirectionPriority(Int32)
 * @see #GridLayoutAnimationController_PRIORITY_COLUMN
 * @see #GridLayoutAnimationController_PRIORITY_NONE
 * @see #GridLayoutAnimationController_PRIORITY_ROW
 */
Int32 GridLayoutAnimationController::GetDirectionPriority()
{
    return mDirectionPriority;
}

/**
 * Specifies the direction priority of the animation. For instance,
 * {@link #GridLayoutAnimationController_PRIORITY_COLUMN} will give priority to columns: the animation
 * will first play on the column, then on the rows.Z
 *
 * @param directionPriority the direction priority of the animation
 *
 * @see #getDirectionPriority()
 * @see #GridLayoutAnimationController_PRIORITY_COLUMN
 * @see #GridLayoutAnimationController_PRIORITY_NONE
 * @see #GridLayoutAnimationController_PRIORITY_ROW
 */
ECode GridLayoutAnimationController::SetDirectionPriority(
    /* [in] */ Int32 directionPriority)
{
    mDirectionPriority = directionPriority;
    return NOERROR;
}

/**
 * {@inheritDoc}
 */
//@Override
Boolean GridLayoutAnimationController::WillOverlap()
{
    return mColumnDelay < 1.0f || mRowDelay < 1.0f;
}

/**
 * {@inheritDoc}
 */
//@Override
Int64 GridLayoutAnimationController::GetDelayForView(
    /* [in] */ IView* view)
{
    AutoPtr<IViewGroupLayoutParams> lp;
    view->GetLayoutParams((IViewGroupLayoutParams**)&lp);
    AnimationParameters* params = (AnimationParameters*)((CViewGroupLayoutParams*)lp.Get())
            ->mLayoutAnimationParameters.Get();

    if (params == NULL) {
        return 0;
    }

    Int32 column = GetTransformedColumnIndex(params);
    Int32 row = GetTransformedRowIndex(params);

    Int32 rowsCount = params->mRowsCount;
    Int32 columnsCount = params->mColumnsCount;

    Int64 duration;
    mAnimation->GetDuration(&duration);
    Float columnDelay = mColumnDelay * duration;
    Float rowDelay = mRowDelay * duration;

    Float totalDelay;
    Int64 viewDelay;

    if (mInterpolator == NULL) {
        ASSERT_SUCCEEDED(CLinearInterpolator::New((IInterpolator**)&mInterpolator));
    }

    switch (mDirectionPriority) {
        case GridLayoutAnimationController_PRIORITY_COLUMN:
            viewDelay = (Int64)(row * rowDelay + column * rowsCount * rowDelay);
            totalDelay = rowsCount * rowDelay + columnsCount * rowsCount * rowDelay;
            break;
        case GridLayoutAnimationController_PRIORITY_ROW:
            viewDelay = (Int64)(column * columnDelay + row * columnsCount * columnDelay);
            totalDelay = columnsCount * columnDelay + rowsCount * columnsCount * columnDelay;
            break;
        case GridLayoutAnimationController_PRIORITY_NONE:
        default:
            viewDelay = (Int64)(column * columnDelay + row * rowDelay);
            totalDelay = columnsCount * columnDelay + rowsCount * rowDelay;
            break;
    }

    Float normalizedDelay = viewDelay / totalDelay;
    mInterpolator->GetInterpolation(normalizedDelay, &normalizedDelay);

    return (Int64)(normalizedDelay * totalDelay);
}

Int32 GridLayoutAnimationController::GetTransformedColumnIndex(
    /* [in] */ AnimationParameters* params)
{
    Int32 index;
    switch (GetOrder()) {
        case LayoutAnimationController_ORDER_REVERSE:
            index = params->mColumnsCount - 1 - params->mColumn;
            break;
        case LayoutAnimationController_ORDER_RANDOM:
            /*if (mRandomizer == null) {
                mRandomizer = new Random();
            }
            index = (Int32) (params.columnsCount * mRandomizer.nextFloat());
            break;*/
        case LayoutAnimationController_ORDER_NORMAL:
        default:
            index = params->mColumn;
            break;
    }

    Int32 direction =
        mDirection & GridLayoutAnimationController_DIRECTION_HORIZONTAL_MASK;
    if (direction == GridLayoutAnimationController_DIRECTION_RIGHT_TO_LEFT) {
        index = params->mColumnsCount - 1 - index;
    }

    return index;
}

Int32 GridLayoutAnimationController::GetTransformedRowIndex(
    /* [in] */ AnimationParameters* params)
{
    Int32 index;
    switch (GetOrder()) {
        case LayoutAnimationController_ORDER_REVERSE:
            index = params->mRowsCount - 1 - params->mRow;
            break;
        case LayoutAnimationController_ORDER_RANDOM:
            /*if (mRandomizer == null) {
                mRandomizer = new Random();
            }
            index = (Int32) (params.rowsCount * mRandomizer.nextFloat());
            break;*/
        case LayoutAnimationController_ORDER_NORMAL:
        default:
            index = params->mRow;
            break;
    }

    Int32 direction = mDirection & GridLayoutAnimationController_DIRECTION_VERTICAL_MASK;
    if (direction == GridLayoutAnimationController_DIRECTION_BOTTOM_TO_TOP) {
        index = params->mRowsCount - 1 - index;
    }

    return index;
}

ECode GridLayoutAnimationController::Init(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    LayoutAnimationController::Init(context, attrs);

    AutoPtr<ITypedArray> a;
    context->ObtainStyledAttributesEx2(
        attrs, ArrayOf<Int32>(R_Styleable_GridLayoutAnimation,
        sizeof(R_Styleable_GridLayoutAnimation) / sizeof(Int32))/*com.android.internal.R.styleable.GridLayoutAnimation*/,
        (ITypedArray**)&a);

    AutoPtr<ITypedValue> value;
    a->PeekValue(
        0/*com.android.internal.R.styleable.GridLayoutAnimation_columnDelay*/,
        (ITypedValue**)&value);

    AutoPtr<Animation::Description> d = Animation::Description::ParseValue(value);
    mColumnDelay = d->mValue;

    a->PeekValue(
        1/*com.android.internal.R.styleable.GridLayoutAnimation_rowDelay*/,
        (ITypedValue**)&value);

    d = Animation::Description::ParseValue(value);
    mRowDelay = d->mValue;

    //noinspection PointlessBitwiseExpression
    a->GetInt32(
        2/*com.android.internal.R.styleable.GridLayoutAnimation_direction*/,
        GridLayoutAnimationController_DIRECTION_LEFT_TO_RIGHT |
        GridLayoutAnimationController_DIRECTION_TOP_TO_BOTTOM,
        &mDirection);

    a->GetInt32(
        3/*com.android.internal.R.styleable.GridLayoutAnimation_directionPriority*/,
        GridLayoutAnimationController_PRIORITY_NONE, &mDirection);

    a->Recycle();

    return NOERROR;
}

ECode GridLayoutAnimationController::Init(
    /* [in] */ IAnimation* animation,
    /* [in] */ Float columnDelay,
    /* [in] */ Float rowDelay)
{
    LayoutAnimationController::Init(animation);
    mColumnDelay = columnDelay;
    mRowDelay = rowDelay;

    return NOERROR;
}
