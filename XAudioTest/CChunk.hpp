#pragma once

#include "stdafx.h"
#include "CAudioFile.hpp"

class CChunk
{
public:

	fourcc_t m_chunkID;
	DWORD m_dwChunkSize = 0;
	BYTE *m_pChunkData = nullptr;

	explicit CChunk(const fourcc_t chunkID, const DWORD dwChunkSize, BYTE * const pChunkData);
	~CChunk();

	const CChunk &read_data(void *buffer, const DWORD bufferSize) const;
	BYTE *get_data() const
	{
		return m_pChunkData;
	}
	DWORD get_data_size() const;
};

CChunk::CChunk(const fourcc_t chunkID, const DWORD dwChunkSize, BYTE * const pChunkData)
	: m_chunkID(chunkID), m_dwChunkSize(dwChunkSize), m_pChunkData(pChunkData)
{
	;
}

CChunk::~CChunk()
{
	if(nullptr != m_pChunkData)
	{
		delete[] m_pChunkData;
	}
}

/*
const CChunk &CChunk::read_data(void *buffer, const DWORD bufferSize) const
{
	if (INVALID_SET_FILE_POINTER == ::SetFilePointer(m_hFile, m_dwOffset, nullptr, FILE_BEGIN))
	{
		throw win32_error(::GetLastError());
	}

	DWORD dwRead = 0;
	if (0 == ::ReadFile(m_hFile, buffer, m_dwChunkDataSize, &dwRead, nullptr))
	{
		throw win32_error(::GetLastError());
	}

	return *this;
}


BYTE *CChunk::get_data() const
{
	BYTE *pBuffer = new BYTE[m_dwChunkDataSize];
	read_data(pBuffer, m_dwChunkDataSize);
	return pBuffer;
}

DWORD CChunk::get_data_size() const
{
	return m_dwChunkDataSize;
}

*/