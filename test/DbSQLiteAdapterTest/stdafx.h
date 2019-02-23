// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#ifndef _WIN32_WINNT		// Allow use of features specific to Windows 10 or later.
#define _WIN32_WINNT 0x0A00	// Change this to the appropriate value to target other versions of Windows.
#endif

#include <stdio.h>
#include <tchar.h>
#include <memory>
#include <string>

// TODO: reference additional headers your program requires here
#include <gtest/gtest.h>
#include <gmock/gmock.h>

#define BOOST_CONFIG_SUPPRESS_OUTDATED_MESSAGE