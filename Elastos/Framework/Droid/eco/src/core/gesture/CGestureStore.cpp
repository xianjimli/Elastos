#include "ext/frameworkdef.h"
#include <gesture/CInstanceLearner.h>
#include <gesture/CGestureStore.h>
#include <gesture/CInstance.h>
#include <gesture/CGestureUtils.h>
#include <gesture/CGesture.h>
#include <Logger.h>

using namespace Elastos;
using namespace Elastos::Utility::Logging;

const Int16 CGestureStore::FILE_FORMAT_VERSION = 1;
const Boolean CGestureStore::PROFILE_LOADING_SAVING = FALSE;
CString CGestureStore::LOG_TAG = GestureConstants_LOG_TAG;
const Int32 CGestureStore::STRING_MAX_LENGTH = 128;

/**
 * GestureLibrary maintains gesture examples and makes predictions on a new
 * gesture
 */
//
//    File format for CGestureStore:
//
//                Nb. bytes   Java type   Description
//                -----------------------------------
//    Header
//                2 bytes     short       File format version number
//                4 bytes     int         Number of entries
//    Entry
//                X bytes     UTF String  Entry name
//                4 bytes     int         Number of gestures
//    Gesture
//                8 bytes     long        Gesture ID
//                4 bytes     int         Number of strokes
//    Stroke
//                4 bytes     int         Number of points
//    Point
//                4 bytes     float       X coordinate of the point
//                4 bytes     float       Y coordinate of the point
//                8 bytes     long        Time stamp
//


CGestureStore::CGestureStore()
{
    mSequenceType = GestureStore_SEQUENCE_SENSITIVE;
    mOrientationStyle = GestureStore_ORIENTATION_SENSITIVE;
    mChanged = FALSE;
    mNamedGestures=new HashMap<String, List<IGesture*> *>();
    CInstanceLearner::New(&mClassifier);
}

CGestureStore::~CGestureStore()
{
    ClearNamedGestures();
    delete mNamedGestures;
    mClassifier->Release();
}

void CGestureStore::Init()
{

}

ECode CGestureStore::constructor()
{
    return NOERROR;
}


/**
* Specify how the gesture library will handle orientation.
* Use ORIENTATION_INVARIANT or ORIENTATION_SENSITIVE
*
* @param style
*/

ECode CGestureStore::SetOrientationStyle(
    /* [in] */ Int32 style)
{
    mOrientationStyle = style;
    return NOERROR;
}

ECode CGestureStore::GetOrientationStyle(
    /* [out] */ Int32 * orientationStyle)
{
    VALIDATE_NOT_NULL(orientationStyle);
    *orientationStyle = mOrientationStyle;
    return NOERROR;
}

/**
* @param type SEQUENCE_INVARIANT or SEQUENCE_SENSITIVE
*/
ECode CGestureStore::SetSequenceType(
    /* [in] */ Int32 type)
{
    mSequenceType = type;
    return NOERROR;
}

/**
* @return SEQUENCE_INVARIANT or SEQUENCE_SENSITIVE
*/
ECode CGestureStore::GetSequenceType(
    /* [out] */ Int32 * sequenceType)
{
    VALIDATE_NOT_NULL(sequenceType);
    *sequenceType = mSequenceType;
    return NOERROR;
}

/**
* Get all the gesture entry names in the library
*
* @return a set of strings
*/
//IObjectContainer<String>*
ECode CGestureStore::GetGestureEntries(
    /* [out, callee] */ ArrayOf<String> ** gestureEntries)
{
    VALIDATE_NOT_NULL(gestureEntries);
    //return mNamedGestures.keySet();
    ArrayOf<String> * ret = NULL;
    //HashMap<String, List<IGesture*> *>::Iterator iter;
    //for (iter=mNamedGestures->Begin(); iter!=mNamedGestures->End(); ++iter) {
    Int32 count = mNamedGestures->GetSize();
    ret = ArrayOf<String>::Alloc(count);
    HashMap<String, List<IGesture*> *>::Iterator iter = mNamedGestures->Begin();
    for (Int32 i=0; iter!=mNamedGestures->End(); ++iter, ++i) {
        (*ret)[i] = ((String) iter->mFirst );
    }
    *gestureEntries = ret;
    return NOERROR;

}

