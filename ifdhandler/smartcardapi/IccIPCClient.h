#ifndef _icc_ipc_client_h_
#define _icc_ipc_client_h_

#include <utils/String16.h>
#include "pcscdefines.h"

#define uint32 unsigned int

namespace android {
	class IccIPCBuffer
	{
		friend class IccIPCClient;
	public:
		void clear();
		void addString(char *str);
		void addByte(int byte);
		void addString16(String16 str);
		bool readBoolean();
		String16 readString16();

	private:
		char mBuffer[MAX_RESPONSE_SIZE+1];
		int mLength;
		int mPosition;
	};

	class IccIPCClient
	{
	public:
		IccIPCClient();
		bool establish();
		bool isConnected();
		bool sendCommand();

		int iccOpenChannel(const String16& aid);
		bool iccCloseChannel(int channel);
		String16 iccExchangeAPDU(
			uint32 cla,
			uint32 command,
			uint32 channel,
			uint32 p1,
			uint32 p2,
			uint32 p3,
			const String16& data);
		bool iccCardPresent();

	private:
		void checkConnection();

	private:
		int mFd;
		bool mConnected;
		IccIPCBuffer mBuffer;
	};
}

#endif