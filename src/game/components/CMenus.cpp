/* (c) Alexandre Díaz. See licence.txt in the root of the distribution for more information. */

#include <engine/CGame.hpp>
#include <engine/CLocalization.hpp>
#include <game/version.h>
#include "CMenus.hpp"
#include <engine/CSystemSound.hpp>

CMenus::CMenus(CGameClient *pGameClient) noexcept
: CComponent(pGameClient)
{
	m_ActiveMenu = m_ActiveModal = NONE;
	m_pEntity = nullptr;
}
CMenus::~CMenus() noexcept
{
	#ifdef DEBUG_DESTRUCTORS
	ups::msgDebug("CMenus", "Deleted");
	#endif
}

void CMenus::setActive(int mid, CEntity *pEnt) noexcept
{
	if (mid < NONE || mid > NUM_MENUS)
		return;

	m_pEntity = pEnt;
	m_ActiveMenu = mid;
}

void CMenus::setActiveModal(int mid) noexcept
{
	if (mid < NONE || mid > NUM_MODALS)
		return;

	m_ActiveModal = mid;
}

void CMenus::draw(sf::RenderTarget& target, sf::RenderStates states) const noexcept
{
	if (Client()->getRenderMode() != RENDER_MODE_NORMAL)
		return;

	target.setView(Client()->getHudView());

	static bool inMenu = false;
	if (!inMenu)
	{
		Client()->Menus().setActive(CMenus::MAIN);
		inMenu = true;
	}

	if (m_ActiveMenu == MAIN)
		renderMenuMain(target, states);
	else if (m_ActiveMenu == CONTROLS)
		renderMenuControls(target, states);

	if (m_ActiveModal == MODAL_KEY_BIND)
		renderModalKeyBind(target, states);
	else if (m_ActiveModal == MODAL_GAMEOVER)
		renderModalGameOver(target, states);
	else if (m_ActiveModal == MODAL_END_SONG)
		renderModalEndSong(target, states);
}

void CMenus::renderModalKeyBind(sf::RenderTarget& target, sf::RenderStates states) const noexcept
{
	sf::FloatRect rectArea;
	Client()->getViewportGlobalBounds(&rectArea, target.getView());

	const float w = 300.0f;
	const float h = 48.0f;
	const sf::FloatRect bounds(rectArea.width/2.0f - w/2.0f, rectArea.height/2.0f - h/2.0f, w, h);
	Client()->UI().doBox(target, states, bounds, sf::Color(36, 36, 36), 1.0f, sf::Color::Cyan);
	Client()->UI().doLabel(target, states, "PRESS ANY KEY...", bounds, sf::Color::White, CUI::TEXT_SIZE_NORMAL, CUI::ALIGN_CENTER);
}

void CMenus::renderModalGameOver(sf::RenderTarget& target, sf::RenderStates states) const noexcept
{
	sf::FloatRect rectArea;
	Client()->getViewportGlobalBounds(&rectArea, target.getView());

	const sf::Vector2f offset = sf::Vector2f(cosf(Client()->getElapsedTime()*0.85f)*12.0f, sinf(Client()->getElapsedTime()*0.85f)*12.0f);

	sf::FloatRect bounds(offset.x, offset.y, g_Config.m_ScreenWidth, g_Config.m_ScreenHeight);
	Client()->UI().doLabel(target, states, _("GAME OVER"), bounds, sf::Color::Red, 82, CUI::ALIGN_CENTER);
	bounds.left -= 2.0f;
	bounds.top -= 2.0f;
	Client()->UI().doLabel(target, states, _("GAME OVER"), bounds, sf::Color::White, 82, CUI::ALIGN_CENTER);
}

void CMenus::renderModalEndSong(sf::RenderTarget& target, sf::RenderStates states) const noexcept
{
	sf::FloatRect rectArea;
	Client()->getViewportGlobalBounds(&rectArea, target.getView());

	const sf::Vector2f offset = sf::Vector2f(cosf(Client()->getElapsedTime()*1.85f)*64.0f, 0.0f);

	sf::FloatRect bounds(offset.x, offset.y, g_Config.m_ScreenWidth, g_Config.m_ScreenHeight);
	Client()->UI().doLabel(target, states, _("YOU WIN!"), bounds, sf::Color(upm::randInt(128, 255), upm::randInt(128, 255), upm::randInt(128, 255)), 116, CUI::ALIGN_CENTER);
	bounds.left -= 2.0f;
	bounds.top -= 2.0f;
	Client()->UI().doLabel(target, states, _("YOU WIN!"), bounds, sf::Color(upm::randInt(0, 128), upm::randInt(0, 128), upm::randInt(0, 128)), 116, CUI::ALIGN_CENTER);
}

