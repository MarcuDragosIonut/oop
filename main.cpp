// grupa 151 Marcu Dragos Ionut
// tema 1 lab poo

#include <SFML/Graphics.hpp>
#include <utility>
#include <iostream>
#include <set>
#include "Noncharacters.h"
#include "Characters.h"
#include "Gamemap.h"

int main()
{
    sf::RenderWindow window(sf::VideoMode(640, 360), "", sf::Style::Close);
    window.setFramerateLimit(60);

    sf::Event event;

    sf::RectangleShape bg;
    bg.setSize(sf::Vector2f(2500, 1000));
    bg.setPosition(-200, 0);
    sf::Color bgcolor(214, 192, 103);
    bg.setFillColor(bgcolor);

    sf::Texture p_txtr, n1_txtr, floor1_txtr, perete_txtr, stea_txtr, e_t, finpost_t, finmesj_t;
    if (!p_txtr.loadFromFile("p.png")) std::cout << "p txtr\n";
    if (!n1_txtr.loadFromFile("n1.png")) std::cout << "n1 txtr\n";
    if (!floor1_txtr.loadFromFile("floor1.png")) std::cout << "floor1 txtr\n";
    if (!perete_txtr.loadFromFile("perete.png")) std::cout << "perete txtr\n";
    if (!stea_txtr.loadFromFile("stea.png")) std::cout << "sabie txtr\n";
    if (!e_t.loadFromFile("e.png")) std::cout << "e txtr\n";
    if (!finpost_t.loadFromFile("fin.png")) std::cout << "finpost txtr\n";
    if (!finmesj_t.loadFromFile("finmes.png")) std::cout << "finmesj txtr\n";
    Player p{p_txtr, 250.0, 200.0};

    Entity stea{ stea_txtr, "stea", 1, 450, 100}, stea2{ stea_txtr, "stea2", 1, 770, -500};
    stea.setEffect("stea", 2, 2);
    stea2.setEffect("stea", 2, 2);
    std::vector<Entity*> itemvect;
    itemvect.push_back(&stea);
    itemvect.push_back(&stea2);

    Harta h;
    h.addTexture("e",e_t);
    h.addTexture("finmesaj", finmesj_t);
    Entity floor1{floor1_txtr, "floor1", 2}, perete{perete_txtr, "perete", 2};
    Entity finmark{finpost_t, "finpost", 3};
    for(double i = 0; i <= 1000; i+=40){
        h.addObj(floor1, {i, 300.0});
        if(i == 0 || i + 40 == 720) h.addObj(perete, {i, 210});
    }
    for(double i = 400; i<= 560; i+=40){
        h.addObj( floor1, {i, 178});
    }
    h.addObj(finmark, {1000, 300-finpost_t.getSize().y});

    std::vector<Npc*> npc_vector;

    Npc n1{n1_txtr, 200, 60}, n2{n1_txtr, 450, 40};
    npc_vector.push_back(&n1);
    npc_vector.push_back(&n2);
    n1.setOrder("patrol");
    n2.setOrder("patrol");

    while (window.isOpen()) {

        while (window.pollEvent(event)) {

            if (event.type == sf::Event::Closed || ( event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) ) {
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
        if(window.hasFocus()) {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
                sf::View winview=window.getView();
                int m = p.Command("d");
                if(m == 1 && 100 < p.getX() && p.getX() < 800 )window.setView(sf::View(sf::Vector2f(winview.getCenter().x + p.getMS() + p.getmsmod(), winview.getCenter().y), winview.getSize()));
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left)){
                sf::View winview=window.getView();
                int m = p.Command("a");
                if(m == 1 && 100 < p.getX() && p.getX() < 800 )window.setView(sf::View(sf::Vector2f(winview.getCenter().x - p.getMS() - p.getmsmod(), winview.getCenter().y), winview.getSize()));
            }
        }
        window.draw(bg);
        if(p.getStatus() == 0)window.draw(p.getSprite());
        h.drawMap(window, p, npc_vector, itemvect);
        for(auto& npc:npc_vector){
            window.draw(npc->getSprite("render"));
        }
        for(auto& item:itemvect){
            if(item->status() == 0)window.draw(item->getSprite());
        }
        window.display();
        window.clear();
    }

    return 0;
}