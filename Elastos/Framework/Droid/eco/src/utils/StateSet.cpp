
#include "utils/StateSet.h"

const ArrayOf<Int32>* StateSet::WILD_CARD = ArrayOf<Int32>::Alloc(0);

/**
 * Return whether the stateSetOrSpec is matched by all StateSets.
 *
 * @param stateSetOrSpec a state set or state spec.
 */
Boolean StateSet::IsWildCard(
    /* [in] */ const ArrayOf<Int32>* stateSetOrSpec)
{
    return stateSetOrSpec->GetLength() == 0 || (*stateSetOrSpec)[0] == 0;
}

/**
 * Return whether the stateSet matches the desired stateSpec.
 *
 * @param stateSpec an array of required (if positive) or
 *        prohibited (if negative) {@link android.view.View} states.
 * @param stateSet an array of {@link android.view.View} states
 */
Boolean StateSet::StateSetMatches(
    /* [in] */ const ArrayOf<Int32>* stateSpec,
    /* [in] */ const ArrayOf<Int32>* stateSet)
{
    if (stateSet == NULL) {
        return (stateSpec == NULL || IsWildCard(stateSpec));
    }
    Int32 stateSpecSize = stateSpec->GetLength();
    Int32 stateSetSize = stateSet->GetLength();
    for (Int32 i = 0; i < stateSpecSize; i++) {
        Int32 stateSpecState = (*stateSpec)[i];
        if (stateSpecState == 0) {
            // We've reached the end of the cases to match against.
            return TRUE;
        }
        Boolean mustMatch;
        if (stateSpecState > 0) {
            mustMatch = TRUE;
        } else {
            // We use negative values to indicate must-NOT-match states.
            mustMatch = FALSE;
            stateSpecState = -stateSpecState;
        }
        Boolean found = FALSE;
        for (Int32 j = 0; j < stateSetSize; j++) {
            Int32 state = (*stateSet)[j];
            if (state == 0) {
                // We've reached the end of states to match.
                if (mustMatch) {
                    // We didn't find this must-match state.
                    return FALSE;
                } else {
                    // Continue checking other must-not-match states.
                    break;
                }
            }
            if (state == stateSpecState) {
                if (mustMatch) {
                    found = TRUE;
                    // Continue checking other other must-match states.
                    break;
                } else {
                    // Any match of a must-not-match state returns FALSE.
                    return FALSE;
                }
            }
        }
        if (mustMatch && !found) {
            // We've reached the end of states to match and we didn't
            // find a must-match state.
            return FALSE;
        }
    }
    return TRUE;
}

/**
 * Return whether the state matches the desired stateSpec.
 *
 * @param stateSpec an array of required (if positive) or
 *        prohibited (if negative) {@link android.view.View} states.
 * @param state a {@link android.view.View} state
 */
Boolean StateSet::StateSetMatches(
    /* [in] */ const ArrayOf<Int32>* stateSpec,
    /* [in] */ Int32 state)
{
    Int32 stateSpecSize = stateSpec->GetLength();
    for (Int32 i = 0; i < stateSpecSize; i++) {
        Int32 stateSpecState = (*stateSpec)[i];
        if (stateSpecState == 0) {
            // We've reached the end of the cases to match against.
            return TRUE;
        }
        if (stateSpecState > 0) {
            if (state != stateSpecState) {
               return FALSE;
            }
        } else {
            // We use negative values to indicate must-NOT-match states.
            if (state == -stateSpecState) {
                // We matched a must-not-match case.
                return FALSE;
            }
        }
    }
    return TRUE;
}

ArrayOf<Int32>* StateSet::TrimStateSet(
    /* [in] */ const ArrayOf<Int32>* states,
    /* [in] */ Int32 newSize)
{
    if (states->GetLength() == newSize) {
        return states->Clone();
    }

    assert(newSize < states->GetLength());
    ArrayOf<Int32>* trimmedStates = ArrayOf<Int32>::Alloc(newSize);
    memcpy(trimmedStates->GetPayload(), states->GetPayload(), newSize * sizeof(Int32));

    return trimmedStates;
}

//public static String dump(Int32[] states) {
//    StringBuilder sb = new StringBuilder();
//
//    Int32 count = states.length;
//    for (Int32 i = 0; i < count; i++) {
//
//        switch (states[i]) {
//        case R.attr.state_window_focused:
//            sb.append("W ");
//            break;
//        case R.attr.state_pressed:
//            sb.append("P ");
//            break;
//        case R.attr.state_selected:
//            sb.append("S ");
//            break;
//        case R.attr.state_focused:
//            sb.append("F ");
//            break;
//        case R.attr.state_enabled:
//            sb.append("E ");
//            break;
//        }
//    }
//
//    return sb.toString();
//}

