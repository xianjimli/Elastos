
#ifndef __VIEWMACRO_H__
#define __VIEWMACRO_H__

#define IVIEW_METHODS_DECL()                                            \
    CARAPI GetVerticalFadingEdgeLength(                                 \
        /* [out] */ Int32* length);                                     \
                                                                        \
    CARAPI SetFadingEdgeLength(                                         \
        /* [in] */ Int32 length);                                       \
                                                                        \
    CARAPI GetHorizontalFadingEdgeLength(                               \
        /* [out] */ Int32* length);                                     \
                                                                        \
    CARAPI GetVerticalScrollbarWidth(                                   \
        /* [out] */ Int32* width);                                      \
                                                                        \
    CARAPI GetHorizontalScrollbarHeight(                                \
        /* [out] */ Int32* height);                                     \
                                                                        \
    CARAPI SetOnFocusChangeListener(                                    \
        /* [in] */ IViewOnFocusChangeListener* l);                      \
                                                                        \
    CARAPI GetOnFocusChangeListener(                                    \
        /* [out] */ IViewOnFocusChangeListener** l);                    \
                                                                        \
    CARAPI SetOnClickListener(                                          \
        /* [in] */ IViewOnClickListener* l);                            \
                                                                        \
    CARAPI SetOnLongClickListener(                                      \
        /* [in] */ IViewOnLongClickListener* l);                        \
                                                                        \
    CARAPI SetOnCreateContextMenuListener(                              \
        /* [in] */ IViewOnCreateContextMenuListener* l);                \
                                                                        \
    CARAPI PerformClick(                                                \
        /* [out] */ Boolean* result);                                   \
                                                                        \
    CARAPI PerformLongClick(                                            \
        /* [out] */ Boolean* result);                                   \
                                                                        \
    CARAPI ShowContextMenu(                                             \
        /* [out] */ Boolean* result);                                   \
                                                                        \
    CARAPI SetOnKeyListener(                                            \
        /* [in] */ IViewOnKeyListener* l);                              \
                                                                        \
    CARAPI SetOnTouchListener(                                          \
        /* [in] */ IViewOnTouchListener* l);                            \
                                                                        \
    CARAPI RequestRectangleOnScreen(                                    \
        /* [in] */ IRect* rectangle,                                    \
        /* [out] */ Boolean* result);                                   \
                                                                        \
    CARAPI RequestRectangleOnScreenEx(                                  \
        /* [in] */ IRect* rectangle,                                    \
        /* [in] */ Boolean immediate,                                   \
        /* [out] */ Boolean* result);                                   \
                                                                        \
    CARAPI ClearFocus();                                                \
                                                                        \
    CARAPI HasFocus(                                                    \
        /* [out] */ Boolean* hasFocus);                                 \
                                                                        \
    CARAPI HasFocusable(                                                \
        /* [out] */ Boolean* hasFocusable);                             \
                                                                        \
    CARAPI DispatchPopulateAccessibilityEvent(                          \
        /* [in] */ IAccessibilityEvent* event,                          \
        /* [out] */ Boolean* result);                                   \
                                                                        \
    CARAPI GetContentDescription(                                       \
        /* [out, callee] */ ArrayOf<Char8>** description);              \
                                                                        \
    CARAPI SetContentDescription(                                       \
        /* [in] */ const ArrayOf<Char8>& contentDescription);           \
                                                                        \
    CARAPI IsFocused(                                                   \
        /* [out] */ Boolean* focused);                                  \
                                                                        \
    CARAPI FindFocus(                                                   \
        /* [out] */ IView** view);                                      \
                                                                        \
    CARAPI SetScrollContainer(                                          \
        /* [in] */ Boolean isScrollContainer);                          \
                                                                        \
    CARAPI GetDrawingCacheQuality(                                      \
        /* [out] */ Int32* quality);                                    \
                                                                        \
    CARAPI SetDrawingCacheQuality(                                      \
        /* [in] */ Int32 quality);                                      \
                                                                        \
    CARAPI GetKeepScreenOn(                                             \
        /* [out] */ Boolean* result);                                   \
                                                                        \
    CARAPI SetKeepScreenOn(                                             \
        /* [in] */ Boolean keepScreenOn);                               \
                                                                        \
    CARAPI GetNextFocusLeftId(                                          \
        /* [out] */ Int32* nextFocusLeftId);                            \
                                                                        \
    CARAPI SetNextFocusLeftId(                                          \
        /* [in] */ Int32 nextFocusLeftId);                              \
                                                                        \
    CARAPI GetNextFocusRightId(                                         \
        /* [out] */ Int32* nextFocusRightId);                           \
                                                                        \
    CARAPI SetNextFocusRightId(                                         \
        /* [in] */ Int32 nextFocusRightId);                             \
                                                                        \
    CARAPI GetNextFocusUpId(                                            \
        /* [out] */ Int32* nextFocusUpId);                              \
                                                                        \
    CARAPI SetNextFocusUpId(                                            \
        /* [in] */ Int32 nextFocusUpId);                                \
                                                                        \
    CARAPI GetNextFocusDownId(                                          \
        /* [out] */ Int32* nextFocusDownId);                            \
                                                                        \
    CARAPI SetNextFocusDownId(                                          \
        /* [in] */ Int32 nextFocusDownId);                              \
                                                                        \
    CARAPI IsShown(                                                     \
        /* [out] */ Boolean* shown);                                    \
                                                                        \
    CARAPI IsFitsSystemWindowsFlagSet(                                  \
        /* [out] */ Boolean* flag);                                     \
                                                                        \
    CARAPI GetVisibility(                                               \
        /* [out] */ Int32* visibility);                                 \
                                                                        \
    CARAPI SetVisibility(                                               \
        /* [in] */ Int32 visibility);                                   \
                                                                        \
    CARAPI IsEnabled(                                                   \
        /* [out] */ Boolean* enabled);                                  \
                                                                        \
    CARAPI SetEnabled(                                                  \
        /* [in] */ Boolean enabled);                                    \
                                                                        \
    CARAPI SetFocusable(                                                \
        /* [in] */ Boolean focusable);                                  \
                                                                        \
    CARAPI SetFocusableInTouchMode(                                     \
        /* [in] */ Boolean focusableInTouchMode);                       \
                                                                        \
    CARAPI SetSoundEffectsEnabled(                                      \
        /* [in] */ Boolean soundEffectsEnabled);                        \
                                                                        \
    CARAPI IsSoundEffectsEnabled(                                       \
        /* [out] */ Boolean* soundEffectsEnabled);                      \
                                                                        \
    CARAPI SetHapticFeedbackEnabled(                                    \
        /* [in] */ Boolean hapticFeedbackEnabled);                      \
                                                                        \
    CARAPI IsHapticFeedbackEnabled(                                     \
        /* [out] */ Boolean* hapticFeedbackEnabled);                    \
                                                                        \
    CARAPI SetWillNotDraw(                                              \
        /* [in] */ Boolean willNotDraw);                                \
                                                                        \
    CARAPI WillNotDraw(                                                 \
        /* [out] */ Boolean* willNotDraw);                              \
                                                                        \
    CARAPI SetWillNotCacheDrawing(                                      \
        /* [in] */ Boolean willNotCacheDrawing);                        \
                                                                        \
    CARAPI WillNotCacheDrawing(                                         \
        /* [out] */ Boolean* cache);                                    \
                                                                        \
    CARAPI IsClickable(                                                 \
        /* [out] */ Boolean* clickable);                                \
                                                                        \
    CARAPI SetClickable(                                                \
        /* [in] */ Boolean clickable);                                  \
                                                                        \
    CARAPI IsLongClickable(                                             \
        /* [out] */ Boolean* longClickable);                            \
                                                                        \
    CARAPI SetLongClickable(                                            \
        /* [in] */ Boolean longClickable);                              \
                                                                        \
    CARAPI SetPressed(                                                  \
        /* [in] */ Boolean pressed);                                    \
                                                                        \
    CARAPI IsPressed(                                                   \
        /* [out] */ Boolean* pressed);                                  \
                                                                        \
    CARAPI IsSaveEnabled(                                               \
        /* [out] */ Boolean* enabled);                                  \
                                                                        \
    CARAPI SetSaveEnabled(                                              \
        /* [in] */ Boolean enabled);                                    \
                                                                        \
    CARAPI GetFilterTouchesWhenObscured(                                \
        /* [out] */ Boolean* enabled);                                  \
                                                                        \
    CARAPI SetFilterTouchesWhenObscured(                                \
        /* [in] */ Boolean enabled);                                    \
                                                                        \
    CARAPI IsFocusable(                                                 \
        /* [out] */ Boolean* focusable);                                \
                                                                        \
    CARAPI IsFocusableInTouchMode(                                      \
        /* [out] */ Boolean* focusable);                                \
                                                                        \
    CARAPI FocusSearch(                                                 \
        /* [in] */ Int32 direction,                                     \
        /* [out] */ IView** view);                                      \
                                                                        \
    CARAPI DispatchUnhandledMove(                                       \
        /* [in] */ IView* focused,                                      \
        /* [in] */ Int32 direction,                                     \
        /* [out] */ Boolean* result);                                   \
                                                                        \
    CARAPI GetFocusables(                                               \
        /* [in] */ Int32 direction,                                     \
        /* [out] */ IObjectContainer** views);                          \
                                                                        \
    CARAPI AddFocusables(                                               \
        /* [in] */ IObjectContainer* views,                             \
        /* [in] */ Int32 direction);                                    \
                                                                        \
    CARAPI AddFocusablesEx(                                             \
        /* [in] */ IObjectContainer* views,                             \
        /* [in] */ Int32 direction,                                     \
        /* [in] */ Int32 focusableMode);                                \
                                                                        \
    CARAPI GetTouchables(                                               \
        /* [out] */ IObjectContainer** views);                          \
                                                                        \
    CARAPI AddTouchables(                                               \
        /* [in] */ IObjectContainer* views);                            \
                                                                        \
    CARAPI RequestFocus(                                                \
        /* [out] */ Boolean* result);                                   \
                                                                        \
    CARAPI RequestFocusEx(                                              \
        /* [in] */ Int32 direction,                                     \
        /* [out] */ Boolean* result);                                   \
                                                                        \
    CARAPI RequestFocusEx2(                                             \
        /* [in] */ Int32 direction,                                     \
        /* [in] */ IRect* previouslyFocusedRect,                        \
        /* [out] */ Boolean* result);                                   \
                                                                        \
    CARAPI RequestFocusFromTouch(                                       \
        /* [out] */ Boolean* result);                                   \
                                                                        \
    CARAPI DispatchStartTemporaryDetach();                              \
                                                                        \
    CARAPI OnStartTemporaryDetach();                                    \
                                                                        \
    CARAPI DispatchFinishTemporaryDetach();                             \
                                                                        \
    CARAPI OnFinishTemporaryDetach();                                   \
                                                                        \
    CARAPI GetKeyDispatcherState(                                       \
        /* [out] */ IDispatcherState** state);                          \
                                                                        \
    CARAPI DispatchKeyEventPreIme(                                      \
        /* [in] */ IKeyEvent* event,                                    \
        /* [out] */ Boolean* result);                                   \
                                                                        \
    CARAPI DispatchKeyEvent(                                            \
        /* [in] */ IKeyEvent* event,                                    \
        /* [out] */ Boolean* result);                                   \
                                                                        \
    CARAPI DispatchKeyShortcutEvent(                                    \
        /* [in] */ IKeyEvent* event,                                    \
        /* [out] */ Boolean* result);                                   \
                                                                        \
    CARAPI DispatchTouchEvent(                                          \
        /* [in] */ IMotionEvent* event,                                 \
        /* [out] */ Boolean* result);                                   \
                                                                        \
    CARAPI OnFilterTouchEventForSecurity(                               \
        /* [in] */ IMotionEvent* event,                                 \
        /* [out] */ Boolean* result);                                   \
                                                                        \
    CARAPI DispatchTrackballEvent(                                      \
        /* [in] */ IMotionEvent* event,                                 \
        /* [out] */ Boolean* result);                                   \
                                                                        \
    CARAPI DispatchWindowFocusChanged(                                  \
        /* [in] */ Boolean hasFocus);                                   \
                                                                        \
    CARAPI OnWindowFocusChanged(                                        \
        /* [in] */ Boolean hasWindowFocus);                             \
                                                                        \
    CARAPI HasWindowFocus(                                              \
        /* [out] */ Boolean* focus);                                    \
                                                                        \
    CARAPI DispatchDisplayHint(                                         \
        /* [in] */ Int32 hint);                                         \
                                                                        \
    CARAPI DispatchWindowVisibilityChanged(                             \
        /* [in] */ Int32 visibility);                                   \
                                                                        \
    CARAPI GetWindowVisibility(                                         \
        /* [out] */ Int32* visibility);                                 \
                                                                        \
    CARAPI GetWindowVisibleDisplayFrame(                                \
        /* [in] */ IRect* outRect);                                     \
                                                                        \
    CARAPI DispatchConfigurationChanged(                                \
        /* [in] */ IConfiguration* newConfig);                          \
                                                                        \
    CARAPI IsInTouchMode(                                               \
        /* [out] */ Boolean* result);                                   \
                                                                        \
    CARAPI GetContext(                                                  \
        /* [out] */ IContext** context);                              \
                                                                        \
    CARAPI OnKeyPreIme(                                                 \
        /* [in] */ Int32 keyCode,                                       \
        /* [in] */ IKeyEvent* event,                                    \
        /* [out] */ Boolean* result);                                   \
                                                                        \
    CARAPI OnKeyShortcut(                                               \
        /* [in] */ Int32 keyCode,                                       \
        /* [in] */ IKeyEvent* event,                                    \
        /* [out] */ Boolean* result);                                   \
                                                                        \
    CARAPI OnCheckIsTextEditor(                                         \
        /* [out] */ Boolean* result);                                   \
                                                                        \
    CARAPI OnCreateInputConnection(                                     \
        /* [in] */ IEditorInfo* outAttrs,                               \
        /* [out] */ IInputConnection** inputConnection);                \
                                                                        \
    CARAPI CheckInputConnectionProxy(                                   \
        /* [in] */ IView* view,                                         \
        /* [out] */ Boolean* result);                                   \
                                                                        \
    CARAPI CreateContextMenu(                                           \
        /* [in] */ IContextMenu* menu);                                 \
                                                                        \
    CARAPI OnTrackballEvent(                                            \
        /* [in] */ IMotionEvent* event,                                 \
        /* [out] */ Boolean* result);                                   \
                                                                        \
    CARAPI OnTouchEvent(                                                \
        /* [in] */ IMotionEvent* event,                                 \
        /* [out] */ Boolean* result);                                   \
                                                                        \
    CARAPI CancelLongPress();                                           \
                                                                        \
    CARAPI SetTouchDelegate(                                            \
        /* [in] */ ITouchDelegate* touchDelegate);                      \
                                                                        \
    CARAPI GetTouchDelegate(                                            \
        /* [out] */ ITouchDelegate** touchDelegate);                    \
                                                                        \
    CARAPI BringToFront();                                              \
                                                                        \
    CARAPI GetParent(                                                   \
        /* [out] */ IViewParent** parent);                              \
                                                                        \
    CARAPI GetScrollX(                                                  \
        /* [out] */ Int32* x);                                          \
                                                                        \
    CARAPI GetScrollY(                                                  \
        /* [out] */ Int32* y);                                          \
                                                                        \
    CARAPI GetWidth(                                                    \
        /* [out] */ Int32* width);                                      \
                                                                        \
    CARAPI GetHeight(                                                   \
        /* [out] */ Int32* height);                                     \
                                                                        \
    CARAPI GetDrawingRect(                                              \
        /* [in] */ IRect* outRect);                                     \
                                                                        \
    CARAPI GetMeasuredWidth(                                            \
        /* [out] */ Int32* width);                                      \
                                                                        \
    CARAPI GetMeasuredHeight(                                           \
        /* [out] */ Int32* height);                                     \
                                                                        \
    CARAPI GetTop(                                                      \
        /* [out] */ Int32* top);                                        \
                                                                        \
    CARAPI GetBottom(                                                   \
        /* [out] */ Int32* bottom);                                     \
                                                                        \
    CARAPI GetLeft(                                                     \
        /* [out] */ Int32* left);                                       \
                                                                        \
    CARAPI GetRight(                                                    \
        /* [out] */ Int32* right);                                      \
                                                                        \
    CARAPI GetHitRect(                                                  \
        /* [in, out] */ IRect* outRect);                                \
                                                                        \
    CARAPI GetFocusedRect(                                              \
        /* [in, out] */ IRect* outRect);                                \
                                                                        \
    CARAPI GetGlobalVisibleRect(                                        \
        /* [in, out] */ IRect* outRect,                                 \
        /* [in, out] */ IPoint* outGlobalOffset,                        \
        /* [out] */ Boolean* result);                                   \
                                                                        \
    CARAPI GetGlobalVisibleRectEx(                                      \
        /* [in, out] */ IRect* outRect,                                 \
        /* [out] */ Boolean* result);                                   \
                                                                        \
    CARAPI GetLocalVisibleRect(                                         \
        /* [in, out] */ IRect* outRect,                                 \
        /* [out] */ Boolean* result);                                   \
                                                                        \
    CARAPI OffsetTopAndBottom(                                          \
        /* [in] */ Int32 offset);                                       \
                                                                        \
    CARAPI OffsetLeftAndRight(                                          \
        /* [in] */ Int32 offset);                                       \
                                                                        \
    CARAPI GetLayoutParams(                                             \
        /* [out] */ IViewGroupLayoutParams** params);                   \
                                                                        \
    CARAPI SetLayoutParams(                                             \
        /* [in] */ IViewGroupLayoutParams* params);                     \
                                                                        \
    CARAPI ScrollTo(                                                    \
        /* [in] */ Int32 x,                                             \
        /* [in] */ Int32 y);                                            \
                                                                        \
    CARAPI ScrollBy(                                                    \
        /* [in] */ Int32 x,                                             \
        /* [in] */ Int32 y);                                            \
                                                                        \
    CARAPI Invalidate(                                                  \
        /* [in] */ IRect* dirty);                                       \
                                                                        \
    CARAPI InvalidateEx(                                                \
        /* [in] */ Int32 left,                                          \
        /* [in] */ Int32 top,                                           \
        /* [in] */ Int32 right,                                         \
        /* [in] */ Int32 bottom);                                       \
                                                                        \
    CARAPI InvalidateEx2();                                             \
                                                                        \
    CARAPI IsOpaque(                                                    \
        /* [out] */ Boolean* opaque);                                   \
                                                                        \
    CARAPI GetHandler(                                                  \
        /* [out] */ IApartment** apartment);                            \
                                                                        \
    CARAPI PostInvalidate();                                            \
                                                                        \
    CARAPI PostInvalidateEx(                                            \
        /* [in] */ Int32 left,                                          \
        /* [in] */ Int32 top,                                           \
        /* [in] */ Int32 right,                                         \
        /* [in] */ Int32 bottom);                                       \
                                                                        \
    CARAPI PostInvalidateDelayed(                                       \
        /* [in] */ Int64 delayMilliseconds);                            \
                                                                        \
    CARAPI PostInvalidateDelayedEx(                                     \
        /* [in] */ Int64 delayMilliseconds,                             \
        /* [in] */ Int32 left,                                          \
        /* [in] */ Int32 top,                                           \
        /* [in] */ Int32 right,                                         \
        /* [in] */ Int32 bottom);                                       \
                                                                        \
    CARAPI ComputeScroll();                                             \
                                                                        \
    CARAPI IsHorizontalFadingEdgeEnabled(                               \
        /* [out] */ Boolean* horizontalFadingEdgeEnabled);              \
                                                                        \
    CARAPI SetHorizontalFadingEdgeEnabled(                              \
        /* [in] */ Boolean horizontalFadingEdgeEnabled);                \
                                                                        \
    CARAPI IsVerticalFadingEdgeEnabled(                                 \
        /* [out] */ Boolean* verticalFadingEdgeEnabled);                \
                                                                        \
    CARAPI SetVerticalFadingEdgeEnabled(                                \
        /* [in] */ Boolean verticalFadingEdgeEnabled);                  \
                                                                        \
    CARAPI IsHorizontalScrollBarEnabled(                                \
        /* [out] */ Boolean* horizontalScrollBarEnabled);               \
                                                                        \
    CARAPI SetHorizontalScrollBarEnabled(                               \
        /* [in] */ Boolean horizontalScrollBarEnabled);                 \
                                                                        \
    CARAPI IsVerticalScrollBarEnabled(                                  \
        /* [out] */ Boolean* verticalScrollBarEnabled);                 \
                                                                        \
    CARAPI SetVerticalScrollBarEnabled(                                 \
        /* [in] */ Boolean verticalScrollBarEnabled);                   \
                                                                        \
    CARAPI SetScrollbarFadingEnabled(                                   \
        /* [in] */ Boolean fadeScrollbars);                             \
                                                                        \
    CARAPI IsScrollbarFadingEnabled(                                    \
        /* [out] */ Boolean* fadeScrollbars);                           \
                                                                        \
    CARAPI SetScrollBarStyle(                                           \
        /* [in] */ Int32 style);                                        \
                                                                        \
    CARAPI GetScrollBarStyle(                                           \
        /* [out] */ Int32* style);                                      \
                                                                        \
    CARAPI AssignParent(                                                \
        /* [in] */ IViewParent* parent);                                \
                                                                        \
    CARAPI GetWindowToken(                                              \
        /* [out] */ IBinder** token);                                   \
                                                                        \
    CARAPI GetApplicationWindowToken(                                   \
        /* [out] */ IBinder** token);                                   \
                                                                        \
    CARAPI SaveHierarchyState(                                          \
        /* [in, out] */ IObjectIntegerMap* container);                  \
                                                                        \
    CARAPI RestoreHierarchyState(                                       \
        /* [in] */ IObjectIntegerMap* container);                       \
                                                                        \
    CARAPI GetDrawingTime(                                              \
        /* [out] */ Int64* time);                                       \
                                                                        \
    CARAPI SetDuplicateParentStateEnabled(                              \
        /* [in] */ Boolean enabled);                                    \
                                                                        \
    CARAPI IsDuplicateParentStateEnabled(                               \
        /* [out] */ Boolean* result);                                   \
                                                                        \
    CARAPI SetDrawingCacheEnabled(                                      \
        /* [in] */ Boolean enabled);                                    \
                                                                        \
    CARAPI IsDrawingCacheEnabled(                                       \
        /* [out] */ Boolean* enabled);                                  \
                                                                        \
    CARAPI GetDrawingCache(                                             \
        /* [out] */ IBitmap** bitmap);                                \
                                                                        \
    CARAPI GetDrawingCacheEx(                                           \
        /* [in] */ Boolean autoScale,                                   \
        /* [out] */ IBitmap** bitmap);                                \
                                                                        \
    CARAPI DestroyDrawingCache();                                       \
                                                                        \
    CARAPI SetDrawingCacheBackgroundColor(                              \
        /* [in] */ Int32 color);                                        \
                                                                        \
    CARAPI GetDrawingCacheBackgroundColor(                              \
        /* [out] */ Int32* color);                                      \
                                                                        \
    CARAPI BuildDrawingCache();                                         \
                                                                        \
    CARAPI BuildDrawingCacheEx(                                         \
        /* [in] */ Boolean autoScale);                                  \
                                                                        \
    CARAPI CreateSnapshot(                                              \
        /* [in] */ BitmapConfig quality,                              \
        /* [in] */ Int32 backgroundColor,                               \
        /* [in] */ Boolean skipChildren,                                \
        /* [out] */ IBitmap** bitmap);                                \
                                                                        \
    CARAPI IsInEditMode(                                                \
        /* [out] */ Boolean* result);                                   \
                                                                        \
    CARAPI Draw(                                                        \
        /* [in] */ ICanvas* canvas);                                  \
                                                                        \
    CARAPI GetSolidColor(                                               \
        /* [out] */ Int32* color);                                      \
                                                                        \
    CARAPI OnFinishInflate();                                           \
                                                                        \
    CARAPI IsLayoutRequested(                                           \
        /* [out] */ Boolean* result);                                   \
                                                                        \
    CARAPI Layout(                                                      \
        /* [in] */ Int32 l,                                             \
        /* [in] */ Int32 t,                                             \
        /* [in] */ Int32 r,                                             \
        /* [in] */ Int32 b);                                            \
                                                                        \
    CARAPI GetResources(                                                \
        /* [out] */ IResources** resources);                            \
                                                                        \
    CARAPI UnscheduleDrawableEx(                                        \
        /* [in] */ IDrawable* who);                                     \
                                                                        \
    CARAPI RefreshDrawableState();                                      \
                                                                        \
    CARAPI GetDrawableState(                                            \
        /* [out, callee] */ ArrayOf<Int32>** state);                    \
                                                                        \
    CARAPI SetBackgroundColor(                                          \
        /* [in] */ Int32 color);                                        \
                                                                        \
    CARAPI SetBackgroundResource(                                       \
        /* [in] */ Int32 resid);                                        \
                                                                        \
    CARAPI SetBackgroundDrawable(                                       \
        /* [in] */ IDrawable* d);                                       \
                                                                        \
    CARAPI GetBackground(                                               \
        /* [out] */ IDrawable** drawable);                              \
                                                                        \
    CARAPI SetPadding(                                                  \
        /* [in] */ Int32 left,                                          \
        /* [in] */ Int32 top,                                           \
        /* [in] */ Int32 right,                                         \
        /* [in] */ Int32 bottom);                                       \
                                                                        \
    CARAPI GetPaddingTop(                                               \
        /* [out] */ Int32* top);                                        \
                                                                        \
    CARAPI GetPaddingBottom(                                            \
        /* [out] */ Int32* bottom);                                     \
                                                                        \
    CARAPI GetPaddingLeft(                                              \
        /* [out] */ Int32* left);                                       \
                                                                        \
    CARAPI GetPaddingRight(                                             \
        /* [out] */ Int32* right);                                      \
                                                                        \
    CARAPI SetSelected(                                                 \
        /* [in] */ Boolean selected);                                   \
                                                                        \
    CARAPI IsSelected(                                                  \
        /* [out] */ Boolean* selected);                                 \
                                                                        \
    CARAPI GetViewTreeObserver(                                         \
        /* [out] */ IViewTreeObserver** observer);                      \
                                                                        \
    CARAPI GetRootView(                                                 \
        /* [out] */ IView** view);                                      \
                                                                        \
    CARAPI GetLocationOnScreen(                                         \
        /* [out] */ Int32* x,                                           \
        /* [out] */ Int32* y);                                          \
                                                                        \
    CARAPI GetLocationInWindow(                                         \
        /* [out] */ Int32* x,                                           \
        /* [out] */ Int32* y);                                          \
                                                                        \
    CARAPI FindViewById(                                                \
        /* [in] */ Int32 id,                                            \
        /* [out] */ IView** view);                                      \
                                                                        \
    CARAPI SetId(                                                       \
        /* [in] */ Int32 id);                                           \
                                                                        \
    CARAPI GetId(                                                       \
        /* [out] */ Int32* id);                                         \
                                                                        \
    CARAPI IsRootNamespace(                                             \
        /* [out] */ Boolean* result);                                   \
                                                                        \
    CARAPI GetBaseline(                                                 \
        /* [out] */ Int32* result);                                     \
                                                                        \
    CARAPI RequestLayout();                                             \
                                                                        \
    CARAPI ForceLayout();                                               \
                                                                        \
    CARAPI Measure(                                                     \
        /* [in] */ Int32 widthMeasureSpec,                              \
        /* [in] */ Int32 heightMeasureSpec);                            \
                                                                        \
    CARAPI SetMinimumHeight(                                            \
        /* [in] */ Int32 minHeight);                                    \
                                                                        \
    CARAPI SetMinimumWidth(                                             \
        /* [in] */ Int32 minWidth);                                     \
                                                                        \
    CARAPI GetAnimation(                                                \
        /* [out] */ IAnimation** animation);                            \
                                                                        \
    CARAPI StartAnimation(                                              \
        /* [in] */ IAnimation* animation);                              \
                                                                        \
    CARAPI ClearAnimation();                                            \
                                                                        \
    CARAPI SetAnimation(                                                \
        /* [in] */ IAnimation* animation);                              \
                                                                        \
    CARAPI GatherTransparentRegion(                                     \
        /* [in] */ IRegion* region,                                     \
        /* [out] */ Boolean* result);                                   \
                                                                        \
    CARAPI PlaySoundEffect(                                             \
        /* [in] */ Int32 soundConstant);                                \
                                                                        \
    CARAPI PerformHapticFeedback(                                       \
        /* [in] */ Int32 feedbackConstant,                              \
        /* [out] */ Boolean* result);                                   \
                                                                        \
    CARAPI PerformHapticFeedbackEx(                                     \
        /* [in] */ Int32 feedbackConstant,                              \
        /* [in] */ Int32 flags,                                         \
        /* [out] */ Boolean* result);                                   \
                                                                        \
    CARAPI OnCloseSystemDialogs(                                        \
        /* [in] */ const String& reason);                                      \
                                                                        \
    CARAPI ApplyDrawableToTransparentRegion(                            \
        /* [in] */ IDrawable* dr,                                       \
        /* [in] */ IRegion* region);                                    \
                                                                        \
    CARAPI GetOverScrollMode(                                           \
        /* [out] */ Int32* mode);                                       \
                                                                        \
    CARAPI SetOverScrollMode(                                           \
        /* [in] */ Int32 overScrollMode);                               \
                                                                        \
    CARAPI SetTag(                                                      \
        /* [in] */ IInterface* tag);                                    \
                                                                        \
    CARAPI GetTag(                                                      \
        /* [out] */ IInterface** tag);


