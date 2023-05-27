#pragma once

#include <memory>
#include <SFML/Graphics.hpp>

class Effect{
    int jumpmod = 0, movmod = 0; // jump power modifier, movement speed modifier
    std::string nume;
public:
    explicit Effect(const std::string& nume_, int jm_, int mm_) : jumpmod {jm_}, movmod {mm_}, nume {nume_} {}
    friend std::ostream& operator<<(std::ostream& os, const Effect& eff) {
        os << eff.nume << '\n';
        return os;
    }
    int getJ();
    int getM();
    const std::string& getName();
};

class Entity {
    int tip = 0, sters = 0;
    std::string ent_nume="def";
    sf::Texture ent_txtr;
    std::shared_ptr<Effect> entef; // efectul entitatii
    int poz_x = 0, poz_y = 0, floor = 0;
public:
    Entity() = default;
    Entity(std::string  nume, int tip_) : tip { tip_ }, ent_nume{std::move( nume )} {}
    Entity(const sf::Texture& texture_, std::string  nume, int tip_) : tip{ tip_ }, ent_nume{std::move( nume )}, ent_txtr { texture_ } {}
    Entity(const sf::Texture& texture_, std::string  nume, int tip_, int x_, int y_) : tip{ tip_ }, ent_nume{std::move( nume )}, ent_txtr { texture_ }, poz_x {x_}, poz_y {y_} {}
    Entity(const Entity& other) : tip{ other.tip }, ent_nume{ other.ent_nume }, ent_txtr{ other.ent_txtr }
    {
        if(other.entef != nullptr){
            entef = std::make_shared<Effect>(*(other.entef));
        }
    }
    Entity& operator=(const Entity& other);
    ~Entity() = default;
    friend std::ostream& operator<<(std::ostream& os, const Entity& ent);
    int getTip() const { return tip; }
    sf::Texture& getTexture(){
        return ent_txtr;
    }

    void setPoz(double x, double y);

    void setPodea(int floor_);

    void sterge();

    int status();


    void setEffect(std::shared_ptr<Effect> newef);

    std::shared_ptr<Effect> getEffect();
    sf::Sprite getSprite(const std::string& caz="render");

};