void CMenus::renderMenuControls(sf::RenderTarget& target, sf::RenderStates states) const noexcept
{
	sf::FloatRect rectArea;
	Client()->getViewportGlobalBounds(&rectArea, target.getView());
	float textW = 0.0f;

	sf::Text text;
	text.setFont(Client()->Assets().getDefaultFont());

	text.setCharacterSize(CUI::TEXT_SIZE_HEADER);
	text.setString(_("CONTROLS"));
	textW = text.getLocalBounds().width;
	text.setPosition(rectArea.width/2.0f-textW/2.0f, 50.0f);
	text.setFillColor(sf::Color::Cyan);
	target.draw(text, states);

	std::map<std::string, int> &binds = Client()->Controls().getCmdBinds();
	std::map<std::string, int>::const_iterator cit = binds.begin();
	int i = 0;
	static int buttonId;
	while (cit != binds.end())
	{
		text.setFillColor(sf::Color::Red);
		text.setCharacterSize(46);
		text.setString((*cit).first);
		textW = text.getLocalBounds().width;
		text.setPosition(rectArea.width/2.0f-190.0f, 240.0f+50.0f*i);
		target.draw(text, states);

		if (Client()->UI().doButton(target, states, &buttonId+i, CControls::getKeyName((*cit).second), sf::FloatRect(rectArea.width/2.0f+80.0f, 250.0f+50.0f*i, 110.0f, 35.0f), CUI::TEXT_SIZE_NORMAL, CUI::ALIGN_CENTER))
		{
			Client()->Menus().setActiveModal(MODAL_KEY_BIND);
			Client()->Controls().listenKeyBind((*cit).first.c_str());
		}
		++cit; ++i;
	}

	static int buttonIdB;
	if (Client()->UI().doButton(target, states, &buttonIdB, _("BACK"), sf::FloatRect(rectArea.width-100.0f, rectArea.height-100.0f, 80.0f, 35.0f), CUI::TEXT_SIZE_NORMAL, CUI::ALIGN_RIGHT))
	{
		Client()->Menus().setActive(MAIN);
		Client()->getSystem<CSystemSound>()->play(CAssetManager::SOUND_MOUSE_CLICK);
	}
}

void CMenus::renderMenuMain(sf::RenderTarget& target, sf::RenderStates states) const noexcept
{
	sf::FloatRect rectArea;
	Client()->getViewportGlobalBounds(&rectArea, target.getView());


	// Title Box
	sf::RectangleShape box(sf::Vector2f(rectArea.width-100, 100.0f));
	box.setPosition(50.0f, 50.0f);
	box.setTexture(Client()->Assets().getTexture(CAssetManager::TEXTURE_MVJAM));
	//box.setOutlineThickness(2.0f);
	//box.setOutlineColor(sf::Color(0, 0, 200, 80));
	target.draw(box, states);

	sf::Text text;
	text.setFont(Client()->Assets().getDefaultFont());

	text.setCharacterSize(CUI::TEXT_SIZE_SMALL);
	char title[128];
	snprintf(title, sizeof(title), "unsigned char* (C) 2018 - v%s", GAME_RELEASE_VERSION);
	text.setString(title);
	float textH = text.getLocalBounds().height;
	text.setPosition(10.0f, rectArea.height-textH-10.0f);
	text.setFillColor(sf::Color::White);
	target.draw(text, states);

	static int buttonIdA;
	if (Client()->UI().doButton(target, states, &buttonIdA, _("START GAME"), sf::FloatRect(rectArea.width/2.0f-125.0f, rectArea.height/2.0f-100.0f, 250.0f, 35.0f), CUI::TEXT_SIZE_NORMAL, CUI::ALIGN_CENTER))
	{
		Client()->initializeGameMode("main");
		Client()->getSystem<CSystemSound>()->play(CAssetManager::SOUND_MOUSE_CLICK);
	}

	static int buttonIdB;
	if (Client()->UI().doButton(target, states, &buttonIdB, _("CONTROLS"), sf::FloatRect(rectArea.width/2.0f-125.0f, rectArea.height/2.0f-60.0f, 250.0f, 35.0f), CUI::TEXT_SIZE_NORMAL, CUI::ALIGN_CENTER))
	{
		Client()->getSystem<CSystemSound>()->play(CAssetManager::SOUND_MOUSE_CLICK);
		Client()->Menus().setActive(CONTROLS);
	}

	static int buttonIdD;
	if (Client()->UI().doButton(target, states, &buttonIdD, _("EXIT"), sf::FloatRect(rectArea.width/2.0f-125.0f, rectArea.height/2.0f, 250.0f, 35.0f), CUI::TEXT_SIZE_NORMAL, CUI::ALIGN_CENTER))
	{
		Client()->getSystem<CSystemSound>()->play(CAssetManager::SOUND_MOUSE_CLICK);
		Client()->close();
	}

	sf::RectangleShape sfmlLogo(sf::Vector2f(200.0f, 70.0f));
	sfmlLogo.setPosition(rectArea.width-480.0f, rectArea.height-80.0f);
	sfmlLogo.setTexture(Client()->Assets().getTexture(CAssetManager::TEXTURE_SFML_LOGO));
	target.draw(sfmlLogo, states);

	sf::RectangleShape fmodLogo(sf::Vector2f(230.0f, 70.0f));
	fmodLogo.setPosition(rectArea.width-250.0f, rectArea.height-80.0f);
	fmodLogo.setTexture(Client()->Assets().getTexture(CAssetManager::TEXTURE_FMOD_LOGO));
	target.draw(fmodLogo, states);
}
