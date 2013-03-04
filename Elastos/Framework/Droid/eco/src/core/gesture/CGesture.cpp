
#include <gesture/CGesture.h>
#include <gesture/CGestureStroke.h>
#include <gesture/CGestureUtils.h>
#include <graphics/CBitmapFactory.h>
#include <graphics/CBitmap.h>
#include <graphics/CCanvas.h>
#include <graphics/CPaint.h>
#include <graphics/CPath.h>
#include <graphics/CRectF.h>
#include <elastos/Mutex.h>
#include <Logger.h>
//#include <Elastos.Framework.Core.h>
#include <elastos/System.h>
#include <Elastos.IO.h>

using namespace Elastos;
using namespace Elastos::Core;
using namespace Elastos::Core::Threading;
using namespace Elastos::Utility::Logging;

static Mutex gMutex4GestureCount;
const Int64 CGesture::GESTURE_ID_BASE = System::GetCurrentTimeMillis();
const Int32 CGesture::BITMAP_RENDERING_WIDTH = 2;
const Boolean CGesture::BITMAP_RENDERING_ANTIALIAS = TRUE;
const Boolean CGesture::BITMAP_RENDERING_DITHER = TRUE;
Int32 CGesture::sGestureCount = 0;

/**
 * A gesture is a hand-drawn shape on a touch screen. It can have one or multiple strokes.
 * Each stroke is a sequence of timed points. A user-defined gesture can be recognized by 
 * a GestureLibrary and a built-in alphabet gesture can be recognized by a LetterRecognizer.
 */
CGesture::CGesture()
{
    gMutex4GestureCount.Lock();
    sGestureCount++;
    gMutex4GestureCount.Unlock();
    mGestureID = GESTURE_ID_BASE + sGestureCount;

    CRectF::New(&mBoundingBox);// = new RectF();
    mStrokes= new List<IGestureStroke* >();
}

CGesture::~CGesture()
{
    UnInit();
}

ECode CGesture::constructor()
{
    //mGestureID = GESTURE_ID_BASE + sGestureCount.incrementAndGet();

    return NOERROR;
}

/*  @Override*/

ECode CGesture::Clone(
    /* [out] */ IGesture ** gesture)
{
    VALIDATE_NOT_NULL(gesture);
    CGesture::New(gesture);
    Int32 count = mStrokes->GetSize();
    for (int i = 0; i < count; i++) {
        IGestureStroke* stroke = (*mStrokes)[i];
        IGestureStroke *obj = NULL;
        stroke->Clone(&obj);
        (*gesture)->AddStroke(obj);
    }
    IRectF* bbx = NULL;
    (*gesture)->GetBoundingBox(&bbx);
    bbx->SetEx(mBoundingBox);
    return NOERROR;
}

/**
 * @return all the strokes of the gesture
 */

List<IGestureStroke* >* CGesture::GetStrokes()
{
    return mStrokes;
}

//IObjectContainer<IGestureStroke* >*
ECode CGesture::GetStrokes(
    /* [out] */ IObjectContainer ** strokes)
{
    VALIDATE_NOT_NULL(strokes);
    //return mStrokes;
    IObjectContainer* ret=NULL;
    CObjectContainer::New(&ret);
    Int32 count = mStrokes->GetSize();
    for (Int32 i=0;i< count;i++) {
        ret->Add( (IInterface *) ((*mStrokes)[i]) );
    }
    *strokes = ret;
    return NOERROR;
}

/**
 * @return the number of strokes included by this gesture
 */
ECode CGesture::GetStrokesCount(
    /* [out] */ Int32 * strokeCount)
{
    VALIDATE_NOT_NULL(strokeCount);
    *strokeCount = mStrokes->GetSize();
    return NOERROR;
}

/**
 * Adds a stroke to the gesture.
 *
 * @param stroke
 */

ECode CGesture::AddStroke(
    /* [in] */ IGestureStroke* stroke)
{
    VALIDATE_NOT_NULL(stroke);
    mStrokes->PushBack(stroke);
    IRectF* boundingBox = NULL;
    stroke->GetBoundingBox(&boundingBox);
    mBoundingBox->UnionEx(boundingBox);
    return NOERROR;
}

