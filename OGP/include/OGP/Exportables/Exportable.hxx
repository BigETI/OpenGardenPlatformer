#pragma once

#if defined(IS_OPEN_GARDEN_PLATFORMER_STATIC)
#	define OGP_API
#elif defined(_MSC_VER)
#	if IS_OPEN_GARDEN_PLATFORMER_EXPORTING
#		define OGP_API __declspec(dllexport)
#	else
#		define OGP_API __declspec(dllimport)
#	endif
#elif defined(__GNUC__)
#	if IS_OPEN_GARDEN_PLATFORMER_EXPORTING
#		define OGP_API __attribute__((visibility("default")))
#	else
#		define OGP_API
#	endif
#else
#	define OGP_API
#	if IS_OPEN_GARDEN_PLATFORMER_EXPORTING
#		pragma warning Unknown dynamic link export semantic.
#	else
#		pragma warning Unknown dynamic link import semantic.
#	endif
#endif