// grupa 151 Marcu Dragos Ionut
// tema 1 lab poo

#include <SFML/Graphics.hpp>
#include <utility>
#include <iostream>
#include <set>

class Entity {
    int tip = 0;
    std::string ent_nume="def";
    sf::Texture ent_txtr;
    int poz_x = 0, poz_y = 0;
    int podea = 0;
public:
    Entity() = default;
    Entity(std::string  nume, int tip_) : tip { tip_ }, ent_nume{std::move( nume )} {}
    Entity(const sf::Texture& texture_, std::string  nume, int tip_) : tip{ tip_ }, ent_nume{std::move( nume )}, ent_txtr { texture_ } {}
    Entity(const sf::Texture& texture_, std::string  nume, int tip_, int x_, int y_) : tip{ tip_ }, ent_nume{std::move( nume )}, ent_txtr { texture_ }, poz_x {x_}, poz_y {y_} {}
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
    sf::Texture& getTexture(){
        return ent_txtr;
    }

    void setPodea(int podea_){
        podea = podea_;
    }

    sf::Sprite getSprite(const std::string& caz="render"){
        sf::Sprite itemsprite;
        itemsprite.setTexture(ent_txtr);
        if(podea == 0 && caz=="render") poz_y += 2;
        itemsprite.setPosition(poz_x, poz_y);
        return itemsprite;
    }

};

class Player {
    double poz_x, poz_y; // pozitie pe harta
    int jump = 0, podea = 0, sus = 0, doublejump = 1, jumpcd = 0;
    int stanga = 0, dreapta = 0;
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

    double getX(){
        return poz_x;
    }

    double getlowHeight(){
        return poz_y - double(player_txtr.getSize().y);
    }

    int getJumpCD(){
        return jumpcd;
    }

    void setJumpCD(int jumpcd_){
        jumpcd = jumpcd_;
    }

    void setCollisions(int podea_, int sus_, int stanga_, int dreapta_){
        podea = podea_;
        sus = sus_;
        stanga = stanga_;
        dreapta = dreapta_;
    }

    void Inv(std::vector<Entity>& vc)
    {
        for (auto& el : inv)
        {
            vc.push_back(el);
            std::cout << "copiere " << *(vc.rbegin()) << '\n';
        }
    }
    sf::Sprite getSprite(const std::string& caz="render")
    {
        sf::Sprite sp;
        sp.setTexture(player_txtr);
        if(caz=="render") {
            if (podea == 0 && jump == 0) poz_y += 2;
            if (jump > 5 && sus == 0) {
                poz_y -= 4;
            }
            if (sus != 0 && jump > 5) jump = 4; // daca se da cu capu' sa cada
            if (jump > 0) jump--;
            if (podea == 1) doublejump = 0;
        }
        sp.setPosition(poz_x, poz_y);
        return sp;
    }

    int Command(const std::string& c){
        int miscare = 0;
        if(c=="jump" && (podea==1 || (jump < 3 && !podea && !doublejump)))
        {
            if(podea == 0 && sus == 0) doublejump = 1;
            jump += 25;
            miscare = 1;
        }
        if(c=="a" && stanga == 0) {
            poz_x -= 2;
            miscare = 1;
        }
        if(c=="d" && dreapta == 0) {
            poz_x += 2;
            miscare = 1;
        }
        return miscare;
    }

};

class Npc {
    double orpoz_x, poz_x, poz_y;
    double dir = 1;
    int podea = 0, sus = 0, stanga = 0, dreapta = 0;
    std::string order = "idle";
    std::vector<Entity> drops;
    sf::Texture npc_txtr;
public:
    explicit Npc(const sf::Texture& txtr_, double poz_x_ = 0.0, double poz_y_ = 0.0)
            : orpoz_x{poz_x_ }, poz_x{ poz_x_ }, poz_y{ poz_y_ }, npc_txtr { txtr_ } {}
    friend std::ostream& operator<<(std::ostream& os, const Npc& npc) {
        os << " Poz npc: " << npc.poz_x << ' ' << npc.poz_y << '\n';
        return os;
    }

    void setOrder(const std::string& ord){
        order = ord;
    }

