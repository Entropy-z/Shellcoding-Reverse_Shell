#include <windows.h>
#include <common.h>

void* MemSet(void* Destination, int Value, size_t Size){
	unsigned char* p = (unsigned char*)Destination;
	while (Size > 0) {
		*p = (unsigned char)Value;
		p++;
		Size--;
	}
	return Destination;
}