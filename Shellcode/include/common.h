#pragma once 

#include <windows.h>

#define C_PTR( x )   ( ( LPVOID    ) ( x ) )
#define U_PTR( x )   ( ( UINT_PTR ) ( x ) )

int   StringCompareA(_In_ LPCSTR String1, _In_ LPCSTR String2);
void* MemSet(void* Destination, int Value, size_t Size);
int   wccmp(const WCHAR *s1, const WCHAR *s2);