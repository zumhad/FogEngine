#pragma once

typedef unsigned char 		uint8;
typedef unsigned short int	uint16;
typedef unsigned int		uint32;
typedef unsigned long long	uint64;

typedef	signed char			int8;
typedef signed short int	int16;
typedef signed int	 		int32;
typedef signed long long	int64;

typedef char				CHAR;
typedef wchar_t				WCHAR;


typedef struct _Rect
{
	int16 left;
	int16 top;
	int16 right;
	int16 bottom;
} Rect;