/* (c) Alexandre Díaz. See licence.txt in the root of the distribution for more information. */

#include <engine/CAssetManager.hpp>
#include "CPlayer.hpp"

CPlayer::CPlayer() noexcept
{
	m_Points = 0;
	m_LineColor = sf::Color::White;
	m_Wins = 20; // Player fail offset
	m_Fails = 0;
	m_Combos = 0;
	m_LastComboTime = 0;
}
CPlayer::~CPlayer() noexcept
{
	#ifdef DEBUG_DESTRUCTORS
	ups::msgDebug("CPlayer", "Deleted");
	#endif
}

void CPlayer::update(float deltaTime) noexcept
{ }

float CPlayer::getRatio() const noexcept
{
	if (m_Wins+m_Fails)
		return (m_Wins*100)/(m_Wins+m_Fails);
	return 100.0f;
}
