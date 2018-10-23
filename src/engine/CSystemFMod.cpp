/* (c) Alexandre Díaz. See licence.txt in the root of the distribution for more information. */

#include <iostream>
#include <cstdlib>
#include <cstring>
#include <base/system.hpp>
#include "CSystemFMod.hpp"

CSystemFMod::CSystemFMod()
{
	m_pFSystem = nullptr;
	m_pFSound = nullptr;
	m_PlayDelaySecs = 0.0f;
	m_PlayTimeStart = 0;
	m_MusicPlay = false;

	for (int i=0; i<NUM_CHANNELS; m_pFChannel[i++]=nullptr);
	for (int i=0; i<NUM_CHANNEL_GROUPS; m_pFChannelGroup[i++]=nullptr);
	for (int i=0; i<NUM_CHANNEL_GROUPS; m_pFDSPFFT[i++]=nullptr);

	clearBuffers();
}
CSystemFMod::~CSystemFMod()
{
	for (int i=0; i<NUM_CHANNELS; ++i)
	{
		if (m_pFChannel[i++])
			m_pFChannel[i++]->stop();
	}

	if (m_pFSound)
		m_pFSound->release();

	for (int i=0; i<NUM_CHANNEL_GROUPS; ++i)
	{
		if (m_pFChannelGroup[i])
		{
			m_pFChannelGroup[i]->release();
			if (m_pFDSPFFT[i])
				m_pFChannelGroup[i]->removeDSP(m_pFDSPFFT[i]);
		}
	}

	for (int i=0; i<NUM_CHANNEL_GROUPS; ++i)
	{
		if (m_pFDSPFFT[i])
			m_pFDSPFFT[i]->release();
	}

	if (m_pFSystem)
		m_pFSystem->release();

	reset();
}

bool CSystemFMod::init() noexcept
{
    FMOD_RESULT FMResult;
    unsigned int FMVersion;

    FMResult = FMOD::System_Create(&m_pFSystem);
    ERRCHECK(FMResult);

    FMResult = m_pFSystem->getVersion(&FMVersion);
	ERRCHECK(FMResult);

	if (FMVersion < FMOD_VERSION)
	{
	   std::cout << "FMOD lib version " << FMVersion << " doesn't match header version " << FMOD_VERSION << std::endl;
	   return false;
	}

    FMResult = m_pFSystem->init(32, FMOD_INIT_NORMAL, 0);
    ERRCHECK(FMResult);

    // Initialize DSP's
    for (int i=0; i<NUM_CHANNEL_GROUPS; ++i)
    {
		FMResult = m_pFSystem->createDSPByType(FMOD_DSP_TYPE_FFT, &m_pFDSPFFT[i]);
		ERRCHECK(FMResult);
		m_pFDSPFFT[i]->setParameterInt(FMOD_DSP_FFT_WINDOWTYPE, FMOD_DSP_FFT_WINDOW_BLACKMANHARRIS);
		m_pFDSPFFT[i]->setParameterInt(FMOD_DSP_FFT_WINDOWSIZE, 4096 * 4);
    }

    // Master Channel Group
    FMResult = m_pFSystem->getMasterChannelGroup(&m_pFChannelGroup[CHANNEL_GROUP_MASTER]);
    ERRCHECK(FMResult);
    m_pFChannelGroup[CHANNEL_GROUP_MASTER]->addDSP(0, m_pFDSPFFT[CHANNEL_GROUP_MASTER]);

    // Background Channel Group
    FMResult = m_pFSystem->createChannelGroup("background", &m_pFChannelGroup[CHANNEL_GROUP_BACKGROUND]);
    ERRCHECK(FMResult);
    m_pFChannelGroup[CHANNEL_GROUP_BACKGROUND]->addDSP(FMOD_CHANNELCONTROL_DSP_TAIL, m_pFDSPFFT[CHANNEL_GROUP_BACKGROUND]);
    m_pFChannelGroup[CHANNEL_GROUP_BACKGROUND]->setVolume(0.0f);

    return true;
}

void CSystemFMod::reset() noexcept
{
	clearBuffers();
}

void CSystemFMod::update(float deltaTime) noexcept
{
	m_pFSystem->update();

	for (int i=0; i<NUM_CHANNEL_GROUPS; calculateFFTSpectrum(i++, SPECLEN));

	if (!m_MusicPlay && m_PlayDelaySecs > 0.0f && ups::timeGet() - m_PlayTimeStart >= m_PlayDelaySecs * ups::timeFreq())
	{
		FMOD_RESULT FMResult = m_pFSystem->playSound(m_pFSound, m_pFChannelGroup[CHANNEL_GROUP_MASTER], false, &m_pFChannel[CHANNEL_DELAYED]);
		if (FMResult == FMOD_OK)
		{
			m_pFChannel[CHANNEL_DELAYED]->setPosition(0, FMOD_TIMEUNIT_MS);
			m_MusicPlay = true;
		}
	}
}

