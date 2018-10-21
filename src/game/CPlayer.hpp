/* (c) Alexandre Díaz. See licence.txt in the root of the distribution for more information. */

#ifndef H_GAME_COMPONENT_PLAYER
#define H_GAME_COMPONENT_PLAYER

#include <game/CEntity.hpp>
#include <game/CComponent.hpp>


class CPlayer
{
public:
	CPlayer() noexcept;
	virtual ~CPlayer() noexcept;

	virtual void update(float deltaTime) noexcept;

	void addPoints(int points) noexcept { m_Points += points; }
	void resetPoints() noexcept { m_Points = 0; }
	const unsigned int getPoints() const noexcept { return m_Points; }
	float getRatio() const noexcept;

	void setLineColor(const sf::Color &color) noexcept { m_LineColor = color; }
	const sf::Color& getLineColor() const noexcept { return m_LineColor; }

	// TODO: Change Place
	unsigned int m_Wins;
	unsigned int m_Fails;
	int m_Combos;
	sf::Int64 m_LastComboTime;

private:
	unsigned int m_Points;
	sf::Color m_LineColor;
};

#endif
