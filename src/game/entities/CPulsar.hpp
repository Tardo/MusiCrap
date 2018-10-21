#ifndef GAME_ENTITIES_PULSAR
#define GAME_ENTITIES_PULSAR

#include "../CEntity.hpp"
//#include <vector>

class CPulsarRing
{
public:
	CPulsarRing(const sf::Vector2f &pos, const sf::Color &color, float size, float thickness, int points)
	{
		m_Pos = pos;
		m_Color = color;
		m_Size = size;
		m_Thickness = thickness;
		m_Points = points;
	}

	sf::Vector2f m_Pos;
	sf::Color m_Color;
	float m_Size;
	float m_Thickness;
	unsigned int m_Points;
};


class CPulsar final : public CEntity
{
public:
	CPulsar(const sf::Vector2f &pos, float size) noexcept;
	~CPulsar() noexcept;

	virtual void tick() noexcept final;

	const float getSize() const noexcept { return m_Size; }
	const std::vector<CPulsarRing>& getRings() const { return m_vRings; }

	const CPulsarRing* ringInZone(float top, float bottom) noexcept;
	bool removeRing(const CPulsarRing &ring) noexcept;
	void clearRings() noexcept;

private:
	float m_Size;
	float m_OrgSize;
	std::vector<CPulsarRing> m_vRings;

	bool m_KickUsed;
	bool m_SnareUsed;
	bool m_AddRing;
};

#endif
