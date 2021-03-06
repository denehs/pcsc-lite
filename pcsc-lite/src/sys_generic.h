/*
 * MUSCLE SmartCard Development ( http://www.linuxnet.com )
 *
 * Copyright (C) 1999
 *  David Corcoran <corcoran@linuxnet.com>
 * Copyright (C) 2002-2009
 *  Ludovic Rousseau <ludovic.rousseau@free.fr>
 *
 * $Id: sys_generic.h 4836 2010-03-19 12:34:32Z rousseau $
 */

/**
 * @file
 * @brief This handles abstract system level calls.
 */

#ifndef __sys_generic_h__
#define __sys_generic_h__

#ifdef __cplusplus
extern "C"
{
#endif

#include <sys/stat.h>
#include <sys/mman.h>

	int SYS_Sleep(int);

	int SYS_USleep(int);

	int SYS_Daemon(int, int);

	int SYS_RandomInt(int, int);

	int SYS_GetSeed(void);

#ifdef __cplusplus
}
#endif

#endif							/* __sys_generic_h__ */
