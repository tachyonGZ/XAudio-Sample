#pragma once

#include <iostream>
#include <string>
#include <exception>
#include <xaudio2.h>
#include <windows.h>

class win32_error : public std::runtime_error
{
public:
	explicit win32_error(DWORD error_id)
		: std::runtime_error(std::to_string(error_id))
	{
		;
	}
};

enum class fourcc_t : DWORD
{
	RIFF = 'FFIR',
	DATA = 'atad',
	FMT = ' tmf',
	WAVE = 'EVAW',
	XWMA = 'AMWX',
	DPDS = 'sdpd',
	LIST = 'TSIL',
	INFO = 'OFNI'
};