/* (c) Alexandre Díaz. See licence.txt in the root of the distribution for more information. */

#include <base/math.hpp>
#include <engine/CGame.hpp>
#include <game/entities/CPulsar.hpp>
#include "CEnemyRender.hpp"

CEnemyRender::CEnemyRender(CGameClient *pGameClient) noexcept
: CComponent(pGameClient)
{ }
CEnemyRender::~CEnemyRender() noexcept
{
	#ifdef DEBUG_DESTRUCTORS
	ups::msgDebug("CEnemyRender", "Deleted");
	#endif
}

void CEnemyRender::draw(sf::RenderTarget& target, sf::RenderStates states) const noexcept
{
	if (!Client()->Controller() || !Client()->Controller()->Context())
		return;

	target.setView(Client()->Camera());

	std::vector<CEntity*> &vEntities = Client()->Controller()->Context()->getEntities();
	std::vector<CEntity*>::const_iterator cit = vEntities.cbegin();
	while (cit != vEntities.cend())
	{
		if ((*cit)->getType() == CEntity::PULSAR)
			renderPulsar(target, states, (*cit));
		++cit;
	}
}

void CEnemyRender::renderPulsar(sf::RenderTarget& target, sf::RenderStates states, CEntity *pEntity) const noexcept
{
	CPulsar *pPulsar = static_cast<CPulsar*>(pEntity);

	// Draw Rings
	const std::vector<CPulsarRing> &vRings = pPulsar->getRings();
	std::vector<CPulsarRing>::const_iterator cit = vRings.begin();
	while (cit != vRings.end())
	{
		const CPulsarRing &ring = (*cit);
		sf::CircleShape circle(ring.m_Size, ring.m_Points);
		circle.setFillColor(sf::Color::Transparent);
		circle.setPosition(ring.m_Pos);
		circle.setOutlineThickness(ring.m_Thickness);
		circle.setOutlineColor(ring.m_Color);
		circle.setOrigin(ring.m_Size, ring.m_Size);
		circle.setScale(sf::Vector2f(2.0f, 1.0f));
		target.draw(circle, states);
		++cit;
	}
}
