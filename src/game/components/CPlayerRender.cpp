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
	if (!Client()->Controller() || !Client()->Controller()->Context() || !Client()->Controller()->Context()->getPlayer() || Client()->getRenderMode() != RENDER_MODE_LIGHTING)
		return;

	target.setView(Client()->Camera());

	renderLine(target, states, Client()->Controller()->Context()->getPlayer());
	renderSpectrum(target, states, Client()->Controller()->Context()->getPlayer());
}

void CPlayerRender::renderLine(sf::RenderTarget& target, sf::RenderStates states, CPlayer *pPlayer) const noexcept
{
	sf::RectangleShape rect(sf::Vector2f(g_Config.m_ScreenWidth, USER_ZONE_HEIGTH));
	rect.setFillColor(pPlayer->getLineColor());
	rect.setPosition(-g_Config.m_ScreenWidth/2.0f, g_Config.m_ScreenHeight/4.0f-USER_ZONE_HEIGTH/2.0f);

	target.draw(rect, states);
}

void CPlayerRender::renderSpectrum(sf::RenderTarget& target, sf::RenderStates states, CPlayer *pPlayer) const noexcept
{
	CSystemFMod *pSystemFMod = Client()->getSystem<CSystemFMod>();
	static int barW = g_Config.m_ScreenWidth/CSystemFMod::SPECLEN;

//	sf::Vertex array[CSystemFMod::SPECLEN];
	sf::RectangleShape box;
	box.setFillColor(pPlayer->getLineColor());
	for (int i=0; i<CSystemFMod::SPECLEN; ++i)
	{
//		float barSize = (pSystemFMod->m_vLeftSpec[i] + pSystemFMod->m_vRightSpec[i])*10000.0f;
//		array[i] = sf::Vertex(sf::Vector2f(-g_Config.m_ScreenWidth/2.0f+barW*i, g_Config.m_ScreenHeight/4.0f - barSize), sf::Color::Red);
		box.setSize(sf::Vector2f(barW, pSystemFMod->m_vLeftSpec[i]*3000.0f));
		box.setOrigin(0.0f, pSystemFMod->m_vLeftSpec[i]*3000.0f);
		box.setPosition(-g_Config.m_ScreenWidth/2.0f+barW*i, g_Config.m_ScreenHeight/4.0f);
		target.draw(box, states);

		box.setSize(sf::Vector2f(barW, pSystemFMod->m_vRightSpec[i]*3000.0f));
		box.setOrigin(0.0f, 0.0f);
		box.setPosition(-g_Config.m_ScreenWidth/2.0f+barW*i, g_Config.m_ScreenHeight/4.0f);
		target.draw(box, states);
	}
//	target.draw(array, CSystemFMod::SPECLEN, sf::LineStrip);
}
