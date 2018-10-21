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
	};

	CSystemFMod();
	~CSystemFMod();

	virtual bool init() noexcept final;
	virtual void reset() noexcept final;
	virtual void update(float deltaTime) noexcept final;

	bool loadMusic(const char *pData);
	bool playMusic(float delay);
	void stopMusic();
	void getEnergy(float *pEnergyKick, float *pEnergySnare);

	unsigned int getLengthInMilis() noexcept;
	unsigned int getCursorPositionInMilis(bool background) noexcept;

	float m_vLeftSpec[SPECLEN];
	float m_vRightSpec[SPECLEN];

protected:
	FMOD::System *m_pFSystem;
	FMOD::Sound *m_pFSound;
	FMOD::Channel *m_pFChannel;
	FMOD::Channel *m_pFChannelBackground;
	FMOD::ChannelGroup *m_pFChannelGroupBackground;
	FMOD::ChannelGroup *m_pFChannelGroup;
	FMOD::DSP *m_pFDSPFFT;

private:
	sf::Int64 m_PlayTimeStart;
	float m_PlayDelaySecs;
	bool m_MusicPlay;
	void calculateFFTSpectrum(unsigned int length);
	void clearBuffers();
};

#endif
