#ifndef _smart_card_api_h
#define _smart_card_api_h

#include <utils/RefBase.h>
#include <utils/String16.h>
#include "IccIPCClient.h"

namespace android {

class SmartcardAPI : public virtual RefBase
{
public:
    SmartcardAPI();
    String16 getReaders(void);
    bool isCardPresent(const String16& reader);
    long long int openBasicChannel(const String16& reader);
    long long int openBasicChannelAid(const String16& reader, const String16& aid);
    long long int openLogicalChannel(const String16& reader, const String16& aid);
    String16 transmit(const long long int handle, const String16& command);
    void closeChannel(const long long int handle);
    String16 getLastError(void);
private:
    int parseChannelNumber(int cla);
private:
    IccIPCClient mClient;
}; //class

}; //namespace


#endif