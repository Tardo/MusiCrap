/* (c) Alexandre Díaz. See licence.txt in the root of the distribution for more information. */

#ifndef H_GAME_CONTROLLER
#define H_GAME_CONTROLLER

#include <base/math.hpp>
#include <game/CContext.hpp>
#include <SFML/Graphics.hpp>
#include <vector>

#define MARGIN_CREATE_OBJECTS	800.0f


class CSpawn
{
public:
	CSpawn()
	{
		m_Pos = VECTOR_ZERO;
		m_Dir = VECTOR_ZERO;
	}
	CSpawn(const sf::Vector2f &pos) noexcept
	{
		m_Pos = pos;
		m_Dir = VECTOR_ZERO;
	}
	CSpawn(const sf::Vector2f &pos, const sf::Vector2f &dir) noexcept
	{
		m_Pos = pos;
		m_Dir = dir;
	}

	sf::Vector2f m_Pos;
	sf::Vector2f m_Dir;
};

class CSpawnCanon : public CSpawn
{
public:
	CSpawnCanon() noexcept
	: CSpawn()
	{
		m_Duration = 0;
		m_Timer = 0;
	}

	CSpawnCanon(const sf::Vector2f &pos, const sf::Vector2f &dir, int duration) noexcept
	: CSpawn(pos, dir)
	{
		m_Duration = duration;
		m_Timer = 0;
	}

	int m_Duration;
	sf::Int64 m_Timer;
};

class CController
{
public:
	CController() noexcept;
	CController(CContext *pContext) noexcept;
	virtual ~CController() noexcept;

	class CGame* Game() const noexcept { return m_pGame; }
	CContext *Context() const noexcept { return m_pGameContext; }

	virtual void tick() noexcept;

	virtual bool onInit() noexcept;
	virtual void onStart() noexcept;
	virtual void onReset() noexcept;

	virtual void updateCamera(float deltaTime) noexcept;

	void createImpactSparkMetal(const sf::Vector2f &worldPos, const sf::Color &color) noexcept;

private:
	class CGame *m_pGame;
	CContext *m_pGameContext;
};

#endif