    void setCollisions(int podea_, int sus_, int stanga_, int dreapta_){
        podea = podea_;
        sus = sus_;
        stanga = stanga_;
        dreapta = dreapta_;
    }

    sf::Sprite getSprite(const std::string& caz="render")
    {
        sf::Sprite sp;
        sp.setTexture(npc_txtr);
        double spoz_x = poz_x, mvvar = 2;
        if(caz=="render") {
            if (podea == 0) poz_y += 2;
            if (order == "patrol") {
                if (std::abs(orpoz_x - spoz_x) > 80) dir *= -1;
                if((dir == -1 && stanga == 0) || (dir == 1 && dreapta== 0)) {
                    spoz_x += dir * mvvar;
                    poz_x = spoz_x;
                }
            }
        }
        sp.setPosition(spoz_x, poz_y);
        return sp;
    }
};

class Harta{
    std::vector< std::pair< Entity, std::pair<double,double> > > Obj;
public:
    friend std::ostream& operator<<(std::ostream& os, const Harta& h) {
        os << " Nr obiecte: " << h.Obj.size() << '\n';
        return os;
    }

    void addObj(const Entity& obj, std::pair<double,double> p){
        Obj.emplace_back(obj,p);
    }
    void drawMap(sf::RenderWindow& window_, Player& plr, std::vector<Npc*> npcvect, std::vector<Entity*> itemvect){
        sf::Sprite ps = plr.getSprite("collision");
        ps.setPosition(ps.getPosition().x, ps.getPosition().y+2);
        sf::FloatRect pb_low = ps.getGlobalBounds();
        ps.setPosition(ps.getPosition().x, ps.getPosition().y-6);
        sf::FloatRect pb_high = ps.getGlobalBounds();
        ps.setPosition(ps.getPosition().x-2, ps.getPosition().y+4);
        sf::FloatRect pb_left = ps.getGlobalBounds();
        ps.setPosition(ps.getPosition().x+4, ps.getPosition().y);
        sf::FloatRect pb_right = ps.getGlobalBounds();
        int podea_ = 0, sus_ = 0, dreapta_ = 0, stanga_ = 0;
        std::vector < std::vector<int> > npccollisions(npcvect.size(), {0, 0, 0, 0});
        std::vector < int > itemcollisions(itemvect.size(), 0);
        for(auto& it:Obj){
            sf::Sprite sp;
            sp.setTexture(it.first.getTexture());
            sp.setPosition(it.second.first, it.second.second); //second.first = x second.second = y
            if (it.second.second >= plr.getlowHeight()) {
                if (sp.getGlobalBounds().intersects(pb_low)) podea_ = 1;
            }
            if (sp.getGlobalBounds().intersects(pb_high)) sus_ = 1;
            if (sp.getGlobalBounds().intersects(pb_right)) dreapta_ = 1;
            if (sp.getGlobalBounds().intersects(pb_left)) stanga_ = 1;
            for(int itemindex = 0; itemindex < itemvect.size(); itemindex++){
                sf::Sprite itemsp = itemvect[itemindex]->getSprite("collision");
                itemsp.setPosition(itemsp.getPosition().x, itemsp.getPosition().y + 2);
                if(itemsp.getGlobalBounds().intersects(sp.getGlobalBounds())) itemcollisions[itemindex] = 1;
            }
            for(int itemindex = 0; itemindex < itemvect.size(); itemindex++){
                itemvect[itemindex]->setPodea(itemcollisions[itemindex]);
            }
            for(int npcindex = 0; npcindex < npcvect.size(); npcindex++) {
                sf::Sprite npcs = npcvect[npcindex]->getSprite("collision");
                npcs.setPosition(npcs.getPosition().x, npcs.getPosition().y+2);
                sf::FloatRect npcb_low = npcs.getGlobalBounds();
                npcs.setPosition(npcs.getPosition().x, npcs.getPosition().y-4);
                sf::FloatRect npcb_high = npcs.getGlobalBounds();
                npcs.setPosition(npcs.getPosition().x-2,npcs.getPosition().y+2);
                sf::FloatRect npcb_left = npcs.getGlobalBounds();
                npcs.setPosition(npcs.getPosition().x+4,npcs.getPosition().y);
                sf::FloatRect npcb_right = npcs.getGlobalBounds();
                if (sp.getGlobalBounds().intersects(npcb_low)) npccollisions[npcindex][0] = 1;
                if (sp.getGlobalBounds().intersects(npcb_high)) npccollisions[npcindex][1] = 1;
                if (sp.getGlobalBounds().intersects(npcb_left)) npccollisions[npcindex][2] = 1;
                if (sp.getGlobalBounds().intersects(npcb_right)) npccollisions[npcindex][3] = 1;
            }
            for(int npcindex = 0; npcindex < npcvect.size(); npcindex++){
                npcvect[npcindex]->setCollisions(npccollisions[npcindex][0], npccollisions[npcindex][1], npccollisions[npcindex][2], npccollisions[npcindex][3]);
            }
            plr.setCollisions(podea_, sus_, stanga_, dreapta_);
            window_.draw(sp);
        }

    }
};

