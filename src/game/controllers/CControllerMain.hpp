/* (c) Alexandre Díaz. See licence.txt in the root of the distribution for more information. */

#ifndef H_GAME_CONTROLLER_MAIN
#define H_GAME_CONTROLLER_MAIN

#include <engine/CGame.hpp>
#include <game/CContext.hpp>
#include <game/CController.hpp>
#include <game/entities/CPulsar.hpp>
#include <vector>
#include <map>


class CSpawnFire final : public CSpawn
{
public:
	CSpawnFire(const sf::Vector2f &pos, const sf::Vector2f &dir, float delay)
	: CSpawn(pos, dir)
	{
		m_Delay = delay;
		m_Active = false;
		m_Timer = 0;
	}

	bool m_Active;
	sf::Int64 m_Timer;
	float m_Delay;
};

class CTeleport final : public CSpawn
{
public:
	CTeleport()
	: CSpawn(VECTOR_ZERO)
	{ }

	sf::Vector2f m_PosOut;
};

class CControllerMain final : public CController
{
public:
	CControllerMain() noexcept;
	CControllerMain(class CContext *pContext) noexcept;
	virtual ~CControllerMain() noexcept final;

	virtual void tick() noexcept final;
	virtual bool onInit() noexcept final;
	virtual void onReset() noexcept final;
	virtual void onStart() noexcept final;
	virtual void onSystemEvent(sf::Event *pEvent) noexcept final;

private:
	CPulsar *m_pPulsar;
	void checkPlayerCombos(CPlayer *pPlayer) noexcept;
};

#endif
