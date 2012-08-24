
#include "ext/frameworkdef.h"
#include "capsule/CapsuleManifestHandler.h"
#include "capsule/CActivityInfo.h"

#ifdef NULL
#undef NULL
#define NULL 0
#endif

int CapsuleManifestHandler::StartDocument()
{
    mState.Push(State(State::ST_Doc, 0));
    return 0;
}

int CapsuleManifestHandler::StartElement(const char * uri, const char * localName, XG::Attributes & attrs)
{
    int rc = 0;
    State parent = mState.Top();
    if (!strcmp("manifest", localName)) {
        if (parent.GetState() == State::ST_Doc) {
            mCapsule->mCapsuleName = String::Duplicate(attrs.NSGetValue(XML_NAMESPACE, "capsule"));
            mCapsule->mApplicationInfo->mCapsuleName = mCapsule->mCapsuleName;
            mState.Push(State(State::ST_Manifest, NULL));
        }
        else {
            rc = -1;
            m_err = "Element \"manifest\" must be the top element!";
        }
    }
    else if (!strcmp("application", localName)) {
        if (parent.GetState() == State::ST_Manifest) {

            assert(mParser->ParseApplication(mCapsule, attrs) == TRUE);

            mState.Push(State(State::ST_Application, NULL));
        }
        else {
            rc = -1;
            m_err = "Element \"application\" must be the direct sub-element of the element \"manifest\"!";
        }
    }
    else if (!strcmp("activity", localName)) {
        if (parent.GetState() == State::ST_Application) {
            CapsuleParser::Activity* activity =
                    mParser->ParseActivity(mCapsule, attrs);

            mState.Push(State(State::ST_Activity, (void*)activity));
        }
        else {
            rc = -1;
            m_err = "Element \"activity\" must be the direct sub-element of the element \"application\"!";
        }
    }
    else if (!strcmp("receiver", localName)) {
        if (parent.GetState() == State::ST_Application) {
            CapsuleParser::Activity* receiver =
                    mParser->ParseBroadcastReceiver(mCapsule, attrs);

            mState.Push(State(State::ST_Receiver, (void*)receiver));
        }
        else {
            rc = -1;
            m_err = "Element \"receiver\" must be the direct sub-element of the element \"application\"!";
        }
    }
    else if (!strcmp("service", localName)) {
        if (parent.GetState() == State::ST_Application) {
            CapsuleParser::Service* service =
                    mParser->ParseService(mCapsule, attrs);

            mState.Push(State(State::ST_Service, (void*)service));
        }
        else {
            rc = -1;
            m_err = "Element \"service\" must be the direct sub-element of the element \"application\"!";
        }
    }
    else if (!strcmp("provider", localName)) {
        if (parent.GetState() == State::ST_Application) {
            CapsuleParser::ContentProvider* provider =
                    mParser->ParseContentProvider(mCapsule, attrs);

            mState.Push(State(State::ST_Provider, (void*)provider));
        }
        else {
            rc = -1;
            m_err = "Element \"provider\" must be the direct sub-element of the element \"application\"!";
        }
    }
    else if (!strcmp("intent-filter", localName)) {
        if (parent.GetState() == State::ST_Activity ||
            parent.GetState() == State::ST_Receiver) {
            CapsuleParser::IntentInfo* intentInfo =
                    mParser->ParseIntent(
                            (CapsuleParser::Activity*)parent.GetData(), attrs);

            mState.Push(State(State::ST_IntentFilter, (void*)intentInfo));
        }
        else {
            rc = -1;
            m_err = "Element \"intent-filter\" must be the direct sub-element of the element \"activity\" or \"receiver\"!";
        }
    }
    else if (!strcmp("action", localName)) {
        if (parent.GetState() == State::ST_IntentFilter) {
            mParser->ParseIntentAction(
                    (CapsuleParser::IntentInfo*)parent.GetData(), attrs);

            mState.Push(State(State::ST_Action, NULL));
        }
        else {
            rc = -1;
            m_err = "Element \"action\" must be the direct sub-element of the element \"intent-filter\"!";
        }
    }
    else if (!strcmp("category", localName)) {
        if (parent.GetState() == State::ST_IntentFilter) {
            mParser->ParseIntentCategory(
                    (CapsuleParser::IntentInfo*)parent.GetData(), attrs);

            mState.Push(State(State::ST_Category, NULL));
        }
        else {
            rc = -1;
            m_err = "Element \"category\" must be the direct sub-element of the element \"intent-filter\"!";
        }
    }

    return rc;
}

int CapsuleManifestHandler::EndElement(const char * uri, const char * localName)
{
    mState.Pop();
    return 0;
}
