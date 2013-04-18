
#include "utils/HierarchicalState.h"


/**
 * @return name of state, but default returns the states
 * class name. An instance name would be better but requiring
 * it seems unnecessary.
 */
String HierarchicalState::GetName()
{
    //String name = getClass().getName();
    //int lastDollar = name.lastIndexOf('$');
    //return name.substring(lastDollar + 1);
    return String("HierarchicalState");
}
