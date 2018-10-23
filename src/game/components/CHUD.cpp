/* (c) Alexandre Díaz. See licence.txt in the root of the distribution for more information. */

#include <engine/CGame.hpp>
#include <engine/CLocalization.hpp>
#include <game/version.h>
#include "CHUD.hpp"

CHUD::CHUD(CGameClient *pGameClient) noexcept
: CComponent(pGameClient)
{ }
CHUD::~CHUD() noexcept
{
	#ifdef DEBUG_DESTRUCTORS
	ups::msgDebug("CHUD", "Deleted");
	#endif
}

void CHUD::draw(sf::RenderTarget& target, sf::RenderStates states) const noexcept
{
	if (Client()->getRenderMode() != RENDER_MODE_NORMAL)
		return;

	target.setView(Client()->getHudView());

	if (Client()->Menus().getActive() == CMenus::NONE)
	{
		renderMessages(target, states);
		renderTime(target, states);
		renderScore(target, states);
	}

	renderCursor(target, states);
}

void CHUD::renderTime(sf::RenderTarget& target, sf::RenderStates states) const noexcept
{
	sf::FloatRect rectArea;
	Client()->getViewportGlobalBounds(&rectArea, target.getView());

	CSystemFMod *pSystemFMod = Client()->getSystem<CSystemFMod>();
	const sf::Int64 elapsedTimeMili = static_cast<const sf::Int64>(pSystemFMod->getLengthInMilis() - pSystemFMod->getCursorPositionInMilis(true));
	if (elapsedTimeMili > 0)
	{
		const int elapsedTimeSeconds = elapsedTimeMili / 1000;
		char aBuff[18];
		sprintf(aBuff, "%02d:%02d.%03d", elapsedTimeSeconds/60, elapsedTimeSeconds%60, (int)(elapsedTimeMili%1000));
		Client()->UI().doLabel(target, states, aBuff, sf::FloatRect(0.0f, 0.0f, rectArea.width, 80.0f), sf::Color::Red, 42u, CUI::ALIGN_CENTER);
	}
}

void CHUD::renderMessages(sf::RenderTarget& target, sf::RenderStates states) const noexcept
{
	if (Client()->Menus().getActive() != CMenus::NONE)
		return;

	sf::FloatRect rectArea;
	Client()->getViewportGlobalBounds(&rectArea, Client()->getHudView());

	sf::Text sfStr;
	sfStr.setFont(Client()->Assets().getDefaultFont());

	if (Client()->m_Debug)
	{
		char aBuff[128];
		sfStr.setCharacterSize(24);
		snprintf(aBuff, sizeof(aBuff), "FPS: %u [Min: %u] [%.2fms]", Client()->m_FPS, Client()->m_MinFPS, Client()->getDeltaTime()*1000.0f);
		sfStr.setString(aBuff);
		sfStr.setPosition(rectArea.width-sfStr.getLocalBounds().width-10.0f, 0.0f);
		sfStr.setFillColor(sf::Color::Red);
		target.draw(sfStr, states);
	}

	if (ups::timeGet() - Client()->m_TimerBroadcast < ups::timeFreq()*Client()->m_BroadcastDuration)
	{
		sfStr.setCharacterSize(72);
		sfStr.setString(Client()->m_aBroadcastMsg);
		sfStr.setPosition(rectArea.width/2.0f-sfStr.getLocalBounds().width/2.0f+2.0f, 120.0f);
		sfStr.setFillColor(sf::Color::Yellow);
		target.draw(sfStr, states);

		sfStr.setPosition(rectArea.width/2.0f-sfStr.getLocalBounds().width/2.0f, 118.0f);
		sfStr.setFillColor(sf::Color::Red);
		target.draw(sfStr, states);
	}

	if (Client()->m_aHelpMsg[0] != 0)
	{
		sfStr.setCharacterSize(92);
		sfStr.setString(Client()->m_aHelpMsg);
		sfStr.setPosition(rectArea.width/2.0f-sfStr.getLocalBounds().width/2.0f+2.0f, 78.0f+2.0f);
		sfStr.setFillColor(sf::Color::Black);
		target.draw(sfStr, states);

		sfStr.setPosition(rectArea.width/2.0f-sfStr.getLocalBounds().width/2.0f, 78.0f);
		sfStr.setFillColor(sf::Color::White);
		target.draw(sfStr, states);
	}
}

void CHUD::renderCursor(sf::RenderTarget& target, sf::RenderStates states) const noexcept
{
	sf::Vertex line[] =
	{
		sf::Vertex(sf::Vector2f(Client()->UI().getMousePos().x-10.0f, Client()->UI().getMousePos().y), sf::Color::White),
		sf::Vertex(sf::Vector2f(Client()->UI().getMousePos().x+10.0f, Client()->UI().getMousePos().y), sf::Color::White),
		sf::Vertex(sf::Vector2f(Client()->UI().getMousePos().x, Client()->UI().getMousePos().y-10.0f), sf::Color::White),
		sf::Vertex(sf::Vector2f(Client()->UI().getMousePos().x, Client()->UI().getMousePos().y+10.0f), sf::Color::White)
	};

	target.draw(line, 4, sf::Lines);
}

void CHUD::renderScore(sf::RenderTarget& target, sf::RenderStates states) const noexcept
{
	if (!Client()->Controller() || !Client()->Controller()->Context() || !Client()->Controller()->Context()->getPlayer())
		return;

	sf::Text sfStr;
	sfStr.setFont(Client()->Assets().getDefaultFont());

	CPlayer *pPlayer = Client()->Controller()->Context()->getPlayer();

	char aBuff[128];

	snprintf(aBuff, sizeof(aBuff), "%.2f%%", pPlayer->getRatio());
	Client()->UI().doLabel(target, states, aBuff, sf::FloatRect(0.0f, 0.0f, g_Config.m_ScreenWidth, 30.0f), sf::Color::Red, 24, CUI::ALIGN_CENTER);

	snprintf(aBuff, sizeof(aBuff), _("Score: %04d"), pPlayer->getPoints());
	Client()->UI().doLabel(target, states, aBuff, sf::FloatRect(5.0f, 5.0f, g_Config.m_ScreenWidth, 30.0f), sf::Color::Red, 24, CUI::ALIGN_LEFT);

}
