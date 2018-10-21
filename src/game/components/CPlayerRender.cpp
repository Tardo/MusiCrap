/* (c) Alexandre Díaz. See licence.txt in the root of the distribution for more information. */

#include <base/math.hpp>
#include <engine/CGame.hpp>
#include "CPlayerRender.hpp"

CPlayerRender::CPlayerRender(CGameClient *pGameClient) noexcept
: CComponent(pGameClient)
{
}
CPlayerRender::~CPlayerRender() noexcept
{
	#ifdef DEBUG_DESTRUCTORS
	ups::msgDebug("CPlayerRender", "Deleted");
	#endif
}

void CPlayerRender::draw(sf::RenderTarget& target, sf::RenderStates states) const noexcept
{
	if (!Client()->Controller() || !Client()->Controller()->Context() || !Client()->Controller()->Context()->getPlayer())
		return;

	target.setView(Client()->Camera());

	if (Client()->getRenderMode() == RENDER_MODE_NORMAL)
	{
		renderLine(target, states, Client()->Controller()->Context()->getPlayer());
	}
	else if (Client()->getRenderMode() == RENDER_MODE_LIGHTING)
	{
		renderLine(target, states, Client()->Controller()->Context()->getPlayer());
	}
}

void CPlayerRender::renderLine(sf::RenderTarget& target, sf::RenderStates states, CPlayer *pPlayer) const noexcept
{
	sf::RectangleShape rect(sf::Vector2f(g_Config.m_ScreenWidth, USER_ZONE_HEIGTH));
	rect.setFillColor(pPlayer->getLineColor());
	rect.setPosition(-g_Config.m_ScreenWidth/2.0f, g_Config.m_ScreenHeight/4.0f+USER_ZONE_HEIGTH);

	target.draw(rect, states);
}
