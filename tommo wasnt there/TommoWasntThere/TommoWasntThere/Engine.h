#pragma once
#include <SFML/Graphics.hpp>
#include "TextureHolder.h"
#include "Tommo.h"
#include "Bob.h"
#include "LevelManager.h"
#include "SoundManager.h"
#include "HUD.h"

using namespace sf;

class Engine
{
private:
	TextureHolder th;

	Tommo m_Tommo;
	Bob m_Bob;
	
	//level manager
	LevelManager m_LM;

	SoundManager m_SM;

	Hud m_Hud;
	int m_FramesSinceLastHUDUpdate = 0;
	int m_TargetFramesPerHUDUpdate = 500;

	const int TILE_SIZE = 50;
	const int VERTS_IN_QUAD = 4;

	const int GRAVITY = 300;

	RenderWindow m_Window;

	View m_MainView;
	View m_LeftView;
	View m_RightView;

	View m_BGMainView;
	View m_BGLeftView;
	View m_BGRightView;
	View m_HudView;


	//sprite background and texture
	Sprite m_BackgroundSprite;
	Texture m_BackgroundTexture;

	bool m_Playing = false;

	bool m_Character1 = true;

	bool m_SplitScreen = false;

	float m_TimeRemaining = 10;

	Time m_GameTimeTotal;

	//
	bool m_NewLevelRequired = true;

	VertexArray m_VALevel;
	int** m_ArrayLevel = NULL;

	Texture m_TextureTiles;

	void input();
	void update(float dtAsSeconds);
	void draw();
	void loadLevel();
	bool detectCollisions(PlayableCharacter& character);

public:
	Engine();
	void run();

};