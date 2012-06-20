
#ifndef __CAPSULEMANIFESTHANDLER_H__
#define __CAPSULEMANIFESTHANDLER_H__

#include "xg/XG_StaticStack.h"
#include "xmlparser/XG_SAXHandler.h"

#include "ext/frameworkdef.h"
#include <elastos/AutoPtr.h>
#include "capsule/CapsuleParser.h"

class CapsuleManifestHandler : public XG::SAXHandler
{
public:
    CapsuleManifestHandler(
        /* [in] */ CapsuleParser* parser,
        /* [in] */ CapsuleParser::Capsule* capsule) :
        mParser(parser), mCapsule(capsule)
    {}

protected:
    int StartDocument();
    int StartElement(const char * uri, const char * localName, XG::Attributes & attrs);
    int EndElement(const char * uri, const char * localName);

private:

private:
    CapsuleParser* mParser;
    CapsuleParser::Capsule* mCapsule;

    class State
    {
    public:
        enum ST
        {
            ST_Doc,
            ST_Manifest,
            ST_Application,
            ST_Activity,
            ST_Receiver,
            ST_Service,
            ST_Provider,
            ST_IntentFilter,
            ST_Action,
            ST_Category,
        };

        State(ST s, void* data) : mState(s), mData(data) {}

        ST GetState()
        {
            return mState;
        }

        void* GetData()
        {
            return mData;
        }

    private:
        ST mState;
        void* mData;
    };

    XG::StaticStack<State, 32> mState;
};


#endif //__CAPSULEMANIFESTHANDLER_H__
