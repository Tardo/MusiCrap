/* (c) Alexandre Díaz. See licence.txt in the root of the distribution for more information. */

#include <base/system.hpp>
#include <engine/CGame.hpp>
#include <game/controllers/CControllerMain.hpp>
#include <game/CController.hpp>


CController::CController() noexcept
{
	m_pGame = CGame::getInstance();
	m_pGameContext = new CContext();
}
CController::CController(class CContext *pContext) noexcept
{
	m_pGame = CGame::getInstance();
	m_pGameContext = pContext;
}
CController::~CController() noexcept
{
	if (m_pGameContext)
		delete m_pGameContext;
	m_pGameContext = nullptr;
}

bool CController::onInit() noexcept
{

	return true;
}

void CController::updateCamera(float deltaTime) noexcept
{
	// Camera Pos
	Game()->Client()->Camera().update(deltaTime);
}

void CController::tick() noexcept
{
    // Update Entities
	Context()->tick();

	// Camera Pos
	updateCamera(Game()->Client()->getDeltaTime());
}

void CController::onStart() noexcept
{
	Game()->Client()->Menus().setActive(CMenus::NONE);
}

void CController::onReset() noexcept
{
	if (m_pGameContext)
	{
		delete m_pGameContext;
		m_pGameContext = 0x0;
	}

	m_pGameContext = new CContext();
	//onStart();
}

void CController::createImpactSparkMetal(const sf::Vector2f &worldPos, const sf::Color &color) noexcept
{
	for (int i=0; i<12; i++)
	{
		CSimpleParticle *pParticle = new CSimpleParticle(sf::BlendAdd, RENDER_FRONT);
		pParticle->m_Pos = worldPos;
		pParticle->m_SizeInit = sf::Vector2f(18.0f, 18.0f);
		pParticle->m_SizeEnd = VECTOR_ZERO;
		pParticle->m_ColorInit = color;
		pParticle->m_ColorEnd = sf::Color::Black;
		pParticle->m_ColorEnd.a = 0;
		pParticle->m_Duration = upm::floatRand(1.2f, 5.8f);
		pParticle->m_Luminance = true;
		pParticle->m_ApplyForces = false;
		pParticle->m_Collide = true;
		pParticle->m_VelType = CSimpleParticle::VEL_LINEAL;
		pParticle->m_TextId = CAssetManager::TEXTURE_BULLET_SPARK;
		pParticle->m_Dir = sf::Vector2f(upm::floatRand(-1.0f, 1.0f), upm::floatRand(-1.0f, 1.0f));
		pParticle->m_Vel = upm::floatRand(20.0f, 60.0f);
	}
}