/**
* Recognize a gesture
*
* @param gesture the query
* @return a list of predictions of possible entries for a given gesture
*/
//IObjectContainer<IPrediction*>*
ECode CGestureStore::Recognize(
    /* [in] */ IGesture *gesture,
    /* [out] */ IObjectContainer ** arrayList)
{
    VALIDATE_NOT_NULL(gesture);
    VALIDATE_NOT_NULL(arrayList);
    ECode ec = NOERROR;
    IInstance * instance = NULL;
    String nulstr=String("");
    CInstance::CreateInstance(mSequenceType,
                              mOrientationStyle,
                              gesture,
                              nulstr,//NULL,
                              &instance);
    ArrayOf<Float> *vector = NULL;
    instance->GetVector(&vector);
    ec = mClassifier->Classify(mSequenceType, mOrientationStyle, *vector,arrayList);
    ArrayOf<Float>::Free(vector);
    instance->Release();
    return ec;
}

/**
* Add a gesture for the entry
*
* @param entryName entry name
* @param gesture
*/
ECode CGestureStore::AddGesture(
    /* [in] */ const String & entryName,
    /* [in] */ IGesture *gesture)
{
    VALIDATE_NOT_NULL(gesture);
    if (entryName.IsNull() || entryName.GetLength() == 0) {
        return NOERROR;
    }

    HashMap<String, List<IGesture*> *>::Iterator it=mNamedGestures->Find(entryName);
    List<IGesture*>* gestures = NULL;
    if (it != mNamedGestures->End())
        gestures = it->mSecond;

    if (gestures == NULL) {
        gestures = new List<IGesture*>();
        (*mNamedGestures)[entryName] = gestures;
    }
    gestures->PushBack(gesture);
    IInstance * ins = NULL;
    ECode ec = NOERROR;
    ec = CInstance::CreateInstance(mSequenceType, mOrientationStyle, gesture, entryName, &ins);
    ec = mClassifier->AddInstance(ins);
    if (!(FAILED(ec)))
        mChanged = TRUE;
    return ec;
}

/**
* Remove a gesture from the library. If there are no more gestures for the
* given entry, the gesture entry will be removed.
*
* @param entryName entry name
* @param gesture
*/

ECode CGestureStore::RemoveGesture(
    /* [in] */ const String & entryName,
    /* [in] */ IGesture *gesture)
{
    VALIDATE_NOT_NULL(gesture);
    HashMap<String, List<IGesture*> *>::Iterator it = mNamedGestures->Find(entryName);
    List<IGesture*>* gestures = NULL;
    if (it != mNamedGestures->End())
        gestures = it->mSecond;


    if (gestures == NULL) {
        return NOERROR;
    }

    gestures->Remove(gesture);

    // if there are no more samples, remove the entry automatically
    if (gestures->IsEmpty()) {
        mNamedGestures->Erase(entryName);
    }
    Int64 id;
    gesture->GetID(&id);
    mClassifier->RemoveInstance(id);
    mChanged = true;
    return NOERROR;

}

/**
* Remove a entry of gestures
*
* @param entryName the entry name
*/
ECode CGestureStore::RemoveEntry(
    /* [in] */ const String & entryName)
{
    mNamedGestures->Erase(entryName);
    mClassifier->RemoveInstances(entryName);
    mChanged = true;
    return NOERROR;
}

/**
* Get all the gestures of an entry
*
* @param entryName
* @return the list of gestures that is under this name
*/
//IObjectContainer<IGesture*>*
ECode CGestureStore::GetGestures(
    /* [in] */ const String & entryName,
    /* [out] */ IObjectContainer ** gestures)
{
    VALIDATE_NOT_NULL(gestures);
    IObjectContainer* ret = NULL;
    CObjectContainer::New(&ret);

    HashMap<String, List<IGesture*> *>::Iterator it = mNamedGestures->Find(entryName);
    List<IGesture *>* gesturesList = NULL;
    if (it != mNamedGestures->End()) {
        gesturesList = it->mSecond;
        for (Int32 i=0;i < (Int32)(gesturesList->GetSize());i++) {
            ret->Add((IGesture *) ((*gesturesList)[i]));
        }
    }
    *gestures = ret;
    return NOERROR;

}

