#include <SFML/Graphics.hpp>
#include <iostream>
#include "GameHandler.h"
#include "Gamemap.h"
#include "Characters.h"




void GameState::RunGame(sf::RenderWindow &window, sf::Event &event, sf::RectangleShape& bg,
                        Player &p, sf::Text &score,
                        Harta& h, std::vector<Entity*> &itemvect,const std::map<std::string,sf::Texture>& Textures) {
    window.draw(bg);
    if(state == 1) {
        while (window.pollEvent(event)) {
            EventHandling1(window, event, p);
        }
        if (window.hasFocus()) {
            ViewAndMovement(window, p, score);
        }
        if (p.getY() > 500) p.Kill();
        window.draw(p.getSprite());
        h.drawMap(window, p, itemvect);
        for (auto &item: itemvect) {
            if (item->status() == 0)window.draw(item->getSprite());
        }
        score.setString(std::to_string(p.getScore()));
        window.draw(score);
    }
    else{
        sf::Sprite menubutton;
        sf::Texture menubuttontxtr = Textures.at("menubutton");
        menubutton.setTexture(menubuttontxtr);
        menubutton.setPosition(320-menubuttontxtr.getSize().x/2, 180-menubuttontxtr.getSize().y);
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed || (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)) {
                window.close();
            }
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left){
                auto mousepoz = sf::Mouse::getPosition(window);
                auto buttonbbox = menubutton.getGlobalBounds();
                if(buttonbbox.contains(window.mapPixelToCoords(mousepoz))){
                    state = 1;
                }
            }
        }
        window.draw(menubutton);
    }
    window.display();
    window.clear();
}

void GameState::EventHandling1(sf::RenderWindow &window, const sf::Event &event, Player &p) {
    if (event.type == sf::Event::Closed || (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)) {
        window.close();
    }
    if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::E) p.Command("e");
    if(event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::E) p.Command("e_release");
    if(p.getJumpCD() == 1 && event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Space) {
        p.setJumpCD(0);
    }
    if (p.getJumpCD() == 0 && event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space) {
        p.Command("jump");
        p.setJumpCD(1);
    }
}

void GameState::ViewAndMovement(sf::RenderWindow &window, Player &p, sf::Text &score) {
    sf::Vector2f score_poz = score.getPosition();
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
        sf::View winview=window.getView();
        int m = p.Command("d");
        if(m == 1 && 100 < p.getX() && p.getX() < 1800 ) {
            window.setView(sf::View(
                    sf::Vector2f(winview.getCenter().x + p.getMS() + p.getmsmod(), winview.getCenter().y),
                    winview.getSize()));
            score_poz.x += p.getMS() + p.getmsmod();
        }
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left)){
        sf::View winview=window.getView();
        int m = p.Command("a");
        if(m == 1 && 100 < p.getX() && p.getX() < 1800 ) {
            window.setView(sf::View(
                    sf::Vector2f(winview.getCenter().x - p.getMS() - p.getmsmod(), winview.getCenter().y),
                    winview.getSize()));
            score_poz.x -= p.getMS() + p.getmsmod();
        }
    }
    score.setPosition(score_poz);
}
