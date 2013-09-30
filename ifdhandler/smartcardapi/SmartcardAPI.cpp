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
    	unsigned char *buff = convert_string16_to_buffer(command, &len);
		LOGI("transmit: %d", len);

		int cla = buff[0] & 0xff;
		int ins = buff[1] & 0xff;
		int p1 = buff[2] & 0xff;
		int p2 = buff[3] & 0xff;
		int p3 = -1;
		if (len > 4) {
			p3 = buff[4] & 0xff;
		}
		String16 data = String16("");
		if (len > 5) {
			data = String16(command, len-5, 5);
		}

		int channelNumber = parseChannelNumber(cla);
		String16 res = String16("9000");


		if (channelNumber == 0) {
			res = mClient.iccExchangeAPDU(cla, ins, 0, p1, p2, p3, data);
		} else {
			LOGI("not implemented!!!!!! transmit logic channel")
			/*
			if ((channelNumber > 0) && (channelId[channelNumber] == 0)) {
				throw new CardException("channel not open");
			}

			try {
				String response = manager.transmitIccLogicalChannel(cla & 0xFC,
						ins, channelId[channelNumber], p1, p2, p3, data);
				return StringToByteArray(response);
			} catch (Exception ex) {
				throw new CardException("transmit command failed");
			}
			*/
		}

		free(buff);
		return res;
    }    

    
    void SmartcardAPI::closeChannel(const long long int handle)
    {
		LOGI("closeChannel");
    }    


    String16 SmartcardAPI::getLastError()
    {
		return String16("");
    }

    int SmartcardAPI::parseChannelNumber(int cla) {
		// bit 7 determines which standard is used
		bool isFirstInterindustryClassByteCoding = ((cla & 0x40) == 0x00);
		
		if(isFirstInterindustryClassByteCoding){
			// First Interindustry Class Byte Coding
			// see 11.1.4.1: channel number is encoded in the 2 rightmost bits
			return cla & 0x03;
		}else{
			// Further Interindustry Class Byte Coding
			// see 11.1.4.2: channel number is encoded in the 4 rightmost bits
			return (cla & 0x0F) + 4;
		}
    }

}; // namespace android