/**
 * Calculates the total length of the gesture. When there are multiple strokes in
 * the gesture, this returns the sum of the lengths of all the strokes.
 *
 * @return the length of the gesture
 */
ECode CGesture::GetLength(
    /* [out] */ Float * length)
{
    VALIDATE_NOT_NULL(length);
    Int32 len = 0;
    List<IGestureStroke* >* strokes = mStrokes;
    Int32 count = strokes->GetSize();

    for (Int32 i = 0; i < count; i++) {
        Float lentmp;
        len += ((*strokes)[i])->GetLength(&lentmp);
     }

    *length = len;
    return NOERROR;
}

 /**
  * @return the bounding box of the gesture
 */

ECode CGesture::GetBoundingBox(
    /* [out] */ IRectF** boundingBox)
{
    VALIDATE_NOT_NULL(boundingBox);
    *boundingBox = mBoundingBox;
    return NOERROR;
}

ECode CGesture::ToPath(
    /* [out] */ IPath ** path)
{
    VALIDATE_NOT_NULL(path);
    return ToPathEx(NULL, path);
}

ECode CGesture::ToPathEx(
    /* [in] */ IPath* path,
    /* [out] */ IPath ** pathret)
{
    VALIDATE_NOT_NULL(pathret);
    if (path == NULL) {
        CPath::New(&path);
    }
    List<IGestureStroke *>* strokes = mStrokes;
    Int32 count = strokes->GetSize();

    for (int i = 0; i < count; i++) {
       IPath * pathtmp = NULL;
       ((*strokes)[i])->GetPath(&pathtmp);
       path->AddPathEx(pathtmp);
    }

    *pathret = path;
    return NOERROR;
}

ECode CGesture::ToPathEx2(
    /* [in] */ Int32 width,
    /* [in] */ Int32 height,
    /* [in] */ Int32 edge,
    /* [in] */ Int32 numSample,
    /* [out] */ IPath ** path)
{
    VALIDATE_NOT_NULL(path);
    return ToPathEx3(NULL, width, height, edge, numSample, path);
}

ECode CGesture::ToPathEx3(
    /* [in] */ IPath* path,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height,
    /* [in] */ Int32 edge,
    /* [in] */ Int32 numSample,
    /* [out] */ IPath ** pathret)
{
    VALIDATE_NOT_NULL(pathret);
    if (path == NULL) {
        CPath::New(&path);
    }
    List<IGestureStroke* >* strokes = mStrokes;
    Int32 count = strokes->GetSize();

    for (Int32 i = 0; i < count; i++) {
        IPath *pathtmp = NULL;
        ((*strokes)[i])->ToPath(width - 2 * edge, height - 2 * edge, numSample, &pathtmp);
        path->AddPathEx(pathtmp);
    }

    *pathret = path;
    return NOERROR;
}

/**
 * Sets the id of the gesture.
 *
 * @param id
 */
ECode CGesture::SetID(
    /* [in] */ Int64 id)
{
    mGestureID = id;
    return NOERROR;
}

/**
 * @return the id of the gesture
 */
ECode CGesture::GetID(
    /* [out] */ Int64* id)
{
    VALIDATE_NOT_NULL(id);
    *id = mGestureID;
    return NOERROR;
}

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
ECode CGesture::ToBitmap(
    /* [in] */ Int32 width,
    /* [in] */ Int32 height,
    /* [in] */ Int32 edge,
    /* [in] */ Int32 numSample,
    /* [in] */ Int32 color,
    /* [out] */ IBitmap ** bmp )
{
    VALIDATE_NOT_NULL(bmp);
    IBitmap* bitmap=NULL;
    CBitmapFactory bmpfactory;
    bmpfactory.CreateBitmapEx3(width, height, BitmapConfig_ARGB_8888,&bitmap);
    ICanvas* canvas=NULL;
    CCanvas::New(bitmap, &canvas);

    canvas->Translate(edge, edge);

    IPaint* paint=NULL;
    CPaint::New(&paint);
    paint->SetAntiAlias(BITMAP_RENDERING_ANTIALIAS);
    paint->SetDither(BITMAP_RENDERING_DITHER);
    paint->SetColor(color);
    paint->SetStyle(PaintStyle_STROKE);
    paint->SetStrokeJoin(PaintJoin_ROUND);
    paint->SetStrokeCap(PaintCap_ROUND);
    paint->SetStrokeWidth(BITMAP_RENDERING_WIDTH);

    List<IGestureStroke *>* strokes = mStrokes;
    Int32 count = strokes->GetSize();

    for (int i = 0; i < count; i++) {
        IPath* path = NULL;
        ((*strokes)[i])->ToPath(width - 2 * edge, height - 2 * edge, numSample, &path);
        canvas->DrawPath(path, paint);
        path->Release();
     }

    paint->Release();
    canvas->Release();

    *bmp = bitmap;
    return NOERROR;
}

