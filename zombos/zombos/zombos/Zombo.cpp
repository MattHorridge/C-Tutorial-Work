// Timber.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <sstream>
#include <SFML/Graphics.hpp>
#include "Player.h">
using namespace sf;

int main()
{
   //game states
    enum class State { PAUSED, LEVELING_UP, GAME_OVER, PLAYING };
    
    //start with gameover state
    State state = State::GAME_OVER;


    Vector2f resolution;
    //get x resolution
    resolution.x = VideoMode::getDesktopMode().width;
    //get y resolution
    resolution.y = VideoMode::getDesktopMode().height;

    RenderWindow window(VideoMode(resolution.x, resolution.y), "Zombos", Style::Fullscreen);

    View mainView(sf::FloatRect(0, 0, resolution.x, resolution.y));

    //clock for timing
    Clock clock;

    //how long is playing state active
    Time gameTimeTotal;

    Vector2f mouseWorldPosition;
    //where is mouse in relation to screen
    Vector2i mouseScreenPosition;

    Player player;

    //Arena Boundaries
    IntRect arena;

    //main loop
    while (window.isOpen()) {
    
        //Handle input
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::KeyPressed) {
                if (event.key.code == Keyboard::Return && state == State::PLAYING) {
                    state = State::PAUSED;
                }
                else if (event.key.code == Keyboard::Return && state == State::PAUSED) {
                    state = State::PLAYING;
                    clock.restart();
                }
                //reset clock so there isnt a frame jump?
                else if (event.key.code == Keyboard::Return && state == State::GAME_OVER) {
                    state = State::LEVELING_UP;
                    //state = State::PLAYING;
                }
                if (state == State::PLAYING) {
                
                }
                
            }



        }//end event polling

        if (Keyboard::isKeyPressed(Keyboard::Escape)) {
            window.close();
        }

        if (state == State::PLAYING) {
            if (Keyboard::isKeyPressed(Keyboard::W)) {
                player.moveUp();
            }
            else {
                player.stopUp();
            }

            if (Keyboard::isKeyPressed(Keyboard::S)) {
                player.moveDown();
            }
            else {
                player.stopDown();
            }

            if (Keyboard::isKeyPressed(Keyboard::A)) {
                player.moveLeft();
            }
            else {
                player.stopLeft();
            }


            if (Keyboard::isKeyPressed(Keyboard::D)) {
                player.moveRight();
            }
            else {
                player.stopRight();
            }



        }

        //leveling up code
        if (state == State::LEVELING_UP) {
            if (event.key.code == Keyboard::Num1)
            {
                state == State::PLAYING;
            }
            if (event.key.code == Keyboard::Num2)
            {
                state == State::PLAYING;
            }
            if (event.key.code == Keyboard::Num3)
            {
                state == State::PLAYING;
            }

            if (event.key.code == Keyboard::Num4)
            {
                state == State::PLAYING;
            }

            if (event.key.code == Keyboard::Num5)
            {
                state == State::PLAYING;
            }

            if (event.key.code == Keyboard::Num6)
            {
                state == State::PLAYING;
            }

            if (state == State::PLAYING) {
                //prepare the level
                arena.width == 500;
                arena.height == 500;
                arena.left = 0;
                arena.top = 0;
                int tileSize = 50;
                player.spawn(arena, resolution, tileSize);

                clock.restart();

            }


        }// end of leveling up


        //update the frame;
        if (state == State::PLAYING) {
            Time dt = clock.restart();
            gameTimeTotal += dt;

            float dtAsSeconds = dt.asSeconds();

            //where is mouse pointer
            mouseScreenPosition = Mouse::getPosition();
            //convert mouse position to coords of mainview
            mouseWorldPosition = window.mapPixelToCoords(Mouse::getPosition(), mainView);
        
            //update the player
            player.update(dtAsSeconds, Mouse::getPosition());

            //make note of players new positon
            Vector2f playerPosition(player.getCenter());

            //make view centre around the player
            mainView.setCenter(player.getCenter());

        }


        /*
            draw the scene
        */

        if (state == State::PLAYING) {
            window.clear();
            //set mainview to be displayed in the window
            //draw everything

            window.setView(mainView);

            window.draw(player.getSprite());
        }

        if (state == State::LEVELING_UP) {}

        if (state == State::PAUSED) {}

        if (state == State::GAME_OVER){}

        window.display();

    
    }


    return 0;



}

