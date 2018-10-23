#include <engine/CGame.hpp>
#include <engine/CAssetManager.hpp>
#include <engine/CConfig.hpp>
#include <engine/CSystemSound.hpp>
#include "CPulsar.hpp"

CPulsar::CPulsar(const sf::Vector2f &pos, float size) noexcept
: CEntity(CEntity::PULSAR, sf::Color::Red)
{
    m_Pos = pos;
    m_OrgSize = m_Size = size;
    m_KickUsed = false;
    m_SnareUsed = false;
    m_AddRing = false;
}

CPulsar::~CPulsar() noexcept
{
	#ifdef DEBUG_DESTRUCTORS
	ups::msgDebug("CPulsar", "Deleted");
	#endif
}

void CPulsar::tick() noexcept
{
	static sf::Color colors[3] = {
		sf::Color::Red,
		sf::Color::Green,
		sf::Color::Blue
	};
	CGame *pGame = CGame::getInstance();
	CSystemFMod *pSystemFMod = pGame->Client()->getSystem<CSystemFMod>();
	float energyKick;
	pSystemFMod->getEnergy(CSystemFMod::CHANNEL_GROUP_BACKGROUND, &energyKick, 0x0);
	energyKick *= 10.0f;
	if (energyKick > g_Config.m_MinKickEnergy)
	{
		if (!m_KickUsed)
			m_vRings.push_back(CPulsarRing(m_Pos, colors[upm::randInt(0, 2)], 0.0f, 1.0f+energyKick*2.5f, pGame->Client()->m_TestVar));
		m_KickUsed = true;
	} else
		m_KickUsed = false;

	std::vector<CPulsarRing>::iterator it = m_vRings.begin();
	while (it != m_vRings.end())
	{
		(*it).m_Size += g_Config.m_PulsarRingVelocity * pGame->Client()->getDeltaTime();
		if ((*it).m_Size >= g_Config.m_ScreenHeight*2.0f)
			it = m_vRings.erase(it);
		else
			++it;
	}
}

const CPulsarRing* CPulsar::ringInZone(float top, float bottom, float *pMidDist) noexcept
{
	std::vector<CPulsarRing>::const_iterator cit = m_vRings.cbegin();
	const float midPos = top+(bottom-top)/2.0f;
	while (cit != m_vRings.cend())
	{
		const float curPosY = (*cit).m_Pos.y + (*cit).m_Size/2.0f;
		if (curPosY >= top && curPosY <= bottom)
		{
			*pMidDist = midPos-curPosY;
			return &(*cit);
		}
		++cit;
	}

	return nullptr;
}

bool CPulsar::removeRing(const CPulsarRing &ring) noexcept
{
	std::vector<CPulsarRing>::iterator it = m_vRings.begin();
	while (it != m_vRings.end())
	{
		if (&(*it) == &ring)
		{
			m_vRings.erase(it);
			return true;
		}
		++it;
	}

	return false;
}

void CPulsar::clearRings() noexcept
{
	m_vRings.clear();
}
