/* (c) Alexandre Díaz. See licence.txt in the root of the distribution for more information. */

#ifndef H_GAME_COMPONENT_PLAYER_RENDER
#define H_GAME_COMPONENT_PLAYER_RENDER

#include <game/CComponent.hpp>
#include <game/CPlayer.hpp>

class CPlayerRender final : public CComponent
{
public:
	CPlayerRender(CGameClient *pGameClient) noexcept;
	~CPlayerRender() noexcept;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const noexcept final;


private:
	void renderLine(sf::RenderTarget& target, sf::RenderStates states, CPlayer *pPlayer) const noexcept;
};

#endif
