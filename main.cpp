// grupa 151 Marcu Dragos Ionut
// tema 1,2,3 lab poo

#include <SFML/Graphics.hpp>
#include <iostream>
#include <map>
#include <set>
#include <memory>
#include "Noncharacters.h"
#include "GameExceptions.h"
#include "Characters.h"
#include "Gamemap.h"
#include "GameHandler.h"

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
    catch(eroare_resursa &err){
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
    catch(eroare_resursa& err){
        std::cout << err.what() << '\n';
    }

    Player p{Textures["p_txtr"], Textures["pdead_txtr"], 250.0, 200.0};

    CharacterTexturesHolder::addTexture(std::string{typeid(Orange).name()},Textures["n_txtr"]);
    CharacterTexturesHolder::addTexture(std::string{typeid(Orange).name()}+"dead",Textures["ndead_txtr"]);
    CharacterTexturesHolder::addTexture(std::string{typeid(Verde).name()},Textures["verde_txtr"]);
    CharacterTexturesHolder::addTexture(std::string{typeid(Verde).name()}+"dead",Textures["verdedead_txtr"]);
    CharacterTexturesHolder::addTexture(std::string{typeid(Mov).name()},Textures["mov_txtr"]);
    CharacterTexturesHolder::addTexture(std::string{typeid(Mov).name()}+"dead",Textures["movdead_txtr"]);
    CharacterTexturesHolder::addTexture(std::string{typeid(Fantoma).name()},Textures["fantoma_txtr"]);
    CharacterTexturesHolder::addTexture(std::string{typeid(Fantoma).name()}+"dead",Textures["fantomadead_txtr"]);

    Entity stea{ Textures["stea_txtr"], "stea", 1, 450, 100};
    Effect efstea{"stea", 2, 2};
    stea.setEffect(std::make_shared<Effect>(efstea));
    Entity stea2 = stea, stea3 = stea;
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
    LevelBuilder::addEntity("floor1", &floor1);
    LevelBuilder::addEntity("perete", &perete);
    LevelBuilder::addEntity("peretemic", &peretemic);
    LevelBuilder::addEntity("finmark", &finmark);
    LevelBuilder::level1(h);

    Orange n1 = CharacterFactory<Orange>::generate(200, 60), n2 = CharacterFactory<Orange>::generate(450, 40);
    Orange n3 = n1 ,n4 = n1;
    n3.setPosition(1500, 210);
    n4.setPosition(1400, -100);
    Verde v1 = CharacterFactory<Verde>::generate(1100, 100);
    Verde v2 = v1;
    Mov mv1 = CharacterFactory<Mov>::generate(1400, -100);
    v2.setPosition(900, 210);
    Fantoma f1 = CharacterFactory<Fantoma>::generate( 500, 100);
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

    GameState::SetUp();
    while (window.isOpen()) {
        GameState::RunGame(window, event, bg, p, score, h, itemvect);
    }

    return 0;
}