#define IVIEW_METHODS_IMPL(className, superClass, overRideClass)        \
ECode className::GetVerticalFadingEdgeLength(                           \
    /* [out] */ Int32* length)                                          \
{                                                                       \
    VALIDATE_NOT_NULL(length);                                          \
    *length = superClass::GetVerticalFadingEdgeLength();                \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::SetFadingEdgeLength(                                   \
    /* [in] */ Int32 length)                                            \
{                                                                       \
    return superClass::SetFadingEdgeLength(length);                     \
}                                                                       \
                                                                        \
ECode className::GetHorizontalFadingEdgeLength(                         \
    /* [out] */ Int32* length)                                          \
{                                                                       \
    VALIDATE_NOT_NULL(length);                                          \
    *length = superClass::GetHorizontalFadingEdgeLength();              \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::GetVerticalScrollbarWidth(                             \
    /* [out] */ Int32* width)                                           \
{                                                                       \
    VALIDATE_NOT_NULL(width);                                           \
                                                                        \
    *width = superClass::GetVerticalScrollbarWidth();                   \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::GetHorizontalScrollbarHeight(                          \
    /* [out] */ Int32* height)                                          \
{                                                                       \
    VALIDATE_NOT_NULL(height);                                          \
                                                                        \
    *height = superClass::GetHorizontalScrollbarHeight();               \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::SetOnFocusChangeListener(                              \
    /* [in] */ IViewOnFocusChangeListener* l)                           \
{                                                                       \
    return superClass::SetOnFocusChangeListener(l);                     \
}                                                                       \
                                                                        \
ECode className::GetOnFocusChangeListener(                              \
    /* [out] */ IViewOnFocusChangeListener** l)                         \
{                                                                       \
    return superClass::GetOnFocusChangeListener(l);                     \
}                                                                       \
                                                                        \
ECode className::SetOnClickListener(                                    \
    /* [in] */ IViewOnClickListener* l)                                 \
{                                                                       \
    return superClass::SetOnClickListener(l);                           \
}                                                                       \
                                                                        \
ECode className::SetOnLongClickListener(                                \
    /* [in] */ IViewOnLongClickListener* l)                             \
{                                                                       \
    return superClass::SetOnLongClickListener(l);                       \
}                                                                       \
                                                                        \
ECode className::SetOnCreateContextMenuListener(                        \
    /* [in] */ IViewOnCreateContextMenuListener* l)                     \
{                                                                       \
    return superClass::SetOnCreateContextMenuListener(l);               \
}                                                                       \
                                                                        \
ECode className::PerformClick(                                          \
    /* [out] */ Boolean* result)                                        \
{                                                                       \
    VALIDATE_NOT_NULL(result);                                          \
    *result = superClass::PerformClick();                               \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::PerformLongClick(                                      \
    /* [out] */ Boolean* result)                                        \
{                                                                       \
    VALIDATE_NOT_NULL(result);                                          \
    *result = superClass::PerformLongClick();                           \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::ShowContextMenu(                                       \
    /* [out] */ Boolean* result)                                        \
{                                                                       \
    VALIDATE_NOT_NULL(result);                                          \
    *result = superClass::ShowContextMenu();                            \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::SetOnKeyListener(                                      \
    /* [in] */ IViewOnKeyListener* l)                                   \
{                                                                       \
    return superClass::SetOnKeyListener(l);                             \
}                                                                       \
                                                                        \
ECode className::SetOnTouchListener(                                    \
    /* [in] */ IViewOnTouchListener* l)                                 \
{                                                                       \
    return superClass::SetOnTouchListener(l);                           \
}                                                                       \
                                                                        \
ECode className::RequestRectangleOnScreen(                              \
    /* [in] */ IRect* rectangle,                                        \
    /* [out] */ Boolean* result)                                        \
{                                                                       \
    VALIDATE_NOT_NULL(result);                                          \
    *result = superClass::RequestRectangleOnScreen(rectangle);          \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::RequestRectangleOnScreenEx(                            \
    /* [in] */ IRect* rectangle,                                        \
    /* [in] */ Boolean immediate,                                       \
    /* [out] */ Boolean* result)                                        \
{                                                                       \
    VALIDATE_NOT_NULL(result);                                          \
    *result = superClass::RequestRectangleOnScreen(rectangle, immediate);\
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::ClearFocus()                                           \
{                                                                       \
    return superClass::ClearFocus();                                    \
}                                                                       \
                                                                        \
ECode className::HasFocus(                                              \
    /* [out] */ Boolean* hasFocus)                                      \
{                                                                       \
    VALIDATE_NOT_NULL(hasFocus);                                        \
                                                                        \
    *hasFocus = superClass::HasFocus();                                 \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::HasFocusable(                                          \
    /* [out] */ Boolean* hasFocusable)                                  \
{                                                                       \
    VALIDATE_NOT_NULL(hasFocusable);                                    \
    *hasFocusable = superClass::HasFocusable();                         \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::DispatchPopulateAccessibilityEvent(                    \
    /* [in] */ IAccessibilityEvent* event,                              \
    /* [out] */ Boolean* result)                                        \
{                                                                       \
    VALIDATE_NOT_NULL(result);                                          \
    *result = superClass::DispatchPopulateAccessibilityEvent(event);    \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::GetContentDescription(                                 \
    /* [out, callee] */ ArrayOf<Char8>** description)                   \
{                                                                       \
    VALIDATE_NOT_NULL(description);                                     \
    *description = superClass::GetContentDescription();                 \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::SetContentDescription(                                 \
    /* [in] */ const ArrayOf<Char8>& contentDescription)                \
{                                                                       \
    return superClass::SetContentDescription(contentDescription);       \
}                                                                       \
                                                                        \
ECode className::IsFocused(                                             \
    /* [out] */ Boolean* focused)                                       \
{                                                                       \
    VALIDATE_NOT_NULL(focused);                                         \
    *focused = superClass::IsFocused();                                 \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::FindFocus(                                             \
    /* [out] */ IView** view)                                           \
{                                                                       \
    VALIDATE_NOT_NULL(view);                                            \
    *view = superClass::FindFocus();                                    \
    if (*view) {                                                        \
        (*view)->AddRef();                                              \
    }                                                                   \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::SetScrollContainer(                                    \
    /* [in] */ Boolean isScrollContainer)                               \
{                                                                       \
    return superClass::SetScrollContainer(isScrollContainer);           \
}                                                                       \
                                                                        \
ECode className::GetDrawingCacheQuality(                                \
    /* [out] */ Int32* quality)                                         \
{                                                                       \
    VALIDATE_NOT_NULL(quality);                                         \
    *quality = superClass::GetDrawingCacheQuality();                    \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::SetDrawingCacheQuality(                                \
    /* [in] */ Int32 quality)                                           \
{                                                                       \
    return superClass::SetDrawingCacheQuality(quality);                 \
}                                                                       \
                                                                        \
ECode className::GetKeepScreenOn(                                       \
    /* [out] */ Boolean* result)                                        \
{                                                                       \
    VALIDATE_NOT_NULL(result);                                          \
    *result = superClass::GetKeepScreenOn();                            \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::SetKeepScreenOn(                                       \
    /* [in] */ Boolean keepScreenOn)                                    \
{                                                                       \
    return superClass::SetKeepScreenOn(keepScreenOn);                   \
}                                                                       \
                                                                        \
ECode className::GetNextFocusLeftId(                                    \
    /* [out] */ Int32* nextFocusLeftId)                                 \
{                                                                       \
    VALIDATE_NOT_NULL(nextFocusLeftId);                                 \
    *nextFocusLeftId = superClass::GetNextFocusLeftId();                \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::SetNextFocusLeftId(                                    \
    /* [in] */ Int32 nextFocusLeftId)                                   \
{                                                                       \
    return superClass::SetNextFocusLeftId(nextFocusLeftId);             \
}                                                                       \
                                                                        \
ECode className::GetNextFocusRightId(                                   \
    /* [out] */ Int32* nextFocusRightId)                                \
{                                                                       \
    VALIDATE_NOT_NULL(nextFocusRightId);                                \
    *nextFocusRightId = superClass::GetNextFocusRightId();              \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::SetNextFocusRightId(                                   \
    /* [in] */ Int32 nextFocusRightId)                                  \
{                                                                       \
    return superClass::SetNextFocusRightId(nextFocusRightId);           \
}                                                                       \
                                                                        \
ECode className::GetNextFocusUpId(                                      \
    /* [out] */ Int32* nextFocusUpId)                                   \
{                                                                       \
    VALIDATE_NOT_NULL(nextFocusUpId);                                   \
    *nextFocusUpId = superClass::GetNextFocusUpId();                    \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::SetNextFocusUpId(                                      \
    /* [in] */ Int32 nextFocusUpId)                                     \
{                                                                       \
    return superClass::SetNextFocusUpId(nextFocusUpId);                 \
}                                                                       \
                                                                        \
ECode className::GetNextFocusDownId(                                    \
    /* [out] */ Int32* nextFocusDownId)                                 \
{                                                                       \
    VALIDATE_NOT_NULL(nextFocusDownId);                                 \
    *nextFocusDownId = superClass::GetNextFocusDownId();                \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::SetNextFocusDownId(                                    \
    /* [in] */ Int32 nextFocusDownId)                                   \
{                                                                       \
    return superClass::SetNextFocusDownId(nextFocusDownId);             \
}                                                                       \
                                                                        \
ECode className::IsShown(                                               \
    /* [out] */ Boolean* shown)                                         \
{                                                                       \
    VALIDATE_NOT_NULL(shown);                                           \
    *shown = superClass::IsShown();                                     \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::IsFitsSystemWindowsFlagSet(                            \
    /* [out] */ Boolean* flag)                                          \
{                                                                       \
    VALIDATE_NOT_NULL(flag);                                            \
    *flag = superClass::IsFitsSystemWindowsFlagSet();                   \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::GetVisibility(                                         \
    /* [out] */ Int32* visibility)                                      \
{                                                                       \
    VALIDATE_NOT_NULL(visibility);                                      \
                                                                        \
    *visibility = superClass::GetVisibility();                          \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::SetVisibility(                                         \
    /* [in] */ Int32 visibility)                                        \
{                                                                       \
    return superClass::SetVisibility(visibility);                       \
}                                                                       \
                                                                        \
ECode className::IsEnabled(                                             \
    /* [out] */ Boolean* enabled)                                       \
{                                                                       \
    VALIDATE_NOT_NULL(enabled);                                         \
    *enabled = superClass::IsEnabled();                                 \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::SetEnabled(                                            \
    /* [in] */ Boolean enabled)                                         \
{                                                                       \
    return superClass::SetEnabled(enabled);                             \
}                                                                       \
                                                                        \
ECode className::SetFocusable(                                          \
    /* [in] */ Boolean focusable)                                       \
{                                                                       \
    return superClass::SetFocusable(focusable);                         \
}                                                                       \
                                                                        \
ECode className::SetFocusableInTouchMode(                               \
    /* [in] */ Boolean focusableInTouchMode)                            \
{                                                                       \
    return superClass::SetFocusableInTouchMode(focusableInTouchMode);   \
}                                                                       \
                                                                        \
ECode className::SetSoundEffectsEnabled(                                \
    /* [in] */ Boolean soundEffectsEnabled)                             \
{                                                                       \
    return superClass::SetSoundEffectsEnabled(soundEffectsEnabled);     \
}                                                                       \
                                                                        \
ECode className::IsSoundEffectsEnabled(                                 \
    /* [out] */ Boolean* soundEffectsEnabled)                           \
{                                                                       \
    VALIDATE_NOT_NULL(soundEffectsEnabled);                             \
    *soundEffectsEnabled = superClass::IsSoundEffectsEnabled();         \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::SetHapticFeedbackEnabled(                              \
    /* [in] */ Boolean hapticFeedbackEnabled)                           \
{                                                                       \
    return superClass::SetHapticFeedbackEnabled(hapticFeedbackEnabled); \
}                                                                       \
                                                                        \
ECode className::IsHapticFeedbackEnabled(                               \
    /* [out] */ Boolean* hapticFeedbackEnabled)                         \
{                                                                       \
    VALIDATE_NOT_NULL(hapticFeedbackEnabled);                           \
    *hapticFeedbackEnabled = superClass::IsHapticFeedbackEnabled();     \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::SetWillNotDraw(                                        \
    /* [in] */ Boolean willNotDraw)                                     \
{                                                                       \
    return superClass::SetWillNotDraw(willNotDraw);                     \
}                                                                       \
                                                                        \
ECode className::WillNotDraw(                                           \
    /* [out] */ Boolean* willNotDraw)                                   \
{                                                                       \
    VALIDATE_NOT_NULL(willNotDraw);                                     \
    *willNotDraw = superClass::WillNotDraw();                           \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::SetWillNotCacheDrawing(                                \
    /* [in] */ Boolean willNotCacheDrawing)                             \
{                                                                       \
    return superClass::SetWillNotCacheDrawing(willNotCacheDrawing);     \
}                                                                       \
                                                                        \
ECode className::WillNotCacheDrawing(                                   \
    /* [out] */ Boolean* cache)                                         \
{                                                                       \
    VALIDATE_NOT_NULL(cache);                                           \
    *cache = superClass::WillNotCacheDrawing();                         \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::IsClickable(                                           \
    /* [out] */ Boolean* clickable)                                     \
{                                                                       \
    VALIDATE_NOT_NULL(clickable);                                       \
    *clickable = superClass::IsClickable();                             \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::SetClickable(                                          \
    /* [in] */ Boolean clickable)                                       \
{                                                                       \
    return superClass::SetClickable(clickable);                         \
}                                                                       \
                                                                        \
ECode className::IsLongClickable(                                       \
    /* [out] */ Boolean* longClickable)                                 \
{                                                                       \
    VALIDATE_NOT_NULL(longClickable);                                   \
    *longClickable = superClass::IsLongClickable();                     \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::SetLongClickable(                                      \
    /* [in] */ Boolean longClickable)                                   \
{                                                                       \
    return superClass::SetLongClickable(longClickable);                 \
}                                                                       \
                                                                        \
ECode className::SetPressed(                                            \
    /* [in] */ Boolean pressed)                                         \
{                                                                       \
    return superClass::SetPressed(pressed);                             \
}                                                                       \
                                                                        \
ECode className::IsPressed(                                             \
    /* [out] */ Boolean* pressed)                                       \
{                                                                       \
    VALIDATE_NOT_NULL(pressed);                                         \
    *pressed = superClass::IsPressed();                                 \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::IsSaveEnabled(                                         \
    /* [out] */ Boolean* enabled)                                       \
{                                                                       \
    VALIDATE_NOT_NULL(enabled);                                         \
    *enabled = superClass::IsSaveEnabled();                             \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::SetSaveEnabled(                                        \
    /* [in] */ Boolean enabled)                                         \
{                                                                       \
    return superClass::SetSaveEnabled(enabled);                         \
}                                                                       \
                                                                        \
ECode className::GetFilterTouchesWhenObscured(                          \
    /* [out] */ Boolean* enabled)                                       \
{                                                                       \
    VALIDATE_NOT_NULL(enabled);                                         \
    *enabled = superClass::GetFilterTouchesWhenObscured();              \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::SetFilterTouchesWhenObscured(                          \
    /* [in] */ Boolean enabled)                                         \
{                                                                       \
    return superClass::SetFilterTouchesWhenObscured(enabled);           \
}                                                                       \
                                                                        \
ECode className::IsFocusable(                                           \
    /* [out] */ Boolean* focusable)                                     \
{                                                                       \
    VALIDATE_NOT_NULL(focusable);                                       \
    *focusable = superClass::IsFocusable();                             \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::IsFocusableInTouchMode(                                \
    /* [out] */ Boolean* focusable)                                     \
{                                                                       \
    VALIDATE_NOT_NULL(focusable);                                       \
    *focusable = superClass::IsFocusableInTouchMode();                  \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::FocusSearch(                                           \
    /* [in] */ Int32 direction,                                         \
    /* [out] */ IView** view)                                           \
{                                                                       \
    VALIDATE_NOT_NULL(view);                                            \
    *view = View::FocusSearch(direction);                               \
    if (*view) {                                                        \
        (*view)->AddRef();                                              \
    }                                                                   \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::DispatchUnhandledMove(                                 \
    /* [in] */ IView* focused,                                          \
    /* [in] */ Int32 direction,                                         \
    /* [out] */ Boolean* result)                                        \
{                                                                       \
    VALIDATE_NOT_NULL(result);                                          \
    *result = superClass::DispatchUnhandledMove(focused, direction);    \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::GetFocusables(                                         \
    /* [in] */ Int32 direction,                                         \
    /* [out] */ IObjectContainer** views)                               \
{                                                                       \
    VALIDATE_NOT_NULL(views);                                           \
    return superClass::GetFocusables(direction, views);                 \
}                                                                       \
                                                                        \
ECode className::AddFocusables(                                         \
    /* [in] */ IObjectContainer* views,                                 \
    /* [in] */ Int32 direction)                                         \
{                                                                       \
    return superClass::AddFocusables(views, direction);                 \
}                                                                       \
                                                                        \
ECode className::AddFocusablesEx(                                       \
    /* [in] */ IObjectContainer* views,                                 \
    /* [in] */ Int32 direction,                                         \
    /* [in] */ Int32 focusableMode)                                     \
{                                                                       \
    return superClass::AddFocusables(views, direction, focusableMode);  \
}                                                                       \
                                                                        \
ECode className::GetTouchables(                                         \
    /* [out] */ IObjectContainer** views)                               \
{                                                                       \
    VALIDATE_NOT_NULL(views);                                           \
    return superClass::GetTouchables(views);                            \
}                                                                       \
                                                                        \
ECode className::AddTouchables(                                         \
    /* [in] */ IObjectContainer* views)                                 \
{                                                                       \
    return superClass::AddTouchables(views);                            \
}                                                                       \
                                                                        \
ECode className::RequestFocus(                                          \
    /* [out] */ Boolean* result)                                        \
{                                                                       \
    VALIDATE_NOT_NULL(result);                                          \
    *result = View::RequestFocus();                                     \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::RequestFocusEx(                                        \
    /* [in] */ Int32 direction,                                         \
    /* [out] */ Boolean* result)                                        \
{                                                                       \
    VALIDATE_NOT_NULL(result);                                          \
    *result = View::RequestFocus(direction);                            \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::RequestFocusEx2(                                       \
    /* [in] */ Int32 direction,                                         \
    /* [in] */ IRect* previouslyFocusedRect,                            \
    /* [out] */ Boolean* result)                                        \
{                                                                       \
    VALIDATE_NOT_NULL(result);                                          \
    *result = superClass::RequestFocus(direction, previouslyFocusedRect);\
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::RequestFocusFromTouch(                                 \
    /* [out] */ Boolean* result)                                        \
{                                                                       \
    VALIDATE_NOT_NULL(result);                                          \
    *result = superClass::RequestFocusFromTouch();                      \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::DispatchStartTemporaryDetach()                         \
{                                                                       \
    return superClass::DispatchStartTemporaryDetach();                  \
}                                                                       \
                                                                        \
ECode className::OnStartTemporaryDetach()                               \
{                                                                       \
    return superClass::OnStartTemporaryDetach();                        \
}                                                                       \
                                                                        \
ECode className::DispatchFinishTemporaryDetach()                        \
{                                                                       \
    return superClass::DispatchFinishTemporaryDetach();                 \
}                                                                       \
                                                                        \
ECode className::OnFinishTemporaryDetach()                              \
{                                                                       \
    return superClass::OnFinishTemporaryDetach();                       \
}                                                                       \
                                                                        \
ECode className::GetKeyDispatcherState(                                 \
    /* [out] */ IDispatcherState** state)                               \
{                                                                       \
    return superClass::GetKeyDispatcherState(state);                    \
}                                                                       \
                                                                        \
ECode className::DispatchKeyEventPreIme(                                \
    /* [in] */ IKeyEvent* event,                                        \
    /* [out] */ Boolean* result)                                        \
{                                                                       \
    VALIDATE_NOT_NULL(result);                                          \
    *result = superClass::DispatchKeyEventPreIme(event);                \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::DispatchKeyEvent(                                      \
    /* [in] */ IKeyEvent* event,                                        \
    /* [out] */ Boolean* result)                                        \
{                                                                       \
    VALIDATE_NOT_NULL(result);                                          \
    *result = overRideClass::DispatchKeyEvent(event);                   \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::DispatchKeyShortcutEvent(                              \
    /* [in] */ IKeyEvent* event,                                        \
    /* [out] */ Boolean* result)                                        \
{                                                                       \
    VALIDATE_NOT_NULL(result);                                          \
    *result = superClass::DispatchKeyShortcutEvent(event);              \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::DispatchTouchEvent(                                    \
    /* [in] */ IMotionEvent* event,                                     \
    /* [out] */ Boolean* result)                                        \
{                                                                       \
    VALIDATE_NOT_NULL(result);                                          \
    *result = overRideClass::DispatchTouchEvent(event);                 \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::OnFilterTouchEventForSecurity(                         \
    /* [in] */ IMotionEvent* event,                                     \
    /* [out] */ Boolean* result)                                        \
{                                                                       \
    VALIDATE_NOT_NULL(result);                                          \
    *result = superClass::OnFilterTouchEventForSecurity(event);         \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::DispatchTrackballEvent(                                \
    /* [in] */ IMotionEvent* event,                                     \
    /* [out] */ Boolean* result)                                        \
{                                                                       \
    VALIDATE_NOT_NULL(result);                                          \
    *result = overRideClass::DispatchTrackballEvent(event);             \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::DispatchWindowFocusChanged(                            \
    /* [in] */ Boolean hasFocus)                                        \
{                                                                       \
    return superClass::DispatchWindowFocusChanged(hasFocus);            \
}                                                                       \
                                                                        \
ECode className::OnWindowFocusChanged(                                  \
    /* [in] */ Boolean hasWindowFocus)                                  \
{                                                                       \
    return superClass::OnWindowFocusChanged(hasWindowFocus);            \
}                                                                       \
                                                                        \
ECode className::HasWindowFocus(                                        \
    /* [out] */ Boolean* focus)                                         \
{                                                                       \
    VALIDATE_NOT_NULL(focus);                                           \
    *focus = superClass::HasWindowFocus();                              \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::DispatchDisplayHint(                                   \
    /* [in] */ Int32 hint)                                              \
{                                                                       \
    return superClass::DispatchDisplayHint(hint);                       \
}                                                                       \
                                                                        \
ECode className::DispatchWindowVisibilityChanged(                       \
    /* [in] */ Int32 visibility)                                        \
{                                                                       \
    return superClass::DispatchWindowVisibilityChanged(visibility);     \
}                                                                       \
                                                                        \
ECode className::GetWindowVisibility(                                   \
    /* [out] */ Int32* visibility)                                      \
{                                                                       \
    VALIDATE_NOT_NULL(visibility);                                      \
    *visibility = superClass::GetWindowVisibility();                    \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::GetWindowVisibleDisplayFrame(                          \
    /* [in] */ IRect* outRect)                                          \
{                                                                       \
    return superClass::GetWindowVisibleDisplayFrame(outRect);           \
}                                                                       \
                                                                        \
ECode className::DispatchConfigurationChanged(                          \
    /* [in] */ IConfiguration* newConfig)                               \
{                                                                       \
    return superClass::DispatchConfigurationChanged(newConfig);         \
}                                                                       \
                                                                        \
ECode className::IsInTouchMode(                                         \
    /* [out] */ Boolean* result)                                        \
{                                                                       \
    VALIDATE_NOT_NULL(result);                                          \
    *result = superClass::IsInTouchMode();                              \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::GetContext(                                            \
    /* [out] */ IContext** context)                                   \
{                                                                       \
    VALIDATE_NOT_NULL(context);                                         \
                                                                        \
    *context = superClass::GetContext();                                \
    if (*context != NULL) {                                             \
        (*context)->AddRef();                                           \
    }                                                                   \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::OnKeyPreIme(                                           \
    /* [in] */ Int32 keyCode,                                           \
    /* [in] */ IKeyEvent* event,                                        \
    /* [out] */ Boolean* result)                                        \
{                                                                       \
    VALIDATE_NOT_NULL(result);                                          \
    *result = superClass::OnKeyPreIme(keyCode, event);                  \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::OnKeyShortcut(                                         \
    /* [in] */ Int32 keyCode,                                           \
    /* [in] */ IKeyEvent* event,                                        \
    /* [out] */ Boolean* result)                                        \
{                                                                       \
    VALIDATE_NOT_NULL(result);                                          \
    *result = superClass::OnKeyPreIme(keyCode, event);                  \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::OnCheckIsTextEditor(                                   \
    /* [out] */ Boolean* result)                                        \
{                                                                       \
    VALIDATE_NOT_NULL(result);                                          \
    *result = superClass::OnCheckIsTextEditor();                        \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::OnCreateInputConnection(                               \
    /* [in] */ IEditorInfo* outAttrs,                                   \
    /* [out] */ IInputConnection** inputConnection)                     \
{                                                                       \
    VALIDATE_NOT_NULL(inputConnection);                                 \
    AutoPtr<IInputConnection> ic = superClass::OnCreateInputConnection(outAttrs);   \
    *inputConnection = ic.Get(); \
    if (*inputConnection != NULL) (*inputConnection)->AddRef(); \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::CheckInputConnectionProxy(                             \
    /* [in] */ IView* view,                                             \
    /* [out] */ Boolean* result)                                        \
{                                                                       \
    VALIDATE_NOT_NULL(result);                                          \
    *result = superClass::CheckInputConnectionProxy(view);              \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::CreateContextMenu(                                     \
    /* [in] */ IContextMenu* menu)                                      \
{                                                                       \
    return superClass::CreateContextMenu(menu);                         \
}                                                                       \
                                                                        \
ECode className::OnTrackballEvent(                                      \
    /* [in] */ IMotionEvent* event,                                     \
    /* [out] */ Boolean* result)                                        \
{                                                                       \
    VALIDATE_NOT_NULL(result);                                          \
    *result = superClass::OnTrackballEvent(event);                      \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::OnTouchEvent(                                          \
    /* [in] */ IMotionEvent* event,                                     \
    /* [out] */ Boolean* result)                                        \
{                                                                       \
    VALIDATE_NOT_NULL(result);                                          \
    *result = overRideClass::OnTouchEvent(event);                       \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::CancelLongPress()                                      \
{                                                                       \
    return superClass::CancelLongPress();                               \
}                                                                       \
                                                                        \
ECode className::SetTouchDelegate(                                      \
    /* [in] */ ITouchDelegate* touchDelegate)                           \
{                                                                       \
    return superClass::SetTouchDelegate(touchDelegate);                 \
}                                                                       \
                                                                        \
ECode className::GetTouchDelegate(                                      \
    /* [out] */ ITouchDelegate** touchDelegate)                         \
{                                                                       \
    return superClass::GetTouchDelegate(touchDelegate);                 \
}                                                                       \
                                                                        \
ECode className::BringToFront()                                         \
{                                                                       \
    return superClass::BringToFront();                                  \
}                                                                       \
                                                                        \
ECode className::GetParent(                                             \
    /* [out] */ IViewParent** parent)                                   \
{                                                                       \
    VALIDATE_NOT_NULL(parent);                                          \
                                                                        \
    *parent = superClass::GetParent();                                  \
    if (*parent != NULL) {                                              \
        (*parent)->AddRef();                                            \
    }                                                                   \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::GetScrollX(                                            \
    /* [out] */ Int32* x)                                               \
{                                                                       \
    VALIDATE_NOT_NULL(x);                                               \
    *x = superClass::GetScrollX();                                      \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::GetScrollY(                                            \
    /* [out] */ Int32* y)                                               \
{                                                                       \
    VALIDATE_NOT_NULL(y);                                               \
    *y = superClass::GetScrollY();                                      \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::GetWidth(                                              \
    /* [out] */ Int32* width)                                           \
{                                                                       \
    VALIDATE_NOT_NULL(width);                                           \
    *width = superClass::GetWidth();                                    \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::GetHeight(                                             \
    /* [out] */ Int32* height)                                          \
{                                                                       \
    VALIDATE_NOT_NULL(height);                                          \
    *height = superClass::GetHeight();                                  \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::GetDrawingRect(                                        \
    /* [in] */ IRect* outRect)                                          \
{                                                                       \
    return superClass::GetDrawingRect(outRect);                         \
}                                                                       \
                                                                        \
ECode className::GetMeasuredWidth(                                      \
    /* [out] */ Int32* width)                                           \
{                                                                       \
    VALIDATE_NOT_NULL(width);                                           \
    *width = superClass::GetMeasuredWidth();                            \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::GetMeasuredHeight(                                     \
    /* [out] */ Int32* height)                                          \
{                                                                       \
    VALIDATE_NOT_NULL(height);                                          \
    *height = superClass::GetMeasuredHeight();                          \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::GetTop(                                                \
    /* [out] */ Int32* top)                                             \
{                                                                       \
    VALIDATE_NOT_NULL(top);                                             \
    *top = superClass::GetTop();                                        \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::GetBottom(                                             \
    /* [out] */ Int32* bottom)                                          \
{                                                                       \
    VALIDATE_NOT_NULL(bottom);                                          \
    *bottom = superClass::GetBottom();                                  \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::GetLeft(                                               \
    /* [out] */ Int32* left)                                            \
{                                                                       \
    VALIDATE_NOT_NULL(left);                                            \
    *left = superClass::GetLeft();                                      \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::GetRight(                                              \
    /* [out] */ Int32* right)                                           \
{                                                                       \
    VALIDATE_NOT_NULL(right);                                           \
    *right = superClass::GetRight();                                    \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::GetHitRect(                                            \
    /* [in, out] */ IRect* outRect)                                     \
{                                                                       \
    return superClass::GetHitRect(outRect);                             \
}                                                                       \
                                                                        \
ECode className::GetFocusedRect(                                        \
    /* [in, out] */ IRect* outRect)                                     \
{                                                                       \
    return superClass::GetFocusedRect(outRect);                         \
}                                                                       \
                                                                        \
ECode className::GetGlobalVisibleRect(                                  \
    /* [in, out] */ IRect* outRect,                                     \
    /* [in, out] */ IPoint* outGlobalOffset,                            \
    /* [out] */ Boolean* result)                                        \
{                                                                       \
    VALIDATE_NOT_NULL(result);                                          \
    *result = superClass::GetGlobalVisibleRect(outRect, outGlobalOffset);\
                                                                        \
    return NOERROR;;                                                    \
}                                                                       \
                                                                        \
ECode className::GetGlobalVisibleRectEx(                                \
    /* [in, out] */ IRect* outRect,                                     \
    /* [out] */ Boolean* result)                                        \
{                                                                       \
    VALIDATE_NOT_NULL(result);                                          \
    *result = superClass::GetGlobalVisibleRect(outRect);                \
                                                                        \
    return NOERROR;;                                                    \
}                                                                       \
                                                                        \
ECode className::GetLocalVisibleRect(                                   \
    /* [in, out] */ IRect* outRect,                                     \
    /* [out] */ Boolean* result)                                        \
{                                                                       \
    VALIDATE_NOT_NULL(result);                                          \
    *result = superClass::GetLocalVisibleRect(outRect);                 \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::OffsetTopAndBottom(                                    \
    /* [in] */ Int32 offset)                                            \
{                                                                       \
    return superClass::OffsetTopAndBottom(offset);                      \
}                                                                       \
                                                                        \
ECode className::OffsetLeftAndRight(                                    \
    /* [in] */ Int32 offset)                                            \
{                                                                       \
    return superClass::OffsetLeftAndRight(offset);                      \
}                                                                       \
                                                                        \
ECode className::GetLayoutParams(                                       \
    /* [out] */ IViewGroupLayoutParams** params)                        \
{                                                                       \
    VALIDATE_NOT_NULL(params);                                          \
    *params = superClass::GetLayoutParams();                            \
                                                                        \
    if (*params) {                                                      \
        (*params)->AddRef();                                            \
    }                                                                   \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::SetLayoutParams(                                       \
    /* [in] */ IViewGroupLayoutParams* params)                          \
{                                                                       \
    return superClass::SetLayoutParams(params);                         \
}                                                                       \
                                                                        \
ECode className::ScrollTo(                                              \
    /* [in] */ Int32 x,                                                 \
    /* [in] */ Int32 y)                                                 \
{                                                                       \
    return superClass::ScrollTo(x, y);                                  \
}                                                                       \
                                                                        \
ECode className::ScrollBy(                                              \
    /* [in] */ Int32 x,                                                 \
    /* [in] */ Int32 y)                                                 \
{                                                                       \
    return superClass::ScrollBy(x, y);                                  \
}                                                                       \
                                                                        \
ECode className::Invalidate(                                            \
    /* [in] */ IRect* dirty)                                            \
{                                                                       \
    return superClass::Invalidate(dirty);                               \
}                                                                       \
                                                                        \
ECode className::InvalidateEx(                                          \
    /* [in] */ Int32 left,                                              \
    /* [in] */ Int32 top,                                               \
    /* [in] */ Int32 right,                                             \
    /* [in] */ Int32 bottom)                                            \
{                                                                       \
    return superClass::Invalidate(left, top, right, bottom);            \
}                                                                       \
                                                                        \
ECode className::InvalidateEx2()                                        \
{                                                                       \
    return superClass::Invalidate();                                    \
}                                                                       \
                                                                        \
ECode className::IsOpaque(                                              \
    /* [out] */ Boolean* opaque)                                        \
{                                                                       \
    VALIDATE_NOT_NULL(opaque);                                          \
    *opaque = superClass::IsOpaque();                                   \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::GetHandler(                                            \
    /* [out] */ IApartment** apartment)                                 \
{                                                                       \
    VALIDATE_NOT_NULL(apartment);                                       \
    *apartment = superClass::GetHandler();                              \
    if (*apartment != NULL) (*apartment)->AddRef();                 \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::PostInvalidate()                                       \
{                                                                       \
    return superClass::PostInvalidate();                                \
}                                                                       \
                                                                        \
ECode className::PostInvalidateEx(                                      \
    /* [in] */ Int32 left,                                              \
    /* [in] */ Int32 top,                                               \
    /* [in] */ Int32 right,                                             \
    /* [in] */ Int32 bottom)                                            \
{                                                                       \
    return superClass::PostInvalidate(left, top, right, bottom);        \
}                                                                       \
                                                                        \
ECode className::PostInvalidateDelayed(                                 \
    /* [in] */ Int64 delayMilliseconds)                                 \
{                                                                       \
    return superClass::PostInvalidateDelayed(delayMilliseconds);        \
}                                                                       \
                                                                        \
ECode className::PostInvalidateDelayedEx(                               \
    /* [in] */ Int64 delayMilliseconds,                                 \
    /* [in] */ Int32 left,                                              \
    /* [in] */ Int32 top,                                               \
    /* [in] */ Int32 right,                                             \
    /* [in] */ Int32 bottom)                                            \
{                                                                       \
    return superClass::PostInvalidateDelayed(                           \
        delayMilliseconds, left, top, right, bottom);                   \
}                                                                       \
                                                                        \
ECode className::ComputeScroll()                                        \
{                                                                       \
    return superClass::ComputeScroll();                                 \
}                                                                       \
                                                                        \
ECode className::IsHorizontalFadingEdgeEnabled(                         \
    /* [out] */ Boolean* horizontalFadingEdgeEnabled)                   \
{                                                                       \
    VALIDATE_NOT_NULL(horizontalFadingEdgeEnabled);                     \
    *horizontalFadingEdgeEnabled = superClass::IsHorizontalFadingEdgeEnabled();\
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::SetHorizontalFadingEdgeEnabled(                        \
    /* [in] */ Boolean horizontalFadingEdgeEnabled)                     \
{                                                                       \
    return superClass::SetHorizontalFadingEdgeEnabled(                  \
        horizontalFadingEdgeEnabled);                                   \
}                                                                       \
                                                                        \
ECode className::IsVerticalFadingEdgeEnabled(                           \
    /* [out] */ Boolean* verticalFadingEdgeEnabled)                     \
{                                                                       \
    VALIDATE_NOT_NULL(verticalFadingEdgeEnabled);                       \
    *verticalFadingEdgeEnabled = superClass::IsVerticalFadingEdgeEnabled();\
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::SetVerticalFadingEdgeEnabled(                          \
    /* [in] */ Boolean verticalFadingEdgeEnabled)                       \
{                                                                       \
    return superClass::SetVerticalFadingEdgeEnabled(                    \
        verticalFadingEdgeEnabled);                                     \
}                                                                       \
                                                                        \
ECode className::IsHorizontalScrollBarEnabled(                          \
    /* [out] */ Boolean* horizontalScrollBarEnabled)                    \
{                                                                       \
    VALIDATE_NOT_NULL(horizontalScrollBarEnabled);                      \
    *horizontalScrollBarEnabled = superClass::IsHorizontalScrollBarEnabled();\
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::SetHorizontalScrollBarEnabled(                         \
    /* [in] */ Boolean horizontalScrollBarEnabled)                      \
{                                                                       \
    return superClass::SetHorizontalScrollBarEnabled(                   \
        horizontalScrollBarEnabled);                                    \
}                                                                       \
                                                                        \
ECode className::IsVerticalScrollBarEnabled(                            \
    /* [out] */ Boolean* verticalScrollBarEnabled)                      \
{                                                                       \
    VALIDATE_NOT_NULL(verticalScrollBarEnabled);                        \
    *verticalScrollBarEnabled = superClass::IsVerticalScrollBarEnabled();\
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::SetVerticalScrollBarEnabled(                           \
    /* [in] */ Boolean verticalScrollBarEnabled)                        \
{                                                                       \
    return superClass::SetVerticalScrollBarEnabled(verticalScrollBarEnabled);\
}                                                                       \
                                                                        \
ECode className::SetScrollbarFadingEnabled(                             \
    /* [in] */ Boolean fadeScrollbars)                                  \
{                                                                       \
    return superClass::SetScrollbarFadingEnabled(fadeScrollbars);       \
}                                                                       \
                                                                        \
ECode className::IsScrollbarFadingEnabled(                              \
    /* [out] */ Boolean* fadeScrollbars)                                \
{                                                                       \
    VALIDATE_NOT_NULL(fadeScrollbars);                                  \
    *fadeScrollbars = superClass::IsScrollbarFadingEnabled();           \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::SetScrollBarStyle(                                     \
    /* [in] */ Int32 style)                                             \
{                                                                       \
    return superClass::SetScrollBarStyle(style);                        \
}                                                                       \
                                                                        \
ECode className::GetScrollBarStyle(                                     \
    /* [out] */ Int32* style)                                           \
{                                                                       \
    VALIDATE_NOT_NULL(style);                                           \
    *style = superClass::GetScrollBarStyle();                           \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
                                                                        \
ECode className::AssignParent(                                          \
    /* [in] */ IViewParent* parent)                                     \
{                                                                       \
    return superClass::AssignParent(parent);                            \
}                                                                       \
                                                                        \
ECode className::GetWindowToken(                                        \
    /* [out] */ IBinder** token)                                        \
{                                                                       \
    VALIDATE_NOT_NULL(token);                                           \
    *token = superClass::GetWindowToken();                              \
    if (*token != NULL) {                                               \
        (*token)->AddRef();                                             \
    }                                                                   \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::GetApplicationWindowToken(                             \
    /* [out] */ IBinder** token)                                        \
{                                                                       \
    VALIDATE_NOT_NULL(token);                                           \
    *token = superClass::GetApplicationWindowToken();                   \
    if (*token) {                                                       \
        (*token)->AddRef();                                             \
    }                                                                   \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::SaveHierarchyState(                                    \
    /* [in, out] */ IObjectIntegerMap* container)                       \
{                                                                       \
    return superClass::SaveHierarchyState(container);                   \
}                                                                       \
                                                                        \
ECode className::RestoreHierarchyState(                                 \
    /* [in] */ IObjectIntegerMap* container)                            \
{                                                                       \
    return superClass::RestoreHierarchyState(container);                \
}                                                                       \
                                                                        \
ECode className::GetDrawingTime(                                        \
    /* [out] */ Int64* time)                                            \
{                                                                       \
    VALIDATE_NOT_NULL(time);                                            \
    *time = superClass::GetDrawingTime();                               \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::SetDuplicateParentStateEnabled(                        \
    /* [in] */ Boolean enabled)                                         \
{                                                                       \
    return superClass::SetDuplicateParentStateEnabled(enabled);         \
}                                                                       \
                                                                        \
ECode className::IsDuplicateParentStateEnabled(                         \
    /* [out] */ Boolean* result)                                        \
{                                                                       \
    VALIDATE_NOT_NULL(result);                                          \
    *result = superClass::IsDuplicateParentStateEnabled();              \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::SetDrawingCacheEnabled(                                \
    /* [in] */ Boolean enabled)                                         \
{                                                                       \
    return superClass::SetDrawingCacheEnabled(enabled);                 \
}                                                                       \
                                                                        \
ECode className::IsDrawingCacheEnabled(                                 \
    /* [out] */ Boolean* enabled)                                       \
{                                                                       \
    VALIDATE_NOT_NULL(enabled);                                         \
    *enabled = superClass::IsDrawingCacheEnabled();                     \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::GetDrawingCache(                                       \
    /* [out] */ IBitmap** bitmap)                                     \
{                                                                       \
    VALIDATE_NOT_NULL(bitmap);                                          \
    *bitmap = superClass::GetDrawingCache();                            \
    if (*bitmap) {                                                      \
        (*bitmap)->AddRef();                                            \
    }                                                                   \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::GetDrawingCacheEx(                                     \
    /* [in] */ Boolean autoScale,                                       \
    /* [out] */ IBitmap** bitmap)                                     \
{                                                                       \
    VALIDATE_NOT_NULL(bitmap);                                          \
    *bitmap = superClass::GetDrawingCache(autoScale);                   \
    if (*bitmap) {                                                      \
        (*bitmap)->AddRef();                                            \
    }                                                                   \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::DestroyDrawingCache()                                  \
{                                                                       \
    return superClass::DestroyDrawingCache();                           \
}                                                                       \
                                                                        \
ECode className::SetDrawingCacheBackgroundColor(                        \
    /* [in] */ Int32 color)                                             \
{                                                                       \
    return superClass::SetDrawingCacheBackgroundColor(color);           \
}                                                                       \
                                                                        \
ECode className::GetDrawingCacheBackgroundColor(                        \
    /* [out] */ Int32* color)                                           \
{                                                                       \
    VALIDATE_NOT_NULL(color);                                           \
    *color = superClass::GetDrawingCacheBackgroundColor();              \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::BuildDrawingCache()                                    \
{                                                                       \
    return superClass::BuildDrawingCache();                             \
}                                                                       \
                                                                        \
ECode className::BuildDrawingCacheEx(                                   \
    /* [in] */ Boolean autoScale)                                       \
{                                                                       \
    superClass::BuildDrawingCache(autoScale);                           \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::CreateSnapshot(                                        \
    /* [in] */ BitmapConfig quality,                                  \
    /* [in] */ Int32 backgroundColor,                                   \
    /* [in] */ Boolean skipChildren,                                    \
    /* [out] */ IBitmap** bitmap)                                     \
{                                                                       \
    VALIDATE_NOT_NULL(bitmap);                                          \
    superClass::CreateSnapshot(quality, backgroundColor, skipChildren, bitmap);\
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::IsInEditMode(                                          \
    /* [out] */ Boolean* result)                                        \
{                                                                       \
    VALIDATE_NOT_NULL(result);                                          \
    *result = superClass::IsInEditMode();                               \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::Draw(                                                  \
    /* [in] */ ICanvas* canvas)                                       \
{                                                                       \
    return superClass::Draw(canvas);                                    \
}                                                                       \
                                                                        \
ECode className::GetSolidColor(                                         \
    /* [out] */ Int32* color)                                           \
{                                                                       \
    VALIDATE_NOT_NULL(color);                                           \
    *color = superClass::GetSolidColor();                               \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::OnFinishInflate()                                      \
{                                                                       \
    return superClass::OnFinishInflate();                               \
}                                                                       \
                                                                        \
ECode className::IsLayoutRequested(                                     \
    /* [out] */ Boolean* result)                                        \
{                                                                       \
    VALIDATE_NOT_NULL(result);                                          \
    *result = superClass::IsLayoutRequested();                          \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::Layout(                                                \
    /* [in] */ Int32 l,                                                 \
    /* [in] */ Int32 t,                                                 \
    /* [in] */ Int32 r,                                                 \
    /* [in] */ Int32 b)                                                 \
{                                                                       \
    return superClass::Layout(l, t, r, b);                              \
}                                                                       \
                                                                        \
ECode className::GetResources(                                          \
    /* [out] */ IResources** resources)                                 \
{                                                                       \
    VALIDATE_NOT_NULL(resources);                                       \
    *resources = superClass::GetResources();                            \
    if (*resources != NULL) {                                           \
        (*resources)->AddRef();                                         \
    }                                                                   \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::UnscheduleDrawableEx(                                  \
    /* [in] */ IDrawable* who)                                          \
{                                                                       \
    return superClass::UnscheduleDrawable(who);                         \
}                                                                       \
                                                                        \
ECode className::RefreshDrawableState()                                 \
{                                                                       \
    return superClass::RefreshDrawableState();                          \
}                                                                       \
                                                                        \
ECode className::GetDrawableState(                                      \
    /* [out, callee] */ ArrayOf<Int32>** state)                         \
{                                                                       \
    VALIDATE_NOT_NULL(state);                                           \
    *state = superClass::GetDrawableState()->Clone();                   \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::SetBackgroundColor(                                    \
    /* [in] */ Int32 color)                                             \
{                                                                       \
    return superClass::SetBackgroundColor(color);                       \
}                                                                       \
                                                                        \
ECode className::SetBackgroundResource(                                 \
    /* [in] */ Int32 resid)                                             \
{                                                                       \
    return superClass::SetBackgroundResource(resid);                    \
}                                                                       \
                                                                        \
ECode className::SetBackgroundDrawable(                                 \
    /* [in] */ IDrawable* d)                                            \
{                                                                       \
    return superClass::SetBackgroundDrawable(d);                        \
}                                                                       \
                                                                        \
ECode className::GetBackground(                                         \
    /* [out] */ IDrawable** drawable)                                   \
{                                                                       \
    VALIDATE_NOT_NULL(drawable);                                        \
    *drawable = superClass::GetBackground();                            \
    if (*drawable) {                                                    \
        (*drawable)->AddRef();                                          \
    }                                                                   \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::SetPadding(                                            \
    /* [in] */ Int32 left,                                              \
    /* [in] */ Int32 top,                                               \
    /* [in] */ Int32 right,                                             \
    /* [in] */ Int32 bottom)                                            \
{                                                                       \
    return superClass::SetPadding(left, top, right, bottom);            \
}                                                                       \
                                                                        \
ECode className::GetPaddingTop(                                         \
    /* [out] */ Int32* top)                                             \
{                                                                       \
    VALIDATE_NOT_NULL(top);                                             \
    *top = superClass::GetPaddingTop();                                 \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::GetPaddingBottom(                                      \
    /* [out] */ Int32* bottom)                                          \
{                                                                       \
    VALIDATE_NOT_NULL(bottom);                                          \
    *bottom = superClass::GetPaddingBottom();                           \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::GetPaddingLeft(                                        \
    /* [out] */ Int32* left)                                            \
{                                                                       \
    VALIDATE_NOT_NULL(left);                                            \
    *left = superClass::GetPaddingLeft();                               \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::GetPaddingRight(                                       \
    /* [out] */ Int32* right)                                           \
{                                                                       \
    VALIDATE_NOT_NULL(right);                                           \
    *right = superClass::GetPaddingRight();                             \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::SetSelected(                                           \
    /* [in] */ Boolean selected)                                        \
{                                                                       \
    return superClass::SetSelected(selected);                           \
}                                                                       \
                                                                        \
ECode className::IsSelected(                                            \
    /* [out] */ Boolean* selected)                                      \
{                                                                       \
    VALIDATE_NOT_NULL(selected);                                        \
    *selected = superClass::IsSelected();                               \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::GetViewTreeObserver(                                   \
    /* [out] */ IViewTreeObserver** observer)                           \
{                                                                       \
    VALIDATE_NOT_NULL(observer);                                        \
    *observer = superClass::GetViewTreeObserver();                      \
    if (*observer != NULL) {                                            \
        (*observer)->AddRef();                                          \
    }                                                                   \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::GetRootView(                                           \
    /* [out] */ IView** view)                                           \
{                                                                       \
    VALIDATE_NOT_NULL(view);                                            \
    *view = superClass::GetRootView();                                  \
    if (*view != NULL) {                                                \
        (*view)->AddRef();                                              \
    }                                                                   \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::GetLocationOnScreen(                                   \
    /* [out] */ Int32* x,                                               \
    /* [out] */ Int32* y)                                               \
{                                                                       \
    VALIDATE_NOT_NULL(x);                                               \
    VALIDATE_NOT_NULL(y);                                               \
                                                                        \
    return superClass::GetLocationOnScreen(x, y);                       \
}                                                                       \
                                                                        \
ECode className::GetLocationInWindow(                                   \
    /* [out] */ Int32* x,                                               \
    /* [out] */ Int32* y)                                               \
{                                                                       \
    VALIDATE_NOT_NULL(x);                                               \
    VALIDATE_NOT_NULL(y);                                               \
                                                                        \
    return superClass::GetLocationInWindow(x, y);                       \
}                                                                       \
                                                                        \
ECode className::FindViewById(                                          \
    /* [in] */ Int32 id,                                                \
    /* [out] */ IView** view)                                           \
{                                                                       \
    VALIDATE_NOT_NULL(view);                                            \
                                                                        \
    *view = superClass::FindViewById(id);                               \
    if (*view != NULL) {                                                \
        (*view)->AddRef();                                              \
    }                                                                   \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::SetId(                                                 \
    /* [in] */ Int32 id)                                                \
{                                                                       \
    return superClass::SetId(id);                                       \
}                                                                       \
                                                                        \
ECode className::GetId(                                                 \
    /* [out] */ Int32* id)                                              \
{                                                                       \
    VALIDATE_NOT_NULL(id);                                              \
                                                                        \
    *id = superClass::GetId();                                          \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::IsRootNamespace(                                       \
    /* [out] */ Boolean* result)                                        \
{                                                                       \
    VALIDATE_NOT_NULL(result);                                          \
                                                                        \
    *result = superClass::IsRootNamespace();                            \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::GetBaseline(                                           \
    /* [out] */ Int32* result)                                          \
{                                                                       \
    VALIDATE_NOT_NULL(result);                                          \
    return superClass::GetBaseline(result);                             \
}                                                                       \
                                                                        \
ECode className::RequestLayout()                                        \
{                                                                       \
    return superClass::RequestLayout();                                 \
}                                                                       \
                                                                        \
ECode className::ForceLayout()                                          \
{                                                                       \
    return superClass::ForceLayout();                                   \
}                                                                       \
                                                                        \
ECode className::Measure(                                               \
    /* [in] */ Int32 widthMeasureSpec,                                  \
    /* [in] */ Int32 heightMeasureSpec)                                 \
{                                                                       \
    return superClass::Measure(widthMeasureSpec, heightMeasureSpec);    \
}                                                                       \
                                                                        \
ECode className::SetMinimumHeight(                                      \
    /* [in] */ Int32 minHeight)                                         \
{                                                                       \
    return superClass::SetMinimumHeight(minHeight);                     \
}                                                                       \
                                                                        \
ECode className::SetMinimumWidth(                                       \
    /* [in] */ Int32 minWidth)                                          \
{                                                                       \
    return superClass::SetMinimumWidth(minWidth);                       \
}                                                                       \
                                                                        \
ECode className::GetAnimation(                                          \
    /* [out] */ IAnimation** animation)                                 \
{                                                                       \
    VALIDATE_NOT_NULL(animation);                                       \
                                                                        \
    *animation = superClass::GetAnimation();                            \
    if (*animation != NULL) {                                           \
        (*animation)->AddRef();                                         \
    }                                                                   \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::StartAnimation(                                        \
    /* [in] */ IAnimation* animation)                                   \
{                                                                       \
    return superClass::StartAnimation(animation);                       \
}                                                                       \
                                                                        \
ECode className::ClearAnimation()                                       \
{                                                                       \
    return superClass::ClearAnimation();                                \
}                                                                       \
                                                                        \
ECode className::SetAnimation(                                          \
    /* [in] */ IAnimation* animation)                                   \
{                                                                       \
    return superClass::SetAnimation(animation);                         \
}                                                                       \
                                                                        \
ECode className::GatherTransparentRegion(                               \
    /* [in] */ IRegion* region,                                         \
    /* [out] */ Boolean* result)                                        \
{                                                                       \
    VALIDATE_NOT_NULL(result);                                          \
    *result = superClass::GatherTransparentRegion(region);              \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::PlaySoundEffect(                                       \
    /* [in] */ Int32 soundConstant)                                     \
{                                                                       \
    return superClass::PlaySoundEffect(soundConstant);                  \
}                                                                       \
                                                                        \
ECode className::PerformHapticFeedback(                                 \
    /* [in] */ Int32 feedbackConstant,                                  \
    /* [out] */ Boolean* result)                                        \
{                                                                       \
    VALIDATE_NOT_NULL(result);                                          \
    *result = superClass::PerformHapticFeedback(feedbackConstant);      \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::PerformHapticFeedbackEx(                               \
    /* [in] */ Int32 feedbackConstant,                                  \
    /* [in] */ Int32 flags,                                             \
    /* [out] */ Boolean* result)                                        \
{                                                                       \
    VALIDATE_NOT_NULL(result);                                          \
    *result = superClass::PerformHapticFeedback(feedbackConstant, flags);\
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::OnCloseSystemDialogs(                                  \
    /* [in] */ const String& reason)                                    \
{                                                                       \
    return superClass::OnCloseSystemDialogs(reason);                    \
}                                                                       \
                                                                        \
ECode className::ApplyDrawableToTransparentRegion(                      \
    /* [in] */ IDrawable* dr,                                           \
    /* [in] */ IRegion* region)                                         \
{                                                                       \
    return superClass::ApplyDrawableToTransparentRegion(dr, region);    \
}                                                                       \
                                                                        \
ECode className::GetOverScrollMode(                                     \
    /* [out] */ Int32* mode)                                            \
{                                                                       \
    VALIDATE_NOT_NULL(mode);                                            \
    *mode = superClass::GetOverScrollMode();                            \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::SetOverScrollMode(                                     \
    /* [in] */ Int32 overScrollMode)                                    \
{                                                                       \
    return superClass::SetOverScrollMode(overScrollMode);               \
}                                                                       \
                                                                        \
ECode className::SetTag(                                                \
    /* [in] */ IInterface* tag)                                         \
{                                                                       \
    return superClass::SetTag(tag);                                     \
}                                                                       \
                                                                        \
ECode className::GetTag(                                                \
    /* [in] */ IInterface** tag)                                        \
{                                                                       \
    VALIDATE_NOT_NULL(tag);                                             \
    *tag = superClass::GetTag();                                        \
    if (*tag != NULL) {                                                 \
        (*tag)->AddRef();                                               \
    }                                                                   \
                                                                        \
    return NOERROR;                                                     \
}

#define IVIEWGROUP_METHODS_DECL()                                       \
    CARAPI GetDescendantFocusability(                                   \
        /* [out] */ Int32* focusability);                               \
                                                                        \
    CARAPI SetDescendantFocusability(                                   \
        /* [in] */ Int32 focusability);                                 \
                                                                        \
    CARAPI GetFocusedChild(                                             \
        /* [out] */ IView** focusedChild);                              \
                                                                        \
    CARAPI OnInterceptTouchEvent(                                       \
        /* [in] */ IMotionEvent* ev,                                    \
        /* [out] */ Boolean* value);                                    \
                                                                        \
    CARAPI SetClipChildren(                                             \
        /* [in] */ Boolean clipChildren);                               \
                                                                        \
    CARAPI SetClipToPadding(                                            \
        /* [in] */ Boolean clipToPadding);                              \
                                                                        \
    CARAPI AddView(                                                     \
        /* [in] */ IView* child);                                       \
                                                                        \
    CARAPI AddViewEx(                                                   \
        /* [in] */ IView* child,                                        \
        /* [in] */ Int32 index);                                        \
                                                                        \
    CARAPI AddViewEx2(                                                  \
        /* [in] */ IView* child,                                        \
        /* [in] */ Int32 width,                                         \
        /* [in] */ Int32 height);                                       \
                                                                        \
    CARAPI AddViewEx3(                                                  \
        /* [in] */ IView* child,                                        \
        /* [in] */ IViewGroupLayoutParams* params);                     \
                                                                        \
    CARAPI AddViewEx4(                                                  \
        /* [in] */ IView* child,                                        \
        /* [in] */ Int32 index,                                         \
        /* [in] */ IViewGroupLayoutParams* params);                     \
                                                                        \
    CARAPI SetOnHierarchyChangeListener(                                \
        /* [in] */ IViewGroupOnHierarchyChangeListener* listener);      \
                                                                        \
    CARAPI RemoveViewInLayout(                                          \
        /* [in] */ IView* view);                                        \
                                                                        \
    CARAPI RemoveViewsInLayout(                                         \
        /* [in] */ Int32 start,                                         \
        /* [in] */ Int32 count);                                        \
                                                                        \
    CARAPI RemoveViewAt(                                                \
        /* [in] */ Int32 index);                                        \
                                                                        \
    CARAPI RemoveViews(                                                 \
        /* [in] */ Int32 start,                                         \
        /* [in] */ Int32 count);                                        \
                                                                        \
    CARAPI RemoveAllViews();                                            \
                                                                        \
    CARAPI RemoveAllViewsInLayout();                                    \
                                                                        \
    CARAPI OffsetDescendantRectToMyCoords(                              \
        /* [in] */ IView* descendant,                                   \
        /* [in] */ IRect* rect);                                        \
                                                                        \
    CARAPI OffsetRectIntoDescendantCoords(                              \
        /* [in] */ IView* descendant,                                   \
        /* [in] */ IRect* rect);                                        \
                                                                        \
    CARAPI OffsetChildrenTopAndBottom(                                  \
        /* [in] */ Int32 offset);                                       \
                                                                        \
    CARAPI StartLayoutAnimation();                                      \
                                                                        \
    CARAPI ScheduleLayoutAnimation();                                   \
                                                                        \
    CARAPI SetLayoutAnimation(                                          \
        /* [in] */ ILayoutAnimationController* controller);             \
                                                                        \
    CARAPI GetLayoutAnimation(                                          \
        /* [out] */ ILayoutAnimationController** controller);           \
                                                                        \
    CARAPI IsAnimationCacheEnabled(                                     \
        /* [out] */ Boolean* enabled);                                  \
                                                                        \
    CARAPI SetAnimationCacheEnabled(                                    \
        /* [in] */ Boolean enabled);                                    \
                                                                        \
    CARAPI IsAlwaysDrawnWithCacheEnabled(                               \
        /* [out] */ Boolean* always);                                   \
                                                                        \
    CARAPI SetAlwaysDrawnWithCacheEnabled(                              \
        /* [in] */ Boolean always);                                     \
                                                                        \
    CARAPI GetPersistentDrawingCache(                                   \
        /* [out] */ Int32* drawingCacheToKeep);                         \
                                                                        \
    CARAPI SetPersistentDrawingCache(                                   \
        /* [in] */ Int32 drawingCacheToKeep);                           \
                                                                        \
    CARAPI GenerateLayoutParams(                                        \
        /* [in] */ IAttributeSet* attrs,                                \
        /* [out] */ IViewGroupLayoutParams** layoutParams);             \
                                                                        \
    CARAPI IndexOfChild(                                                \
        /* [in] */ IView* child,                                        \
        /* [out] */ Int32* index);                                      \
                                                                        \
    CARAPI GetChildCount(                                               \
        /* [out] */ Int32* count);                                      \
                                                                        \
    CARAPI GetChildAt(                                                  \
        /* [in] */ Int32 index,                                         \
        /* [out] */ IView** child);                                     \
                                                                        \
    CARAPI ClearDisappearingChildren();                                 \
                                                                        \
    CARAPI GetLayoutAnimationListener(                                  \
        /* [out] */ IAnimationListener** listener);                     \
                                                                        \
    CARAPI SetAddStatesFromChildren(                                    \
        /* [in] */ Boolean addsStates);                                 \
                                                                        \
    CARAPI AddStatesFromChildren(                                       \
        /* [out] */ Boolean* drawable);                                 \
                                                                        \
    CARAPI SetLayoutAnimationListener(                                  \
        /* [in] */ IAnimationListener* animationListener);


#define IVIEWGROUP_METHODS_IMPL(className, superClass, overRideClass)   \
ECode className::GetDescendantFocusability(                             \
    /* [out] */ Int32* focusability)                                    \
{                                                                       \
    VALIDATE_NOT_NULL(focusability);                                    \
    *focusability = superClass::GetDescendantFocusability();            \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::SetDescendantFocusability(                             \
    /* [in] */ Int32 focusability)                                      \
{                                                                       \
    return superClass::SetDescendantFocusability(focusability);         \
}                                                                       \
                                                                        \
ECode className::GetFocusedChild(                                       \
    /* [out] */ IView** focusedChild)                                   \
{                                                                       \
    VALIDATE_NOT_NULL(focusedChild);                                    \
    *focusedChild = superClass::GetFocusedChild();                      \
    if (*focusedChild) {                                                \
        (*focusedChild)->AddRef();                                      \
    }                                                                   \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::OnInterceptTouchEvent(                                 \
    /* [in] */ IMotionEvent* ev,                                        \
    /* [out] */ Boolean* value)                                         \
{                                                                       \
    VALIDATE_NOT_NULL(value);                                           \
    *value = overRideClass::OnInterceptTouchEvent(ev);                  \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::SetClipChildren(                                       \
    /* [in] */ Boolean clipChildren)                                    \
{                                                                       \
    superClass::SetClipChildren(clipChildren);                          \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::SetClipToPadding(                                      \
    /* [in] */ Boolean clipToPadding)                                   \
{                                                                       \
    superClass::SetClipToPadding(clipToPadding);                        \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::AddView(                                               \
    /* [in] */ IView* child)                                            \
{                                                                       \
    return superClass::AddView(child);                                  \
}                                                                       \
                                                                        \
ECode className::AddViewEx(                                             \
    /* [in] */ IView* child,                                            \
    /* [in] */ Int32 index)                                             \
{                                                                       \
    return superClass::AddView(child, index);                           \
}                                                                       \
                                                                        \
ECode className::AddViewEx2(                                            \
    /* [in] */ IView* child,                                            \
    /* [in] */ Int32 width,                                             \
    /* [in] */ Int32 height)                                            \
{                                                                       \
    return superClass::AddView(child, width ,height);                   \
}                                                                       \
                                                                        \
ECode className::AddViewEx3(                                            \
    /* [in] */ IView* child,                                            \
    /* [in] */ IViewGroupLayoutParams* params)                          \
{                                                                       \
    return superClass::AddView(child, params);                          \
}                                                                       \
                                                                        \
ECode className::AddViewEx4(                                            \
    /* [in] */ IView* child,                                            \
    /* [in] */ Int32 index,                                             \
    /* [in] */ IViewGroupLayoutParams* params)                          \
{                                                                       \
    return superClass::AddView(child, index, params);                   \
}                                                                       \
                                                                        \
ECode className::SetOnHierarchyChangeListener(                          \
    /* [in] */ IViewGroupOnHierarchyChangeListener* listener)           \
{                                                                       \
    return superClass::SetOnHierarchyChangeListener(listener);          \
}                                                                       \
                                                                        \
ECode className::RemoveViewInLayout(                                    \
    /* [in] */ IView* view)                                             \
{                                                                       \
    return superClass::RemoveViewInLayout(view);                        \
}                                                                       \
                                                                        \
ECode className::RemoveViewsInLayout(                                   \
    /* [in] */ Int32 start,                                             \
    /* [in] */ Int32 count)                                             \
{                                                                       \
    return superClass::RemoveViewsInLayout(start, count);               \
}                                                                       \
                                                                        \
ECode className::RemoveViewAt(                                          \
    /* [in] */ Int32 index)                                             \
{                                                                       \
    return superClass::RemoveViewAt(index);                             \
}                                                                       \
                                                                        \
ECode className::RemoveViews(                                           \
    /* [in] */ Int32 start,                                             \
    /* [in] */ Int32 count)                                             \
{                                                                       \
    return superClass::RemoveAllViews();                                \
}                                                                       \
                                                                        \
ECode className::RemoveAllViews()                                       \
{                                                                       \
    return superClass::RemoveAllViews();                                \
}                                                                       \
                                                                        \
ECode className::RemoveAllViewsInLayout()                               \
{                                                                       \
    return superClass::RemoveAllViewsInLayout();                        \
}                                                                       \
                                                                        \
ECode className::OffsetDescendantRectToMyCoords(                        \
    /* [in] */ IView* descendant,                                       \
    /* [in] */ IRect* rect)                                             \
{                                                                       \
    return superClass::OffsetDescendantRectToMyCoords(descendant, rect);\
}                                                                       \
                                                                        \
ECode className::OffsetRectIntoDescendantCoords(                        \
    /* [in] */ IView* descendant,                                       \
    /* [in] */ IRect* rect)                                             \
{                                                                       \
    return superClass::OffsetRectIntoDescendantCoords(descendant, rect);\
}                                                                       \
                                                                        \
ECode className::OffsetChildrenTopAndBottom(                            \
    /* [in] */ Int32 offset)                                            \
{                                                                       \
    return superClass::OffsetChildrenTopAndBottom(offset);              \
}                                                                       \
                                                                        \
ECode className::StartLayoutAnimation()                                 \
{                                                                       \
    return superClass::StartLayoutAnimation();                          \
}                                                                       \
                                                                        \
ECode className::ScheduleLayoutAnimation()                              \
{                                                                       \
    return superClass::ScheduleLayoutAnimation();                       \
}                                                                       \
                                                                        \
ECode className::SetLayoutAnimation(                                    \
    /* [in] */ ILayoutAnimationController* controller)                  \
{                                                                       \
    return superClass::SetLayoutAnimation(controller);                  \
}                                                                       \
                                                                        \
ECode className::GetLayoutAnimation(                                    \
    /* [out] */ ILayoutAnimationController** controller)                \
{                                                                       \
    VALIDATE_NOT_NULL(controller);                                      \
    AutoPtr<ILayoutAnimationController> ctrl=                           \
            superClass::GetLayoutAnimation();                           \
    *controller = ctrl.Get();                                           \
    if (*controller) (*controller)->AddRef();                           \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::IsAnimationCacheEnabled(                               \
    /* [out] */ Boolean* enabled)                                       \
{                                                                       \
    VALIDATE_NOT_NULL(enabled);                                         \
    *enabled = superClass::IsAnimationCacheEnabled();                   \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::SetAnimationCacheEnabled(                              \
    /* [in] */ Boolean enabled)                                         \
{                                                                       \
    return superClass::SetAnimationCacheEnabled(enabled);               \
}                                                                       \
                                                                        \
ECode className::IsAlwaysDrawnWithCacheEnabled(                         \
    /* [out] */ Boolean* always)                                        \
{                                                                       \
    VALIDATE_NOT_NULL(always);                                          \
    *always = superClass::IsAlwaysDrawnWithCacheEnabled();              \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::SetAlwaysDrawnWithCacheEnabled(                        \
    /* [in] */ Boolean always)                                          \
{                                                                       \
    return superClass::SetAlwaysDrawnWithCacheEnabled(always);          \
}                                                                       \
                                                                        \
ECode className::GetPersistentDrawingCache(                             \
    /* [out] */ Int32* drawingCacheToKeep)                              \
{                                                                       \
    VALIDATE_NOT_NULL(drawingCacheToKeep);                              \
    *drawingCacheToKeep = superClass::GetPersistentDrawingCache();      \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::SetPersistentDrawingCache(                             \
    /* [in] */ Int32 drawingCacheToKeep)                                \
{                                                                       \
    return superClass::IsAnimationCacheEnabled();                                           \
}                                                                       \
                                                                        \
ECode className::GenerateLayoutParams(                                  \
    /* [in] */ IAttributeSet* attrs,                                    \
    /* [out] */ IViewGroupLayoutParams** layoutParams)                  \
{                                                                       \
    VALIDATE_NOT_NULL(layoutParams);                                    \
    AutoPtr<IViewGroupLayoutParams> params                              \
        = superClass::GenerateLayoutParams(attrs);                      \
    *layoutParams = params.Get();                                       \
    if (*layoutParams) {                                                \
        (*layoutParams)->AddRef();                                      \
    }                                                                   \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::IndexOfChild(                                          \
    /* [in] */ IView* child,                                            \
    /* [out] */ Int32* index)                                           \
{                                                                       \
    VALIDATE_NOT_NULL(index);                                           \
    *index = superClass::IndexOfChild(child);                           \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::GetChildCount(                                         \
    /* [out] */ Int32* count)                                           \
{                                                                       \
    VALIDATE_NOT_NULL(count);                                           \
                                                                        \
    *count = superClass::GetChildCount();                               \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::GetChildAt(                                            \
    /* [in] */ Int32 index,                                             \
    /* [out] */ IView** child)                                          \
{                                                                       \
    VALIDATE_NOT_NULL(child);                                           \
                                                                        \
    AutoPtr<IView> _child = ViewGroup::GetChildAt(index);               \
    *child = _child;                                                    \
                                                                        \
    if (*child != NULL) {                                               \
        (*child)->AddRef();                                             \
    }                                                                   \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::ClearDisappearingChildren()                            \
{                                                                       \
    return superClass::ClearDisappearingChildren();                     \
}                                                                       \
                                                                        \
ECode className::GetLayoutAnimationListener(                            \
    /* [out] */ IAnimationListener** listener)                          \
{                                                                       \
    VALIDATE_NOT_NULL(listener);                                        \
    AutoPtr<IAnimationListener> l =                                     \
            superClass::GetLayoutAnimationListener();                   \
    *listener = l.Get();                                                \
    if (*listener) (*listener)->AddRef();                               \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::SetAddStatesFromChildren(                              \
    /* [in] */ Boolean addsStates)                                      \
{                                                                       \
    return superClass::SetAddStatesFromChildren(addsStates);            \
}                                                                       \
                                                                        \
ECode className::AddStatesFromChildren(                                 \
    /* [out] */ Boolean* drawable)                                      \
{                                                                       \
    VALIDATE_NOT_NULL(drawable);                                        \
    *drawable = superClass::AddStatesFromChildren();                    \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::SetLayoutAnimationListener(                            \
    /* [in] */ IAnimationListener* animationListener)                   \
{                                                                       \
    return superClass::SetLayoutAnimationListener(animationListener);   \
}

#define IVIEWPARENT_METHODS_DECL()                                      \
    CARAPI RequestLayoutEx();                                           \
                                                                        \
    CARAPI IsLayoutRequestedEx(                                         \
        /* [out] */ Boolean* result);                                   \
                                                                        \
    CARAPI RequestTransparentRegion(                                    \
        /* [in] */ IView* child);                                       \
                                                                        \
    CARAPI InvalidateChild(                                             \
        /* [in] */ IView* child,                                        \
        /* [in] */ IRect* r);                                           \
                                                                        \
    CARAPI InvalidateChildInParent(                                     \
        /* [in] */ ArrayOf<Int32>* location,                            \
        /* [in] */ IRect* r,                                            \
        /* [out] */ IViewParent** parent);                              \
                                                                        \
    CARAPI GetParentEx(                                                 \
        /* [out] */ IViewParent** parent);                              \
                                                                        \
    CARAPI RequestChildFocus(                                           \
        /* [in] */ IView* child,                                        \
        /* [in] */ IView* focused);                                     \
                                                                        \
    CARAPI RecomputeViewAttributes(                                     \
        /* [in] */ IView* child);                                       \
                                                                        \
    CARAPI ClearChildFocus(                                             \
        /* [in] */ IView* child);                                       \
                                                                        \
    CARAPI GetChildVisibleRect(                                         \
        /* [in] */ IView* child,                                        \
        /* [in] */ IRect* r,                                            \
        /* [in] */ IPoint* offset,                                      \
        /* [out] */ Boolean* result);                                   \
                                                                        \
    CARAPI FocusSearchEx(                                               \
        /* [in] */ IView* focused,                                      \
        /* [in] */ Int32 direction,                                     \
        /* [out] */ IView** focus);                                     \
                                                                        \
    CARAPI BringChildToFront(                                           \
        /* [in] */ IView* child);                                       \
                                                                        \
    CARAPI FocusableViewAvailable(                                      \
        /* [in] */ IView* v);                                           \
                                                                        \
    CARAPI ShowContextMenuForChild(                                     \
        /* [in] */ IView* originalView,                                 \
        /* [out] */ Boolean* result);                                   \
                                                                        \
    CARAPI CreateContextMenuEx(                                         \
        /* [in] */ IContextMenu* menu);                                 \
                                                                        \
    CARAPI ChildDrawableStateChanged(                                   \
        /* [in] */ IView* child);                                       \
                                                                        \
    CARAPI RequestDisallowInterceptTouchEvent(                          \
        /* [in] */ Boolean disallowIntercept);                          \
                                                                        \
    CARAPI RequestChildRectangleOnScreen(                               \
        /* [in] */ IView* child,                                        \
        /* [in] */ IRect* rectangle,                                    \
        /* [in] */ Boolean immediate,                                   \
        /* [out] */ Boolean* result);                                   \

#define IVIEWPARENT_METHODS_IMPL(className, superClass, overRideClass)  \
ECode className::RequestLayoutEx()                                      \
{                                                                       \
    return superClass::RequestLayout();                                 \
}                                                                       \
                                                                        \
ECode className::IsLayoutRequestedEx(                                   \
    /* [out] */ Boolean* result)                                        \
{                                                                       \
    VALIDATE_NOT_NULL(result);                                          \
    *result = superClass::IsLayoutRequested();                          \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::RequestTransparentRegion(                              \
    /* [in] */ IView* child)                                            \
{                                                                       \
    return superClass::RequestTransparentRegion(child);                 \
}                                                                       \
                                                                        \
ECode className::InvalidateChild(                                       \
    /* [in] */ IView* child,                                            \
    /* [in] */ IRect* r)                                                \
{                                                                       \
    return superClass::InvalidateChild(child, r);                       \
}                                                                       \
                                                                        \
ECode className::InvalidateChildInParent(                               \
    /* [in] */ ArrayOf<Int32>* location,                                \
    /* [in] */ IRect* r,                                                \
    /* [out] */ IViewParent** parent)                                   \
{                                                                       \
    VALIDATE_NOT_NULL(parent);                                          \
    AutoPtr<IViewParent> p = superClass::InvalidateChildInParent(location, r);\
    *parent = p;                                                        \
    if (*parent) (*parent)->AddRef();                                   \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::GetParentEx(                                           \
    /* [out] */ IViewParent** parent)                                   \
{                                                                       \
    VALIDATE_NOT_NULL(parent);                                          \
    AutoPtr<IViewParent> p = superClass::GetParent();                   \
    *parent = p;                                                        \
    if (*parent) (*parent)->AddRef();                                   \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::RequestChildFocus(                                     \
    /* [in] */ IView* child,                                            \
    /* [in] */ IView* focused)                                          \
{                                                                       \
    return superClass::RequestChildFocus(child, focused);               \
}                                                                       \
                                                                        \
ECode className::RecomputeViewAttributes(                               \
    /* [in] */ IView* child)                                            \
{                                                                       \
    return superClass::RecomputeViewAttributes(child);                  \
}                                                                       \
                                                                        \
ECode className::ClearChildFocus(                                       \
    /* [in] */ IView* child)                                            \
{                                                                       \
    return superClass::ClearChildFocus(child);                          \
}                                                                       \
                                                                        \
ECode className::GetChildVisibleRect(                                   \
    /* [in] */ IView* child,                                            \
    /* [in] */ IRect* r,                                                \
    /* [in] */ IPoint* offset,                                          \
    /* [out] */ Boolean* result)                                        \
{                                                                       \
    VALIDATE_NOT_NULL(result);                                          \
    return superClass::GetChildVisibleRect(child, r, offset, result);   \
}                                                                       \
                                                                        \
ECode className::FocusSearchEx(                                         \
    /* [in] */ IView* focused,                                          \
    /* [in] */ Int32 direction,                                         \
    /* [out] */ IView** focus)                                          \
{                                                                       \
    VALIDATE_NOT_NULL(focus);                                           \
    AutoPtr<IView> v = superClass::FocusSearch(focused, direction);     \
    *focus = v;                                                         \
    if (*focus) (*focus)->AddRef();                                     \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::BringChildToFront(                                     \
    /* [in] */ IView* child)                                            \
{                                                                       \
    return superClass::BringChildToFront(child);                        \
}                                                                       \
                                                                        \
ECode className::FocusableViewAvailable(                                \
    /* [in] */ IView* v)                                                \
{                                                                       \
    return superClass::FocusableViewAvailable(v);                       \
}                                                                       \
                                                                        \
ECode className::ShowContextMenuForChild(                               \
    /* [in] */ IView* originalView,                                     \
    /* [out] */ Boolean* result)                                        \
{                                                                       \
    VALIDATE_NOT_NULL(result);                                          \
    *result = superClass::ShowContextMenuForChild(originalView);        \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::CreateContextMenuEx(                                   \
    /* [in] */ IContextMenu* menu)                                      \
{                                                                       \
    return superClass::CreateContextMenu(menu);                         \
}                                                                       \
                                                                        \
ECode className::ChildDrawableStateChanged(                             \
    /* [in] */ IView* child)                                            \
{                                                                       \
    return superClass::ChildDrawableStateChanged(child);                \
}                                                                       \
                                                                        \
ECode className::RequestDisallowInterceptTouchEvent(                    \
    /* [in] */ Boolean disallowIntercept)                               \
{                                                                       \
    return superClass::RequestDisallowInterceptTouchEvent(disallowIntercept);\
}                                                                       \
                                                                        \
ECode className::RequestChildRectangleOnScreen(                         \
    /* [in] */ IView* child,                                            \
    /* [in] */ IRect* rectangle,                                        \
    /* [in] */ Boolean immediate,                                       \
    /* [out] */ Boolean* result)                                        \
{                                                                       \
    VALIDATE_NOT_NULL(result);                                          \
    *result = superClass::RequestChildRectangleOnScreen(child, rectangle, immediate);\
                                                                        \
    return NOERROR;                                                     \
}

#define IVIEWMANAGER_METHODS_DECL()                                     \
    CARAPI AddViewEx5(                                                  \
        /* [in] */ IView* view,                                         \
        /* [in] */ IViewGroupLayoutParams* params);                     \
                                                                        \
    CARAPI UpdateViewLayout(                                            \
        /* [in] */ IView* view,                                         \
        /* [in] */ IViewGroupLayoutParams* params);                     \
                                                                        \
    CARAPI RemoveView(                                                  \
        /* [in] */ IView* view);

#define IVIEWMANAGER_METHODS_IMPL(className, superClass, overRideClass) \
ECode className::AddViewEx5(                                            \
    /* [in] */ IView* view,                                             \
    /* [in] */ IViewGroupLayoutParams* params)                          \
{                                                                       \
    return superClass::AddView(view, params);                           \
}                                                                       \
                                                                        \
ECode className::UpdateViewLayout(                                      \
    /* [in] */ IView* view,                                             \
    /* [in] */ IViewGroupLayoutParams* params)                          \
{                                                                       \
    return superClass::UpdateViewLayout(view, params);                  \
}                                                                       \
                                                                        \
ECode className::RemoveView(                                            \
    /* [in] */ IView* view)                                             \
{                                                                       \
    return superClass::RemoveView(view);                                \
}

#define IDrawableCallback_METHODS_DECL()                                \
    CARAPI InvalidateDrawable(                                          \
        /* [in] */ IDrawable* drawable);                                \
                                                                        \
    CARAPI ScheduleDrawable(                                            \
        /* [in] */ IDrawable* who,                                      \
        /* [in] */ IRunnable* what,                                     \
        /* [in] */ Int64 when);                                         \
                                                                        \
    CARAPI UnscheduleDrawable(                                          \
        /* [in] */ IDrawable* who,                                      \
        /* [in] */ IRunnable* what);

#define IDrawableCallback_METHODS_IMPL(className, superClass, overRideClass) \
ECode className::InvalidateDrawable(                                    \
    /* [in] */ IDrawable* drawable)                                     \
{                                                                       \
    return superClass::InvalidateDrawable(drawable);                    \
}                                                                       \
                                                                        \
ECode className::ScheduleDrawable(                                      \
    /* [in] */ IDrawable* who,                                          \
    /* [in] */ IRunnable* what,                                         \
    /* [in] */ Int64 when)                                              \
{                                                                       \
    return superClass::ScheduleDrawable(who, what, when);               \
}                                                                       \
                                                                        \
ECode className::UnscheduleDrawable(                                    \
    /* [in] */ IDrawable* who,                                          \
    /* [in] */ IRunnable* what)                                         \
{                                                                       \
    return superClass::UnscheduleDrawable(who, what);                   \
}

#define IKeyEventCallback_METHODS_DECL()                                \
    CARAPI OnKeyDown(                                                   \
        /* [in] */ Int32 keyCode,                                       \
        /* [in] */ IKeyEvent* event,                                    \
        /* [out] */ Boolean* result);                                   \
                                                                        \
    CARAPI OnKeyLongPress(                                              \
        /* [in] */ Int32 keyCode,                                       \
        /* [in] */ IKeyEvent* event,                                    \
        /* [out] */ Boolean* result);                                   \
                                                                        \
    CARAPI OnKeyUp(                                                     \
        /* [in] */ Int32 keyCode,                                       \
        /* [in] */ IKeyEvent* event,                                    \
        /* [out] */ Boolean* result);                                   \
                                                                        \
    CARAPI OnKeyMultiple(                                               \
        /* [in] */ Int32 keyCode,                                       \
        /* [in] */ Int32 count,                                         \
        /* [in] */ IKeyEvent* event,                                    \
        /* [out] */ Boolean* result);

#define IKeyEventCallback_METHODS_IMPL(className, superClass, overRideClass) \
ECode className::OnKeyDown(                                             \
    /* [in] */ Int32 keyCode,                                           \
    /* [in] */ IKeyEvent* event,                                        \
    /* [out] */ Boolean* result)                                        \
{                                                                       \
    VALIDATE_NOT_NULL(result);                                          \
    *result = superClass::OnKeyDown(keyCode, event);                    \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::OnKeyLongPress(                                        \
    /* [in] */ Int32 keyCode,                                           \
    /* [in] */ IKeyEvent* event,                                        \
    /* [out] */ Boolean* result)                                        \
{                                                                       \
    VALIDATE_NOT_NULL(result);                                          \
    *result = superClass::OnKeyLongPress(keyCode, event);               \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::OnKeyUp(                                               \
    /* [in] */ Int32 keyCode,                                           \
    /* [in] */ IKeyEvent* event,                                        \
    /* [out] */ Boolean* result)                                        \
{                                                                       \
    VALIDATE_NOT_NULL(result);                                          \
    *result = superClass::OnKeyUp(keyCode, event);                      \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::OnKeyMultiple(                                         \
    /* [in] */ Int32 keyCode,                                           \
    /* [in] */ Int32 repeatCount,                                       \
    /* [in] */ IKeyEvent* event,                                        \
    /* [out] */ Boolean* result)                                        \
{                                                                       \
    VALIDATE_NOT_NULL(result);                                          \
    *result = superClass::OnKeyMultiple(keyCode, repeatCount, event);   \
                                                                        \
    return NOERROR;                                                     \
}

#define IAccessibilityEventSource_METHODS_DECL()                        \
    CARAPI SendAccessibilityEvent(                                      \
        /* [in] */ Int32 eventType);                                    \
                                                                        \
    CARAPI SendAccessibilityEventUnchecked(                             \
        /* [in] */ IAccessibilityEvent* event);

#define IAccessibilityEventSource_METHODS_IMPL(className, superClass, overRideClass) \
ECode className::SendAccessibilityEvent(                                \
    /* [in] */ Int32 eventType)                                         \
{                                                                       \
    return superClass::SendAccessibilityEvent(eventType);               \
}                                                                       \
                                                                        \
ECode className::SendAccessibilityEventUnchecked(                       \
    /* [in] */ IAccessibilityEvent* event)                              \
{                                                                       \
    return superClass::SendAccessibilityEventUnchecked(event);          \
}

#endif  //__VIEWMACRO_H__
