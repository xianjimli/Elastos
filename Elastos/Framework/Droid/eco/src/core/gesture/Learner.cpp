
#include <gesture/Learner.h>

/**
 * The abstract class of a gesture learner
 */
Learner::~Learner()
{
    mInstances.Clear();
//    delete mInstances;
}

/**
 * Add an instance to the learner
 *
 * @param instance
 */
ECode Learner::AddInstance(IInstance* instance)
{
    VALIDATE_NOT_NULL(instance);
    mInstances.PushBack(instance);
    return NOERROR;
}

/**
 * Retrieve all the instances
 *
 * @return instances
 */
//List<IInstance* >*
ECode Learner::GetInstances(
    /* [out] */ IObjectContainer ** ret)
{
    VALIDATE_NOT_NULL(ret);
    //return (&mInstances);
	//return CloneInstances();
    IObjectContainer* objc = NULL;
    CObjectContainer::New(&objc);
    Int32 size=mInstances.GetSize();
    for (Int32 i=0; i<size; i++)
    {
        //ret->PushBack(mInstances[i]);
        objc->Add((IInterface*) mInstances[i]);
    }
    *ret = objc;
    return NOERROR;
}

/**
 * Remove an instance based on its id
 *
 * @param id
 */

ECode Learner::RemoveInstance(
    /* [in] */Int64 id)
{
    List<IInstance*>instances = mInstances; //CloneInstances(); //mInstances;
    Int32 count = mInstances.GetSize();
    for (Int32 i = 0; i < count; i++)
    {
        IInstance* instance = mInstances[i];
        Int64 instanceId;
        instance->GetId(&instanceId);
        if (id == instanceId)//instance->GetId())
        {
        	mInstances.Remove(instance);

        }
    }
    return NOERROR;
}

/**
 * Remove all the instances of a category
 *
 * @param name the category name
 */

ECode Learner::RemoveInstances(
    /* [in] */ const String& name)
{
    //List<IInstance* > toDelete = new List<IInstance* >();
    //List<IInstance* > instances = mInstances;
    Int32 count = mInstances.GetSize();

    for (Int32 i = 0; i < count; i++)
    {
        IInstance* instance = mInstances[i];
        // the label can be null, as specified in Instance
        String label;
        instance->GetLabel(&label);
        if ((label.IsNull() && name.IsNull()) //name==null
            || (!label.IsNull() && label.Equals(name)))
        {
        	mInstances.Remove(instance);
        }
    }
    return NOERROR;
}

ECode Learner::Classify(
    /* [in] */ Int32 sequenceType,
    /* [in] */ Int32 orientationType,
    /* [in] */ ArrayOf<Float>* vector,
    /* [out] */ IObjectContainer ** ret)
{
    return NOERROR;
}

/*
IObjectContainer* Learner::CloneInstances()
{
    IObjectContainer* objc = NULL;
    CObjectContainer::New(&objc);
    //List<IInstance* >* ret=new List<IInstance* >();
    Int32 size=mInstances.GetSize();
    for (Int32 i=0; i<size; i++)
    {
        //ret->PushBack(mInstances[i]);
    	objc->Add(mInstances[i]);
    }
    return ret;
}
*/

