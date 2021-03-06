#include <utils/Log.h>
#include <sys/socket.h>
#include <sys/un.h>
#include "IccIPCClient.h"
#include "proxy.h"

#undef LOG_TAG
#define LOG_TAG "IccIPCClient"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__);

#define CLIENT_SOCK_FILE "/dev/socket/icc.client.sock"
#define SERVER_SOCK_FILE "/dev/socket/icc.server.sock"

#define ICC_IPC_COMMAND_OPEN_CHANNEL "iccOpenChannel"
#define ICC_IPC_COMMAND_CLOSE_CHANNEL "iccCloseChannel"
#define ICC_IPC_COMMAND_EXCHANGE_APDU "iccExchangeAPDU"
#define ICC_IPC_COMMAND_CARD_PRESENT "iccCardPresent"

namespace android {

	void IccIPCBuffer::clear() {
		mPosition = 0;
		mLength = 0;
	}

	void IccIPCBuffer::addString(char *str) {
		strcpy(mBuffer+mLength, str);
		mLength += strlen(str);
		mBuffer[mLength++] = '\0';
	}

	void IccIPCBuffer::addByte(int byte) {
		unsigned char b = byte & 0xff;
		*(mBuffer+mLength) = b;
		mLength++;
	}

	void IccIPCBuffer::addString16(String16 str) {
		int len;
		unsigned char *buff = convert_string16_to_buffer(str, &len);
		*(mBuffer+mLength) = len;
		mLength++;
		for (int i=0; i<len; i++)
			*(mBuffer+mLength+i) = buff[i];
		mLength += len;
		free(buff);
	}

	bool IccIPCBuffer::readBoolean() {
		bool *ptr = (bool *)(mBuffer + mPosition);
		mPosition ++;
		return *ptr;
	}

	String16 IccIPCBuffer::readString16() {
		int len = *((unsigned char *)(mBuffer + mPosition));
		mPosition++;
		String16 res = convert_buffer_to_string16((unsigned char *)(mBuffer + mPosition), len);
		return res;
	}

	IccIPCClient::IccIPCClient()
		: mFd (-1)
		, mConnected (false)
	{
	}

	bool IccIPCClient::establish() {
		LOGI("establish");
		struct sockaddr_un addr;
		mFd = socket(PF_UNIX, SOCK_DGRAM, 0);
		if (mFd < 0) {
			LOGI("socket error");
			return false;
		}
		memset(&addr, 0, sizeof(addr));
		addr.sun_family = AF_UNIX;
		strcpy(addr.sun_path, CLIENT_SOCK_FILE);
		unlink(CLIENT_SOCK_FILE);
		if (bind(mFd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
			LOGI("bind error");
			return false;
		}

		memset(&addr, 0, sizeof(addr));
		addr.sun_family = AF_UNIX;
		strcpy(addr.sun_path, SERVER_SOCK_FILE);
		if (connect(mFd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
			LOGI("connect error");
			return false;
		}
		mBuffer.clear();
		return true;
	}

	bool IccIPCClient::sendCommand() {
		if (send(mFd, mBuffer.mBuffer, mBuffer.mLength, 0) < 0) {
			LOGI("send error");
			return false;
		}
		mBuffer.mPosition = 0;
		mBuffer.mLength = recv(mFd, mBuffer.mBuffer, MAX_RESPONSE_SIZE+1, 0);
		if (mBuffer.mLength < 0) {
			LOGI("recv error");
			mBuffer.mLength = 0;
			return false;
		}
		else {
			LOGI("recv %d bytes", mBuffer.mLength);
		}
		return true;
	}

	bool IccIPCClient::isConnected() {
		return mConnected;
	}

	void IccIPCClient::checkConnection() {
		if (!isConnected()) {
			establish();
		}
	}

	int IccIPCClient::iccOpenChannel(const String16& aid) {
		LOGI("iccOpenChannel");
		checkConnection();
		return 1;
	}

	bool IccIPCClient::iccCloseChannel(int channel) {
		LOGI("iccCloseChannel");
		checkConnection();
		return true;
	}

	String16 IccIPCClient::iccExchangeAPDU(
		uint32 cla,
		uint32 command,
		uint32 channel,
		uint32 p1,
		uint32 p2,
		uint32 p3,
		const String16& data) {
		LOGI("iccExchangeAPDU");
		checkConnection();
		mBuffer.clear();
		mBuffer.addString(ICC_IPC_COMMAND_EXCHANGE_APDU);
		mBuffer.addByte(cla);
		mBuffer.addByte(command);
		mBuffer.addByte(channel);
		mBuffer.addByte(p1);
		mBuffer.addByte(p2);
		mBuffer.addByte(p3);
		mBuffer.addString16(data);
		sendCommand();
		return mBuffer.readString16();
	}

	bool IccIPCClient::iccCardPresent() {
		LOGI("iccCardPresent");
		checkConnection();
		mBuffer.clear();
		mBuffer.addString(ICC_IPC_COMMAND_CARD_PRESENT);
		sendCommand();
		return mBuffer.readBoolean();
	}
}