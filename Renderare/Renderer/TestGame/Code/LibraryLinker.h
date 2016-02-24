#pragma once

// Check windows
#if _WIN32 || _WIN64
#if _WIN64
#define ENV64BIT
#else
#define ENV32BIT
#endif
#endif

// Check GCC
#if __GNUC__
#if __x86_64__ || __ppc64__
#define ENV64BIT
#else
#define ENV32BIT
#endif
#endif

#if NDEBUG
#if defined ENV64BIT
#pragma comment (lib, "CubeRendererx64.lib")
#else
#pragma comment (lib, "CubeRenderer.lib")
#endif
#else
#if defined ENV64BIT
#pragma comment (lib, "CubeRendererDx64.lib")
#else
#pragma comment (lib, "CubeRendererD.lib")
#endif
#endif