/**
 * Creates a bitmap of the gesture with a transparent background.
 *
 * @param width
 * @param height
 * @param inset
 * @param color
 * @return the bitmap
 */
ECode CGesture::ToBitmapEx(
    /* [in] */ Int32 width,
    /* [in] */ Int32 height,
    /* [in] */ Int32 inset,
    /* [in] */ Int32 color,
    /* [out] */ IBitmap ** bmp)
{
    VALIDATE_NOT_NULL(bmp);
    IBitmap* bitmap=NULL;
    CBitmapFactory bmpfactory;
    bmpfactory.CreateBitmapEx3(width, height, BitmapConfig_ARGB_8888,&bitmap);
    ICanvas* canvas=NULL;
    CCanvas::New(bitmap, &canvas);

    IPaint* paint=NULL;
    CPaint::New(&paint);
    paint->SetAntiAlias(BITMAP_RENDERING_ANTIALIAS);
    paint->SetDither(BITMAP_RENDERING_DITHER);
    paint->SetColor(color);
    paint->SetStyle(PaintStyle_STROKE);
    paint->SetStrokeJoin(PaintJoin_ROUND);
    paint->SetStrokeCap(PaintCap_ROUND);
    paint->SetStrokeWidth(BITMAP_RENDERING_WIDTH);

    IPath* path = NULL;
    ToPath(&path);
    IRectF* bounds=NULL;
    CRectF::New(&bounds);
    path->ComputeBounds(bounds, TRUE);

    Float boundsWidth;
    Float boundsHeight;
    bounds->Width(&boundsWidth);
    bounds->Height(&boundsHeight);
    Float sx = (width - 2 * inset) / boundsWidth;
    Float sy = (height - 2 * inset) / boundsHeight;
    Float scale = sx > sy ? sy : sx;
    paint->SetStrokeWidth(2.0f / scale);

    Float boundsLeft = ((CRectF*) bounds)->mLeft;
    Float boundsTop = ((CRectF*) bounds)->mTop;
    path->OffsetEx(-boundsLeft + (width - boundsWidth * scale) / 2.0f,
                -boundsTop + (height - boundsHeight * scale) / 2.0f);

    canvas->Translate(inset, inset);
    canvas->Scale(scale, scale);

    canvas->DrawPath(path, paint);

    bounds->Release();
    path->Release();
    paint->Release();
    canvas->Release();

    *bmp = bitmap;
    return NOERROR;
}

ECode CGesture::Serialize(
    /* [in] */ IDataOutputStream* out)
{
    VALIDATE_NOT_NULL(out);
    List<IGestureStroke* >* strokes = mStrokes;
    Int32 count = strokes->GetSize();

    IDataOutput * dout = IDataOutput::Probe(out);
    // Write gesture ID
    dout->WriteInt64(mGestureID);
    // Write number of strokes
    dout->WriteInt32(count);

    for (Int32 i = 0; i < count; i++) {
        ((*strokes)[i])->Serialize(out);
    }
    return NOERROR;
}

