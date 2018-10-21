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
	CGame *pGame = CGame::getInstance();
	CSystemFMod *pSystemFMod = pGame->Client()->getSystem<CSystemFMod>();
	float energyKick, energySnare;
	pSystemFMod->getEnergy(&energyKick, &energySnare);
	energyKick *= 10.0f;
	energySnare *= 10.0f;

	if (energyKick > 1.0f || energySnare > 0.6f)
	{
		if (!m_KickUsed)
			m_AddRing = true;
		m_KickUsed = true;
	} else
	{
		m_KickUsed = false;
	}

	if (m_AddRing)
	{
		static sf::Color colors[3] = {
			sf::Color::Red,
			sf::Color::Green,
			sf::Color::Blue
		};
		m_vRings.push_back(CPulsarRing(m_Pos, colors[upm::randInt(0, 2)], 0.0f, (energyKick+energySnare)*4.0f, pGame->Client()->m_TestVar));
		m_AddRing = false;
	}


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

const CPulsarRing* CPulsar::ringInZone(float top, float bottom) noexcept
{
	std::vector<CPulsarRing>::const_iterator cit = m_vRings.cbegin();
	while (cit != m_vRings.cend())
	{
		const float curPosY = (*cit).m_Pos.y + (*cit).m_Size/2.0f;
		if (curPosY >= top && curPosY <= bottom)
			return &(*cit);
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
