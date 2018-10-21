/* (c) Alexandre Díaz. See licence.txt in the root of the distribution for more information. */

#include "CAssetManager.hpp"
#include <game/CGameClient.hpp>
#include <base/system.hpp>
#include <SFML/System/Lock.hpp>
#include <cstring>

sf::Mutex CAssetManager::s_Mutex;
CAssetManager::CAssetManager(Zpg *pStorage) noexcept
{
	m_pStorage = pStorage;

	m_Loaded = 0;
	m_LoadErrors = false;
	m_aCurrentLoadAssetPath[0] = 0;
	m_pDataDefFont = nullptr;
}
CAssetManager::~CAssetManager() noexcept
{
	#ifdef DEBUG_DESTRUCTORS
	ups::msgDebug("CAssetManager", "Deleted");
	#endif
}

bool CAssetManager::isLoaded() const noexcept
{
	if (!g_Config.m_UseShaders)
		return m_Loaded == NUM_TOTAL-MAX_SHADERS;
	return m_Loaded == NUM_TOTAL;
}

void CAssetManager::load() noexcept
{
	if (isLoaded())
		return;

	sf::Lock lock(s_Mutex);

	const unsigned char *pFileData = nullptr;
	unsigned long fileSize = 0;

	m_pDataDefFont = m_pStorage->getFileData("data/font.ttf", &fileSize);
	m_DefFont.loadFromMemory(m_pDataDefFont, fileSize);

	LOAD_TEXTURE(TEXTURE_SFML_LOGO, "data/fx/sfml_logo.png")
	LOAD_TEXTURE(TEXTURE_FMOD_LOGO, "data/fx/fmod_logo.png")
	LOAD_TEXTURE(TEXTURE_MVJAM, "data/fx/mvjam.png")
	LOAD_TEXTURE(TEXTURE_BULLET_SPARK, "data/fx/bullet_spark.png")

	LOAD_SOUND(SOUND_MOUSE_CLICK, "data/sfx/mouse_click.wav")
	LOAD_SOUND(SOUND_DAMAGE, "data/sfx/damage.wav")
	LOAD_SOUND(SOUND_GOOD_L, "data/sfx/good_l.wav")
	LOAD_SOUND(SOUND_GOOD_M, "data/sfx/good_m.wav")
	LOAD_SOUND(SOUND_GOOD_H, "data/sfx/good_h.wav")
	LOAD_SOUND(SOUND_ERROR, "data/sfx/error.wav")

	if (g_Config.m_UseShaders)
	{
		LOAD_SHADER(SHADER_CHROMATIC_ABERRATION, "data/shaders/chromatic_aberration.frag", sf::Shader::Fragment)
		LOAD_SHADER(SHADER_BLOOM, "data/shaders/bloom.frag", sf::Shader::Fragment)
		LOAD_SHADER(SHADER_METABALL, "data/shaders/metaball.frag", sf::Shader::Fragment)
	}

	s_Mutex.unlock();
}

sf::Texture* CAssetManager::getTexture(int textId) noexcept
{
	if (textId < 0 || textId >= MAX_TEXTURES)
		return nullptr;

	return &m_vTextures[textId];
}

const sf::SoundBuffer& CAssetManager::getSound(int soundId) noexcept
{
	return m_vSounds[soundId];
}

sf::Shader* CAssetManager::getShader(int shaderId) noexcept
{
	if (shaderId < 0 || shaderId >= MAX_SHADERS || !g_Config.m_UseShaders)
		return 0x0;

	return &m_vShaders[shaderId];
}
