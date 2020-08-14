#include "Engine.h"
#include <SFML/Graphics.hpp>
#include <sstream>
using namespace sf;

void Engine::update(float dtAsSeconds) {
	if (m_NewLevelRequired) {
		
		loadLevel();
	}

	if (m_Playing) {

		m_Tommo.update(dtAsSeconds);
		m_Bob.update(dtAsSeconds);


		if (detectCollisions(m_Tommo) && detectCollisions(m_Bob)) {
			m_NewLevelRequired = true;
			//win sound
		}
		else {
			detectCollisions(m_Bob);
		}

		if (m_Bob.getFeet().intersects(m_Tommo.getHead())) {
			m_Bob.stopFalling(m_Tommo.getHead().top);
		}
		else if (m_Tommo.getFeet().intersects(m_Bob.getHead())) {
			m_Tommo.stopFalling(m_Bob.getHead().top);
		}

		m_TimeRemaining -= dtAsSeconds;
		if (m_TimeRemaining <= 0)
		{
			m_NewLevelRequired = true;

		}
	}


	if (m_SplitScreen) {
		m_LeftView.setCenter(m_Tommo.getCenter());
		m_RightView.setCenter(m_Bob.getCenter());
	}
	else
	{
		if (m_Character1) {
			m_MainView.setCenter(m_Tommo.getCenter());
		}
		else
		{
			m_MainView.setCenter(m_Bob.getCenter());
		}
	}

	



}