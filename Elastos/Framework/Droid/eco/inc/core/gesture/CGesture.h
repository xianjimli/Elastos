
#ifndef __CGESTURE_H__
#define __CGESTURE_H__

#include "_CGesture.h"
#include "ext/frameworkdef.h"
#include <elastos.h>
#include <elastos/List.h>
#include <Elastos.IO.h>

using namespace Elastos;
using namespace Elastos::Core;

/**
 * A gesture is a hand-drawn shape on a touch screen. It can have one or multiple strokes.
 * Each stroke is a sequence of timed points. A user-defined gesture can be recognized by
 * a GestureLibrary and a built-in alphabet gesture can be recognized by a LetterRecognizer.
 */
//public class Gesture implements Parcelable 
CarClass(CGesture)
{

public:

   CGesture();

    virtual ~CGesture();

    CARAPI constructor();

    /* @Override */
    CARAPI Clone(
        /* [out] */ IGesture ** gesture);
    
    /**
     * @return all the strokes of the gesture
     */
    CARAPI GetStrokes(
        /* [out] */ IObjectContainer ** strokes); //<IGestureStroke*> 

    /**
     * @return the number of strokes included by this gesture
     */
    CARAPI GetStrokesCount(
        /* [out] */ Int32 * strokeCount);

    /**
     * Adds a stroke to the gesture.
     * 
     * @param stroke
     */
    CARAPI AddStroke(
         /* [in] */ IGestureStroke* stroke);

    /**
     * Calculates the total length of the gesture. When there are multiple strokes in
     * the gesture, this returns the sum of the lengths of all the strokes.
     * 
     * @return the length of the gesture
     */
    CARAPI GetLength(
        /* [out] */ Float * length);

    /**
     * @return the bounding box of the gesture
     */
    CARAPI GetBoundingBox(
        /* [out] */ IRectF ** rectf);

    CARAPI ToPath(
        /* [out] */ IPath ** path);

    CARAPI ToPathEx(
        /* [in] */ IPath* path,
        /* [out] */ IPath ** pathret);

    CARAPI ToPathEx2(
        /* [in] */ Int32 width,
        /* [in] */ Int32 height,
        /* [in] */ Int32 edge,
        /* [in] */ Int32 numSample,
        /* [out] */ IPath ** path);

    CARAPI ToPathEx3(
        /* [in] */ IPath* path,
        /* [in] */ Int32 width,
        /* [in] */ Int32 height,
        /* [in] */ Int32 edge,
        /* [in] */ Int32 numSample,
        /* [out] */ IPath ** pathret);

    /**
     * Sets the id of the gesture.
     * 
     * @param id
     */
    CARAPI SetID(
        /* [in] */ Int64 id);

    /**
     * @return the id of the gesture
     */
    CARAPI GetID(
        /* [out] */ Int64* id);

    /**
     * Creates a bitmap of the gesture with a transparent background.
     * 
     * @param width width of the target bitmap
     * @param height height of the target bitmap
     * @param edge the edge
     * @param numSample
     * @param color
     * @return the bitmap
     */

    CARAPI ToBitmap(
        /* [in] */ Int32 width,
        /* [in] */ Int32 height,
        /* [in] */ Int32 edge,
        /* [in] */ Int32 numSample,
        /* [in] */ Int32 color,
        /* [out] */ IBitmap ** bmp );


    /**
     * Creates a bitmap of the gesture with a transparent background.
     * 
     * @param width
     * @param height
     * @param inset
     * @param color
     * @return the bitmap
     */
    CARAPI ToBitmapEx(
        /* [in] */ Int32 width,
        /* [in] */ Int32 height,
        /* [in] */ Int32 inset,
        /* [in] */ Int32 color,
        /* [out] */ IBitmap ** bitmap);

    CARAPI Serialize(
        /* [in] */ IDataOutputStream* outStream);

    static CARAPI Deserialize(
        /* [in] */ IDataInputStream* instream,
        /* [out] */ IGesture **gesture);

    static CARAPI DeserializeEx(
        /* [in] */ IParcel* instream,
        /* [out] */ CGesture **gesture);

    /*
    public static final Parcelable.Creator<Gesture> CREATOR = new Parcelable.Creator<Gesture>() {
        public Gesture createFromParcel(Parcel in) {
            Gesture gesture = null;
            final long gestureID = in.readLong();

            final DataInputStream inStream = new DataInputStream(
                    new ByteArrayInputStream(in.createByteArray()));

            try {
                gesture = deserialize(inStream);
            } catch (IOException e) {
                Log.e(GestureConstants.LOG_TAG, "Error reading Gesture from parcel:", e);
            } finally {
                GestureUtils.closeStream(inStream);
            }

            if (gesture != null) {
                gesture.mGestureID = gestureID;
            }

            return gesture;
        }

        public Gesture[] newArray(int size) {
            return new Gesture[size];
        }
    };
    */

    CARAPI WriteToParcel(
        /* [in] */ IParcel* out);

//    CARAPI WriteToParcel(
//        /* [in] */ IParcel* dest,
//        /* [in] */ Int32 flags);

    CARAPI DescribeContents(
        /* [out] */ Int32 * ret);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel * in);
protected:
    List<IGestureStroke* >* mStrokes;
    IRectF* mBoundingBox;// = new RectF();

private:
    static const Int64 GESTURE_ID_BASE;// = System::GetCurrentTimeMillis();
    static const Int32 BITMAP_RENDERING_WIDTH;// = 2;
    static const Boolean BITMAP_RENDERING_ANTIALIAS;// = TRUE;
    static const Boolean BITMAP_RENDERING_DITHER;// = TRUE;

    //private static final AtomicInteger sGestureCount = new AtomicInteger(0);
    static Int32 sGestureCount;// = 0;

    // the same as its instance ID
    Int64 mGestureID;

    void ClearStrokes();

    CARAPI_(List<IGestureStroke*> *) GetStrokes();

    void UnInit();
};

#endif //__CGESTURE_H__

