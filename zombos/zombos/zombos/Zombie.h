#pragma once

#include <SFML/Graphics.hpp>

using namespace sf;

class Zombie {
private:
	//how fast is each zombie type?
	const float BLOATER_SPEED = 40;
	const float CHASER_SPEED = 80;
	const float CRAWLER_SPEED = 20;

	//health
	const float BLOATER_HEALTH = 2;
	const float CHASER_HEATLH = 1;
	const float CRAWLER_HEALTH = 1;

	const int MAX_VARIANCE = 30;
	const int OFFSET = 101 - MAX_VARIANCE;

	Vector2f m_Position;

	//sprite for the zombie
	Sprite m_Sprite;

	float m_Speed;

	float m_Health;

	bool m_Alive;

public:
	bool hit();

	bool isAlive();

	void spawn(float startX, float startY, int type, int seed);

	FloatRect getPosition();

	Sprite getSprite();

	void update(float elapsedTime, Vector2f playerLocation);


};