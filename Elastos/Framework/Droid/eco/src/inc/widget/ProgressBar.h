
#include "view/View.h"

/**
 * <p>
 * Visual indicator of progress in some operation.  Displays a bar to the user
 * representing how far the operation has progressed; the application can
 * change the amount of progress (modifying the length of the bar) as it moves
 * forward.  There is also a secondary progress displayable on a progress bar
 * which is useful for displaying intermediate progress, such as the buffer
 * level during a streaming playback progress bar.
 * </p>
 *
 * <p>
 * A progress bar can also be made indeterminate. In indeterminate mode, the
 * progress bar shows a cyclic animation. This mode is used by applications
 * when the length of the task is unknown.
 * </p>
 *
 * <p>The following code example shows how a progress bar can be used from
 * a worker thread to update the user interface to notify the user of progress:
 * </p>
 *
 * <pre class="prettyprint">
 * public class MyActivity extends Activity {
 *     private static final Int32 PROGRESS = 0x1;
 *
 *     private ProgressBar mProgress;
 *     private Int32 mProgressStatus = 0;
 *
 *     private Handler mHandler = new Handler();
 *
 *     protected void onCreate(Bundle icicle) {
 *         super.onCreate(icicle);
 *
 *         setContentView(R.layout.progressbar_activity);
 *
 *         mProgress = (ProgressBar) findViewById(R.id.progress_bar);
 *
 *         // Start lengthy operation in a background thread
 *         new Thread(new Runnable() {
 *             public void run() {
 *                 while (mProgressStatus < 100) {
 *                     mProgressStatus = doWork();
 *
 *                     // Update the progress bar
 *                     mHandler.post(new Runnable() {
 *                         public void run() {
 *                             mProgress.setProgress(mProgressStatus);
 *                         }
 *                     });
 *                 }
 *             }
 *         }).start();
 *     }
 * }
 * </pre>
 *
 * <p><strong>XML attributes</b></strong>
 * <p>
 * See {@link android.R.styleable#ProgressBar ProgressBar Attributes},
 * {@link android.R.styleable#View View Attributes}
 * </p>
 *
 * <p><strong>Styles</b></strong>
 * <p>
 * @attr ref android.R.styleable#Theme_progressBarStyle
 * @attr ref android.R.styleable#Theme_progressBarStyleSmall
 * @attr ref android.R.styleable#Theme_progressBarStyleLarge
 * @attr ref android.R.styleable#Theme_progressBarStyleHorizontal
 * </p>
 */

class ProgressBar : public View
{
private:
    class RefreshProgressRunnable  : public Runnable
    {
    public:
        RefreshProgressRunnable(
            /* [in] */ Int32 id,
            /* [in] */ Int32 progress,
            /* [in] */ Boolean fromUser,
            /* [in] */ ProgressBar* owner);

        CARAPI Run();

        CARAPI_(void) Setup(
            /* [in] */ Int32 id,
            /* [in] */ Int32 progress,
            /* [in] */ Boolean fromUser);

    private:
        Int32 mId;
        Int32 mProgress;
        Boolean mFromUser;

        ProgressBar* mOwner;
    };

public:
    /**
     * Create a new progress bar with range 0...100 and initial progress of 0.
     * @param context the application environment
     */
    ProgressBar(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs = NULL,
        /* [in] */ Int32 defStyle = 0x01010077/*R.attr.progressBarStyle*/);

//    virtual CARAPI_(AutoPtr<IShape>) GetDrawableShape();

    /**
     * <p>Indicate whether this progress bar is in indeterminate mode.</p>
     *
     * @return TRUE if the progress bar is in indeterminate mode
     */
    //synchronized
    virtual CARAPI_(Boolean) IsIndeterminate();

    /**
     * <p>Change the indeterminate mode for this progress bar. In indeterminate
     * mode, the progress is ignored and the progress bar shows an infinite
     * animation instead.</p>
     *
     * If this progress bar's style only supports indeterminate mode (such as the circular
     * progress bars), then this will be ignored.
     *
     * @param indeterminate TRUE to enable the indeterminate mode
     */
    //synchronized
    virtual CARAPI SetIndeterminate(
        /* [in] */ Boolean indeterminate);

    /**
     * <p>Get the drawable used to draw the progress bar in
     * indeterminate mode.</p>
     *
     * @return a {@link android.graphics.drawable.Drawable} instance
     *
     * @see #setIndeterminateDrawable(android.graphics.drawable.Drawable)
     * @see #setIndeterminate(Boolean)
     */
    virtual CARAPI_(AutoPtr<IDrawable>) GetIndeterminateDrawable();

    /**
     * <p>Define the drawable used to draw the progress bar in
     * indeterminate mode.</p>
     *
     * @param d the new drawable
     *
     * @see #getIndeterminateDrawable()
     * @see #setIndeterminate(Boolean)
     */
    virtual CARAPI SetIndeterminateDrawable(
        /* [in] */ IDrawable* d);

