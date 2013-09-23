/*
 * MUSCLE SmartCard Development ( http://www.linuxnet.com )
 *
 * Copyright (C) 1999
 *  David Corcoran <corcoran@linuxnet.com>
 * Copyright (C) 2004-2008
 *  Ludovic Rousseau <ludovic.rousseau@free.fr>
 *
 * $Id: testpcsc.c 4703 2010-02-02 10:34:18Z rousseau $
 */

/**
 * @file
 * @brief This is a test program for pcsc-lite.
 */

#include "config.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pcsclite.h"
#include "winscard.h"
#include "reader.h"

#define PANIC 0
#define DONT_PANIC 1

#define USE_AUTOALLOCATE

#define BLUE "\33[34m"
#define RED "\33[31m"
#define BRIGHT_RED "\33[01;31m"
#define GREEN "\33[32m"
#define NORMAL "\33[0m"
#define MAGENTA "\33[35m"

static void test_rv(LONG rv, SCARDCONTEXT hContext, int dont_panic)
{
	if (rv != SCARD_S_SUCCESS)
	{
		if (dont_panic)
			printf(BLUE "%s (don't panic)\n" NORMAL, pcsc_stringify_error(rv));
		else
		{
			printf(RED "%s\n" NORMAL, pcsc_stringify_error(rv));
			(void)SCardReleaseContext(hContext);
			exit(-1);
		}
	}
	else
		(void)puts(pcsc_stringify_error(rv));
}


int sc_hex_to_bin(const char *in, char *out, size_t *outlen)
{
	const int GENERAL_ERROR = 1;	
	int err = 0;
	size_t left, count = 0;

	if ( (in == NULL) || (out == NULL) || (outlen == NULL) )
		return GENERAL_ERROR;

        left = *outlen;

	while (*in != '\0') {
		int byte = 0, nybbles = 2;

		while (nybbles-- && *in && *in != ':') {
			char c;
			byte <<= 4;
			c = *in++;
			if ('0' <= c && c <= '9')
				c -= '0';
			else
			if ('a' <= c && c <= 'f')
				c = c - 'a' + 10;
			else
			if ('A' <= c && c <= 'F')
				c = c - 'A' + 10;
			else {
				err = GENERAL_ERROR; //invalid character
				printf("invalid character\n");
				goto out;
			}
			byte |= c;
		}
		if (*in == ':')
			in++;
		if (left <= 0) {
                        err = GENERAL_ERROR; //buffer too small
			printf("buffer too small\n");
			break;
		}
		out[count++] = (char) byte;
		left--;
	}

out:
	*outlen = count;
	return err;
}


int sc_bin_to_hex(const char *in, size_t in_len, char *out, size_t out_len,
		  int in_sep)
{
	const int GENERAL_ERROR = 1;		
	unsigned int	n, sep_len;
	char		*pos, *end, sep;

	sep = (char)in_sep;
	sep_len = sep > 0 ? 1 : 0;
	pos = out;
	end = out + out_len;
	for (n = 0; n < in_len; n++) {
		if (pos + 3 + sep_len >= end)
			return GENERAL_ERROR;
		if (n && sep_len)
			*pos++ = sep;
		sprintf(pos, "%02x", in[n]);
		pos += 2;
	}
	*pos = '\0';
	return 0;
}


