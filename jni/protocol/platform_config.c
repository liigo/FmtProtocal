#include <stdio.h>
#include <stdlib.h>
#include "platform_config.h"

#ifdef __linux__
	#include <unistd.h>
	#include <stdbool.h>
#else
	#include <string.h>
#endif


#ifndef HAVE_FUNC_ULTOA
char * ultoa (unsigned long value, char *string, int radix)
{
	char *dst;
	char digits[32];
	int i, n;

	dst = string;
	if (radix < 2 || radix > 36)
	{
		*dst = 0;
		return (string);
	}
	i = 0;
	do
	{
		n = value % (unsigned long)radix;
		digits[i++] = (n < 10 ? (char)n+'0' : (char)n-10+'a');
		value /= (unsigned long)radix;
	} while (value != 0);
	while (i > 0)
		*dst++ = digits[--i];
	*dst = 0;
	return (string);
}
#endif

#ifndef HAVE_FUNC_ITOA
char * itoa (int value, char *string, int radix)
{
	char *dst;
	char digits[32];
	unsigned x;
	int i, n;

	dst = string;
	if (radix < 2 || radix > 36)
	{
		*dst = 0;
		return (string);
	}
	if (radix == 10 && value < 0)
	{
		*dst++ = '-';
		x = -value;
	}
	else
		x = value;
	i = 0;
	do
	{
		n = x % (unsigned)radix;
		digits[i++] = (n < 10 ? (char)n+'0' : (char)n-10+'a');
		x /= (unsigned)radix;
	} while (x != 0);
	while (i > 0)
		*dst++ = digits[--i];
	*dst = 0;
	return (string);
}
#endif

#ifndef HAVE_FUNC_ATOI
int atoi (const char *string)
{
	int n;
	char neg;

	n = 0; neg = 0;
	while (*string == ' ' || *string == '\t') ++string;
	if (*string == '+')
		++string;
	else if (*string == '-')
	{
		++string; neg = 1;
	}
	while (*string >= '0' && *string <= '9')
	{
		n = n*10 + (*string - '0');
		++string;
	}
	return (neg ? -n : n);
}
#endif

#ifndef HAVE_FUNC_LTOA
char *ltoa (long value, char *string, int radix)
{
	char *dst;
	char digits[32];
	unsigned long x;
	int i, n;

	dst = string;
	if (radix < 2 || radix > 36)
	{
		*dst = 0;
		return (string);
	}
	if (radix == 10 && value < 0)
	{
		*dst++ = '-';
		x = -value;
	}
	else
		x = value;
	i = 0;
	do
	{
		n = x % (unsigned long)radix;
		digits[i++] = (n < 10 ? (char)n+'0' : (char)n-10+'a');
		x /= (unsigned long)radix;
	} while (x != 0);
	while (i > 0)
		*dst++ = digits[--i];
	*dst = 0;
	return (string);
}
#endif