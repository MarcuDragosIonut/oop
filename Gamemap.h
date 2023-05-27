#pragma once

#include "Characters.h"

class Harta{
    int lvlwon = 0;
    std::vector< std::pair< Entity, std::pair<double,double> > > Obj;
    std::vector<Npc*> npcvect;
    std::map<std::string, sf::Texture > Textures;

    void Item_pickup(sf::RenderWindow &window_, Player &plr, std::vector<Entity *> &itemvect, const sf::Sprite &ps);

    void Player_col(Player &plr, const sf::Sprite &ps, const sf::FloatRect &pb_low, const sf::FloatRect &pb_high,
                    const sf::FloatRect &pb_left, const sf::FloatRect &pb_right,
                    const std::pair<Entity, std::pair<double, double>> &it, const sf::Sprite &sp, int &podea_,
                    int &sus_,
                    int &dreapta_, int &stanga_);

    void Item_col(const std::vector<Entity *> &itemvect, std::vector<int> &itemcollisions, const sf::Sprite &sp) const;

    void Npc_col(Player &plr, const sf::Sprite &pscurent, const sf::FloatRect &pb_low,
                 std::vector<std::vector<int>> &npccollisions, const sf::Sprite &sp) const;

    void Drawnpcs(sf::RenderWindow& window_);

public:
    Harta() = default;
    Harta(const Harta& other) = delete;
    Harta& operator=(const Harta& other) = delete;
    ~Harta(){
        npcvect.clear();
    }
    friend std::ostream& operator<<(std::ostream& os, const Harta& h);

    static Harta& get_map();

    void addNpc(Npc& npc);

    void addObj(const Entity& obj, std::pair<double,double> p);

    void addTexture(const std::string& str, const sf::Texture& txtr);

    void drawMap(sf::RenderWindow& window_, Player& plr, std::vector<Entity*> itemvect);
};


class LevelBuilder{
protected:
    inline static std::map<std::string, Entity*> Entities;
public:
    static void addEntity(const std::string& nume_ent, Entity* ent);
    static void level1(Harta& harta);
};
