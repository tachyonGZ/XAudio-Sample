#include "stdafx.h"

#include "CAudioFile.hpp"

int main(int argc, char **argv)
{
	HRESULT hRes = ::CoInitializeEx(nullptr, COINIT::COINIT_MULTITHREADED);
	if (FAILED(hRes))
	{
		return hRes;
	}

	IXAudio2 *pXAudio2 = nullptr;
	if (FAILED(hRes = ::XAudio2Create(&pXAudio2, 0U, XAUDIO2_DEFAULT_PROCESSOR)))
	{
		return hRes;
	}

	IXAudio2MasteringVoice *pMasterVoice = nullptr;
	if (FAILED(hRes = pXAudio2 -> CreateMasteringVoice(&pMasterVoice)))
	{
		return hRes;
	}

	CAudioFile audioFile(TEXT("D:\\Download\\XAudioTest.wav"));

	WAVEFORMATEXTENSIBLE waveFormatX{ 0 };
	audioFile.read_wave_format(waveFormatX);
	/*
	try{
		audioFile.read_wave_format(waveFormatX);
	}catch (const std::exception &e) {
		std::cerr << e.what() << std::endl;
		return hRes;
	}
	*/

	IXAudio2SourceVoice *pSourceVoice = nullptr;
	if (FAILED(hRes = pXAudio2 -> CreateSourceVoice(&pSourceVoice, reinterpret_cast<WAVEFORMATEX *>(&waveFormatX))))
	{
		return hRes;
	}

	XAUDIO2_BUFFER buffer{ 0 };
	audioFile.read_buffer(buffer);
	/*
	try {
		audioFile.read_buffer(buffer);
	}catch (const std::exception &e) {
		std::cerr << e.what() << std::endl;
		return hRes;
	}
	*/


	if (FAILED(hRes = pSourceVoice -> SubmitSourceBuffer(&buffer)))
	{
		return hRes;
	}

	std::cout << "×¼±¸²¥·Å²âÊÔÒôÆµ..." << std::endl;

	if (FAILED(hRes = pSourceVoice -> Start(0)))
	{
		return hRes;
	}

	std::cout << "²âÊÔÒôÆµ²¥·ÅÍê±Ï" << std::endl;
	return 0;
}