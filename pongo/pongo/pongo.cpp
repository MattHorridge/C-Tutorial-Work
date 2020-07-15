

#include <iostream>
#include <sstream>
#include <SFML/Graphics.hpp>
#include "Bat.h"
#include "Ball.h"



int main()
{

    //create a video mode object
    VideoMode vm(1920, 1080);

    //create and open a window for the game using render window

    RenderWindow window(vm, "Pongo", Style::Fullscreen);
    int score = 0;
    int lives = 3;

    //constructor contains starting positions
    Bat bat(1920 / 2, 1080 - 20);

    Ball ball(1920 / 2, 0);

    Text hud;

    Font font;
    font.loadFromFile("fonts/DS-DIGI.ttf");

    hud.setFont(font);

    hud.setCharacterSize(75);

    hud.setFillColor(Color::White);

    hud.setPosition(20, 20);

    Clock clocko;

    //main loop
    while (window.isOpen()) {
        //TODO: Hnadle Player input

        Event event;
        while (window.pollEvent(event)) 
        {
            if (event.type == Event::Closed) {
                window.close();
            }

            if (Keyboard::isKeyPressed(Keyboard::Escape)) {
                window.close();
            }

            if (Keyboard::isKeyPressed(Keyboard::Left)) {
                bat.moveLeft();
            }
            else {
                bat.stopLeft();
            
            }

            if (Keyboard::isKeyPressed(Keyboard::Right)) {
                bat.moveRight();
            }
            else {
                bat.stopRight();

            }
        }


        Time dt = clocko.restart();
        bat.update(dt);
        ball.update(dt);

        std::stringstream ss;
        ss << "Score:" << score << " Lives:" << lives;

        hud.setString(ss.str());


        //Handle ball collision
        if (ball.getPosition().top > window.getSize().y) {
        
            ball.reboundBottom();
            lives--;

            if (lives < 1) {
                score = 0;
                lives = 3;
            }

        }

        if (ball.getPosition().top < 0) {
        
            ball.reboundBatOrTop();
            score++;
        }


        if (ball.getPosition().left < 0 || ball.getPosition().left + ball.getPosition().width > window.getSize().x) {
            ball.reboundSides();
        }

        //bat hit detection
        if (ball.getPosition().intersects(bat.getPosition())) {

            ball.reboundBatOrTop();
        }




        window.clear();
        window.draw(hud);
        window.draw(bat.getShape());
        window.draw(ball.getShape());
        window.display();

        //TODO: Update bat and ball

        //TODO: Draw bat and ball
    }


   
    return 0;



}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