    /**
     * <p>Get the drawable used to draw the progress bar in
     * progress mode.</p>
     *
     * @return a {@link android.graphics.drawable.Drawable} instance
     *
     * @see #setProgressDrawable(android.graphics.drawable.Drawable)
     * @see #setIndeterminate(Boolean)
     */
    virtual CARAPI_(AutoPtr<IDrawable>) GetProgressDrawable();

    /**
     * <p>Define the drawable used to draw the progress bar in
     * progress mode.</p>
     *
     * @param d the new drawable
     *
     * @see #getProgressDrawable()
     * @see #setIndeterminate(Boolean)
     */
    virtual CARAPI SetProgressDrawable(
        /* [in] */ IDrawable* d);

    /**
     * @return The drawable currently used to draw the progress bar
     */
    virtual CARAPI_(AutoPtr<IDrawable>) GetCurrentDrawable();

    using View::PostInvalidate;

    //@Override
    CARAPI PostInvalidate();

    virtual CARAPI_(void) OnProgressRefresh(
        /* [in] */ Float scale,
        /* [in] */ Boolean fromUser);

    /**
     * <p>Set the current progress to the specified value. Does not do anything
     * if the progress bar is in indeterminate mode.</p>
     *
     * @param progress the new progress, between 0 and {@link #getMax()}
     *
     * @see #setIndeterminate(Boolean)
     * @see #isIndeterminate()
     * @see #getProgress()
     * @see #incrementProgressBy(Int32)
     */
    //synchronized
    virtual CARAPI SetProgress(
        /* [in] */ Int32 progress);

    //synchronized
    virtual CARAPI_(void) SetProgress(
        /* [in] */ Int32 progress,
        /* [in] */ Boolean fromUser);

    /**
     * <p>
     * Set the current secondary progress to the specified value. Does not do
     * anything if the progress bar is in indeterminate mode.
     * </p>
     *
     * @param secondaryProgress the new secondary progress, between 0 and {@link #getMax()}
     * @see #setIndeterminate(Boolean)
     * @see #isIndeterminate()
     * @see #getSecondaryProgress()
     * @see #incrementSecondaryProgressBy(Int32)
     */
    //synchronized
    virtual CARAPI SetSecondaryProgress(
        /* [in] */ Int32 secondaryProgress);

    /**
     * <p>Get the progress bar's current level of progress. Return 0 when the
     * progress bar is in indeterminate mode.</p>
     *
     * @return the current progress, between 0 and {@link #getMax()}
     *
     * @see #setIndeterminate(Boolean)
     * @see #isIndeterminate()
     * @see #setProgress(Int32)
     * @see #setMax(Int32)
     * @see #getMax()
     */
    //synchronized
    virtual CARAPI_(Int32) GetProgress();

    /**
     * <p>Get the progress bar's current level of secondary progress. Return 0 when the
     * progress bar is in indeterminate mode.</p>
     *
     * @return the current secondary progress, between 0 and {@link #getMax()}
     *
     * @see #setIndeterminate(Boolean)
     * @see #isIndeterminate()
     * @see #setSecondaryProgress(Int32)
     * @see #setMax(Int32)
     * @see #getMax()
     */
    //synchronized
    virtual CARAPI_(Int32) GetSecondaryProgress();

    /**
     * <p>Return the upper limit of this progress bar's range.</p>
     *
     * @return a positive integer
     *
     * @see #setMax(Int32)
     * @see #getProgress()
     * @see #getSecondaryProgress()
     */
    //synchronized
    virtual CARAPI_(Int32) GetMax();

    /**
     * <p>Set the range of the progress bar to 0...<tt>max</tt>.</p>
     *
     * @param max the upper range of this progress bar
     *
     * @see #getMax()
     * @see #setProgress(Int32)
     * @see #setSecondaryProgress(Int32)
     */
    //synchronized
    virtual CARAPI SetMax(
        /* [in] */ Int32 max);

    /**
     * <p>Increase the progress bar's progress by the specified amount.</p>
     *
     * @param diff the amount by which the progress must be increased
     *
     * @see #setProgress(Int32)
     */
    //synchronized
    CARAPI IncrementProgressBy(
        /* [in] */ Int32 diff);

    /**
     * <p>Increase the progress bar's secondary progress by the specified amount.</p>
     *
     * @param diff the amount by which the secondary progress must be increased
     *
     * @see #setSecondaryProgress(Int32)
     */
    //synchronized
    CARAPI IncrementSecondaryProgressBy(
        /* [in] */ Int32 diff);

    using View::StartAnimation;

    /**
     * <p>Start the indeterminate progress animation.</p>
     */
    virtual CARAPI_(void) StartAnimation();

    /**
     * <p>Stop the indeterminate progress animation.</p>
     */
    virtual CARAPI_(void) StopAnimation();

    /**
     * Sets the acceleration curve for the indeterminate animation.
     * The interpolator is loaded as a resource from the specified context.
     *
     * @param context The application environment
     * @param resID The resource identifier of the interpolator to load
     */
    virtual CARAPI SetInterpolator(
        /* [in] */ IContext* context,
        /* [in] */ Int32 resID);

