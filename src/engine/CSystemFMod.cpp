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
	m_pFChannel = nullptr;
	m_pFChannelBackground = nullptr;
	m_pFChannelGroup = nullptr;
	m_pFChannelGroupBackground = nullptr;
	m_pFDSPFFT = nullptr;
	m_PlayDelaySecs = 0.0f;
	m_PlayTimeStart = 0;
	m_MusicPlay = false;
}
CSystemFMod::~CSystemFMod()
{
	m_pFChannel->stop();
	m_pFChannelBackground->stop();
	m_pFSound->release();
	m_pFChannelGroup->release();
	m_pFChannelGroupBackground->removeDSP(m_pFDSPFFT);
	m_pFChannelGroupBackground->release();
	m_pFDSPFFT->release();
	m_pFSystem->release();
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

    FMResult = m_pFSystem->createDSPByType(FMOD_DSP_TYPE_FFT, &m_pFDSPFFT);
    ERRCHECK(FMResult);
    m_pFDSPFFT->setParameterInt(FMOD_DSP_FFT_WINDOWTYPE, FMOD_DSP_FFT_WINDOW_BLACKMANHARRIS);
    m_pFDSPFFT->setParameterInt(FMOD_DSP_FFT_WINDOWSIZE, 4096 * 4);

    FMResult = m_pFSystem->getMasterChannelGroup(&m_pFChannelGroup);
    ERRCHECK(FMResult);
    //m_pFChannelGroup->addDSP(0, m_pFDSPFFT);

    FMResult = m_pFSystem->createChannelGroup("background", &m_pFChannelGroupBackground);
    ERRCHECK(FMResult);
    m_pFChannelGroupBackground->addDSP(FMOD_CHANNELCONTROL_DSP_TAIL, m_pFDSPFFT);
    m_pFChannelGroupBackground->setVolume(0.0f);

    return true;
}

void CSystemFMod::reset() noexcept
{

}

void CSystemFMod::update(float deltaTime) noexcept
{
	m_pFSystem->update();
	calculateFFTSpectrum(SPECLEN);

	if (!m_MusicPlay && m_PlayDelaySecs > 0.0f && ups::timeGet() - m_PlayTimeStart >= m_PlayDelaySecs * ups::timeFreq())
	{
		FMOD_RESULT FMResult = m_pFSystem->playSound(m_pFSound, m_pFChannelGroup, false, &m_pFChannel);
		if (FMResult == FMOD_OK)
		{
			m_pFChannel->setPosition(0, FMOD_TIMEUNIT_MS);
			m_MusicPlay = true;
		}
	}
}

bool CSystemFMod::loadMusic(const char *pData)
{
	FMOD_RESULT FMResult = FMOD_OK;

	if (m_pFSound)
		m_pFSound->release();
    FMResult = m_pFSystem->createSound(pData, FMOD_LOOP_NORMAL, 0, &m_pFSound);
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

	FMResult = m_pFSystem->playSound(m_pFSound, m_pFChannelGroupBackground, false, &m_pFChannelBackground);
	if (FMResult != FMOD_OK)
		return false;
	m_pFChannelBackground->setPosition(0, FMOD_TIMEUNIT_MS);

	return true;
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

unsigned int CSystemFMod::getCursorPositionInMilis(bool background) noexcept
{
	if (!m_pFChannelBackground)
		return 0;

	FMOD_RESULT FMResult = FMOD_OK;
	unsigned int cursorPos;
	if (background)
		m_pFChannelBackground->getPosition(&cursorPos, FMOD_TIMEUNIT_MS);
	else
		m_pFChannel->getPosition(&cursorPos, FMOD_TIMEUNIT_MS);
	ERRCHECK(FMResult);
	return cursorPos;
}

void CSystemFMod::stopMusic()
{
	FMOD_RESULT FMResult;
	if (m_pFChannelGroup && m_pFChannelGroupBackground)
	{
		FMResult = m_pFChannelGroupBackground->stop();
		ERRCHECK(FMResult);
		FMResult = m_pFChannelGroup->stop();
		ERRCHECK(FMResult);
		m_PlayDelaySecs = 0.0f;
		clearBuffers();
	}
}

void CSystemFMod::calculateFFTSpectrum(unsigned int length)
{
	FMOD_DSP_PARAMETER_FFT *fft;
	float dom_freq = 0.0f;

	m_pFDSPFFT->getParameterData(FMOD_DSP_FFT_SPECTRUMDATA, (void **)&fft, 0, 0, 0);
	m_pFDSPFFT->getParameterFloat(FMOD_DSP_FFT_DOMINANT_FREQ, &dom_freq, 0, 0);
	if (fft->numchannels < 2)
		return;

	for (unsigned int bin = 0; bin < length; bin++)
	{
		m_vLeftSpec[bin] = fft->spectrum[0][bin];
		m_vRightSpec[bin] = fft->spectrum[1][bin];
	}
}

void CSystemFMod::getEnergy(float *pEnergyKick, float *pEnergySnare)
{
	float LeftSum = 0.0f;
	float RightSum = 0.0f;

	// Detectar golpe de bombo
	for (unsigned int i=30; i<60; i++)
	{
		LeftSum += m_vLeftSpec[i];
		RightSum += m_vRightSpec[i];
	}
	*pEnergyKick = LeftSum*LeftSum + RightSum*RightSum;

	// Detectar golpe de caja
	LeftSum = RightSum = 0;
	for (unsigned int i=60; i<90; i++)
	{
		LeftSum += m_vLeftSpec[i];
		RightSum += m_vRightSpec[i];
	}
	*pEnergySnare = LeftSum*LeftSum + RightSum*RightSum;
}

void CSystemFMod::clearBuffers()
{
	memset(m_vLeftSpec, 0, sizeof(float)*SPECLEN);
	memset(m_vRightSpec, 0, sizeof(float)*SPECLEN);
}

void ERRCHECK_fn(FMOD_RESULT result, const char *file, int line)
{
    if (result != FMOD_OK)
    {
        std::cout << file << "(" << line << "): FMOD error " << result << " - " << FMOD_ErrorString(result) << std::endl;
        exit(-1);
    }
}
