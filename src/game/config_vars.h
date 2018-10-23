/* (c) Alexandre Díaz. See licence.txt in the root of the distribution for more information. */

#ifndef ENGINE_CONFIG_VARS_H
#define ENGINE_CONFIG_VARS_H
#undef ENGINE_CONFIG_VARS_H  // this file will be included several times

MACRO_CONFIG_STR(Lang, lang, 18, "spanish", CConfig::SAVE)

MACRO_CONFIG_BOOL(Sfx, sfx, true, CConfig::SAVE)
MACRO_CONFIG_BOOL(Music, music, false, CConfig::SAVE)
MACRO_CONFIG_BOOL(VSync, vsync, true, CConfig::SAVE)
MACRO_CONFIG_BOOL(FullScreen, fullscreen, false, CConfig::SAVE)
MACRO_CONFIG_BOOL(UseShaders, use_shaders, true, CConfig::SAVE)
MACRO_CONFIG_INT(ScreenWidth, screen_width, 1024, 1, 1920, CConfig::SAVE)
MACRO_CONFIG_INT(ScreenHeight, screen_height, 768, 1, 1080, CConfig::SAVE)
MACRO_CONFIG_BOOL(CursorGrab, cursor_grab, false, CConfig::SAVE)
MACRO_CONFIG_BOOL(CursorShow, cursor_show, false, CConfig::SAVE)

MACRO_CONFIG_FLOAT(PulsarRingVelocity, pulsar_ring_velocity, 350.0f, 0.1f, 9999.0f, CConfig::SAVE)

MACRO_CONFIG_FLOAT(MinKickEnergy, min_kick_energy, 7.5f, 0.1f, 9999.0f, CConfig::SAVE)

MACRO_CONFIG_STR(SongFilename, song_filename, 512, "music.ogg", CConfig::SAVE)
MACRO_CONFIG_INT(MinComboCount, min_combo_count, 3, 1, 9999, CConfig::SAVE)
MACRO_CONFIG_FLOAT(MinPlayerRatio, min_player_ratio, 75.0f, 0.1f, 100.0f, CConfig::SAVE)

MACRO_CONFIG_COLOR(ButtonNormalFillColor, button_normal_fill_color, 2112594, CConfig::SAVE)
MACRO_CONFIG_COLOR(ButtonNormalOutlineColor, button_normal_outline_color, 0, CConfig::SAVE)
MACRO_CONFIG_COLOR(ButtonNormalTextColor, button_normal_text_color, 3300095, CConfig::SAVE)
MACRO_CONFIG_COLOR(ButtonFocusFillColor, button_focus_fill_color, 2117688, CConfig::SAVE)
MACRO_CONFIG_COLOR(ButtonFocusOutlineColor, button_focus_outline_color, 0, CConfig::SAVE)
MACRO_CONFIG_COLOR(ButtonFocusTextColor, button_focus_text_color, 845428735, CConfig::SAVE)

#endif
