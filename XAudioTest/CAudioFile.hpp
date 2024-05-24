#pragma once

#include "stdafx.h"

#include "CChunk.hpp"







class CAudioFile
{

private:
	HANDLE m_hFile = nullptr;	// �ļ����
	fourcc_t m_fileType;

public:

#ifdef UNICODE
	CAudioFile(std::wstring_view fileName);
#else
	CAudioFile(const std::string fileName);
#endif

	/**
		@brief ͨ��fourccѰ�Ҷ���chunk���ݶ�
	**/
	DWORD find_chunk(const fourcc_t fourcc) const;

	

	CChunk load_chunk(const fourcc_t fourcc) const;
};

CChunk CAudioFile::load_chunk(const fourcc_t fourcc) const
{
	DWORD dwOffset = find_chunk(fourcc);
	
	// ����chunkID��ռ�ĳߴ磬��Ϊ�Ѿ�����֪����Ŀ��chunk��chunkID
	dwOffset += sizeof(fourcc_t);

	if (INVALID_SET_FILE_POINTER == ::SetFilePointer(m_hFile, dwOffset, nullptr, FILE_BEGIN))
	{
		throw win32_error(::GetLastError());
	}

	DWORD dwRead = 0;
	DWORD dwChunkSize = 0;
	if (0 == ::ReadFile(m_hFile, &dwChunkSize, sizeof(dwChunkSize), &dwRead, nullptr))
	{
		throw win32_error(::GetLastError());
	}

	BYTE *pBuffer = new BYTE[dwChunkSize];

	if (0 == ::ReadFile(m_hFile, pBuffer, dwChunkSize, &dwRead, nullptr))
	{
		throw win32_error(::GetLastError());
	}

	return CChunk{fourcc, dwChunkSize, pBuffer};
}

DWORD CAudioFile::find_chunk(const fourcc_t fourcc) const
{
	HRESULT hRes = S_OK;
	if (INVALID_SET_FILE_POINTER == ::SetFilePointer(m_hFile, 0, nullptr, FILE_BEGIN))
	{
		throw win32_error(::GetLastError());
	}

	DWORD dwRIFFDataSize = 0;
	fourcc_t fileFourcc;
	fourcc_t listType;
	fourcc_t chunkID;
	DWORD dwChunkSize = 0;
	DWORD dwOffset = 0; // �ļ���ȡƫ����

	//CChunk chunk{m_hFile};
		
	while (hRes == S_OK)
	{
		DWORD dwRead = 0;
		if (0 == ::ReadFile(m_hFile, &chunkID, sizeof(fourcc_t), &dwRead, nullptr))
		{
			throw win32_error(::GetLastError());
		}

		if (0 == ::ReadFile(m_hFile, &dwChunkSize, sizeof(DWORD), &dwRead, nullptr))
		{
			throw win32_error(::GetLastError());
		}

		switch (chunkID)
		{
		case fourcc_t::RIFF:
			dwRIFFDataSize = dwChunkSize;
			dwChunkSize = sizeof(DWORD);
			if (0 == ::ReadFile(m_hFile, &fileFourcc, sizeof(DWORD), &dwRead, nullptr))
			{
				throw win32_error(::GetLastError());
			}
			break;
		/*
		case fourcc_t::LIST:
			chunk.m_dwChunkDataSize = sizeof(DWORD);
			if (0 == ::ReadFile(m_hFile, &listType, sizeof(DWORD), &dwRead, nullptr))
			{
				throw win32_error(::GetLastError());
			}
		*/
		default:
			if (INVALID_SET_FILE_POINTER == ::SetFilePointer(m_hFile, dwChunkSize, nullptr, FILE_CURRENT))
			{
				throw win32_error(::GetLastError());
			}
		}

		if(fourcc == chunkID)
		{
			// �ҵ������fourccƥ���chunk
			break;
		}

		dwOffset += dwChunkSize + sizeof(DWORD) * 2;;
		// ChunkData��ChunkID �� ChunkSize ��ռ�ĳߴ�	
	}

	return dwOffset;
}




CAudioFile::CAudioFile(std::wstring_view fileName)
{
	m_hFile = ::CreateFile(
		fileName.data(),
		GENERIC_READ,
		FILE_SHARE_READ,
		nullptr,
		OPEN_EXISTING,
		0,
		nullptr
	);

	if (INVALID_HANDLE_VALUE == m_hFile || nullptr == m_hFile)
	{
		//std::cerr << ::GetLastError() << std::endl;
		throw win32_error(::GetLastError());
	}

	CChunk &&chunk = find_chunk(fourcc_t::RIFF);
	chunk.read_data(&m_fileType, sizeof(fourcc_t));
	
	if (fourcc_t::WAVE != m_fileType)
	{
		throw std::exception("��֧�ֵ��ļ�����");
		// ��ƥ���RIFF�ļ�����
	}

}