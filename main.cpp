// grupa 151 Marcu Dragos Ionut
// tema 1,2,3 lab poo

#include <SFML/Graphics.hpp>
#include <iostream>
#include <map>
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
    sf::Font textfont;
    try{
        loadfont(textfont.loadFromFile("textures/arcade.TTF"));
    }
    catch(eroare_textura &err){
        std::cout << err.what() << '\n';
    }
    sf::Text score;
    score.setFont(textfont);
    score.setFillColor(sf::Color::Black);
    score.setString("0");
    score.setPosition(12, 0);

    std::map<std::string, sf::Texture> Textures;
    try {
        // player
        loadtxtr(Textures["p_txtr"].loadFromFile("textures/p.png"));
        loadtxtr(Textures["pdead_txtr"].loadFromFile("textures/pmort.png"));

        //npcs
        loadtxtr(Textures["n_txtr"].loadFromFile("textures/n1.png"));
        loadtxtr(Textures["ndead_txtr"].loadFromFile("textures/n1mort.png"));
        loadtxtr(Textures["verde_txtr"].loadFromFile("textures/verde.png"));
        loadtxtr(Textures["verdedead_txtr"].loadFromFile("textures/verdemort.png"));
        loadtxtr(Textures["mov_txtr"].loadFromFile("textures/mov.png"));
        loadtxtr(Textures["movdead_txtr"].loadFromFile("textures/movmort.png"));
        loadtxtr(Textures["fantoma_txtr"].loadFromFile("textures/fantoma.png"));
        loadtxtr(Textures["fantomadead_txtr"].loadFromFile("textures/fantomamort.png"));

        //blocks
        loadtxtr(Textures["floor1_txtr"].loadFromFile("textures/floor1.png"));
        loadtxtr(Textures["perete_txtr"].loadFromFile("textures/perete.png"));
        loadtxtr(Textures["peretemic_txtr"].loadFromFile("textures/peretemic.png"));
        loadtxtr(Textures["finpost_t"].loadFromFile("textures/fin.png"));

        //items + buton
        loadtxtr(Textures["stea_txtr"].loadFromFile("textures/stea.png"));
        loadtxtr(Textures["e"].loadFromFile("textures/e.png"));

        //mesaje
        loadtxtr(Textures["finmesj"].loadFromFile("textures/finmes.png"));
        loadtxtr(Textures["mortmesj"].loadFromFile("textures/mortmes.png"));
    }
    catch(eroare_textura& err){
        std::cout << err.what() << '\n';
    }

    Player p{Textures["p_txtr"], Textures["pdead_txtr"], 250.0, 200.0};

    CharacterFactory::addTexture("n_txtr",Textures["n_txtr"]);
    CharacterFactory::addTexture("ndead_txtr",Textures["ndead_txtr"]);
    CharacterFactory::addTexture("verde_txtr",Textures["verde_txtr"]);
    CharacterFactory::addTexture("verdedead_txtr",Textures["verdedead_txtr"]);
    CharacterFactory::addTexture("mov_txtr",Textures["mov_txtr"]);
    CharacterFactory::addTexture("movdead_txtr",Textures["movdead_txtr"]);
    CharacterFactory::addTexture("fantoma_txtr",Textures["fantoma_txtr"]);
    CharacterFactory::addTexture("fantomadead_txtr",Textures["fantomadead_txtr"]);

    Entity stea{ Textures["stea_txtr"], "stea", 1, 450, 100};
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

    Harta& h=Harta::get_map(); // singleton
    Entity floor1{Textures["floor1_txtr"], "floor1", 2}, perete{Textures["perete_txtr"], "perete", 2};
    Entity peretemic{Textures["peretemic_txtr"],"perete_mic", 2};
    Entity finmark{Textures["finpost_t"], "finpost", 3};
    h.addTexture("e",Textures["e"]);
    h.addTexture("finmesaj", Textures["finmesj"]);
    h.addTexture("mortmesaj", Textures["mortmesj"]);

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
    h.addObj(finmark, {2000, 300-Textures["finpost_t"].getSize().y});
    //map building

    Orange n1 = CharacterFactory::orange(200, 60), n2 = CharacterFactory::orange(450, 40);
    Orange n3 = n1 ,n4 = n1;
    n3.setPosition(1500, 210);
    n4.setPosition(1400, -100);
    Verde v1 = CharacterFactory::verde(1100, 100);
    Verde v2 = v1;
    Mov mv1 = CharacterFactory::mov(1400, -100);
    v2.setPosition(900, 210);
    Fantoma f1 = CharacterFactory::fantoma( 500, 100);
    h.addNpc(n1);
    h.addNpc(n2);
    h.addNpc(n3);
    h.addNpc(n4);
    h.addNpc(v1);
    h.addNpc(v2);
    h.addNpc(mv1);
    h.addNpc(f1);
    n1.setOrder("patrol");
    n2.setOrder("patrol");
    n3.setOrder("patrol");
    n4.setOrder("patrol");
    v1.setOrder("patrol");
    v2.setOrder("fly");
    mv1.setOrder("jump");
    f1.setOrder("patrol");

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
        if(p.getY() > 500) p.Kill();
        window.draw(bg);
        window.draw(p.getSprite());
        h.drawMap(window, p, itemvect);
        for(auto& item:itemvect){
            if(item->status() == 0)window.draw(item->getSprite());
        }
        score.setString(std::to_string(p.getScore()));
        window.draw(score);
        window.display();
        window.clear();
    }

    return 0;
}