/*
 * MUSCLE SmartCard Development ( http://www.linuxnet.com )
 *
 * Copyright (C) 2001-2004
 *  David Corcoran <corcoran@linuxnet.com>
 * Copyright (C) 2003-2004
 *  Damien Sauveron <damien.sauveron@labri.fr>
 * Copyright (C) 2002-2009
 *  Ludovic Rousseau <ludovic.rousseau@free.fr>
 *
 * $Id: winscard_msg.h 4967 2010-05-29 11:28:16Z rousseau $
 */

/**
 * @file
 * @brief This defines some structures and \#defines to be used over
 * the transport layer.
 */

#ifndef __winscard_msg_h__
#define __winscard_msg_h__

#include <stdint.h>

/** Major version of the current message protocol */
#define PROTOCOL_VERSION_MAJOR 4
/** Minor version of the current message protocol */
#define PROTOCOL_VERSION_MINOR 0

#ifdef __cplusplus
extern "C"
{
#endif

	/**
	 * @brief Information transmitted in \ref CMD_VERSION Messages.
	 */
	struct version_struct
	{
		int32_t major;	/**< IPC major \ref PROTOCOL_VERSION_MAJOR */
		int32_t minor;	/**< IPC minor \ref PROTOCOL_VERSION_MINOR */
		uint32_t rv;
	};

	/**
	 * @brief header structure for client/server message data exchange.
	 */
	struct rxHeader
	{
		uint32_t size;		/**< size of the message expluding this header */
		uint32_t command;	/**< one of the \c pcsc_msg_commands */
	};

	/**
	 * @brief Commands available to use in the field \c sharedSegmentMsg.command.
	 */
	enum pcsc_msg_commands
	{
		CMD_ENUM_FIRST,
		SCARD_ESTABLISH_CONTEXT = 0x01,	/**< used by SCardEstablishContext() */
		SCARD_RELEASE_CONTEXT = 0x02,	/**< used by SCardReleaseContext() */
		SCARD_LIST_READERS = 0x03,		/**< used by SCardListReaders() */
		SCARD_CONNECT = 0x04,			/**< used by SCardConnect() */
		SCARD_RECONNECT = 0x05,			/**< used by SCardReconnect() */
		SCARD_DISCONNECT = 0x06,		/**< used by SCardDisconnect() */
		SCARD_BEGIN_TRANSACTION = 0x07,	/**< used by SCardBeginTransaction() */
		SCARD_END_TRANSACTION = 0x08,	/**< used by SCardEndTransaction() */
		SCARD_TRANSMIT = 0x09,			/**< used by SCardTransmit() */
		SCARD_CONTROL = 0x0A,			/**< used by SCardControl() */
		SCARD_STATUS = 0x0B,			/**< used by SCardStatus() */
		SCARD_GET_STATUS_CHANGE = 0x0C,	/**< used by SCardGetStatusChange() */
		SCARD_CANCEL = 0x0D,			/**< used by SCardCancel() */
		SCARD_CANCEL_TRANSACTION = 0x0E,	/**< used by SCardCancelTransaction() */
		SCARD_GET_ATTRIB = 0x0F,		/**< used by SCardGetAttrib() */
		SCARD_SET_ATTRIB = 0x10,		/**< used by SCardSetAttrib() */
		CMD_VERSION = 0x11,				/**< get the client/server protocol version */
		CMD_GET_READERS_STATE = 0x12,	/**< get the readers state */
		CMD_WAIT_READER_STATE_CHANGE = 0x13,	/**< wait for a reader state change */
		CMD_STOP_WAITING_READER_STATE_CHANGE = 0x14,	/**< stop waiting for a reader state change */
		CMD_ENUM_LAST
	};

	struct client_struct
	{
		uint32_t hContext;
	};

	/**
	 * @brief Information contained in \ref CMD_WAIT_READER_STATE_CHANGE Messages.
	 */
	struct wait_reader_state_change
	{
		uint32_t timeOut;	/**< timeout in ms */
		uint32_t rv;
	};

	/**
	 * @brief Information contained in \ref SCARD_ESTABLISH_CONTEXT Messages.
	 *
	 * These data are passed throw the field \c sharedSegmentMsg.data.
	 */
	struct establish_struct
	{
		uint32_t dwScope;
		uint32_t hContext;
		uint32_t rv;
	};

	/**
	 * @brief Information contained in \ref SCARD_RELEASE_CONTEXT Messages.
	 *
	 * These data are passed throw the field \c sharedSegmentMsg.data.
	 */
	struct release_struct
	{
		uint32_t hContext;
		uint32_t rv;
	};

	/**
	 * @brief contained in \ref SCARD_CONNECT Messages.
	 *
	 * These data are passed throw the field \c sharedSegmentMsg.data.
	 */
	struct connect_struct
	{
		uint32_t hContext;
		char szReader[MAX_READERNAME];
		uint32_t dwShareMode;
		uint32_t dwPreferredProtocols;
		int32_t hCard;
		uint32_t dwActiveProtocol;
		uint32_t rv;
	};

	/**
	 * @brief contained in \ref SCARD_RECONNECT Messages.
	 *
	 * These data are passed throw the field \c sharedSegmentMsg.data.
	 */
	struct reconnect_struct
	{
		int32_t hCard;
		uint32_t dwShareMode;
		uint32_t dwPreferredProtocols;
		uint32_t dwInitialization;
		uint32_t dwActiveProtocol;
		uint32_t rv;
	};

	/**
	 * @brief contained in \ref SCARD_DISCONNECT Messages.
	 *
	 * These data are passed throw the field \c sharedSegmentMsg.data.
	 */
	struct disconnect_struct
	{
		int32_t hCard;
		uint32_t dwDisposition;
		uint32_t rv;
	};

	/**
	 * @brief contained in \ref SCARD_BEGIN_TRANSACTION Messages.
	 *
	 * These data are passed throw the field \c sharedSegmentMsg.data.
	 */
	struct begin_struct
	{
		int32_t hCard;
		uint32_t rv;
	};

	/**
	 * @brief contained in \ref SCARD_END_TRANSACTION Messages.
	 *
	 * These data are passed throw the field \c sharedSegmentMsg.data.
	 */
	struct end_struct
	{
		int32_t hCard;
		uint32_t dwDisposition;
		uint32_t rv;
	};

	/**
	 * @brief contained in \ref SCARD_CANCEL Messages.
	 *
	 * These data are passed throw the field \c sharedSegmentMsg.data.
	 */
	struct cancel_struct
	{
		int32_t hContext;
		uint32_t rv;
	};

	/**
	 * @brief contained in \ref SCARD_CANCEL_TRANSACTION Messages.
	 *
	 * These data are passed throw the field \c sharedSegmentMsg.data.
	 */
	struct cancel_transaction_struct
	{
		int32_t hCard;
		uint32_t rv;
	};

	/**
	 * @brief contained in \ref SCARD_STATUS Messages.
	 *
	 * These data are passed throw the field \c sharedSegmentMsg.data.
	 */
	struct status_struct
	{
		int32_t hCard;
		char mszReaderNames[MAX_READERNAME];
		uint32_t pcchReaderLen;
		uint32_t dwState;
		uint32_t dwProtocol;
		uint8_t pbAtr[MAX_ATR_SIZE];
		uint32_t pcbAtrLen;
		uint32_t rv;
	};

	/**
	 * @brief contained in \ref SCARD_TRANSMIT Messages.
	 *
	 * These data are passed throw the field \c sharedSegmentMsg.data.
	 */
	struct transmit_struct
	{
		int32_t hCard;
		uint32_t ioSendPciProtocol;
		uint32_t ioSendPciLength;
		uint32_t cbSendLength;
		uint32_t ioRecvPciProtocol;
		uint32_t ioRecvPciLength;
		uint32_t pcbRecvLength;
		uint32_t rv;
	};

	/**
	 * @brief contained in \ref SCARD_CONTROL Messages.
	 *
	 * These data are passed throw the field \c sharedSegmentMsg.data.
	 */
	struct control_struct
	{
		int32_t hCard;
		uint32_t dwControlCode;
		uint32_t cbSendLength;
		uint32_t cbRecvLength;
		uint32_t dwBytesReturned;
		uint32_t rv;
	};

	/**
	 * @brief contained in \ref SCARD_GET_ATTRIB and \c  Messages.
	 *
	 * These data are passed throw the field \c sharedSegmentMsg.data.
	 */
	struct getset_struct
	{
		int32_t hCard;
		uint32_t dwAttrId;
		uint8_t pbAttr[MAX_BUFFER_SIZE];
		uint32_t cbAttrLen;
		uint32_t rv;
	};

	/*
	 * Now some function definitions
	 */

	int32_t ClientSetupSession(uint32_t *);
	int32_t ClientCloseSession(uint32_t);
	int32_t InitializeSocket(void);
	int32_t ProcessEventsServer(/*@out@*/ uint32_t *);
	int32_t MessageSend(void *buffer, uint64_t buffer_size, int32_t filedes);
	int32_t MessageReceive(/*@out@*/ void *buffer, uint64_t buffer_size,
		int32_t filedes);
	int32_t MessageReceiveTimeout(uint32_t command, /*@out@*/ void *buffer,
		uint64_t buffer_size, int32_t filedes, int32_t timeOut);
	int32_t MessageSendWithHeader(uint32_t command, uint32_t dwClientID, uint64_t size, void *data);
	void CleanupSharedSegment(int32_t, const char *);

#ifdef __cplusplus
}
#endif

#endif