void testrun (	SCARDHANDLE* p_hCard, SCARDCONTEXT *p_hContext, SCARD_IO_REQUEST *p_pioSendPci, unsigned char ucLogChannel)
{
	
	SCARDHANDLE hCard = *p_hCard;
	SCARDCONTEXT hContext = *p_hContext;
	long rv;
	DWORD i;
	int p, iReader;
	int iList[16];
	SCARD_IO_REQUEST pioRecvPci;
	SCARD_IO_REQUEST pioSendPci = *p_pioSendPci;
	unsigned char bSendBuffer[MAX_BUFFER_SIZE];
	unsigned char bRecvBuffer[MAX_BUFFER_SIZE];
	DWORD send_length, length;	
	
	/* APDU select file */
	printf("\n------------- Test Channel %d --------------------\n",ucLogChannel);

	printf("Select Applet\n");
	send_length = 12;
	memcpy(bSendBuffer, "\x00\xA4\x04\x00\x07\xD2\x76\x00\x01\x18\x01\x01", send_length);
	bSendBuffer[0]|=ucLogChannel;	
	for (i=0; i<send_length; i++)
		printf(" %02X", bSendBuffer[i]);
	printf("\n");
	length = sizeof(bRecvBuffer);

	printf("Testing SCardTransmit\t\t: ");
	rv = SCardTransmit(hCard, &pioSendPci, bSendBuffer, send_length,
		&pioRecvPci, bRecvBuffer, &length);
	test_rv(rv, hContext, PANIC);
	printf(" card response:" GREEN);
	for (i=0; i<length; i++)
		printf(" %02X", bRecvBuffer[i]);
	printf("\n" NORMAL);

	printf("\nCASE 1 APDU\n");
	send_length = 5;
	memcpy(bSendBuffer, "\x00\x01\x00\x00\x00", send_length);
	bSendBuffer[0]|=ucLogChannel;	
	for (i=0; i<send_length; i++)
		printf(" %02X", bSendBuffer[i]);
	printf("\n");
	length = sizeof(bRecvBuffer);

	printf("Testing SCardTransmit\t\t: ");
	rv = SCardTransmit(hCard, &pioSendPci, bSendBuffer, send_length,
		&pioRecvPci, bRecvBuffer, &length);
	test_rv(rv, hContext, PANIC);
	printf(" card response:" GREEN);
	for (i=0; i<length; i++)
		printf(" %02X", bRecvBuffer[i]);
	printf("\n" NORMAL);

	printf("\nCASE 2 APDU\n");
	send_length = 5;
	memcpy(bSendBuffer, "\x00\x02\x00\x00\x10", send_length);
	bSendBuffer[0]|=ucLogChannel;	
	for (i=0; i<send_length; i++)
		printf(" %02X", bSendBuffer[i]);
	printf("\n");
	length = sizeof(bRecvBuffer);

	printf("Testing SCardTransmit\t\t: ");
	rv = SCardTransmit(hCard, &pioSendPci, bSendBuffer, send_length,
		&pioRecvPci, bRecvBuffer, &length);
	test_rv(rv, hContext, PANIC);
	printf(" card response:" GREEN);
	for (i=0; i<length; i++)
		printf(" %02X", bRecvBuffer[i]);
	printf("\n" NORMAL);

	printf("\nCASE 3 APDU\n");
	send_length = 21;
	memcpy(bSendBuffer, "\x00\x03\x00\x00\x10\x00\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0A\x0B\x0C\x0D\x0E\x0F", send_length);
	bSendBuffer[0]|=ucLogChannel;	
	for (i=0; i<send_length; i++)
		printf(" %02X", bSendBuffer[i]);
	printf("\n");
	length = sizeof(bRecvBuffer);

	printf("Testing SCardTransmit\t\t: ");
	rv = SCardTransmit(hCard, &pioSendPci, bSendBuffer, send_length,
		&pioRecvPci, bRecvBuffer, &length);
	test_rv(rv, hContext, PANIC);
	printf(" card response:" GREEN);
	for (i=0; i<length; i++)
		printf(" %02X", bRecvBuffer[i]);
	printf("\n" NORMAL);

	printf("\nCASE 4 APDU\n");
	send_length = 22;
	memcpy(bSendBuffer, "\x00\x04\x00\x00\x10\x00\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0A\x0B\x0C\x0D\x0E\x0F\00", send_length);
	for (i=0; i<send_length; i++)
		printf(" %02X", bSendBuffer[i]);
	printf("\n");
	length = sizeof(bRecvBuffer);

	printf("Testing SCardTransmit\t\t: ");
	rv = SCardTransmit(hCard, &pioSendPci, bSendBuffer, send_length,
		&pioRecvPci, bRecvBuffer, &length);
	test_rv(rv, hContext, PANIC);
	printf(" card response:" GREEN);
	for (i=0; i<length; i++)
		printf(" %02X", bRecvBuffer[i]);
	printf("\n" NORMAL);


/*
	printf("\nLong APDU\n");
	send_length = 259;  //253 byte command data + 5 byte header + 1 byte Le	
	bSendBuffer[0] = 0x00; 	bSendBuffer[1] = 0x04; 	bSendBuffer[2] = 0x00; 	bSendBuffer[3] = 0x00; 	bSendBuffer[4] = 0xFD;
	unsigned int iIndex = 0;	
	for (iIndex=5;iIndex<send_length-1;++iIndex) { bSendBuffer[iIndex]=iIndex-5; }
	bSendBuffer[send_length-1] = 0x00;

	for (i=0; i<send_length; i++)
		printf(" %02X", bSendBuffer[i]);
	printf("\n");
	length = sizeof(bRecvBuffer);

	printf("Testing SCardTransmit\t\t: ");
	rv = SCardTransmit(hCard, &pioSendPci, bSendBuffer, send_length,
		&pioRecvPci, bRecvBuffer, &length);
	test_rv(rv, hContext, PANIC);
	printf(" card response:" GREEN);
	for (i=0; i<length; i++)
		printf(" %02X", bRecvBuffer[i]);
	printf("\n" NORMAL);
*/

}



