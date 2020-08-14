#include "Engine.h"
void Engine::draw()
{
	//rubout last frame
	m_Window.clear(Color::White);
	if (!m_SplitScreen) {
		//switch to background view
		m_Window.setView(m_BGMainView);

		//draw background
		m_Window.draw(m_BackgroundSprite);

		m_Window.setView(m_MainView);


		//DRAW LEVEL
		m_Window.draw(m_VALevel, &m_TextureTiles);

		m_Window.draw(m_Tommo.getSprite());
		m_Window.draw(m_Bob.getSprite());

	}
	else
	{
		//Split-screen view is active
		//switch to background
		m_Window.setView(m_BGLeftView);
		m_Window.draw(m_BackgroundSprite);

		m_Window.setView(m_LeftView);


		//DRAW LEVEL
		m_Window.draw(m_VALevel, &m_TextureTiles);


		m_Window.draw(m_Bob.getSprite());

		m_Window.draw(m_Tommo.getSprite());
		

		m_Window.setView(m_BGRightView);

		m_Window.draw(m_BackgroundSprite);


		m_Window.setView(m_RightView);

		//DRAW LEVEL
		m_Window.draw(m_VALevel, &m_TextureTiles);



		m_Window.draw(m_Tommo.getSprite());
		m_Window.draw(m_Bob.getSprite());

	}


	m_Window.setView(m_HudView);
	m_Window.display();
}