bool CSystemFMod::loadMusic(const char *pData)
{
	FMOD_RESULT FMResult = FMOD_OK;

	if (m_pFSound)
		m_pFSound->release();
    FMResult = m_pFSystem->createSound(pData, 0, 0, &m_pFSound);
    if (FMResult != FMOD_OK)
    	return false;

    return true;
}

bool CSystemFMod::playMusic(float delay)
{
	FMOD_RESULT FMResult = FMOD_OK;
	stopMusic();

	m_PlayDelaySecs = delay;
	m_PlayTimeStart = ups::timeGet();
	m_MusicPlay = false;

	FMResult = m_pFSystem->playSound(m_pFSound, m_pFChannelGroup[CHANNEL_GROUP_BACKGROUND], false, &m_pFChannel[CHANNEL_SPEC]);
	if (FMResult != FMOD_OK)
		return false;
	m_pFChannel[CHANNEL_SPEC]->setPosition(0, FMOD_TIMEUNIT_MS);

	return true;
}

bool CSystemFMod::isPlaying(int channel)
{
	bool playing = false;
	m_pFChannel[channel]->isPlaying(&playing);
	return playing;
}

unsigned int CSystemFMod::getLengthInMilis() noexcept
{
	if (!m_pFSound)
		return 0;

	FMOD_RESULT FMResult = FMOD_OK;
	unsigned int seconds;
	FMResult = m_pFSound->getLength(&seconds, FMOD_TIMEUNIT_MS);
	ERRCHECK(FMResult);
	return seconds;
}

unsigned int CSystemFMod::getCursorPositionInMilis(int channel) noexcept
{
	if (!m_pFChannel[channel])
		return 0;

	FMOD_RESULT FMResult = FMOD_OK;
	unsigned int cursorPos;
	m_pFChannel[channel]->getPosition(&cursorPos, FMOD_TIMEUNIT_MS);
	ERRCHECK(FMResult);
	return cursorPos;
}

void CSystemFMod::stopMusic()
{
	FMOD_RESULT FMResult;

	for (int i=0; i<NUM_CHANNEL_GROUPS; ++i)
	{
		if (m_pFChannelGroup[i])
		{
			FMResult = m_pFChannelGroup[i]->stop();
			ERRCHECK(FMResult);
		}
	}

	m_MusicPlay = false;
	m_PlayDelaySecs = 0.0f;
}

void CSystemFMod::calculateFFTSpectrum(int channelGroup, unsigned int length)
{
	FMOD_DSP_PARAMETER_FFT *fft;
	m_pFDSPFFT[channelGroup]->getParameterData(FMOD_DSP_FFT_SPECTRUMDATA, (void **)&fft, 0, 0, 0);
	if (!fft || fft->numchannels < 2)
		return;

	memcpy(m_vLeftSpec[channelGroup], fft->spectrum[0], length);
	memcpy(m_vRightSpec[channelGroup], fft->spectrum[1], length);
}

void CSystemFMod::getEnergy(int channelGroup, float *pEnergyKick, float *pEnergySnare)
{
	float LeftSum = 0.0f;
	float RightSum = 0.0f;

	// Detectar golpe de bombo
	if (pEnergyKick)
	{
		for (unsigned int i=5; i<30; i++)
		{
			LeftSum += m_vLeftSpec[channelGroup][i];
			RightSum += m_vRightSpec[channelGroup][i];
		}
		*pEnergyKick = LeftSum*LeftSum + RightSum*RightSum;
	}

	// Detectar golpe de caja
	if (pEnergySnare)
	{
		LeftSum = RightSum = 0;
		for (unsigned int i=30; i<40; i++)
		{
			LeftSum += m_vLeftSpec[channelGroup][i];
			RightSum += m_vRightSpec[channelGroup][i];
		}
		*pEnergySnare = LeftSum*LeftSum + RightSum*RightSum;
	}
}

void CSystemFMod::clearBuffers()
{
	for (int i=0; i<NUM_CHANNEL_GROUPS; ++i)
	{
		memset(m_vLeftSpec[i], 0, sizeof(float)*SPECLEN);
		memset(m_vRightSpec[i], 0, sizeof(float)*SPECLEN);
	}
}

void ERRCHECK_fn(FMOD_RESULT result, const char *file, int line)
{
    if (result != FMOD_OK)
    {
        std::cout << file << "(" << line << "): FMOD error " << result << " - " << FMOD_ErrorString(result) << std::endl;
        exit(-1);
    }
}
