/* (c) Alexandre Díaz. See licence.txt in the root of the distribution for more information. */

#ifndef H_ASSET_MANAGER
#define H_ASSET_MANAGER
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/System/Mutex.hpp>
#include <Zpg/Zpg.hpp>

#define MAX_PATH_LENGTH	128

#define LOAD_TEXTURE(_a,_b) 	strncpy(m_aCurrentLoadAssetPath, _b, MAX_PATH_LENGTH); pFileData = m_pStorage->getFileData(_b, &fileSize); if (!m_vTextures[_a].loadFromMemory(pFileData, fileSize)) { m_LoadErrors = true; return; } else { ++m_Loaded; }
#define LOAD_SOUND(_a,_b) 		strncpy(m_aCurrentLoadAssetPath, _b, MAX_PATH_LENGTH); pFileData = m_pStorage->getFileData(_b, &fileSize); if (!m_vSounds[_a].loadFromMemory(pFileData, fileSize)) { m_LoadErrors = true; return; } else { ++m_Loaded; }
#define LOAD_SHADER(_a,_b,_c) 	strncpy(m_aCurrentLoadAssetPath, _b, MAX_PATH_LENGTH); pFileData = m_pStorage->getFileData(_b, &fileSize); if (!m_vShaders[_a].loadFromMemory(Zpg::toString(pFileData, fileSize), _c)) { m_LoadErrors = true; return; } else { ++m_Loaded;  }
#define LOAD_SHADERS(_a,_b,_c) 	strncpy(m_aCurrentLoadAssetPath, _b, MAX_PATH_LENGTH); pFileData = m_pStorage->getFileData(_b, &fileSize); pFileDataB = m_pStorage->getFileData(_c, &fileSizeB); if (!m_vShaders[_a].loadFromMemory(Zpg::toString(pFileData, fileSize), Zpg::toString(pFileDataB, fileSizeB))) { m_LoadErrors = true; return; } else { ++m_Loaded; }


class CAssetManager final
{
	static sf::Mutex s_Mutex;
public:
	enum
	{
		TEXTURE_SFML_LOGO=0,
		TEXTURE_FMOD_LOGO,
		TEXTURE_MVJAM,
		TEXTURE_BULLET_SPARK,
		MAX_TEXTURES,

		SOUND_MOUSE_CLICK=0,
		SOUND_DAMAGE,
		SOUND_GOOD_L,
		SOUND_GOOD_M,
		SOUND_GOOD_H,
		SOUND_ERROR,
		MAX_SOUNDS,

		SHADER_CHROMATIC_ABERRATION=0,
		SHADER_BLOOM,
		SHADER_METABALL,
		MAX_SHADERS,

		NUM_TOTAL = MAX_TEXTURES + MAX_SOUNDS + MAX_SHADERS,

		MUSIC_MAIN=0,
		MUSIC_MAIN_MENU,
		MAX_SONGS,
	};

	CAssetManager(Zpg *pStorage) noexcept;
	~CAssetManager() noexcept;

	const sf::Font& getDefaultFont() const noexcept { return m_DefFont; }
	sf::Texture* getTexture(int textId) noexcept;
	sf::Shader* getShader(int shaderId) noexcept;
	const sf::SoundBuffer& getSound(int soundId) noexcept;

	void load() noexcept;
	bool isLoaded() const noexcept;
	bool hasErrors() const noexcept { return m_LoadErrors; }
	int getLoadedNum() const noexcept { return m_Loaded; }
	const char* getCurrentLoadAssetPath() const noexcept { return m_aCurrentLoadAssetPath; }

protected:
	Zpg *m_pStorage;

private:
	sf::Texture m_vTextures[MAX_TEXTURES];
	sf::SoundBuffer m_vSounds[MAX_SOUNDS];
	sf::Shader m_vShaders[MAX_SHADERS];
	const unsigned char *m_pDataDefFont;
	sf::Font m_DefFont;
	int m_Loaded;
	bool m_LoadErrors;
	char m_aCurrentLoadAssetPath[MAX_PATH_LENGTH];
};

#endif
