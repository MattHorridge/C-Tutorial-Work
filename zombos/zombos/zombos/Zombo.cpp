// Timber.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <sstream>
#include <SFML/Graphics.hpp>
#include "Player.h"
#include "ZombieArena.h"
#include "TextureHolder.h"
#include "Bullet.h"
using namespace sf;

int main()
{


    //Textureholder class
    TextureHolder holder;


    //every 4 vertices is a quad?

   //game states
    enum class State { PAUSED, LEVELING_UP, GAME_OVER, PLAYING };
    
    //start with gameover state
    State state = State::GAME_OVER;


    Vector2f resolution;
    //get x resolution
    resolution.x = VideoMode::getDesktopMode().width;
    //get y resolution
    resolution.y = VideoMode::getDesktopMode().height;

    RenderWindow window(VideoMode(resolution.x, resolution.y), "Zombos", Style::Resize);

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


    //create background
    VertexArray background;
    
    Texture TextureBackground;

    

    TextureBackground = TextureHolder::GetTexture("graphics/background_sheet.png");



    //prepare horde of zombies
    int numZOmbies;
    int numZombiesAlive;
    Zombie* zombies = nullptr;

    //bullets
    Bullet bullets[100];
    int currentBullet = 0;
    int bulletsSpare = 24;
    int bulletsInClip = 6;
    int clipSize = 6;
    float fireRate = 1;

    Time lastPressed;


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
                if (state == State::PLAYING) 
                {
                    //reloading
                    if (event.key.code == Keyboard::R) {
                        if (bulletsSpare >= clipSize) {
                            bulletsInClip = clipSize;
                            bulletsSpare -= clipSize;
                        }
                        else if (bulletsSpare > 0) {
                            bulletsInClip = bulletsSpare;
                            bulletsSpare = 0;
                        }
                        else {
                            //todo
                        }
                    }

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

            //fire gun
            if (Mouse::isButtonPressed(sf::Mouse::Left)) {

                
            }



        }

        //leveling up code
        if (state == State::LEVELING_UP) {



            if (event.key.code == Keyboard::Num1)
            {
                state = State::PLAYING;
            }
            if (event.key.code == Keyboard::Num2)
            {
                state = State::PLAYING;
            }
            if (event.key.code == Keyboard::Num3)
            {
                state = State::PLAYING;
            }

            if (event.key.code == Keyboard::Num4)
            {
                state = State::PLAYING;
            }

            if (event.key.code == Keyboard::Num5)
            {
                state = State::PLAYING;
            }

            if (event.key.code == Keyboard::Num6)
            {
                state = State::PLAYING;
            }

            if (state == State::PLAYING) {
                //prepare the level
                arena.width = 500;
                arena.height = 500;
                arena.left = 0;
                arena.top = 0;
                int tileSize = createBackground(background, arena);
                player.spawn(arena, resolution, tileSize);


                //create zombie horde
                numZOmbies = 10;
                delete[] zombies;
                zombies = createHorde(numZOmbies, arena);
                numZombiesAlive = numZOmbies;


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

            //loop through each Zombie and update;
            for (int i = 0; i < numZOmbies; i++) {
                if (zombies[i].isAlive()) {
                    zombies[i].update(dt.asSeconds(), playerPosition);
                }
            }
        }


        /*
            draw the scene
        */

        if (state == State::PLAYING) {
            window.clear();
            //set mainview to be displayed in the window
            //draw everything

            window.setView(mainView);
            window.draw(background, &TextureBackground);

            //draw the zombies;
            for (int i = 0; i < numZOmbies; i++) {
                window.draw(zombies[i].getSprite());
            }

            window.draw(player.getSprite());
        }

        if (state == State::LEVELING_UP) {}

        if (state == State::PAUSED) {}

        if (state == State::GAME_OVER){}

        window.display();

        
    }

    delete[] zombies;

    return 0;



}

