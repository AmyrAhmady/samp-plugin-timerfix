#pragma once

#include <amx/amx.h>
#include <string>

#define USENAMETABLE(hdr) \
	((hdr)->defsize==sizeof(AMX_FUNCSTUBNT))

#define NUMENTRIES(hdr,field,nextfield) \
	(unsigned)(((hdr)->nextfield - (hdr)->field) / (hdr)->defsize)

#define GETENTRY(hdr,table,index) \
	(AMX_FUNCSTUB *)((unsigned char*)(hdr) + (unsigned)(hdr)->table + (unsigned)index*(hdr)->defsize)

#define GETENTRYNAME(hdr,entry) \
	(USENAMETABLE(hdr) ? \
		(char *)((unsigned char*)(hdr) + (unsigned)((AMX_FUNCSTUBNT*)(entry))->nameofs) : \
		((AMX_FUNCSTUB*)(entry))->name)

inline void AMXAPI amx_Redirect(AMX* amx, char* from, ucell to, AMX_NATIVE* store) {
	AMX_HEADER* hdr = (AMX_HEADER*)amx->base;
	AMX_FUNCSTUB* func;
	for (int idx = 0, num = NUMENTRIES(hdr, natives, libraries); idx != num; ++idx) {
		func = GETENTRY(hdr, natives, idx);
		if (!strcmp(from, GETENTRYNAME(hdr, func))) {
			if (store) {
				*store = (AMX_NATIVE)func->address;
			}
			func->address = to;
			return;
		}
	}
}

inline int AMXAPI amx_GetCString(AMX* amx, cell param, char*& dest) {
	cell* ptr;
	amx_GetAddr(amx, param, &ptr);
	int len;
	amx_StrLen(ptr, &len);
	dest = (char*)malloc((len + 1) * sizeof(char));
	if (dest != NULL) {
		amx_GetString(dest, ptr, 0, UNLIMITED);
		dest[len] = 0;
		return len;
	}
	return 0;
}

inline void AMXAPI amx_SetCString(AMX* amx, cell param, char* str, int len) {
	cell* dest;
	amx_GetAddr(amx, param, &dest);
	amx_SetString(dest, str, 0, 0, len);
}

inline std::string AMXAPI amx_GetCppString(AMX* amx, cell param) {
	char* tmp;
	amx_StrParam(amx, param, tmp);
	if (tmp != NULL) {
		return tmp;
	}
	return "";
}

inline void AMXAPI amx_SetCppString(AMX* amx, cell param, std::string str, int len) {
	cell* dest;
	amx_GetAddr(amx, param, &dest);
	amx_SetString(dest, str.c_str(), 0, 0, len);
}
