
#ifndef __CGESTURESTORE_H__
#define __CGESTURESTORE_H__

#include "_CGestureStore.h"
#include "ext/frameworkext.h"
#include <elastos.h>
#include <os/SystemClock.h>
#include <gesture/Learner.h>
#include <elastos/List.h>
#include <elastos/HashMap.h>
#include <elastos/Set.h>
#include <elastos/Map.h>
#include <Elastos.IO.h>

using namespace Elastos;
using namespace Elastos::Core;

/**
 * A timed point of a gesture stroke. Multiple points form a stroke.
 */
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
CarClass(CGestureStore)
{

public:
	CGestureStore();

    virtual ~CGestureStore();

    CARAPI constructor();

    /**
     * Specify how the gesture library will handle orientation. 
     * Use ORIENTATION_INVARIANT or ORIENTATION_SENSITIVE
     * 
     * @param style
     */
    CARAPI SetOrientationStyle(
        /* [in] */ Int32 style);

    CARAPI GetOrientationStyle(
        /* [out] */ Int32 * orientationStyle);

    /**
     * @param type SEQUENCE_INVARIANT or SEQUENCE_SENSITIVE
     */
    CARAPI SetSequenceType(
        /* [in] */ Int32 type);

    /**
     * @return SEQUENCE_INVARIANT or SEQUENCE_SENSITIVE
     */
    CARAPI GetSequenceType(
        /* [out] */ Int32 * sequenceType);

    /**
     * Get all the gesture entry names in the library
     * 
     * @return a set of strings 
     *///IObjectContainer<String>*
    //
    //CARAPI GetGestureEntries(
    //    /* [out] */ IObjectContainer ** gestureEntries);

    ///IObjectContainer<String>*
    CARAPI GetGestureEntries(
       /* [out, callee] */ ArrayOf<String> ** gestureEntries);

    /**
     * Recognize a gesture
     * 
     * @param gesture the query
     * @return a list of predictions of possible entries for a given gesture
     */ //IObjectContainer<IPrediction*> 
    CARAPI Recognize( 
         /* [in] */ IGesture *gesture,
        /* [out] */ IObjectContainer ** arrayList);

    /**
     * Add a gesture for the entry
     * 
     * @param entryName entry name
     * @param gesture
     */
    CARAPI AddGesture(
         /* [in] */ const String & entryName,
         /* [in] */ IGesture *gesture);
    /**
     * Remove a gesture from the library. If there are no more gestures for the
     * given entry, the gesture entry will be removed.
     * 
     * @param entryName entry name
     * @param gesture
     */
    CARAPI RemoveGesture(
        /* [in] */ const String & entryName,
        /* [in] */ IGesture *gesture);

    /**
     * Remove a entry of gestures
     *
     * @param entryName the entry name
     */
    CARAPI RemoveEntry(
        /* [in] */ const String & entryName);

    /**
     * Get all the gestures of an entry
     * 
     * @param entryName
     * @return the list of gestures that is under this name
     */ //IObjectContainer<IGesture*> *
    CARAPI GetGestures(
        /* [in] */ const String & entryName,
        /* [out] */ IObjectContainer ** gestures);

    CARAPI HasChanged(
        /* [out] */ Boolean * hasChanged);

    /**
     * Save the gesture library
     */
    CARAPI Save(
        /* [in] */ IOutputStream *stream);

    CARAPI SaveEx(
        /* [in] */ IOutputStream *stream,
        /* [in] */ Boolean closeStream);

    /**
     * Load the gesture library
     */
    CARAPI Load(
        /* [in] */ IInputStream *stream);

    CARAPI LoadEx(
        /* [in] */ IInputStream *stream,
        /* [in] */ Boolean closeStream);

    CARAPI GetLearner(
        /* [out] */ ILearner ** learner);

private:
    CARAPI ReadFormatV1(
        /* [in] */ IDataInputStream * in);

    static const Int16 FILE_FORMAT_VERSION;// = 1;

    static const Boolean PROFILE_LOADING_SAVING;// = FALSE;

    Int32 mSequenceType;// = CGestureStore::SEQUENCE_SENSITIVE;
    Int32 mOrientationStyle;// = CGestureStore::ORIENTATION_SENSITIVE;

    HashMap<String, List<IGesture*>* >* mNamedGestures;
    //        new HashMap<String, List<CGesture*> *>();

    //private Learner mClassifier;
    //Learner class is with virtual method,
    //so it can not be created with an instance, use IInstanceLearner instead.

    ILearner* mClassifier;

    Boolean mChanged;// = FALSE;
    
    static CString LOG_TAG;// = CGestureConstants::LOG_TAG;

    static const Int32 STRING_MAX_LENGTH;// = 128;

    // new mNamedGestures instance
    void Init();

    CARAPI_(ArrayOf<Byte>*) StringToByteArray(
        /* [in] */ const String& str);

    CARAPI ReadString(
        /* [in] */ IDataInputStream *in,
        /* [out] */ String* str);

    void ClearNamedGestures();
};

#endif //__CGESTURESTORE_H__

