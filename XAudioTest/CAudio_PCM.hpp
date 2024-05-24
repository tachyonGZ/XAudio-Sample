#pragma once

#include "stdafx.h"
#include "CAudioFile.hpp"

class CAudioPCM
{
private:
	CChunk format, data;
public:
	CAudioPCM(const CAudioFile &audioFile);
	
	/**
		@brief 从pcm格式中读取波形图并填充结构
	**/
	std::unique_ptr<WAVEFORMATEXTENSIBLE> get_wave_format() const;

	/**
		@brief 从pcm格式中读取有效数据
	**/
	const void get_data(XAUDIO2_BUFFER &buffer) const;
};

CAudioPCM::CAudioPCM(const CAudioFile &audioFile)
	: format(audioFile.load_chunk(fourcc_t::FMT)), data(audioFile.load_chunk(fourcc_t::DATA))
{
	;
}


const void CAudioPCM::get_wave_format() const
{
	return format.m_pChunkData
	find_chunk(fourcc_t::FMT).read_data(&waveFormatX, sizeof(WAVEFORMATEXTENSIBLE));
	return *this;
}

void CAudioPCM::get_data(XAUDIO2_BUFFER &buffer) const
{
	CChunk chunk = find_chunk(fourcc_t::DATA);
	buffer.AudioBytes = chunk.get_data_size();
	buffer.pAudioData = chunk.get_data();
	buffer.Flags = XAUDIO2_END_OF_STREAM;
	return *this;
}
