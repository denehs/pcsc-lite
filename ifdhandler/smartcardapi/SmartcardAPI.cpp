#include "SmartcardAPI.h"

#include <utils/Debug.h>
#include <utils/Log.h>
#include <utils/String8.h>

#include <private/utils/Static.h>
#include "proxy.h"

#undef LOG_TAG
#define LOG_TAG "ifdhandler SmartcardAPI"

#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__);

namespace android {

	SmartcardAPI::SmartcardAPI()
	{
		mClient.establish();
	}

    String16 SmartcardAPI::getReaders(void)
    {
		return String16("SIM: UICC"); // currently only sim available
    }

    bool SmartcardAPI::isCardPresent(const String16& reader)
    {
    	return mClient.iccCardPresent();
    }

    long long int SmartcardAPI::openBasicChannel(const String16& reader)
    {
		LOGI("openBasicChannel");	 
		return -1;
    }


    long long int SmartcardAPI::openBasicChannelAid(const String16& reader, const String16& aid)
    {
		LOGI("openBasicChannelAid");	
		return -1;
    }
    
    long long int SmartcardAPI::openLogicalChannel(const String16& reader, const String16& aid)
    {
		LOGI("openLogicalChannel");
		return -1;
    }
    
    String16 SmartcardAPI::transmit(const long long int handle, const String16& command)
    {
    	int len;
    	unsigned char *str = convert_string16_to_string8(command, &len);
		LOGI("transmit: %s", str);
		free(str);
		return String16("error in transact_transmit");
    }    

    
    void SmartcardAPI::closeChannel(const long long int handle)
    {
		LOGI("closeChannel");
    }    


    String16 SmartcardAPI::getLastError()
    {
		return String16("");
    }    

}; // namespace android