ECode CGestureStore::HasChanged(
    /* [out] */ Boolean * hasChanged)
{
    VALIDATE_NOT_NULL(hasChanged);
    *hasChanged = mChanged;
    return NOERROR;
}

/**
* Save the gesture library
*/
ECode CGestureStore::Save(
    /* [in] */ IOutputStream * stream)
{
    VALIDATE_NOT_NULL(stream);
    return SaveEx(stream, FALSE);
}

ECode CGestureStore::SaveEx(
    /* [in] */ IOutputStream * stream,
    /* [in] */ Boolean closeStream)
{
    VALIDATE_NOT_NULL(stream);
    /*
	AutoPtr<IBufferedOutputStream> outs;
	if (FAILED(CBufferedOutputStream::New(stream,
			                               GestureConstants::IO_BUFFER_SIZE,
			                               (IBufferedOutputStream **) &outs)))
	{
		return E_RUNTIME_EXCEPTION;
	}
	*/

    AutoPtr<IDataOutputStream> out;
    if (FAILED(CDataOutputStream::New(stream,(IDataOutputStream **) &out)))
    {
        return E_RUNTIME_EXCEPTION;
    }

    Int64 start;
    if (PROFILE_LOADING_SAVING) {
        start = SystemClock::GetElapsedRealtime();
    }

    HashMap<String, List<IGesture*> *> *maps = mNamedGestures;

    IDataOutput *dout=IDataOutput::Probe(out);
    // Write version number
    dout->WriteInt16(FILE_FORMAT_VERSION);
    // Write number of entries
    dout->WriteInt32(maps->GetSize());

    HashMap<String, List<IGesture*> *>::Iterator iter = maps->Begin();
    for (Int32 i=0; iter!=maps->End(); ++iter, ++i) {
        String key= iter->mFirst;
        List<IGesture*> *examples= iter->mSecond;
        Int32 count = examples->GetSize();

        // Write entry name
        ArrayOf<Byte>* utf = StringToByteArray(key);
        dout->WriteBytes(*utf); //??no WriteUTF api
        ArrayOf<Byte>::Free(utf);

        // Write number of examples for this entry
        dout->WriteInt32(count);

        for (Int32 j = 0; j < count; j++) {
            ((IGesture*)((*examples)[i]))->Serialize(out);
        }
    }
    stream->Flush(); //no Flush api

    if (PROFILE_LOADING_SAVING) {
        Int64 end = SystemClock::GetElapsedRealtime();
        Int64 diff= end - start;
        String tmp = String::FromInt64(diff);
        String log = String("Saving gestures library = ") + tmp + String(" ms");
        Logger::D(LOG_TAG,log.string());
    }

    mChanged = FALSE;
    if (closeStream) {
        CGestureUtils::CloseStream((ICloseable *)stream);
    }

    return NOERROR;

}

/**
* Load the gesture library
*/
ECode CGestureStore::Load(
    /* [in] */ IInputStream *stream)
{
    VALIDATE_NOT_NULL(stream);
    return LoadEx(stream, FALSE);
}

ECode CGestureStore::LoadEx(
    /* [in] */ IInputStream *stream,
    /* [in] */ Boolean closeStream)
{
    VALIDATE_NOT_NULL(stream);
    //CDataInputStream *in = NULL;
    AutoPtr<IDataInputStream> in;
    if (FAILED(CDataInputStream::New(stream,(IDataInputStream **) &in)))
    {
        return E_RUNTIME_EXCEPTION;
    }

    Int64 start;
    if (PROFILE_LOADING_SAVING) {
        start = SystemClock::GetElapsedRealtime();
    }

    IDataInput* din = IDataInput::Probe(in);
    // Read file format version number
    Int16 versionNumber = 0;
    if (FAILED(din->ReadInt16(&versionNumber)) )
        return E_RUNTIME_EXCEPTION;

    switch (versionNumber) {
        case 1:
            ReadFormatV1(in);
            break;
    }

    if (PROFILE_LOADING_SAVING) {
        Int64 end = SystemClock::GetElapsedRealtime();
        Int64 diff= end - start;
        String tmp = String::FromInt64(diff);
        String log = String("Loading gestures library = ") + tmp + String(" ms");
        Logger::D(LOG_TAG, log.string());
    }

    if (closeStream) {
        CGestureUtils::CloseStream((ICloseable *)stream);
    }

    return NOERROR;
}


