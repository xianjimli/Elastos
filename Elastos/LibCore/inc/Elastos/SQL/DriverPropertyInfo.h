#ifdef _DRIVERPROPERTYINFO_H_
#define _DRIVERPROPERTYINFO_H_

#include <elastos.h>

using namespace Elastos;

class DriverPropertyInfo {
public:
    DriverPropertyInfo();

    ~DriverPropertyInfo();

    /**
     * Creates a {@code DriverPropertyInfo} instance with the supplied name and
     * value. Other class members take their default values.
     *
     * @param name
     *            The property name.
     * @param value
     *            The property value.
     */
    CARAPI Init(
        /**[in]**/String name, 
        /**[in]**/String value);
public:
    /**
     * If the value member can be chosen from a set of possible values, they are
     * contained here. Otherwise choices is {@code null}.
     */
    String[] mChoices;

    /**
     * A description of the property. May be {@code null}.
     */
    String mDescription;

    /**
     * The name of the property.
     */
    String mName;

    /**
     * {@code true} when the value member must be provided during {@code
     * Driver.connect}. {@code false} otherwise.
     */
    Boolean mRequired;

    /**
     * The current value associated with this property. It is depending on the
     * data gathered by the {@code getPropertyInfo} method, the general Java
     * environment and the driver's default values.
     */
    String mValue;
};
#endif//_DRIVERPROPERTYINFO_H_