int main()
{
    sf::RenderWindow window(sf::VideoMode(640, 360), "", sf::Style::Close);
    window.setFramerateLimit(60);

    sf::Event event;

    sf::RectangleShape bg;
    bg.setSize(sf::Vector2f(1000, 1000));
    bg.setPosition(0, 0);
    sf::Color bgcolor(214, 192, 103);
    bg.setFillColor(bgcolor);

    sf::Texture p_txtr, n1_txtr, floor1_txtr, perete_txtr, sabie_txtr;
    if (!p_txtr.loadFromFile("p.png")) std::cout << "p txtr\n";
    if (!n1_txtr.loadFromFile("n1.png")) std::cout << "n1 txtr\n";
    if (!floor1_txtr.loadFromFile("floor1.png")) std::cout << "floor1 txtr\n";
    if (!perete_txtr.loadFromFile("perete.png")) std::cout << "perete txtr\n";
    if (!sabie_txtr.loadFromFile("sabia.png")) std::cout << "sabie txtr\n";

    Player p{p_txtr, 250.0, 200.0};

    Entity sabie{ sabie_txtr, "sabie", 1, 450, 100};
    std::vector<Entity*> itemvect;
    itemvect.push_back(&sabie);

    Harta h;
    Entity floor1{floor1_txtr, "floor1", 2}, perete{perete_txtr, "perete", 2};
    for(double i = 0; i <= 700; i+=40){
        h.addObj(floor1, {i, 300.0});
        if(i == 0 || i + 40 > 700) h.addObj(perete, {i, 210});
    }
    for(double i = 400; i<= 560; i+=40){
        h.addObj( floor1, {i, 178});
    }
    Entity e1{"e1", 1};
    p.AddItem(e1);
    std::vector<Entity> v;
    p.Inv(v);

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
            if(p.getJumpCD() == 1 && event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Space) {
                p.setJumpCD(0);
            }
            if (p.getJumpCD() == 0 && event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space) {
                p.Command("jump");
                p.setJumpCD(1);
            }
            //pt ca vreau doar pe apasare folosesc event pt space nu sf::Keyboard::isKeyPressed
        }
        if(window.hasFocus()) {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
                sf::View winview=window.getView();
                int m = p.Command("d");
                if(m == 1 && 100 < p.getX() && p.getX() < 500 )window.setView(sf::View(sf::Vector2f(winview.getCenter().x + 2, winview.getCenter().y), winview.getSize()));
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left)){
                sf::View winview=window.getView();
                int m = p.Command("a");
                if(m == 1 && 100 < p.getX() && p.getX() < 500 )window.setView(sf::View(sf::Vector2f(winview.getCenter().x - 2, winview.getCenter().y), winview.getSize()));
            }
        }
        window.draw(bg);
        h.drawMap(window, p, npc_vector, itemvect);
        for(auto& npc:npc_vector){
            window.draw(npc->getSprite("render"));
        }
        for(auto& item:itemvect){
            window.draw(item->getSprite());
        }
        window.draw(p.getSprite());
        window.display();
        window.clear();
    }

    return 0;
}