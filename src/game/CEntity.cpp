/* (c) Alexandre Díaz. See licence.txt in the root of the distribution for more information. */

#include "CEntity.hpp"
#include <base/math.hpp>
#include <base/system.hpp>
#include <engine/CGame.hpp>
#include <game/CGameClient.hpp>


CEntity::CEntity(int type, const sf::Color color)
{
	m_Id = -1;
	m_Health = -1;
	m_Type = type;
	m_ToDelete = false;
	m_TimerStateAction = 0;
	m_Color = color;
	m_TextureId = -1;
	m_ContactFx = FX_NONE;
	m_SelfDelete = true;

	CGame *pGame = CGame::getInstance();
	m_Id = pGame->Client()->Controller()->Context()->addEntity(this);
}

CEntity::~CEntity()
{ }


