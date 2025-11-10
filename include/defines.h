#pragma once

#ifndef __DEFINES_H__
#define __DEFINES_H__

// Library Includes
#include <d3dx9.h>

// Local Includes

// Types

typedef char Int8;
typedef unsigned char UInt8;
typedef short Int16;
typedef unsigned short UInt16;
typedef wchar_t WChar16;
typedef int Int32;
typedef unsigned int UInt32;
typedef __int64 Int64;
typedef unsigned __int64 UInt64;
typedef float Float32;
typedef double Float64;

typedef long VertexType;

typedef enum
{
	INVAILID_PRIMITIVETYPE,
	IGPT_POINTLIST,		// Num. of Primitives == Num. of Indices.
	IGPT_LINELIST,		// Num. of Primitives == Indices / 2.
	IGPT_LINESTRIP,		// Num. of Primitives == Num. of Indices - 1.
	IGPT_TRIANGLELIST,	// Num. of Primitives == Num. of Indices / 3.
	IGPT_TRIANGLESTRIP,	// Num. of Primitives == Num. of Indices - 2.
	IGPT_TRIANGLEFAN,	// Num. of Primitives == Num. of Indices - 2.
}EIgPrimitiveType;

#define VALIDATE(a) if (!a) return (false)

#define PI 3.14159265f

#define KEYDOWN(a) GetAsyncKeyState(a) & 0x8000

#define VALIDATE(a) if (!a) return (false)

#define CHECKHRESULT(a, b) if(FAILED(a)) b = true

#endif // __DEFINES_H__



