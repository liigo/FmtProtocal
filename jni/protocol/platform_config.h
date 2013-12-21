/* config.h.in.  Generated from cmake by autoheader.  */
#ifndef __PLATFORM_CONFIG_H
#define __PLATFORM_CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif

#if !defined(_WIN32) && !defined(_WIN64)
/* #undef HAVE_FUNC_ULTOA */
/* #undef HAVE_FUNC_LTOA */
/* #undef HAVE_FUNC_ITOA */
	#define HAVE_FUNC_ATOI

#else
	#define	HAVE_FUNC_ULTOA
	#define	HAVE_FUNC_LTOA
	#define	HAVE_FUNC_ITOA
	#define	HAVE_FUNC_ATOI
#endif


#ifdef __linux__
	#define	_ultoa(x , y , z)		ultoa(x, y, z)
	#define	_ltoa(x , y , z)		ltoa(x, y, z)
	#define	_ltoa(x , y , z)		ltoa(x, y, z)
#endif

#ifndef HAVE_FUNC_ULTOA
	char * ultoa (unsigned long value, char *string, int radix);
#endif

#ifndef HAVE_FUNC_LTOA
	char * ltoa (long value, char *string, int radix);
#endif

#ifndef HAVE_FUNC_ITOA
	char * itoa (int value, char *string, int radix);
#endif

#ifndef HAVE_FUNC_ATOI
	int atoi (const char *string);
#endif




#ifdef __cplusplus
};
#endif

#endif
