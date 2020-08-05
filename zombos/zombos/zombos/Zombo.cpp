// Timber.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <sstream>
#include <fstream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Player.h"
#include "ZombieArena.h"
#include "TextureHolder.h"
#include "Bullet.h"
#include "Pickup.h"


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

    //hide mouse pointer
    window.setMouseCursorVisible(false);
    Sprite spriteCrosshair;
    Texture textureCrosshair = TextureHolder::GetTexture("graphics/crosshair.png");
    spriteCrosshair.setTexture(textureCrosshair);
    spriteCrosshair.setOrigin(25, 25);

    Pickup healthPickup(1);
    Pickup ammoPickup(2);

    int score = 0;
    int hiScore = 0;


    //home/game over screen
    Sprite spriteGameOver;
    Texture textureGameOver = TextureHolder::GetTexture("graphics/background.png");
    spriteGameOver.setTexture(textureGameOver);

    spriteGameOver.setPosition(0, 0);

    View hudView(sf::FloatRect(0, 0, resolution.x, resolution.y));

    Sprite spriteAmmoIcon;
    Texture textureAmmoIcon = TextureHolder::GetTexture("graphics/ammo_icon.png");
    spriteAmmoIcon.setTexture(textureAmmoIcon);
    spriteAmmoIcon.setPosition(20, 980);

    Font font;
    font.loadFromFile("fonts/zombiecontrol.ttf");

    //paused
    Text pausedText;
    pausedText.setFont(font);
    pausedText.setCharacterSize(155);
    pausedText.setFillColor(Color::White);
    pausedText.setPosition(400, 400);
    pausedText.setString("Press Enter \ntocontinue");

    //game over
    Text gameOverText;
    gameOverText.setFont(font);
    gameOverText.setCharacterSize(125);
    gameOverText.setFillColor(Color::White);
    gameOverText.setPosition(250, 850);
    gameOverText.setString("Press Enter to play");


    // Levelling up
    Text levelUpText;
    levelUpText.setFont(font);
    levelUpText.setCharacterSize(80);
    levelUpText.setFillColor(Color::White);
    levelUpText.setPosition(150, 250);
    std::stringstream levelUpStream;
    levelUpStream <<
        "1- Increased rate of fire" <<
        "\n2- Increased clip size(next reload)" <<
        "\n3- Increased max health" <<
        "\n4- Increased run speed" <<
        "\n5- More and better health pickups" <<
        "\n6- More and better ammo pickups";
    levelUpText.setString(levelUpStream.str());

    // Ammo
    Text ammoText;
    ammoText.setFont(font);
    ammoText.setCharacterSize(55);
    ammoText.setFillColor(Color::White);
    ammoText.setPosition(200, 980);

    // Score
    Text scoreText;
    scoreText.setFont(font);
    scoreText.setCharacterSize(55);
    scoreText.setFillColor(Color::White);
    scoreText.setPosition(20, 0);

    std::ifstream inputFile("gamedata/scores.txt");

    if (inputFile.is_open()) {
        inputFile >> hiScore;
        inputFile.close();
    }

    Text hiScoreText;
    hiScoreText.setFont(font);
    hiScoreText.setCharacterSize(55);
    hiScoreText.setFillColor(Color::White);
    hiScoreText.setPosition(1400, 0);
    std::stringstream s;
    s << "Hi Score:" << hiScore;
    hiScoreText.setString(s.str());

    // Zombies remaining
    Text zombiesRemainingText;
    zombiesRemainingText.setFont(font);
    zombiesRemainingText.setCharacterSize(55);
    zombiesRemainingText.setFillColor(Color::White);
    zombiesRemainingText.setPosition(1500, 980);
    zombiesRemainingText.setString("Zombies: 100");

    // Wave number
    int wave = 0;
    Text waveNumberText;
    waveNumberText.setFont(font);
    waveNumberText.setCharacterSize(55);
    waveNumberText.setFillColor(Color::White);
    waveNumberText.setPosition(1250, 980);
    waveNumberText.setString("Wave: 0");

    // Health bar
    RectangleShape healthBar;
    healthBar.setFillColor(Color::Red);
    healthBar.setPosition(450, 980);



    // When did we last update the HUD?
    int framesSinceLastHUDUpdate = 0;
    // What time was the last update
    Time timeSinceLastUpdate;
    // How often (in frames) should we update the HUD
    int fpsMeasurementFrameInterval = 1000;

    // Prepare the hit sound
    SoundBuffer hitBuffer;
    hitBuffer.loadFromFile("sound/hit.wav");
    Sound hit;
    hit.setBuffer(hitBuffer);

    // Prepare the splat sound
    SoundBuffer splatBuffer;
    splatBuffer.loadFromFile("sound/splat.wav");
    sf::Sound splat;
    splat.setBuffer(splatBuffer);

    // Prepare the shoot sound
    SoundBuffer shootBuffer;
    shootBuffer.loadFromFile("sound/shoot.wav");
    Sound shoot;
    shoot.setBuffer(shootBuffer);

    // Prepare the reload sound
    SoundBuffer reloadBuffer;
    reloadBuffer.loadFromFile("sound/reload.wav");
    Sound reload;
    reload.setBuffer(reloadBuffer);

    // Prepare the failed sound
    SoundBuffer reloadFailedBuffer;
    reloadFailedBuffer.loadFromFile("sound/reload_failed.wav");
    Sound reloadFailed;
    reloadFailed.setBuffer(reloadFailedBuffer);

    // Prepare the powerup sound
    SoundBuffer powerupBuffer;
    powerupBuffer.loadFromFile("sound/powerup.wav");
    Sound powerup;
    powerup.setBuffer(powerupBuffer);

    // Prepare the pickup sound
    SoundBuffer pickupBuffer;
    pickupBuffer.loadFromFile("sound/pickup.wav");
    Sound pickup;
    pickup.setBuffer(pickupBuffer);


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
                    wave = 0;
                    score = 0;
                    currentBullet = 0;
                    bulletsSpare = 7000;
                    clipSize = 12;
                    fireRate = 1;
                    player.resetPlayerStats();
                    //state = State::PLAYING;
                }
                if (state == State::PLAYING) 
                {
                    //reloading
                    if (event.key.code == Keyboard::R) {
                        if (bulletsSpare >= clipSize) {
                            bulletsInClip = clipSize;
                            bulletsSpare -= clipSize;
                            reload.play();
                        }
                        else if (bulletsSpare > 0) {
                            bulletsInClip = bulletsSpare;
                            bulletsSpare = 0;
                            reload.play();
                        }
                        else {
                            //todo
                            reloadFailed.play();
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

                if (gameTimeTotal.asMilliseconds() - lastPressed.asMilliseconds() > 1000 / fireRate && bulletsInClip > 0) 
                {
                    bullets[currentBullet].shoot(player.getCenter().x, player.getCenter().y,
                        mouseWorldPosition.x, mouseWorldPosition.y);
                    currentBullet++;
                    if (currentBullet > 99) {
                        currentBullet = 0;
                    }
                    lastPressed = gameTimeTotal;
                    shoot.play();
                    bulletsInClip--;
                }
            }



        }

        //leveling up code
        if (state == State::LEVELING_UP) {



            if (event.key.code == Keyboard::Num1)
            {
                fireRate++;
                state = State::PLAYING;
            }
            if (event.key.code == Keyboard::Num2)
            {
                clipSize += clipSize;
                state = State::PLAYING;
            }
            if (event.key.code == Keyboard::Num3)
            {
                player.upgradeHealth();
                state = State::PLAYING;
            }

            if (event.key.code == Keyboard::Num4)
            {
                player.upgradeSpeed();
                state = State::PLAYING;
            }

            if (event.key.code == Keyboard::Num5)
            {
                healthPickup.upgrade();
                state = State::PLAYING;
            }

            if (event.key.code == Keyboard::Num6)
            {
                ammoPickup.upgrade();
                state = State::PLAYING;
            }

            if (state == State::PLAYING) {
                //prepare the level

                wave++;

                arena.width = 700 * wave;
                arena.height = 700 *wave;
                arena.left = 0;
                arena.top = 0;
                int tileSize = createBackground(background, arena);
                player.spawn(arena, resolution, tileSize);

                //pickups
                healthPickup.setArena(arena);
                ammoPickup.setArena(arena);

                //create zombie horde
                numZOmbies = 5* (pow(wave, 3));
                delete[] zombies;
                zombies = createHorde(numZOmbies, arena);
                numZombiesAlive = numZOmbies;

                powerup.play();

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

            spriteCrosshair.setPosition(mouseWorldPosition);
        
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

            for (int i = 0; i < 100; i++) {
                if (bullets[i].isInFlight()) {
                    bullets[i].update(dtAsSeconds);
                }
            }

            //update pickups
            healthPickup.update(dtAsSeconds);
            ammoPickup.update(dtAsSeconds);



            //collision detection
            for (int i = 0; i < 100; i++) {
                for (int j = 0; j < numZOmbies; j++) {

                    if (bullets[i].isInFlight() && zombies[j].isAlive()) {

                        if (bullets[i].getPosition().intersects(zombies[j].getPosition())) {
                            //stop bullet
                            bullets[i].stop();

                            if (zombies[j].hit()) {
                                score += 10;
                                if (score >= hiScore) {
                                    hiScore = score;
                                }

                                numZombiesAlive--;

                                if (numZombiesAlive == 0) {
                                    state = State::LEVELING_UP;
                                }
                            }
                            splat.play();
                        }
                    }
                }
            }//end of bullet detection

            for (int i = 0; i < numZOmbies; i++) {

                FloatRect playerposition = player.getPosition();
                Zombie thisZombie = zombies[i];
                FloatRect zombiePosition = thisZombie.getPosition();

                if (player.getPosition().intersects(zombiePosition)) {
                    std::cout << "hit\n";
                    
                }
               

                if (player.getPosition().intersects(zombies[i].getPosition()) && zombies[i].isAlive()) {
                    if (player.hit(gameTimeTotal)) {
                        //todo
                        hit.play();
                    }

                    if (player.getHealth() <= 0) {
                        state = State::GAME_OVER;
                        std::ofstream outputFile("gamedata/scores.txt");

                        outputFile << hiScore;
                        outputFile.close();
                    }
                }
            }// end of zombie attacking player detection


            if (player.getPosition().intersects(healthPickup.getPosition()) && healthPickup.isSpawned())
            {
                player.increaseHealthLevel(healthPickup.gotIt());
            }


            if (player.getPosition().intersects(ammoPickup.getPosition()) && ammoPickup.isSpawned())
            {
                bulletsSpare += ammoPickup.gotIt();
            }

            //healthbar
            healthBar.setSize(Vector2f(player.getHealth() * 3, 50));

            framesSinceLastHUDUpdate++;
            // Calculate FPS every fpsMeasurementFrameInterval frames
            if (framesSinceLastHUDUpdate > fpsMeasurementFrameInterval)
            {

                // Update game HUD text
                std::stringstream ssAmmo;
                std::stringstream ssScore;
                std::stringstream ssHiScore;
                std::stringstream ssWave;
                std::stringstream ssZombiesAlive;

                // Update the ammo text
                ssAmmo << bulletsInClip << "/" << bulletsSpare;
                ammoText.setString(ssAmmo.str());

                // Update the score text
                ssScore << "Score:" << score;
                scoreText.setString(ssScore.str());

                // Update the high score text
                ssHiScore << "Hi Score:" << hiScore;
                hiScoreText.setString(ssHiScore.str());

                // Update the wave
                ssWave << "Wave:" << wave;
                waveNumberText.setString(ssWave.str());

                // Update the high score text
                ssZombiesAlive << "Zombies:" << numZombiesAlive;
                zombiesRemainingText.setString(ssZombiesAlive.str());

                framesSinceLastHUDUpdate = 0;
                timeSinceLastUpdate = Time::Zero;
            }// End HUD update


            //end of collison detection
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

            for (int i = 0; i < 100; i++) {
                if (bullets[i].isInFlight()) {
                    window.draw(bullets[i].getShape());
                }
            }


            window.draw(player.getSprite());


            if (ammoPickup.isSpawned()) {
            
                window.draw(ammoPickup.getSprite());
            }

            if (healthPickup.isSpawned()) {

                window.draw(healthPickup.getSprite());
            }




            window.draw(spriteCrosshair);

            window.setView(hudView);

            // Draw all the HUD elements
            window.draw(spriteAmmoIcon);
            window.draw(ammoText);
            window.draw(scoreText);
            window.draw(hiScoreText);
            window.draw(healthBar);
            window.draw(waveNumberText);
            window.draw(zombiesRemainingText);
        }

        if (state == State::LEVELING_UP) {
            window.draw(spriteGameOver);
            window.draw(levelUpText);
        }

        if (state == State::PAUSED)
        {
            window.draw(pausedText);
        }

        if (state == State::GAME_OVER)
        {
            window.draw(spriteGameOver);
            window.draw(gameOverText);
            window.draw(scoreText);
            window.draw(hiScoreText);
        }

        window.display();

        
    }

    delete[] zombies;

    return 0;



}

