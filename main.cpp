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
    Entity() = default;
    Entity(std::string  nume, int tip_) : tip { tip_ }, ent_nume{std::move( nume )} {}
    Entity(const sf::Texture& texture_, std::string  nume, int tip_) : tip{ tip_ }, ent_nume{std::move( nume )}, ent_txtr { texture_ } {}
    Entity(const Entity& other) : tip{ other.tip }, ent_nume{ other.ent_nume }, ent_txtr{ other.ent_txtr } {}
    Entity& operator=(const Entity& other) {
        tip = other.tip;
        ent_nume = other.ent_nume;
        ent_txtr = other.ent_txtr;
        return *this;
    }
    ~Entity() = default;
    friend std::ostream& operator<<(std::ostream& os, const Entity& ent) {
        os << " Nume " << ent.ent_nume << " Tip " << ent.tip << '\n';
        return os;
    }
    int getTip() const { return tip; }

};

class Player {
    double poz_x, poz_y; // pozitie pe harta
    int jump = 0;
    std::vector<Entity> inv;
    sf::Texture player_txtr;
public:
    explicit Player(const sf::Texture& texture_, double poz_x_ = 0.0, double poz_y_ = 0.0) : poz_x { poz_x_ }, poz_y{ poz_y_ }, player_txtr{ texture_ } {}
    friend std::ostream& operator<<(std::ostream& os, const Player& plr) {
        os << " Poz player: " << plr.poz_x << ' ' << plr.poz_y << '\n';
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
        double spoz_x = poz_x, spoz_y = poz_y;
        if(jump > 23){
            poz_y -= 3;
        }
        else if(jump > 0 && jump < 20){
            poz_y += 3;
        }
        if(jump > 0) jump--;
        sp.setPosition(spoz_x, spoz_y);
        return sp;
    }
    void Command(const std::string& c){
        if(c=="jump" && jump == 0) jump = 43;
        if(c=="a") poz_x -= 2.5;
        if(c=="d") poz_x += 2.5;
    }

};

class Npc {
    double orpoz_x, poz_x, poz_y;
    double dir = 1;
    std::string order = "idle";
    sf::Texture npc_txtr;
public:
    explicit Npc(const sf::Texture& txtr_, double poz_x_ = 0.0, double poz_y_ = 0.0)
            : orpoz_x{poz_x_ }, poz_x{ poz_x_ }, poz_y{ poz_y_ }, npc_txtr { txtr_ } {}
    friend std::ostream& operator<<(std::ostream& os, const Npc& npc) {
        os << " Poz npc: " << npc.poz_x << ' ' << npc.poz_y << '\n';
        return os;
    }
    sf::Sprite getSprite()
    {
        sf::Sprite sp;
        sp.setTexture(npc_txtr);
        double spoz_x = poz_x, spoz_y = poz_y, mvvar = 1.5;
        if(order=="patrol"){
            if(std::abs(orpoz_x-spoz_x) > 80) dir *= -1;
            spoz_x += dir*mvvar;
            poz_x = spoz_x;
        }
        sp.setPosition(spoz_x, spoz_y);
        return sp;
    }
    void setOrder(const std::string& ord){
        order = ord;
    }
};

/*
class Harta{
    std::vector< std::pair< Entity, std::pair<double,double> > > Obj;
};
*/
int main()
{
    sf::RenderWindow window(sf::VideoMode(640, 360), "", sf::Style::Close);
    window.setFramerateLimit(60);

    sf::Event event;

    sf::RectangleShape bg;
    bg.setSize(sf::Vector2f(500, 500));
    bg.setPosition(0, 0);
    bg.setFillColor(sf::Color::Black);

    sf::Texture p_txtr, n1_txtr;
    if (!p_txtr.loadFromFile("p.png")) std::cout << "p txtr\n";
    if (!n1_txtr.loadFromFile("n1.png")) std::cout << "n1 txtr\n";


    //Harta h;
    Player p{p_txtr, 250.0, 250.0};
    Entity e1{"e1", 1}, e2{"e2", 1};
    p.AddItem(e1);
    p.AddItem(e2);
    std::vector<Entity> v;
    p.Inv(v);
    for (const auto & i : v) std::cout << i << '\n';

    Npc n1{n1_txtr, 200, 60};
    n1.setOrder("patrol");
    while (window.isOpen()) {

        while (window.pollEvent(event)) {

            if (event.type == sf::Event::Closed || ( event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) ) {
                window.close();
            }
        }
        if(window.hasFocus()) {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) p.Command("jump");
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) p.Command("d");
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) p.Command("a");
        }
        window.draw(bg);
        window.draw(n1.getSprite());
        window.draw(p.getSprite());
        window.display();
        window.clear();
    }

    return 0;
}