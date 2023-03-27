#include <SFML/Graphics.hpp>

class Player {
    double poz_x, poz_y; // pozitie pe harta
    int jump = 0, podea = 0, sus = 0, doublejump = 1, jumpcd = 0, jp = 4;
    int stanga = 0, dreapta = 0, ms = 2;
    int interact = 0, mort = 0;
    std::vector<std::pair<Effect,sf::Clock>> plref;
    sf::Texture player_txtr;

public:
    explicit Player(const sf::Texture& texture_, double poz_x_ = 0.0, double poz_y_ = 0.0) : poz_x { poz_x_ }, poz_y{ poz_y_ }, player_txtr{ texture_ } {}
    friend std::ostream& operator<<(std::ostream& os, const Player& plr) {
        os << " Poz player: " << plr.poz_x << ' ' << plr.poz_y << '\n';
        return os;
    }
    void AddEff(Entity e) {
        if (e.getTip() == 1)
        {
            sf::Clock timpefect;
            plref.push_back({e.getEffect(), timpefect});
            std::cout << e.getEffect().getM() << ' ' << e.getEffect().getJ() << '\n';
        }
    }

    int getStatus(){
        return mort;
    }

    int interaction(){
        return interact;
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

    int getMS(){
        return ms;
    }

    int getJP(){
        return jp;
    }

    int getmsmod(){
        int msmod = 0;
        for(auto& it:plref){
            if(it.second.getElapsedTime() < sf::seconds(30)){
                msmod += it.first.getM();
            }
        }
        return msmod;
    }

    int getjpmod(){
        int jpmod = 0;
        for(auto& it:plref){
            if(it.second.getElapsedTime() < sf::seconds(30)){
                jpmod += it.first.getJ();
            }
        }
        return jpmod;
    }

    void setCollisions(int podea_, int sus_, int stanga_, int dreapta_){
        podea = podea_;
        sus = sus_;
        stanga = stanga_;
        dreapta = dreapta_;
        if(stanga_ != 0)std::cout << stanga_ << '\n';
    }

    sf::Sprite getSprite(const std::string& caz="render")
    {
        sf::Sprite sp;
        sp.setTexture(player_txtr);
        if(caz=="render") {
            int jpmod = getjpmod();
            if (podea == 0 && jump == 0) poz_y += 2;
            if (jump > 5 && sus == 0) {
                poz_y -= jp+jpmod;
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
        int msmod = getmsmod();
        if(c=="a") {
            double oldpoz = poz_x;
            poz_x -= ms+msmod-stanga;
            if(poz_x != oldpoz)miscare = 1;
        }
        if(c=="d") {
            double oldpoz = poz_x;
            poz_x += ms+msmod-dreapta;
            if(poz_x != oldpoz)miscare = 1;
        }
        if(c=="e") interact = 1;
        if(c=="e_release") interact = 0;
        return miscare;
    }

};

class Npc {
    double orpoz_x, poz_x, poz_y;
    double dir = 1;
    int podea = 0, sus = 0, stanga = 0, dreapta = 0;
    std::string order = "idle";
    Entity drop;
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
