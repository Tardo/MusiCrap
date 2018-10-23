/* (c) Alexandre Díaz. See licence.txt in the root of the distribution for more information. */

#ifndef H_ENGINE_FMOD
#define H_ENGINE_FMOD

#include "ISystem.hpp"
#include <FMOD/include/fmod.hpp>
#include <FMOD/include/fmod_errors.h>

void ERRCHECK_fn(FMOD_RESULT result, const char *file, int line);
#define ERRCHECK(_result) ERRCHECK_fn(_result, __FILE__, __LINE__)

class CSystemFMod : public ISystem
{
public:
	enum
	{
		SPECLEN=256,

		CHANNEL_SPEC = 0,
		CHANNEL_DELAYED,
		NUM_CHANNELS,

		CHANNEL_GROUP_MASTER = 0,
		CHANNEL_GROUP_BACKGROUND,
		NUM_CHANNEL_GROUPS,
	};

	CSystemFMod();
	~CSystemFMod();

	virtual bool init() noexcept final;
	virtual void reset() noexcept final;
	virtual void update(float deltaTime) noexcept final;

	bool loadMusic(const char *pData);
	bool playMusic(float delay);
	void stopMusic();

	void getEnergy(int channelGroup, float *pEnergyKick, float *pEnergySnare);
	unsigned int getCursorPositionInMilis(int channel) noexcept;

	unsigned int getLengthInMilis() noexcept;

	bool isPlaying(int channel);
	bool isMusicPlay() const noexcept { return m_MusicPlay; }

	float m_vLeftSpec[NUM_CHANNEL_GROUPS][SPECLEN];
	float m_vRightSpec[NUM_CHANNEL_GROUPS][SPECLEN];

protected:
	FMOD::System *m_pFSystem;
	FMOD::Sound *m_pFSound;
	FMOD::Channel *m_pFChannel[NUM_CHANNELS];
	FMOD::ChannelGroup *m_pFChannelGroup[NUM_CHANNEL_GROUPS];
	FMOD::DSP *m_pFDSPFFT[NUM_CHANNEL_GROUPS];

private:
	sf::Int64 m_PlayTimeStart;
	float m_PlayDelaySecs;
	bool m_MusicPlay;
	void calculateFFTSpectrum(int channelGroup, unsigned int length);
	void clearBuffers();
};

#endif
