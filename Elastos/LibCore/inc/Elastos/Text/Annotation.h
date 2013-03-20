#ifndef __ANNOTATION_H__
#define __ANNOTATION_H__

#include "cmdef.h"
#include <elastos.h>
#include <elastos/AutoPtr.h>
#include "Elastos.Text_server.h"

using namespace Elastos;

class Annotation {
public:
    /**
     * Constructs a new {@code Annotation}.
     *
     * @param attribute the attribute attached to this annotation. This may be
     *        {@code null}.
     */
    CARAPI Init(
        /* [in] */ IInterface* attribute);

    /**
     * Returns the value of this annotation. The value may be {@code null}.
     *
     * @return the value of this annotation or {@code null}.
     */
    virtual CARAPI GetValue(
        /* [out] */ IInterface** value);

    /**
     * Returns this annotation in string representation.
     *
     * @return the string representation of this annotation.
     */
    //@Override
    //public String toString() {
    //    return getClass().getName() + "[value=" + value + ']';
    //}

private:
    AutoPtr<IInterface> value;

};
#endif //__ANNOTATION_H__