int main(/*@unused@*/ int argc, /*@unused@*/ char **argv)
{
	SCARDHANDLE hCard;
	SCARDCONTEXT hContext;
	SCARD_READERSTATE_A rgReaderStates[1];
	DWORD dwReaderLen, dwState, dwProt, dwAtrLen;
	DWORD dwPref, dwReaders = 0;
	char *pcReaders = NULL, *mszReaders;
#ifdef USE_AUTOALLOCATE
	unsigned char *pbAtr = NULL;
#else
	unsigned char pbAtr[MAX_ATR_SIZE];
#endif
	union {
		unsigned char as_char[100];
		DWORD as_DWORD;
		uint32_t as_uint32_t;
	} buf;
	DWORD dwBufLen;
	unsigned char *pbAttr = NULL;
	DWORD pcbAttrLen;
	char *mszGroups;
	DWORD dwGroups = 0;
	long rv;
	DWORD i;
	int p, iReader;
	int iList[16];
	SCARD_IO_REQUEST pioRecvPci;
	SCARD_IO_REQUEST pioSendPci;
	unsigned char bSendBuffer[MAX_BUFFER_SIZE];
	unsigned char bRecvBuffer[MAX_BUFFER_SIZE];
	DWORD send_length, length;

	(void)argc;
	(void)argv;

	printf("\nMUSCLE PC/SC Lite unitary test Program\n\n");

	printf(MAGENTA "THIS PROGRAM IS NOT DESIGNED AS A TESTING TOOL FOR END USERS!\n");
	printf("Do NOT use it unless you really know what you do.\n\n" NORMAL);

	printf("Testing SCardEstablishContext\t: ");
	rv = SCardEstablishContext(SCARD_SCOPE_SYSTEM, NULL, NULL, &hContext);
	test_rv(rv, hContext, PANIC);

	printf("Testing SCardIsValidContext\t: ");
	rv = SCardIsValidContext(hContext);
	test_rv(rv, hContext, PANIC);

	printf("Testing SCardIsValidContext\t: ");
	rv = SCardIsValidContext(hContext+1);
	test_rv(rv, hContext, DONT_PANIC);

	printf("Testing SCardListReaderGroups\t: ");
#ifdef USE_AUTOALLOCATE
	dwGroups = SCARD_AUTOALLOCATE;
	rv = SCardListReaderGroups(hContext, (LPSTR)&mszGroups, &dwGroups);
#else
	rv = SCardListReaderGroups(hContext, NULL, &dwGroups);
	test_rv(rv, hContext, PANIC);

	printf("Testing SCardListReaderGroups\t: ");
	mszGroups = calloc(dwGroups, sizeof(char));
	rv = SCardListReaderGroups(hContext, mszGroups, &dwGroups);
#endif
	test_rv(rv, hContext, PANIC);

	/*
	 * Have to understand the multi-string here
	 */
	p = 0;
	for (i = 0; i+1 < dwGroups; i++)
	{
		++p;
		printf(GREEN "Group %02d: %s\n" NORMAL, p, &mszGroups[i]);
		while (mszGroups[++i] != 0) ;
	}

#ifdef USE_AUTOALLOCATE
	printf("Testing SCardFreeMemory\t\t: ");
	rv = SCardFreeMemory(hContext, mszGroups);
	test_rv(rv, hContext, PANIC);
#else
	free(mszGroups);
#endif

wait_for_card_again:
	mszGroups = NULL;
	printf("Testing SCardListReaders\t: ");
	rv = SCardListReaders(hContext, mszGroups, NULL, &dwReaders);
	test_rv(rv, hContext, DONT_PANIC);
	if (SCARD_E_NO_READERS_AVAILABLE == rv)
	{
		printf("Testing SCardGetStatusChange \n");
		printf("Please insert a working reader\t: ");
		(void)fflush(stdout);
		rgReaderStates[0].szReader = "\\\\?PnP?\\Notification";
		rgReaderStates[0].dwCurrentState = SCARD_STATE_EMPTY;

		rv = SCardGetStatusChange(hContext, INFINITE, rgReaderStates, 1);
		test_rv(rv, hContext, PANIC);
	}

	printf("Testing SCardListReaders\t: ");
#ifdef USE_AUTOALLOCATE
	dwReaders = SCARD_AUTOALLOCATE;
	rv = SCardListReaders(hContext, mszGroups, (LPSTR)&mszReaders, &dwReaders);
#else
	rv = SCardListReaders(hContext, mszGroups, NULL, &dwReaders);
	test_rv(rv, hContext, PANIC);

	printf("Testing SCardListReaders\t: ");
	mszReaders = calloc(dwReaders, sizeof(char));
	rv = SCardListReaders(hContext, mszGroups, mszReaders, &dwReaders);
#endif
	test_rv(rv, hContext, DONT_PANIC);

	/*
	 * Have to understand the multi-string here
	 */
	p = 0;
	for (i = 0; i+1 < dwReaders; i++)
	{
		++p;
		printf(GREEN "Reader %02d: %s\n" NORMAL, p, &mszReaders[i]);
		iList[p] = i;
		while (mszReaders[++i] != 0) ;
	}

	if (p > 1)
		do
		{
			char input[80];

			printf("Enter the reader number\t\t: ");
			(void)fgets(input, sizeof(input), stdin);
			(void)sscanf(input, "%d", &iReader);

			if (iReader > p || iReader <= 0)
				printf("Invalid Value - try again\n");
		}
		while (iReader > p || iReader <= 0);
	else
		iReader = 1;

	rgReaderStates[0].szReader = &mszReaders[iList[iReader]];
	rgReaderStates[0].dwCurrentState = SCARD_STATE_EMPTY;

	printf("Waiting for card insertion\t: ");
	(void)fflush(stdout);
	rv = SCardGetStatusChange(hContext, INFINITE, rgReaderStates, 1);
	test_rv(rv, hContext, PANIC);
	if (rgReaderStates[0].dwEventState & SCARD_STATE_UNKNOWN)
	{
		printf("\nA reader has been connected/disconnected\n");
		goto wait_for_card_again;
	}

	printf("Testing SCardConnect\t\t: ");
	rv = SCardConnect(hContext, &mszReaders[iList[iReader]],
		SCARD_SHARE_SHARED, SCARD_PROTOCOL_T0 | SCARD_PROTOCOL_T1,
		&hCard, &dwPref);
	test_rv(rv, hContext, PANIC);

	switch(dwPref)
	{
		case SCARD_PROTOCOL_T0:
			pioSendPci = *SCARD_PCI_T0;
			break;
		case SCARD_PROTOCOL_T1:
			pioSendPci = *SCARD_PCI_T1;
			break;
		case SCARD_PROTOCOL_RAW:
			pioSendPci = *SCARD_PCI_RAW;
			break;
		default:
			printf("Unknown protocol\n");
			return -1;
	}

	int bBreak = 0;

	while (1)
	{
		char inputCommand[1024];	
		char inputAPDU[1024];
		int inputAPDULen = sizeof(inputAPDU);
		printf("Enter APDU to send, (e.g. 00:A4:04:00:00)\n");
		printf("Command APDU: ");
		(void)fgets(inputCommand, sizeof(inputCommand), stdin);
		
		int stringlen = strlen(inputCommand);
		if( inputCommand[stringlen-1] == '\n' ) { inputCommand[stringlen-1] = 0; } //remove newline

		int bError = sc_hex_to_bin(inputCommand, inputAPDU, &inputAPDULen);
		//printf("debug - value bError: %i\n",bError);
		if (bError) { printf("Error parsing input\n\n"); continue; }

		send_length = inputAPDULen;
		if (inputAPDULen == 0) { break; }
		printf("debug inputAPDULen: %i\n",inputAPDULen); 
		memcpy(bSendBuffer, inputAPDU, send_length);
		length = sizeof(bRecvBuffer);

		printf("Testing SCardTransmit:\n "); printf("-> ");
		for (i=0; i<send_length; i++) { printf(" %02X", bSendBuffer[i]); }
		printf("\n");
		rv = SCardTransmit(hCard, &pioSendPci, bSendBuffer, send_length,
			&pioRecvPci, bRecvBuffer, &length);
		test_rv(rv, hContext, PANIC);
		printf("<- " GREEN);
		for (i=0; i<length; i++)
			printf(" %02X", bRecvBuffer[i]);
		printf("\n" NORMAL);
	}

	testrun(&hCard, &hContext, &pioSendPci, 0);
	testrun(&hCard, &hContext, &pioSendPci, 1);
	testrun(&hCard, &hContext, &pioSendPci, 2);
	testrun(&hCard, &hContext, &pioSendPci, 3);

	printf("Testing SCardControl\t\t: ");
#ifdef PCSC_PRE_120
	{
		char buffer[1024] = "Foobar";
		DWORD cbRecvLength = sizeof(buffer);

		rv = SCardControl(hCard, buffer, 7, buffer, &cbRecvLength);
	}
#else
	{
		char buffer[1024] = { 0x02 };
		DWORD cbRecvLength = sizeof(buffer);

		rv = SCardControl(hCard, SCARD_CTL_CODE(1), buffer, 1, buffer,
			sizeof(buffer), &cbRecvLength);
		if (cbRecvLength && (SCARD_S_SUCCESS == rv))
		{
			for (i=0; i<cbRecvLength; i++)
				printf("%c", buffer[i]);
			printf(" ");
		}
	}
#endif
	test_rv(rv, hContext, DONT_PANIC);

	printf("Testing SCardGetAttrib\t\t: ");
#ifdef USE_AUTOALLOCATE
	pcbAttrLen = SCARD_AUTOALLOCATE;
	rv = SCardGetAttrib(hCard, SCARD_ATTR_DEVICE_FRIENDLY_NAME, (unsigned char *)&pbAttr,
		&pcbAttrLen);
#else
	rv = SCardGetAttrib(hCard, SCARD_ATTR_DEVICE_FRIENDLY_NAME, NULL, &pcbAttrLen);
	test_rv(rv, hContext, DONT_PANIC);
	if (rv == SCARD_S_SUCCESS)
	{
		printf("SCARD_ATTR_DEVICE_FRIENDLY_NAME length: " GREEN "%ld\n" NORMAL, pcbAttrLen);
		pbAttr = malloc(pcbAttrLen);
	}

	printf("Testing SCardGetAttrib\t\t: ");
	rv = SCardGetAttrib(hCard, SCARD_ATTR_DEVICE_FRIENDLY_NAME, pbAttr, &pcbAttrLen);
#endif
	test_rv(rv, hContext, DONT_PANIC);
	if (rv == SCARD_S_SUCCESS)
		printf("SCARD_ATTR_DEVICE_FRIENDLY_NAME: " GREEN "%s\n" NORMAL, pbAttr);

#ifdef USE_AUTOALLOCATE
	printf("Testing SCardFreeMemory\t\t: ");
	rv = SCardFreeMemory(hContext, pbAttr);
	test_rv(rv, hContext, PANIC);
#else
	if (pbAttr)
		free(pbAttr);
#endif

	printf("Testing SCardGetAttrib\t\t: ");
#ifdef USE_AUTOALLOCATE
	pcbAttrLen = SCARD_AUTOALLOCATE;
	rv = SCardGetAttrib(hCard, SCARD_ATTR_ATR_STRING, (unsigned char *)&pbAttr,
		&pcbAttrLen);
#else
	rv = SCardGetAttrib(hCard, SCARD_ATTR_ATR_STRING, NULL, &pcbAttrLen);
	test_rv(rv, hContext, DONT_PANIC);
	if (rv == SCARD_S_SUCCESS)
	{
		printf("SCARD_ATTR_ATR_STRING length: " GREEN "%ld\n" NORMAL, pcbAttrLen);
		pbAttr = malloc(pcbAttrLen);
	}

	printf("Testing SCardGetAttrib\t\t: ");
	rv = SCardGetAttrib(hCard, SCARD_ATTR_ATR_STRING, pbAttr, &pcbAttrLen);
#endif
	test_rv(rv, hContext, DONT_PANIC);
	if (rv == SCARD_S_SUCCESS)
	{
		printf("SCARD_ATTR_ATR_STRING length: " GREEN "%ld\n" NORMAL, pcbAttrLen);
		printf("SCARD_ATTR_ATR_STRING: " GREEN);
		for (i = 0; i < pcbAttrLen; i++)
			printf("%02X ", pbAttr[i]);
		printf("\n" NORMAL);
	}

#ifdef USE_AUTOALLOCATE
	printf("Testing SCardFreeMemory\t\t: ");
	rv = SCardFreeMemory(hContext, pbAttr);
	test_rv(rv, hContext, PANIC);
#else
	if (pbAttr)
		free(pbAttr);
#endif

	printf("Testing SCardGetAttrib\t\t: ");
	dwBufLen = sizeof(buf);
	rv = SCardGetAttrib(hCard, SCARD_ATTR_VENDOR_IFD_VERSION, buf.as_char, &dwBufLen);
	test_rv(rv, hContext, DONT_PANIC);
	if (rv == SCARD_S_SUCCESS)
		printf("Vendor IFD version\t\t: " GREEN "0x%08lX\n" NORMAL,
			buf.as_DWORD);

	printf("Testing SCardGetAttrib\t\t: ");
	dwBufLen = sizeof(buf);
	rv = SCardGetAttrib(hCard, SCARD_ATTR_MAXINPUT, buf.as_char, &dwBufLen);
	test_rv(rv, hContext, DONT_PANIC);
	if (rv == SCARD_S_SUCCESS)
	{
		if (dwBufLen == sizeof(uint32_t))
			printf("Max message length\t\t: " GREEN "%d\n" NORMAL,
				buf.as_uint32_t);
		else
			printf(RED "Wrong size" NORMAL);
	}

	printf("Testing SCardGetAttrib\t\t: ");
	dwBufLen = sizeof(buf);
	rv = SCardGetAttrib(hCard, SCARD_ATTR_VENDOR_NAME, buf.as_char, &dwBufLen);
	test_rv(rv, hContext, DONT_PANIC);
	if (rv == SCARD_S_SUCCESS)
		printf("Vendor name\t\t\t: " GREEN "%s\n" NORMAL, buf.as_char);

	printf("Testing SCardSetAttrib\t\t: ");
	rv = SCardSetAttrib(hCard, SCARD_ATTR_ATR_STRING, (LPCBYTE)"", 1);
	test_rv(rv, hContext, DONT_PANIC);

	printf("Testing SCardStatus\t\t: ");

#ifdef USE_AUTOALLOCATE
	dwReaderLen = SCARD_AUTOALLOCATE;
	dwAtrLen = SCARD_AUTOALLOCATE;
	rv = SCardStatus(hCard, (LPSTR)&pcReaders, &dwReaderLen, &dwState, &dwProt,
		(LPBYTE)&pbAtr, &dwAtrLen);
#else
	dwReaderLen = 100;
	pcReaders   = malloc(sizeof(char) * 100);
	dwAtrLen    = MAX_ATR_SIZE;

	rv = SCardStatus(hCard, pcReaders, &dwReaderLen, &dwState, &dwProt,
		pbAtr, &dwAtrLen);
#endif
	test_rv(rv, hContext, PANIC);

	printf("Current Reader Name\t\t: " GREEN "%s\n" NORMAL, pcReaders);
	printf("Current Reader State\t\t: " GREEN "0x%.4lx\n" NORMAL, dwState);
	printf("Current Reader Protocol\t\t: T=" GREEN "%ld\n" NORMAL, dwProt - 1);
	printf("Current Reader ATR Size\t\t: " GREEN "%ld" NORMAL " bytes\n",
		dwAtrLen);
	printf("Current Reader ATR Value\t: " GREEN);

	for (i = 0; i < dwAtrLen; i++)
	{
		printf("%02X ", pbAtr[i]);
	}
	printf(NORMAL "\n");

#ifdef USE_AUTOALLOCATE
	printf("Testing SCardFreeMemory\t\t: ");
	rv = SCardFreeMemory(hContext, pcReaders);
	test_rv(rv, hContext, PANIC);
	printf("Testing SCardFreeMemory\t\t: ");
	rv = SCardFreeMemory(hContext, pbAtr);
	test_rv(rv, hContext, PANIC);
#else
	if (pcReaders)
		free(pcReaders);
#endif

	if (rv != SCARD_S_SUCCESS)
	{
		(void)SCardDisconnect(hCard, SCARD_RESET_CARD);
		(void)SCardReleaseContext(hContext);
	}

	printf("Press enter: ");
	(void)getchar();
	printf("Testing SCardReconnect\t\t: ");
	rv = SCardReconnect(hCard, SCARD_SHARE_SHARED,
		SCARD_PROTOCOL_T0 | SCARD_PROTOCOL_T1, SCARD_UNPOWER_CARD, &dwPref);
	test_rv(rv, hContext, PANIC);

	printf("Testing SCardDisconnect\t\t: ");
	rv = SCardDisconnect(hCard, SCARD_UNPOWER_CARD);
	test_rv(rv, hContext, PANIC);

#ifdef USE_AUTOALLOCATE
	printf("Testing SCardFreeMemory\t\t: ");
	rv = SCardFreeMemory(hContext, mszReaders);
	test_rv(rv, hContext, PANIC);
#else
	free(mszReaders);
#endif

	printf("Testing SCardReleaseContext\t: ");
	rv = SCardReleaseContext(hContext);
	test_rv(rv, hContext, PANIC);

	printf("\n");
	printf("PC/SC Test Completed Successfully !\n");

	return 0;
}