ECode CGestureStore::ReadFormatV1(
    /* [in] */ IDataInputStream *in)
{
    VALIDATE_NOT_NULL(in);
    //final Learner classifier = mClassifier;
    //final HashMap<String, ArrayList<Gesture>> namedGestures = mNamedGestures;
    //namedGestures.clear();

    ClearNamedGestures();
    IDataInput *din = IDataInput::Probe(in);

    // Number of entries in the library
    Int32 entriesCount =0;
    if (FAILED(din->ReadInt32(&entriesCount)))
        return E_RUNTIME_EXCEPTION;

    for (Int32 i = 0; i < entriesCount; i++) {
        // Entry name
        //String name = din->ReadUTF();//??
        String name;
        ReadString(in, &name);
        // Number of gestures
        Int32 gestureCount =0;
        din->ReadInt32(&gestureCount);

        List<IGesture*> * gestures = new List<IGesture*>();
        for (Int32 j = 0; j < gestureCount; j++) {
            IGesture * gesture=NULL;
            CGesture::Deserialize(in, &gesture);
            gestures->PushBack(gesture);
            IInstance* ins = NULL;
            CInstance::CreateInstance(mSequenceType, mOrientationStyle, gesture, name, &ins);
            mClassifier->AddInstance(ins);
        }

        (*mNamedGestures)[name]=gestures;
    }
    return NOERROR;
}

ECode CGestureStore::GetLearner(
    /* [out] */ ILearner ** learner)
{
    VALIDATE_NOT_NULL(learner);
    *learner = mClassifier;
    return NOERROR;
}

ArrayOf<Byte>* CGestureStore::StringToByteArray(
    /* [in] */ const String& str)
{
    Int32 len = str.GetLength();
    ArrayOf<Byte>* ret = ArrayOf<Byte>::Alloc(len);
    for (Int32 i = 0; i < len; i++) {
        (*ret)[i] = str[i];
    }
    return ret;
}

ECode CGestureStore::ReadString(
    /* [in] */ IDataInputStream *in,
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(in);// != NULL);
    VALIDATE_NOT_NULL(str);// != NULL);

    StringBuf* line = StringBuf::Alloc(STRING_MAX_LENGTH);
    Boolean foundTerminator = FALSE;
    IDataInput *din = IDataInput::Probe(in);
    while (TRUE) {
        Byte nextByte = -1;
        ECode ec = din->ReadByte(&nextByte);
        if (FAILED(ec)) {
            if (line->GetLength() == 0 && !foundTerminator) {
                *str = NULL;
                StringBuf::Free(line);
                return NOERROR;
            }
            //string lenght exceeds STRING_MAX_LENGTH
            if (!foundTerminator) {
                line->Append((Char8)'\0');
                *str = line->GetPayload();
                StringBuf::Free(line);
                return NOERROR;
            }
        } else if (nextByte == (Byte)'\0') {
             line->Append((Char8)nextByte);
             *str = line->GetPayload();
             StringBuf::Free(line);
             return NOERROR;
        } else {
            line->Append((Char8)nextByte);
        }
    }
    return NOERROR;
}

void CGestureStore::ClearNamedGestures()
{
    HashMap<String, List<IGesture*> *>::Iterator iter = mNamedGestures->Begin();
    for (Int32 i=0; iter!=mNamedGestures->End(); ++iter, ++i) {
        List<IGesture *> *gestures = (iter->mSecond);
        Int32 count = gestures->GetSize();
        for (Int32 j = 0; j < count; j++) {
            IGesture *gesture=(*gestures)[i];
            gesture->Release();
        }
        gestures->Clear();
        delete gestures;
    }
    mNamedGestures->Clear();

}

