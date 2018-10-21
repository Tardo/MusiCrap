/* (c) Alexandre Díaz. See licence.txt in the root of the distribution for more information. */

#ifndef H_GAME_COMPONENT_ENEMY_RENDER
#define H_GAME_COMPONENT_ENEMY_RENDER

#include <game/CComponent.hpp>
#include <game/CEntity.hpp>

class CEnemyRender final : public CComponent
{
public:
	CEnemyRender(CGameClient *pGameClient) noexcept;
	~CEnemyRender() noexcept;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const noexcept final;

private:
	void renderPulsar(sf::RenderTarget& target, sf::RenderStates states, CEntity *pEntity) const noexcept;
};

#endif
