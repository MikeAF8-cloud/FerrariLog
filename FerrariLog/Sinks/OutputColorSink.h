#pragma once

#ifdef _WIN32
#include "WinColorSink.h"
#else
#include "AnsiColorSink.h"
#endif

namespace FerrariLog {
	namespace Sinks {

#ifdef _WIN32
		using outputcolor_stdout_st = wincolor_stdout_st;
		using outputcolor_stdout_mt = wincolor_stdout_mt;

		using outputcolor_stderr_st = wincolor_stderr_st;
		using outputcolor_stderr_mt = wincolor_stderr_mt;
#else
		using outputcolor_stdout_st = ansicolor_stdout_st;
		using outputcolor_stdout_mt	= ansicolor_stdout_mt;
														 
		using outputcolor_stderr_st	= ansicolor_stderr_st;
		using outputcolor_stderr_mt	= ansicolor_stderr_mt;
#endif
	}
}