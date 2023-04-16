// grupa 151 Marcu Dragos Ionut
// tema 1,2,3 lab poo

#include <SFML/Graphics.hpp>
#include <iostream>
#include <set>
#include <memory>
#include "Noncharacters.h"
#include "Characters.h"
#include "Gamemap.h"

int main()
{
    sf::RenderWindow window(sf::VideoMode(640, 360), "", sf::Style::Close);
    window.setFramerateLimit(60);

    sf::Event event;

    sf::RectangleShape bg;
    bg.setSize(sf::Vector2f(5000, 1000));
    bg.setPosition(-200, 0);
    sf::Color bgcolor(214, 192, 103);
    bg.setFillColor(bgcolor);

    sf::Texture p_txtr, n_txtr, verde_txtr, mov_txtr; // characters
    sf::Texture floor1_txtr, perete_txtr, peretemic_txtr, stea_txtr, e_t, finpost_t; // map blocks si map items
    sf::Texture mortmesj_t, finmesj_t; // mesaje
    sf::Texture pdead_txtr, ndead_txtr, verdedead_txtr, movdead_txtr; // dead characters

    // player
    try{
        loadtxtr(p_txtr.loadFromFile("textures\\p.png"));
        loadtxtr(pdead_txtr.loadFromFile("textures\\pmort.png"));
    }
    catch(eroare_textura& err){
        std::cout << "player: " << err.what() << '\n';
    }

    //npcs
    try{
        loadtxtr(n_txtr.loadFromFile("textures\\n1.png"));
        loadtxtr(ndead_txtr.loadFromFile("textures\\n1mort.png"));
        loadtxtr(verde_txtr.loadFromFile("textures\\verde.png"));
        loadtxtr(verdedead_txtr.loadFromFile("textures\\verdemort.png"));
        loadtxtr(mov_txtr.loadFromFile("textures\\mov.png"));
        loadtxtr(movdead_txtr.loadFromFile("textures\\movmort.png"));
    }
    catch(eroare_textura& err){
        std::cout << "npc: " << err.what() << '\n';
    }

    //blocks
    try{
        loadtxtr(floor1_txtr.loadFromFile("textures\\floor1.png"));
        loadtxtr(perete_txtr.loadFromFile("textures\\perete.png"));
        loadtxtr(peretemic_txtr.loadFromFile("textures\\peretemic.png"));
        loadtxtr(finpost_t.loadFromFile("textures\\fin.png"));
    }
    catch(eroare_textura& err){
        std::cout << "blocks: " << err.what() << '\n';
    }

    //items + buton
    try{
        loadtxtr(stea_txtr.loadFromFile("textures\\stea.png"));
        loadtxtr(e_t.loadFromFile("textures\\e.png"));
    }
    catch(eroare_textura& err){
        std::cout << "items: " << err.what() << '\n';
    }

    //mesaje
    try{
        loadtxtr(finmesj_t.loadFromFile("textures\\finmes.png"));
        loadtxtr(mortmesj_t.loadFromFile("textures\\mortmes.png"));
    }
    catch(eroare_textura& err){
        std::cout << "mesaje: " << err.what() << '\n';
    }

    Player p{p_txtr, pdead_txtr, 250.0, 200.0};

    Entity stea{ stea_txtr, "stea", 1, 450, 100};
    Effect efstea{"stea", 2, 2};
    stea.setEffect(std::make_shared<Effect>(efstea));
    Entity stea2 = stea, stea3;
    stea3 = stea;
    stea2.setPoz(900, -500);
    stea3.setPoz(1500, -500);
    std::vector<Entity*> itemvect;
    itemvect.push_back(&stea);
    itemvect.push_back(&stea2);
    itemvect.push_back(&stea3);

    Harta h;
    h.addTexture("e",e_t);
    h.addTexture("finmesaj", finmesj_t);
    h.addTexture("mortmesaj", mortmesj_t);
    Entity floor1{floor1_txtr, "floor1", 2}, perete{perete_txtr, "perete", 2};
    Entity peretemic{peretemic_txtr,"perete_mic", 2};
    Entity finmark{finpost_t, "finpost", 3};

    //map building
    for(double i = 0; i <= 2000; i+=40){
        if(i < 1250 || i > 1360)h.addObj(floor1, {i, 300.0});
        if(i == 0 || i + 40 == 720) h.addObj(perete, {i, 210});
    }
    for(double i = 400; i<= 560; i+=40){
        h.addObj( floor1, {i, 180});
    }
    for(double i = 800; i<= 920; i+=40){
        h.addObj( floor1, {i, 120});
    }
    for(double i = 1400; i<= 1520; i+=40){
        h.addObj( floor1, {i, 120});
    }
    h.addObj(perete, {1400, 210});
    for(double i = 1240; i>950 ; i-=120) h.addObj(peretemic, {i, 255});
    for(double i = 1600; i<=1760 ; i+=40){
        h.addObj(floor1, {i, 210});
    }
    h.addObj(finmark, {2000, 300-finpost_t.getSize().y});
    //map building

    Orange n1{n_txtr, ndead_txtr, 200, 60}, n2{n_txtr, ndead_txtr, 450, 40};
    Orange n3 = n1 ,n4 = n1;
    n3.setPosition(1500, 210);
    n4.setPosition(1400, -100);
    Verde v1{ verde_txtr, verdedead_txtr, 1100, 100};
    Verde v2 = v1;
    Mov mv1{ mov_txtr, movdead_txtr, 1400, -100};
    v2.setPosition(900, 210);
    h.addNpc(n1);
    h.addNpc(n2);
    h.addNpc(n3);
    h.addNpc(n4);
    h.addNpc(v1);
    h.addNpc(v2);
    h.addNpc(mv1);
    n1.setOrder("patrol");
    n2.setOrder("patrol");
    n3.setOrder("patrol");
    n4.setOrder("patrol");
    v1.setOrder("patrol");
    v2.setOrder("fly");
    mv1.setOrder("jump");

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
                if(m == 1 && 100 < p.getX() && p.getX() < 1800 )window.setView(sf::View(sf::Vector2f(winview.getCenter().x + p.getMS() + p.getmsmod(), winview.getCenter().y), winview.getSize()));
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left)){
                sf::View winview=window.getView();
                int m = p.Command("a");
                if(m == 1 && 100 < p.getX() && p.getX() < 1800 )window.setView(sf::View(sf::Vector2f(winview.getCenter().x - p.getMS() - p.getmsmod(), winview.getCenter().y), winview.getSize()));
            }
        }
        if(p.getY() > 500) p.Kill();
        window.draw(bg);
        window.draw(p.getSprite());
        h.drawMap(window, p, itemvect);
        for(auto& item:itemvect){
            if(item->status() == 0)window.draw(item->getSprite());
        }
        window.display();
        window.clear();
    }

    return 0;
}