ECode CGesture::Deserialize(
    /* [in] */ IDataInputStream* in,
    /* [out] */ IGesture **gesture)
{
    VALIDATE_NOT_NULL(in);
    VALIDATE_NOT_NULL(gesture);
    IGesture* gestureRet;
    CGesture::New(&gestureRet);

    IDataInput * din = IDataInput::Probe(in);
    // Gesture ID
    Int64 gestureID;
    din->ReadInt64(&gestureID);
    gestureRet->SetID(gestureID);
    // Number of strokes
    Int32 count;
    din->ReadInt32(&count);

    for (Int32 i = 0; i < count; i++) {
        IGestureStroke *gestureStroke = NULL;
        CGestureStroke::Deserialize(in, &gestureStroke);
        gestureRet->AddStroke(gestureStroke);
    }

    *gesture = gestureRet;
    return NOERROR;
}
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

ECode CGesture::WriteToParcel(
    /* [in] */ IParcel* out)
{
    VALIDATE_NOT_NULL(out);
    out->WriteInt64(mGestureID);

    IByteArrayOutputStream* byteStream=NULL;
    CByteArrayOutputStream::New(GestureConstants_IO_BUFFER_SIZE, &byteStream);
    IDataOutputStream* outStream=NULL;
    CDataOutputStream::New(byteStream, &outStream);

    ECode ec=Serialize(outStream);
    if (FAILED(ec)) {
        Logger::E(GestureConstants_LOG_TAG, CString("Error writing Gesture to parcel:"));
    } else {
        ArrayOf<Byte>* bytes = NULL;
        byteStream->ToByteArray(&bytes);
        for (Int32 i=0; i < bytes->GetLength(); i++)
        {
            Byte d=(*bytes)[i];
            out->WriteByte(d);
        }
    }

    //CGestureUtils::CloseStream(outStream);
    CGestureUtils::CloseStream((ICloseable *)byteStream);
    outStream->Release();
    byteStream->Release();
    return ec;
}

ECode CGesture::DescribeContents(
    /* [out] */ Int32 * ret)
{
    VALIDATE_NOT_NULL(ret);
    *ret = 0;
    return NOERROR;
}

ECode CGesture::ReadFromParcel(
    /* [in] */ IParcel * in)
{
    VALIDATE_NOT_NULL(in);
    Int64 gestureID;
    in->ReadInt64(&gestureID);

    CGesture *gesture = NULL;
    ECode ec = DeserializeEx(in, &gesture);
    if (FAILED(ec)) {
       Logger::E(GestureConstants_LOG_TAG, CString("Error reading Gesture to parcel:"));
    }

    if (gesture != NULL) {
        //this = gesture;
        ClearStrokes();
        Int32 count = gesture->mStrokes->GetSize();
        for (int i = 0; i < count; i++) {
            IGestureStroke* stroke = (*(gesture->mStrokes))[i];
            IGestureStroke *obj = NULL;
            stroke->Clone(&obj);
            AddStroke(obj);
        }

        IRectF* bbx = NULL;
        gesture->GetBoundingBox(&bbx);
        mBoundingBox->SetEx(bbx);

        mGestureID = gestureID;
        delete gesture;
    }

    return ec;
}

void CGesture::ClearStrokes()
{
    Int32 count = mStrokes->GetSize();
    for (Int32 i=0; i<count; i++) {
        IGestureStroke * gs = (*mStrokes)[i];
        gs->Release();
    }
}

void CGesture::UnInit()
{
    mBoundingBox->Release();
    ClearStrokes();
    delete mStrokes;
}

ECode CGesture::DeserializeEx(
    /* [in] */ IParcel* in,
    /* [out] */ CGesture **gesture)
{
    VALIDATE_NOT_NULL(in);
    VALIDATE_NOT_NULL(gesture);
    CGesture* gestureRet = new CGesture();

    // Gesture ID
    Int64 gestureID;
    in->ReadInt64(&gestureID);
    gestureRet->SetID(gestureID);
    // Number of strokes
    Int32 count;
    in->ReadInt32(&count);

    for (Int32 i = 0; i < count; i++) {
        IGestureStroke *gestureStroke = NULL;
        CGestureStroke::DeserializeEx(in, &gestureStroke);
        gestureRet->AddStroke(gestureStroke);
    }

    *gesture = gestureRet;
    return NOERROR;
}


