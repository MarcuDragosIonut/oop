// grupa 151 Marcu Dragos Ionut
// tema 1 lab poo

#include <SFML/Graphics.hpp>
#include <utility>
#include <iostream>


class Entity {
    int tip = 0;
    std::string ent_nume="def";
    sf::Texture ent_txtr;
public:
    Entity() {}
    Entity(std::string  nume, int tip_) : tip { tip_ }, ent_nume{std::move( nume )} {}
    Entity(const sf::Texture& texture_, std::string  nume, int tip_) : tip{ tip_ }, ent_nume{std::move( nume )}, ent_txtr { texture_ } {}
    Entity(const Entity& other) : tip{ other.tip }, ent_nume{ other.ent_nume }, ent_txtr{ other.ent_txtr } {}
    Entity& operator=(const Entity& other) {
        tip = other.tip;
        ent_nume = other.ent_nume;
        ent_txtr = other.ent_txtr;
        return *this;
    }
    ~Entity() {}
    friend std::ostream& operator<<(std::ostream& os, const Entity& ent) {
        os << " Nume " << ent.ent_nume << " Tip " << ent.tip << '\n';
        return os;
    }
    int getTip() const { return tip; }

};

class Player {
    double poz_x, poz_y; // pozitie pe harta
    double hp = 10.0;
    std::vector<Entity> inv;
    sf::Texture player_txtr;
public:
    Player(sf::Texture texture_, double poz_x_ = 0.0, double poz_y_ = 0.0) : player_txtr{ texture_ }, poz_x { poz_x_ }, poz_y{ poz_y_ } {}
    friend std::ostream& operator<<(std::ostream& os, const Player& plr) {
        os << "Hp: " << plr.hp << " Poz player: " << plr.poz_x << ' ' << plr.poz_y << '\n';
        return os;
    }
    void AddItem(const Entity& e) {
        if (e.getTip() == 1 && inv.size() < 10)
        {
            inv.push_back(e);
        }
    }
    void Inv(std::vector<Entity>& vc)
    {
        for (auto& el : inv)
        {
            vc.push_back(el);
            std::cout << "copiere " << *(vc.rbegin()) << '\n';
        }
    }
    sf::Sprite getSprite()
    {
        sf::Sprite sp;
        sp.setTexture(player_txtr);
        sp.setPosition(poz_x, poz_y);
        return sp;
    }
    void Command(const sf::Keyboard::Key c){
        switch(c){
            case sf::Keyboard::W : poz_y += 0.5;
            case sf::Keyboard::S : poz_y -= 0.5;
            case sf::Keyboard::D : poz_x += 0.5;
            case sf::Keyboard::A : poz_x -= 0.5;
            default:
        }
    }

};

class Npc {
    double poz_x, poz_y, hp, at;
    sf::Texture npc_txtr;
public:
    Npc(sf::Texture txtr_, double poz_x_ = 0.0, double poz_y_ = 0.0, double hp_ = 10.0 , double at_ = 2.0) : poz_x{ poz_x_ }, poz_y{ poz_y_ }, hp{ hp_ }, at{ at_ }, npc_txtr { txtr_ } {}
    friend std::ostream& operator<<(std::ostream& os, const Npc& npc) {
        os << "Hp: " << npc.hp << " At "<< npc.at << " Poz npc: " << npc.poz_x << ' ' << npc.poz_y << '\n';
        return os;
    }
    sf::Sprite getSprite()
    {
        sf::Sprite sp;
        sp.setTexture(npc_txtr);
        sp.setPosition(poz_x, poz_y);
        return sp;
    }
};

int main()
{
    sf::RenderWindow window(sf::VideoMode(1280, 720), "", sf::Style::Close);
    window.setFramerateLimit(60);

    sf::Event event;

    sf::RectangleShape bg;
    bg.setSize(sf::Vector2f(500, 500));
    bg.setPosition(0, 0);
    bg.setFillColor(sf::Color::Black);
    window.draw(bg);

    sf::Texture p_txtr, n1_txtr;
    if (!p_txtr.loadFromFile("p.png")) std::cout << "p txtr\n";
    if (!n1_txtr.loadFromFile("n1.png")) std::cout << "n1 txtr\n";

    Player p{p_txtr, 250.0, 250.0};
    Entity e1{"e1", 1}, e2{"e2", 1};
    p.AddItem(e1);
    p.AddItem(e2);
    std::vector<Entity> v;
    p.Inv(v);
    for (const auto & i : v) std::cout << i << '\n';

    Npc n1{n1_txtr, 30, 60};
    while (window.isOpen()) {

        while (window.pollEvent(event)) {

            if (event.type == sf::Event::Closed) {

                window.close();
            }
        }

        }
        window.draw(n1.getSprite());
        window.draw(p.getSprite());
        window.display();
    }

    return 0;
}