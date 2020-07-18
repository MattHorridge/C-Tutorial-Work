#pragma once

#include <SFML/Graphics.hpp>
//using namespace sf;
class Player {

	//private member variables
private:
	const float START_SPEED = 200;
	const float START_HEALTH = 100;

	sf::Vector2f m_Position;
	sf::Sprite m_Sprite;

	//Add a texture 
	sf::Texture m_Texture;

	//what is screen resolution
	sf::Vector2f m_Resolution;

	//size of current arena
	sf::IntRect m_Arena;

	//how big is each tile in arena
	int m_TileSize;

	//Which direction is the player moving
	bool m_UpPressed;
	bool m_DownPressed;
	bool m_LeftPressed;
	bool m_RightPressed;
	//player health
	int m_Health;

	int m_MaxHealth;

	//When was player last hit
	sf::Time m_LastHit;

	//speed in pixels per second
	float m_Speed;

public:
	Player();
	void spawn(sf::IntRect arena, sf::Vector2f resolution, int tileSize);
	void resetPlayerStats();

	bool hit(sf::Time timeHit);

	sf::Time getLastHitTime();

	sf::FloatRect getPosition();

	sf::Vector2f getCenter();

	float getRotation();

	sf::Sprite getSprite();

	void moveLeft();

	void moveRight();

	void moveUp();

	void moveDown();

	void stopLeft();

	void stopRight();

	void stopUp();

	void stopDown();

	//Once per frame
	void update(float elapsedTime, sf::Vector2i mousePosition);


	void upgradeSpeed();

	void upgradeHealth();


	void increaseHealthLevel(int amount);

	int getHealth();





};
