/* (c) Alexandre Díaz. See licence.txt in the root of the distribution for more information. */

#ifndef H_GAME_CONTEXT
#define H_GAME_CONTEXT

#include <game/CPlayer.hpp>
#include <vector>

class CContext final
{
public:
	CContext() noexcept;
	~CContext() noexcept;

	int addEntity(CEntity *pEntity) noexcept;
	CEntity* getEntity(sf::Uint64 id) const noexcept
	{
		std::vector<CEntity*>::const_iterator cit = m_vpEntities.cbegin();
		while (cit != m_vpEntities.cend())
		{
			if ((*cit)->getID() == id)
				return (*cit);
			++cit;
		}
		return nullptr;
	}
	std::vector<CEntity*>& getEntities() noexcept { return m_vpEntities; }

	void tick() noexcept;

	CPlayer* getPlayer() noexcept { return m_pPlayer; }
	void createPlayer() noexcept;

protected:
	sf::Uint64 m_GID;

private:
	std::vector<CEntity*> m_vpEntities;

	CPlayer *m_pPlayer;

	void clearTrash() noexcept;
};

#endif
