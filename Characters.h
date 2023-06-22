#pragma once

#include "Noncharacters.h"

class Character{
protected:
    double poz_x, poz_y;
    static inline int gravity = 3;
    int left = 0, right = 0, up = 0, floor = 0; // pt collision
    sf::Texture char_txtr, char_dead_txtr;
    int ms = 2, jp = 4; // movement speed, jump power
    int mort = 0;
public:
    Character(const sf::Texture& texture_, const sf::Texture& texture_dead_, double poz_x_ = 0.0, double poz_y_ = 0.0) :
            poz_x { poz_x_ }, poz_y{ poz_y_ }, char_txtr{ texture_ }, char_dead_txtr {texture_dead_} {}
    Character(const Character& other) : poz_x {other.poz_x}, poz_y {other.poz_y}, char_txtr {other.char_txtr}, char_dead_txtr {other.char_dead_txtr} {}
    ~Character() = default;
    double getX() const;
    double getY() const;
    int getMS() const;
    int getJP() const;
    int getStatus() const;
    void Kill();
    static int getGravity();
    void setCollisions(int floor_, int up_, int left_, int right_);
    virtual void setMovement() = 0;
    virtual sf::Sprite getSprite(const std::string& caz="render" );

};

class Player : public Character{
    int jump = 0, doublejump = 1, jumpcd = 0; // jump: cat mai are de sarit, doublejump:
    int interact = 0; // daca a luat un item (entitate)
    int score = 0;
    std::vector<std::pair<Effect,sf::Clock>> plref;
public:
    Player(const sf::Texture &texture, const sf::Texture &textureDead, double pozX, double pozY): Character(texture, textureDead, pozX, pozY) {}
    friend std::ostream& operator<<(std::ostream& os, const Player& plr) {
        os << " Poz player: " << plr.poz_x << ' ' << plr.poz_y << '\n';
        return os;
    }
    void AddEff(Entity& e);

    void IncreaseScore(int added_score);

    int getScore();

    int interaction() const;

    int getJumpCD() const;

    void setJumpCD(int jumpcd_);


    int getmsmod();

    int getjpmod();
    void setMovement() override;

    int Command(const std::string& c);
};

class Npc : public Character{
protected:
    int dir = 0;
    int score_value = 1;
    std::string order = "idle";
public:
    Npc(const sf::Texture& t, const sf::Texture& tdead, double x, double y) : Character(t, tdead, x, y) { ms = 2;}
    friend std::ostream& operator<<(std::ostream& os, const Npc& npc) {
        os << " Poz npc: " << npc.poz_x << ' ' << npc.poz_y << '\n';
        return os;
    }
    virtual ~Npc() = default;

    int getScoreValue();

    void setOrder(const std::string& ord);

    void setPosition(double x, double y);

    //virtual void setMovement() = 0;
};

class Orange : public Npc{
public:
    Orange(const sf::Texture &t, const sf::Texture &tdead, double x, double y) : Npc(t, tdead, x, y) {}
    ~Orange() = default;
    void setMovement() override;
};

class Verde : public Npc{
public:
    Verde(const sf::Texture &t, const sf::Texture &tdead, double x, double y);
    ~Verde() = default;
    void setMovement() override;
};

class Mov : public Npc{
    int airtime = 0, transparent = 0;
    sf::Clock movc;
public:
    Mov(const sf::Texture &t, const sf::Texture &tdead, double x, double y);
    ~Mov() = default;
    void setMovement() override;
    sf::Sprite getSprite(const std::string& caz="renger") override;
};

class Fantoma: public Npc{
    int movlen = 0; // cat s-a miscat
public:
    Fantoma(const sf::Texture& t, const sf::Texture& tmort, double x, double y);
    ~Fantoma() = default;
    void setMovement() override;
};

class CharacterTexturesHolder{
    std::map<std::string, sf::Texture> CharacterTextures;
public:
    void addTexture(std::string nume, const sf::Texture& textura);
    sf::Texture getCharTexture(const std::string& charname);
};

template <typename T>
class CharacterFactory{
public:
    static T generate(CharacterTexturesHolder& cth, double x, double y) {
        std::string charname = typeid(T).name();
        return T(cth.getCharTexture(charname),
                 cth.getCharTexture(charname+"dead"), x, y);
    }
};

