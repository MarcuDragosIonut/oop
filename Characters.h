#include <SFML/Graphics.hpp>

class Character{
protected:
    double poz_x, poz_y;
    int left = 0, right = 0, up = 0, floor = 0;
    sf::Texture char_txtr, char_dead_txtr;
    int ms = 2, jp = 4;
    int mort = 0;
public:
    Character(const sf::Texture& texture_, const sf::Texture& texture_dead_, double poz_x_ = 0.0, double poz_y_ = 0.0) :
    poz_x { poz_x_ }, poz_y{ poz_y_ }, char_txtr{ texture_ }, char_dead_txtr {texture_dead_} {}
    Character(const Character& other) : poz_x {other.poz_x}, poz_y {other.poz_y}, char_txtr {other.char_txtr}, char_dead_txtr {other.char_dead_txtr} {}
    ~Character(){}
    double getX(){
        return poz_x;
    }
    double getY(){
        return poz_y;
    }
    double getlowHeight(){
        return poz_y - double(char_txtr.getSize().y);
    }
    int getMS(){
        return ms;
    }
    int getJP(){
        return jp;
    }
    int getStatus(){
        return mort;
    }
    void setCollisions(int floor_, int up_, int left_, int right_){
        floor = floor_;
        up = up_;
        left = left_;
        right = right_;
    }
    virtual void setMovement() = 0;
    virtual sf::Sprite getSprite(const std::string& caz="render" ){
        if(caz=="render"){
            setMovement();
        }
        sf::Sprite sp;
        sp.setTexture(char_txtr);
        sp.setPosition(poz_x, poz_y);
        return sp;
    }

};

class Player : public Character{
    int jump = 0, doublejump = 1, jumpcd = 0;
    int interact = 0;
    std::vector<std::pair<Effect,sf::Clock>> plref;
public:
    Player(const sf::Texture &texture, const sf::Texture &textureDead, double pozX, double pozY): Character(texture, textureDead, pozX, pozY) {}
    friend std::ostream& operator<<(std::ostream& os, const Player& plr) {
        os << " Poz player: " << plr.poz_x << ' ' << plr.poz_y << '\n';
        return os;
    }
    void AddEff(Entity e) {
        if (e.getTip() == 1)
        {
            sf::Clock timpefect;
            plref.push_back({e.getEffect(), timpefect});
        }
    }

    int interaction(){
        return interact;
    }

    int getJumpCD(){
        return jumpcd;
    }

    void setJumpCD(int jumpcd_){
        jumpcd = jumpcd_;
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
    void setMovement() override{
        int jpmod = getjpmod();
        if (floor == 0 && jump == 0) poz_y += 2;
        if (jump > 5) {
            poz_y -= jp+jpmod-up;
        }
        if (up != 0 && jump > 5) jump = 4; // daca se da cu capu' sa cada
        if (jump > 0) jump--;
        if (floor == 1) doublejump = 0;
    }

    int Command(const std::string& c){
        int miscare = 0;
        if(c=="jump" && (floor==1 || (jump < 3 && !floor && !doublejump)))
        {
            if(floor == 0 && up == 0) doublejump = 1;
            jump += 25;
            miscare = 1;
        }
        int msmod = getmsmod();
        if(c=="a") {
            double oldpoz = poz_x;
            poz_x -= ms+msmod-left;
            if(poz_x != oldpoz)miscare = 1;
        }
        if(c=="d") {
            double oldpoz = poz_x;
            poz_x += ms+msmod-right;
            if(poz_x != oldpoz)miscare = 1;
        }
        if(c=="e") interact = 1;
        if(c=="e_release") interact = 0;
        return miscare;
    }
};

class Npc : public Character{
    int dir = 0;
    std::string order = "idle";
public:
    Npc(const sf::Texture& t, const sf::Texture& tdead, double x, double y) : Character(t, tdead, x, y) { ms = 2;}
    friend std::ostream& operator<<(std::ostream& os, const Npc& npc) {
        os << " Poz npc: " << npc.poz_x << ' ' << npc.poz_y << '\n';
        return os;
    }

    void setOrder(const std::string& ord){
        if(ord=="patrol") dir = 1;
        order = ord;
    }

    void setMovement() override{
        if(floor == 0) poz_y += 2;
        if(order == "patrol"){
            if(dir == 1){
                if(right > 0) dir = -1;
                if(floor == 1)poz_x += ms - right;
            }
            if(dir == -1){
                if(left > 0) dir = 1;
                if(floor == 1)poz_x -= ms - left;
            }
        }
    }
};
