﻿//
//  Copyright (C) 2013-2014 mogemimi.
//
//  Distributed under the MIT License.
//  See accompanying file LICENSE.md or copy at
//  http://enginetrouble.net/pomdog/LICENSE.md for details.
//

#include "MSWaveAudioLoader.hpp"

#include "Pomdog/Audio/AudioClip.hpp"
#include "Pomdog/Utility/Assert.hpp"
#include "Pomdog/Config/Platform.hpp"
#if defined(POMDOG_PLATFORM_MACOSX) || defined(POMDOG_PLATFORM_APPLE_IOS)
#include "../../SoundSystem.OpenAL/AudioClipAL.hpp"
#include <AudioToolbox/AudioFile.h>
#include <AudioToolbox/AudioConverter.h>
#elif defined(POMDOG_PLATFORM_WIN32) || defined(POMDOG_PLATFORM_XBOX_ONE)
#include "MakeFourCC.hpp"
#include "../../SoundSystem.XAudio2/AudioClipXAudio2.hpp"
#include "../../Platform.Win32/Win32Prerequisites.hpp"
#include <mmsystem.h>
#endif
#include <memory>
#include <utility>

namespace Pomdog {
namespace Details {
namespace {
#if defined(POMDOG_PLATFORM_MACOSX) || defined(POMDOG_PLATFORM_APPLE_IOS)
//-----------------------------------------------------------------------
static std::unique_ptr<AudioClip> LoadMSWave_Apple(std::string const& filePath)
{
	CFStringRef filename = CFStringCreateWithCString(nullptr, filePath.c_str(), kCFStringEncodingUTF8);
	CFURLRef url = CFURLCreateWithFileSystemPath(nullptr, filename, kCFURLPOSIXPathStyle, false);
	CFRelease(filename);

	AudioFileID audioFile;
	auto errorCode = AudioFileOpenURL(url, kAudioFileReadPermission, kAudioFileWAVEType, &audioFile);
	CFRelease(url);
	
	if (errorCode != noErr)
	{
		///@todo Not implemeneted
		// Error
		return {};
	}
	
	AudioStreamBasicDescription basicDescription;
	UInt32 propertySize = sizeof(basicDescription);
	errorCode = AudioFileGetProperty(audioFile, kAudioFilePropertyDataFormat, &propertySize, &basicDescription);
	
	if (errorCode != noErr)
	{
		///@todo Not implemeneted
		// Error
		AudioFileClose(audioFile);
		return {};
	}
	
	if (basicDescription.mFormatID != kAudioFormatLinearPCM)
	{
		///@todo Not implemeneted
		// Error
		AudioFileClose(audioFile);
		return {};
	}

	UInt64 audioDataByteCount = 0;
	propertySize = sizeof(audioDataByteCount);
	errorCode = AudioFileGetProperty(audioFile, kAudioFilePropertyAudioDataByteCount, &propertySize, &audioDataByteCount);
	
	if (errorCode != noErr)
	{
		///@todo Not implemeneted
		// Error
		AudioFileClose(audioFile);
		return {};
	}
	
	Float64 estimatedDuration = 0;
	propertySize = sizeof(estimatedDuration);
	errorCode = AudioFileGetProperty(audioFile, kAudioFilePropertyEstimatedDuration, &propertySize, &estimatedDuration);

	if (errorCode != noErr)
	{
		///@todo Not implemeneted
		// Error
		AudioFileClose(audioFile);
		return {};
	}
	
	AudioClipSource audioClip;
	audioClip.SampleRate = basicDescription.mSampleRate;
	audioClip.Duration = DurationSeconds{estimatedDuration};
	
	if (basicDescription.mChannelsPerFrame == 1) {
		audioClip.Channels = AudioChannels::Mono;
	}
	else if (basicDescription.mChannelsPerFrame == 2) {
		audioClip.Channels = AudioChannels::Stereo;
	}
	else {
		///@todo Not implemeneted
		// Error: Not supported
		AudioFileClose(audioFile);
		return {};
	}
	
	if (basicDescription.mBitsPerChannel == 8) {
		audioClip.BitsPerSample = 8;
	}
	else if (basicDescription.mBitsPerChannel == 16) {
		audioClip.BitsPerSample = 16;
	}
	else {
		///@todo Not implemeneted
		// Error: Not supported
		AudioFileClose(audioFile);
		return {};
	}

	audioClip.Data.resize(audioDataByteCount);
	UInt32 byteCountToRead = static_cast<UInt32>(audioClip.Data.size());
	errorCode = AudioFileReadBytes(audioFile, false, 0, &byteCountToRead, audioClip.Data.data());
	AudioFileClose(audioFile);
	
	if (errorCode != noErr)
	{
		///@todo Not implemeneted
		// error: FUS RO DAH!
		return {};
	}
	
	if (byteCountToRead != audioDataByteCount)
	{
		///@todo Not implemeneted
		// error: FUS RO DAH!
		return {};
	}
		
	return std::move(audioClip);
}
#elif defined(POMDOG_PLATFORM_WIN32) || defined(POMDOG_PLATFORM_XBOX_ONE)
//-----------------------------------------------------------------------
static MMCKINFO ReadRiffChunk(HMMIO ioHandle)
{
	POMDOG_ASSERT(ioHandle);

	MMCKINFO riffChunk;

	auto mmResult = ::mmioDescend(ioHandle, &riffChunk, nullptr, 0);
	if (MMSYSERR_NOERROR != mmResult)
	{
		// error: FUS RO DAH!
		///@todo Not implemented
		// throw "Cannot read Multi-Media I/O wave file."
	}

	if (MakeFourCC('R', 'I', 'F', 'F') != riffChunk.ckid) {
		// error: FUS RO DAH!
		///@todo Not implemented
		// throw "Cannot read wave file. this file is a invalid wave file"
	}

	if (MakeFourCC('W', 'A', 'V', 'E') != riffChunk.fccType) {
		// error: FUS RO DAH!
		///@todo Not implemented
		// throw "Cannot read wave file. this file is a invalid wave file"
	}

	return std::move(riffChunk);
}
//-----------------------------------------------------------------------
static std::vector<std::uint8_t> ReadWaveFormat(HMMIO ioHandle, MMCKINFO const& riffChunk)
{
	POMDOG_ASSERT(ioHandle);

	MMCKINFO chunkInfo;
	chunkInfo.ckid = MakeFourCC('f', 'm', 't', ' ');

	auto mmResult = ::mmioDescend(ioHandle, &chunkInfo, &riffChunk, MMIO_FINDCHUNK);
	if (MMSYSERR_NOERROR != mmResult)
	{
		// error: FUS RO DAH!
		///@todo Not implemented
		// throw "Failed to search the input file for the 'fmt ' chunk"
	}

	if (chunkInfo.cksize < sizeof(PCMWAVEFORMAT))
	{
		// error: FUS RO DAH!
		///@todo Not implemented
	}

	PCMWAVEFORMAT pcmWaveFormat;

	auto byteSize = ::mmioRead(ioHandle, reinterpret_cast<HPSTR>(&pcmWaveFormat), sizeof(pcmWaveFormat));
	if (byteSize != sizeof(pcmWaveFormat))
	{
		// error: FUS RO DAH!
		///@todo Not implemented
		// throw "Failed to  Read the 'fmt ' chunk into <pcmWaveFormat>"
	}

	std::vector<std::uint8_t> waveFormat;

	if (WAVE_FORMAT_PCM == pcmWaveFormat.wf.wFormatTag)
	{
		waveFormat.resize(sizeof(WAVEFORMATEX));

		static_assert(sizeof(WAVEFORMATEX) >= sizeof(pcmWaveFormat), "");
		POMDOG_ASSERT(waveFormat.size() >= sizeof(pcmWaveFormat));
		std::memcpy(waveFormat.data(), &pcmWaveFormat, sizeof(pcmWaveFormat));

		reinterpret_cast<WAVEFORMATEX*>(waveFormat.data())->cbSize = 0;
	}
	else
	{
		WORD extraBytes = 0;
		byteSize = ::mmioRead(ioHandle, reinterpret_cast<CHAR*>(&extraBytes), sizeof(WORD));
		if (byteSize != sizeof(WORD))
		{
			// error: FUS RO DAH!
			///@todo Not implemented
			// throw "Failed to  Read the extraBytes"
		}

		waveFormat.resize(sizeof(WAVEFORMATEX) + extraBytes);

		static_assert(sizeof(WAVEFORMATEX) >= sizeof(pcmWaveFormat), "");
		POMDOG_ASSERT(waveFormat.size() >= sizeof(pcmWaveFormat));
		std::memcpy(waveFormat.data(), &pcmWaveFormat, sizeof(pcmWaveFormat));

		reinterpret_cast<WAVEFORMATEX*>(waveFormat.data())->cbSize = extraBytes;

		byteSize = ::mmioRead(ioHandle, reinterpret_cast<CHAR*>(waveFormat.data() + sizeof(WAVEFORMATEX)), extraBytes);
		if (byteSize != extraBytes)
		{
			// error: FUS RO DAH!
			///@todo Not implemented
			// throw "Failed to read the extra data"
		}
	}

	mmResult = ::mmioAscend(ioHandle, &chunkInfo, 0);
	if (MMSYSERR_NOERROR != mmResult)
	{
		// error: FUS RO DAH!
		///@todo Not implemented
		// throw "Failed to ascend the input file out of the 'fmt ' chunk"
	}

	return std::move(waveFormat);
}
//-----------------------------------------------------------------------
static MMCKINFO SeekDataChunk(HMMIO ioHandle, MMCKINFO const& riffChunk)
{
	POMDOG_ASSERT(ioHandle);

	constexpr LONG seekErrorCode = -1;
	if (seekErrorCode == ::mmioSeek(ioHandle, riffChunk.dwDataOffset + sizeof(FOURCC), SEEK_SET)) {
		// error: FUS RO DAH!
		///@todo Not implemented
	}

	MMCKINFO dataChunk;
	dataChunk.ckid = MakeFourCC('d', 'a', 't', 'a');

	auto mmResult = ::mmioDescend(ioHandle, &dataChunk, &riffChunk, MMIO_FINDCHUNK);
	if (MMSYSERR_NOERROR != mmResult)
	{
		// error: FUS RO DAH!
		///@todo Not implemented
	}
	return std::move(dataChunk);
}
//-----------------------------------------------------------------------
static std::vector<std::uint8_t> ReadWaveAudioData(HMMIO ioHandle, MMCKINFO const& dataChunk)
{
	POMDOG_ASSERT(ioHandle);

	MMIOINFO mmioInfoIn;
	if (0 != ::mmioGetInfo(ioHandle, &mmioInfoIn, 0))
	{
		///@todo Not implemeneted
		// error: FUS RO DAH!
		// "buffer size is 0"
		return{};
	}

	if (dataChunk.cksize <= 0) {
		///@todo Not implemeneted
		// error: FUS RO DAH!
		return{};
	}
	
	std::vector<std::uint8_t> result;
	result.resize(dataChunk.cksize);

	for (DWORD index = 0; index < dataChunk.cksize; ++index)
	{
		if (mmioInfoIn.pchNext == mmioInfoIn.pchEndRead)
		{
			auto mmResult = ::mmioAdvance(ioHandle, &mmioInfoIn, MMIO_READ);
			if (MMSYSERR_NOERROR != mmResult) {
				///@todo Not implemeneted
				// error: FUS RO DAH!
				break;
			}

			if (mmioInfoIn.pchNext == mmioInfoIn.pchEndRead) {
				///@todo Not implemeneted
				// error: FUS RO DAH!
				break;
			}
		}

		*(result.data() + index) = *(reinterpret_cast<::BYTE*>(mmioInfoIn.pchNext));
		++mmioInfoIn.pchNext;
	}

	if (0 != ::mmioSetInfo(ioHandle, &mmioInfoIn, 0))
	{
		///@todo Not implemeneted
		// error: FUS RO DAH!
	}

	return std::move(result);
}
//-----------------------------------------------------------------------
static AudioChannels ToAudioChannels(WAVEFORMATEX const& format)
{
	switch (format.nChannels) {
	case 1:
		return AudioChannels::Mono;
	case 2:
		return AudioChannels::Stereo;
	default:
		break;
	}
	POMDOG_ASSERT(false);
	return AudioChannels::Mono;
}
//-----------------------------------------------------------------------
static std::unique_ptr<AudioClip> LoadMSWave_Win32(std::string const& filePath)
{
	HMMIO ioHandle = ::mmioOpen(const_cast<LPSTR>(filePath.c_str()), nullptr, MMIO_ALLOCBUF | MMIO_READ);

	if (!ioHandle) {
		// error: FUS RO DAH!
		///@todo Not implemented
		// throw "Cannot open Multi-Media I/O wave file"
	}

	try {
		auto riffChunk = ReadRiffChunk(ioHandle);
		auto waveFormat = ReadWaveFormat(ioHandle, riffChunk);
		auto dataChunk = SeekDataChunk(ioHandle, riffChunk);
		auto audioData = ReadWaveAudioData(ioHandle, dataChunk);

		::mmioClose(ioHandle, 0);
		ioHandle = nullptr;

		using Details::SoundSystem::XAudio2::AudioClipXAudio2;
		auto nativeAudioClip = std::make_unique<AudioClipXAudio2>(
			std::move(audioData), std::move(waveFormat));

		auto format = nativeAudioClip->WaveFormat();
		POMDOG_ASSERT(format);

		auto audioClip = std::make_unique<AudioClip>(std::move(nativeAudioClip),
			format->nSamplesPerSec, format->wBitsPerSample, ToAudioChannels(*format));

		return std::move(audioClip);
	}
	catch (std::exception const&) {
		// error: FUS RO DAH!
		///@todo Not implemented

		if (ioHandle) {
			::mmioClose(ioHandle, 0);
			ioHandle = nullptr;
		}
	}

	return {};
}
#endif
}// unnamed namespace
 //-----------------------------------------------------------------------
std::unique_ptr<AudioClip> MSWaveAudioLoader::Load(std::string const& filePath)
{
#if defined(POMDOG_PLATFORM_MACOSX) || defined(POMDOG_PLATFORM_APPLE_IOS)
	return LoadMSWave_Apple(filePath);
#elif defined(POMDOG_PLATFORM_WIN32) || defined(POMDOG_PLATFORM_XBOX_ONE)
	return LoadMSWave_Win32(filePath);
#else
#error "TODO: Not implemented"
#endif
}
//-----------------------------------------------------------------------
}// namespace Details
}// namespace Pomdog
