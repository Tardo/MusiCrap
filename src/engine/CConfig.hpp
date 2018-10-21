/* (c) Alexandre Díaz. See licence.txt in the root of the distribution for more information. */
/* Original code from teeworlds source */

#ifndef H_ENGINE_CONFIG
#define H_ENGINE_CONFIG

#include <list>
#include <SFML/Graphics/Color.hpp>

enum
{
	WEAPON_GRENADE_LAUNCHER,
	WEAPON_JET_PACK,
	WEAPON_CANON_BALL,
	WEAPON_VISCOSITY_LAUNCHER,
	NUM_WEAPONS
};

struct CConfiguration
{
	#define MACRO_CONFIG_INT(Name,ScriptName,Def,Min,Max,Save) int m_##Name;
	#define MACRO_CONFIG_FLOAT(Name,ScriptName,Def,Min,Max,Save) float m_##Name;
	#define MACRO_CONFIG_STR(Name,ScriptName,Len,Def,Save) char m_##Name[Len+1];
	#define MACRO_CONFIG_BOOL(Name,ScriptName,Def,Save) bool m_##Name;
	#define MACRO_CONFIG_COLOR(Name,ScriptName,Def,Save) sf::Color m_##Name;
	#include <game/config_vars.h>
	#undef MACRO_CONFIG_INT
	#undef MACRO_CONFIG_FLOAT
	#undef MACRO_CONFIG_STR
	#undef MACRO_CONFIG_BOOL
	#undef MACRO_CONFIG_COLOR
};

extern CConfiguration g_Config;

class CConfig final
{
public:
	enum
	{
		SAVE=1,
	};

	CConfig(const char *pPathFile) noexcept;

	bool load() noexcept;
	void reset() noexcept;
	bool save() noexcept;

private:
	char m_PathFile[512];

	void escapeParam(char *pDst, const char *pSrc, int size) noexcept;
};

#endif
