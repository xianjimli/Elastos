
#ifndef __CLEARNER_H__
#define __CLEARNER_H__

#include "_CGesturePoint.h"
#include "ext/frameworkext.h" //for interface I[class name]
#include <elastos/List.h>

using namespace Elastos;
using namespace Elastos::Core;

/**
 * The abstract class of a gesture learner
 */
class Learner//: public ILearner
{
public:
    virtual ~Learner();

    /**
     * Add an instance to the learner
     * 
     * @param instance
     */
    CARAPI AddInstance(
        /* [in] */ IInstance* instance);

    /**
     * Retrieve all the instances
     * 
     * @return instances
     */
    //List<IInstance *> *)
    CARAPI GetInstances(
        /* [out] */ IObjectContainer ** ret);

    /**
     * Remove an instance based on its id
     * 
     * @param id
     */
    CARAPI RemoveInstance(
        /* [in] */ Int64 id);

    /**
     * Remove all the instances of a category
     * 
     * @param name the category name
     */
    CARAPI RemoveInstances(
        /* [in] */ const String& name);

    //private final ArrayList<Instance> mInstances = new ArrayList<Instance>();
    virtual CARAPI Classify(
        /* [in] */ Int32 sequenceType,
        /* [in] */ Int32 orientationType,
        /* [in] */ ArrayOf<Float>* vector,
        /* [out] */ IObjectContainer ** ret); //= 0;

private:
    List<IInstance *> mInstances;

};

#endif // __CLEARNER_H__
