/* (c) Alexandre Díaz. See licence.txt in the root of the distribution for more information. */

#include <base/math.hpp>
#include <base/system.hpp>
#include <engine/CAssetManager.hpp>
#include <engine/CSystemSound.hpp>
#include <engine/CLocalization.hpp>
#include <game/controllers/CControllerMain.hpp>
#include <game/CContext.hpp>
#include <cstring>

CControllerMain::CControllerMain() noexcept
: CController()
{
	m_pPulsar = nullptr;
}
CControllerMain::CControllerMain(class CContext *pContext) noexcept
: CController(pContext)
{
	m_pPulsar = nullptr;
}
CControllerMain::~CControllerMain() noexcept
{
	Game()->Client()->getSystem<CSystemSound>()->stopBackgroundMusic();
	Game()->Client()->getSystem<CSystemFMod>()->stopMusic();
	m_pPulsar->clearRings();


	#ifdef DEBUG_DESTRUCTORS
	ups::msgDebug("CControllerMain", "Deleted");
	#endif
}

void CControllerMain::onSystemEvent(sf::Event *pEvent) noexcept
{ }

bool CControllerMain::onInit() noexcept
{
	return CController::onInit();
}

void CControllerMain::onReset() noexcept
{
	Game()->Client()->getSystem<CSystemSound>()->stopBackgroundMusic();
	Game()->Client()->getSystem<CSystemFMod>()->stopMusic();

	return CController::onReset();
}

void CControllerMain::tick() noexcept
{
	CController::tick();

	CPlayer *pMainPlayer = Context()->getPlayer();
	CSystemSound *pSystemSound = Game()->Client()->getSystem<CSystemSound>();
	float ringOffset = 0.0f;
	const CPulsarRing *pRing = m_pPulsar->ringInZone(g_Config.m_ScreenHeight/4.0f-USER_ZONE_HEIGTH, g_Config.m_ScreenHeight/4.0f+USER_ZONE_HEIGTH/2.0f, &ringOffset);
	static CPulsarRing *pLastRing = nullptr;

	// Check invalid rings
	if (!pRing && pLastRing)
	{
		pLastRing->m_Color = sf::Color::White;
		pLastRing = nullptr;
		pSystemSound->play(CAssetManager::SOUND_ERROR, 100.0f);
		pMainPlayer->addPoints(-175);
		++pMainPlayer->m_Fails;
		checkPlayerCombos(pMainPlayer);
	}

	// Check Player Ratio
	if (pMainPlayer->getRatio() < g_Config.m_MinPlayerRatio)
	{
		Game()->Client()->Menus().setActiveModal(CMenus::MODAL_GAMEOVER);
		CSystemFMod *pSystemFMod = Game()->Client()->getSystem<CSystemFMod>();
		pSystemFMod->stopMusic();
	}

	// Check end song
	CSystemFMod *pSystemFMod = Game()->Client()->getSystem<CSystemFMod>();
	if (pSystemFMod->isMusicPlay() && !pSystemFMod->isPlaying(CSystemFMod::CHANNEL_DELAYED))
	{
		Game()->Client()->Menus().setActiveModal(CMenus::MODAL_END_SONG);
	}

	if (Game()->Client()->hasFocus() && Game()->Client()->Menus().getActiveModal() == CMenus::NONE)
	{
		// Player Control
		int selectedSound = 0;
		if (Game()->Client()->Controls().isKeyPressed("lineRed"))
			pMainPlayer->setLineColor(sf::Color::Red);
		else if (Game()->Client()->Controls().isKeyPressed("lineGreen"))
		{
			selectedSound = 1;
			pMainPlayer->setLineColor(sf::Color::Green);
		}
		else if (Game()->Client()->Controls().isKeyPressed("lineBlue"))
		{
			selectedSound = 2;
			pMainPlayer->setLineColor(sf::Color::Blue);
		}
		else
			pMainPlayer->setLineColor(sf::Color::White);

		// Interact with Pulsar Rings
		static bool keyPressed = false;
		const bool fire = Game()->Client()->Controls().isKeyPressed("lineRed") || Game()->Client()->Controls().isKeyPressed("lineGreen") || Game()->Client()->Controls().isKeyPressed("lineBlue");
		if (!keyPressed && fire)
		{
			keyPressed = true;
			if (pRing && pRing->m_Color == pMainPlayer->getLineColor())
			{
				m_pPulsar->removeRing(*pRing);
				pSystemSound->play(CAssetManager::SOUND_GOOD_L + selectedSound, 100.0f);
				pMainPlayer->addPoints(100);
				++pMainPlayer->m_Wins;

				if (fabs(ringOffset) <= 5.0f)
				{
					++pMainPlayer->m_Combos;
					pMainPlayer->m_LastComboTime = ups::timeGet();
					if (pMainPlayer->m_Combos >= g_Config.m_MinComboCount)
					{
						char aBuff[255];
						snprintf(aBuff, sizeof(aBuff), _("Perfect!!! x%d"), pMainPlayer->m_Combos);
						Game()->Client()->showBroadcastMessage(aBuff, 0.6f);
					}
				} else if (fabs(ringOffset) <= 10.0f)
				{
					Game()->Client()->showBroadcastMessage(_("So Good!"), 0.6f);
					checkPlayerCombos(pMainPlayer);
				}
				else if (fabs(ringOffset) <= USER_ZONE_HEIGTH/2.0f)
				{
					Game()->Client()->showBroadcastMessage(_("Can do it better"), 0.6f);
					checkPlayerCombos(pMainPlayer);
				}
				pRing = nullptr;
			}
			else
			{
				pSystemSound->play(CAssetManager::SOUND_ERROR, 100.0f);
				pMainPlayer->addPoints(-175);
				++pMainPlayer->m_Fails;
				checkPlayerCombos(pMainPlayer);
			}
		} else if (!fire)
		{
			keyPressed = false;
		}

		pLastRing = const_cast<CPulsarRing*>(pRing);
	}
}

void CControllerMain::checkPlayerCombos(CPlayer *pPlayer) noexcept
{
	if (pPlayer->m_Combos && pPlayer->m_Combos >= g_Config.m_MinComboCount)
	{
		pPlayer->addPoints(100*pPlayer->m_Combos);
		pPlayer->m_Combos = 0;

		for (int i=0; i<g_Config.m_ScreenWidth; i+=50)
			createImpactSparkMetal(sf::Vector2f(-g_Config.m_ScreenWidth/2.0f + i, g_Config.m_ScreenHeight/4.0f-USER_ZONE_HEIGTH/2.0f), sf::Color::White);
	}
}

void CControllerMain::onStart() noexcept
{
	CController::onStart();
	Context()->createPlayer();
	Game()->Client()->Menus().setActiveModal(CMenus::NONE);
	Game()->Client()->Camera().setCenter(0.0f ,0.0f);

	m_pPulsar = new CPulsar(sf::Vector2f(0.0f, -g_Config.m_ScreenHeight/2.0f), 85.0f);

	// Play Music
	CSystemFMod *pSystemFMod = Game()->Client()->getSystem<CSystemFMod>();
	pSystemFMod->loadMusic(g_Config.m_SongFilename);

	// Calculate Start Time
	const float min = m_pPulsar->getPosition().y;
	const float max = g_Config.m_ScreenHeight/4.0f+USER_ZONE_HEIGTH/2.0f;
	pSystemFMod->playMusic((max - min)/g_Config.m_PulsarRingVelocity * 2.0f);
}