    /**
     * Sets the acceleration curve for the indeterminate animation.
     * Defaults to a linear interpolation.
     *
     * @param interpolator The interpolator which defines the acceleration curve
     */
    virtual CARAPI SetInterpolator(
        /* [in] */ IInterpolator* interpolator);

    /**
     * Gets the acceleration curve type for the indeterminate animation.
     *
     * @return the {@link Interpolator} associated to this animation
     */
    virtual CARAPI_(AutoPtr<IInterpolator>) GetInterpolator();

    virtual CARAPI SetVisibility(
        /* [in] */ Int32 v);

    //@Override
    CARAPI InvalidateDrawable(
            /* [in] */ IDrawable* dr);

    //@Override
    CARAPI_(AutoPtr<IParcelable>) OnSaveInstanceState();

    //@Override
    CARAPI_(void) OnRestoreInstanceState(
        /* [in] */ IParcelable* state);

protected:
    ProgressBar();

    CARAPI Init(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs = NULL,
        /* [in] */ Int32 defStyle = 0x01010077/*R.attr.progressBarStyle*/);

    //@Override
    CARAPI_(Boolean) VerifyDrawable(
            /* [in] */ IDrawable* who);

    //@Override
    CARAPI_(void) OnVisibilityChanged(
        /* [in] */ IView* changedView,
        /* [in] */ Int32 visibility);

    //@Override
    CARAPI_(void) OnSizeChanged(
        /* [in] */ Int32 w,
        /* [in] */ Int32 h,
        /* [in] */ Int32 oldw,
        /* [in] */ Int32 oldh);

    //@Override
    //synchronized
    CARAPI_(void) OnDraw(
        /* [in] */ ICanvas* canvas);

    //@Override
    //synchronized
    CARAPI_(void) OnMeasure(
        /* [in] */ Int32 widthMeasureSpec,
        /* [in] */ Int32 heightMeasureSpec);

    //@Override
    CARAPI DrawableStateChanged();

    //@Override
    CARAPI_(void) OnAttachedToWindow();

    //@Override
    CARAPI_(void) OnDetachedFromWindow();

    CARAPI_(void) SetSecondaryProgressLocked(
        /* [in] */ Int32 secondaryProgress);

    virtual CARAPI_(Mutex*) GetSelfLock() = 0;

private:
    CARAPI InitFromAttributes(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ Int32 defStyle);

    /**
     * Converts a drawable to a tiled version of itself. It will recursively
     * traverse layer and state list drawables.
     */
    CARAPI_(AutoPtr<IDrawable>) Tileify(
        /* [in] */ IDrawable* drawable,
        /* [in] */ Boolean clip);

    /**
     * Convert a AnimationDrawable for use as a barberpole animation.
     * Each frame of the animation is wrapped in a ClipDrawable and
     * given a tiling BitmapShader.
     */
    CARAPI_(AutoPtr<IDrawable>) TileifyIndeterminate(
        /* [in] */ IDrawable* drawable);

    /**
     * <p>
     * Initialize the progress bar's default values:
     * </p>
     * <ul>
     * <li>progress = 0</li>
     * <li>max = 100</li>
     * <li>animation duration = 4000 ms</li>
     * <li>indeterminate = FALSE</li>
     * <li>behavior = repeat</li>
     * </ul>
     */
    CARAPI_(void) InitProgressBar();

    //synchronized
    CARAPI_(void) DoRefreshProgress(
        /* [in] */ Int32 id,
        /* [in] */ Int32 progress,
        /* [in] */ Boolean fromUser);

    //the caller of RefreshProgress already locks
    //so, rename RefreshProgress to RefreshProgressLocked
    CARAPI_(void) RefreshProgressLocked(
        /* [in] */ Int32 id,
        /* [in] */ Int32 progress,
        /* [in] */ Boolean fromUser);

    CARAPI_(void) SetProgressLocked(
            /* [in] */ Int32 progress,
            /* [in] */ Boolean fromUser);

public:
    static const Int32 MAX_LEVEL = 10000;
    static const Int32 ANIMATION_RESOLUTION = 200;

    Int32 mMinWidth;
    Int32 mMaxWidth;
    Int32 mMinHeight;
    Int32 mMaxHeight;

    Int32 mProgress;
    Int32 mSecondaryProgress;
    Int32 mMax;

    Int32 mBehavior;
    Int32 mDuration;
    Boolean mIndeterminate;
    Boolean mOnlyIndeterminate;
    AutoPtr<ITransformation> mTransformation;
//    AutoPtr<IAlphaAnimation> mAnimation;
    AutoPtr<IDrawable> mIndeterminateDrawable;
    AutoPtr<IDrawable> mProgressDrawable;
    AutoPtr<IDrawable> mCurrentDrawable;
    AutoPtr<IBitmap> mSampleTile;
    Boolean mNoInvalidate;
    AutoPtr<IInterpolator> mInterpolator;
    AutoPtr<RefreshProgressRunnable> mRefreshProgressRunnable;
    Int64 mUiThreadId;
    Boolean mShouldStartAnimationDrawable;
    Int64 mLastDrawTime;

    Boolean mInDrawing;
};
