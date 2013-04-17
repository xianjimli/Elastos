
#include "app/CLauncherActivityIconResizer.h"
#include "app/CLauncherActivity.h"
#include "graphics/CPaintFlagsDrawFilter.h"
#include "graphics/ElPixelFormat.h"
#include "graphics/drawable/CBitmapDrawable.h"


CLauncherActivityIconResizer::constructor(ILauncherActivity* lActivity)
{
    mIconWidth = -1;
    mIconHeight = -1;
    mLauncherActivity = (CLauncherActivity*)lActivity;
    AutoPtr<IPaintFlagsDrawFilter> drawfilter;
    CPaintFlagsDrawFilter::New(Paint_DITHER_FLAG, Paint_FILTER_BITMAP_FLAG, 
        (IPaintFlagsDrawFilter**)&drawfilter);
    mCanvas->SetDrawFilter(drawfilter);           
    AutoPtr<IResources> resources;
    mLauncherActivity->GetResources((IResources**)&resources);
    resources->GetDimension(0x01050000/*android.R.dimen.app_icon_size*/, (Float*)&mIconHeight);
    mIconWidth = mIconHeight;
}

ECode CLauncherActivityIconResizer::CreateIconThumbnail(
    /* [in] */ IDrawable* icon,
    /* [out] */ IDrawable** drawable)
{
    Int32 width = mIconWidth;
    Int32 height = mIconHeight;

    Int32 iconWidth;
    icon->GetIntrinsicWidth(&iconWidth);
    Int32 iconHeight;
    icon->GetIntrinsicHeight(&iconHeight);

    if (icon->Probe(EIID_IPaintDrawable) != NULL) {
        AutoPtr<IPaintDrawable> painter = (IPaintDrawable*) icon;
        painter->SetIntrinsicWidth(width);
        painter->SetIntrinsicHeight(height);
    }

    if (width > 0 && height > 0) {
        if (width < iconWidth || height < iconHeight) {
            const Int64 ratio = (Int64) (iconWidth / iconHeight);

            if (iconWidth > iconHeight) {
                height = (Int32) (width / ratio);
            }
            else if (iconHeight > iconWidth) {
                width = (int) (height * ratio);
            }

            Int32 opacity;
            icon->GetOpacity(&opacity);
            const BitmapConfig c = opacity != ElPixelFormat::OPAQUE ?
                    BitmapConfig_ARGB_8888 : BitmapConfig_RGB_565;
            AutoPtr<IBitmap> thumb;// = Bitmap.createBitmap(mIconWidth, mIconHeight, c);
            AutoPtr<ICanvas> canvas = mCanvas;
            canvas->SetBitmap(thumb);
            // Copy the old bounds to restore them later
            // If we were to do oldBounds = icon.getBounds(),
            // the call to setBounds() that follows would
            // change the same instance and we would lose the
            // old bounds
            AutoPtr<IRect> rect;
            icon->GetBounds((IRect**)&rect);
            mOldBounds->SetEx(rect);
            const Int32 x = (mIconWidth - width) / 2;
            const Int32 y = (mIconHeight - height) / 2;
            icon->SetBounds(x, y, x + width, y + height);
            icon->Draw(canvas);
            icon->SetBoundsEx(mOldBounds);
            AutoPtr<IResources> resources;
            mLauncherActivity->GetResources((IResources**)&resources);
            CBitmapDrawable::New(resources, thumb, (IBitmapDrawable**)&icon);
        }
        else if (iconWidth < width && iconHeight < height) {
            const BitmapConfig c = BitmapConfig_ARGB_8888;
            AutoPtr<IBitmap> thumb;// = Bitmap.createBitmap(mIconWidth, mIconHeight, c);
            AutoPtr<ICanvas> canvas = mCanvas;
            canvas->SetBitmap(thumb);
            AutoPtr<IRect> rect;
            icon->GetBounds((IRect**)&rect);
            mOldBounds->SetEx(rect);
            const Int32 x = (width - iconWidth) / 2;
            const Int32 y = (height - iconHeight) / 2;
            icon->SetBounds(x, y, x + iconWidth, y + iconHeight);
            icon->Draw(canvas);
            icon->SetBoundsEx(mOldBounds);
            AutoPtr<IResources> resources;
            mLauncherActivity->GetResources((IResources**)&resources);
            CBitmapDrawable::New(resources, thumb, (IBitmapDrawable**)&icon);
        }
    }

    *drawable = icon;
    return NOERROR;
}