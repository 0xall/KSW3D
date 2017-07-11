
// if you don't want to use unicode system, undefine UNICODE
//#undef UNICODE

#if defined NDEBUG		// if not debugging mode,
#  undef _DEBUG
#endif	/* NDEBUG */

//#undef _DEBUG

// if you want debugging on KSW3D, include this
#include "KSW3DDebug.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <tchar.h>

#include <string>
#include <map>
#include <vector>
#include <algorithm>

/* directx */
#include <d3d9.h>
#include <d3dx9.h>
#include <d3dx9math.h>

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "dxguid.lib")

#if defined UNICODE
typedef std::basic_string<TCHAR> tstring;
#else
typedef std::string tstring;
#endif