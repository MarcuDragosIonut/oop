#include "Noncharacters.h"
#include <iostream>
#include <SFML/Graphics.hpp>
#include <memory>

int Effect::getJ() {
    return jumpmod;
}

int Effect::getM() {
    return movmod;
}

const std::string &Effect::getName() {
    return nume;
}

Entity &Entity::operator=(const Entity &other) {
    tip = other.tip;
    ent_nume = other.ent_nume;
    ent_txtr = other.ent_txtr;
    entef = std::make_shared<Effect>(*(other.entef));
    return *this;
}

std::ostream &operator<<(std::ostream &os, const Entity &ent) {
    os << " Nume " << ent.ent_nume << " Tip " << ent.tip << '\n';
    return os;
}

void Entity::setPoz(double x, double y) {
    poz_x = x;
    poz_y = y;
}

void Entity::setPodea(int floor_) {
    floor = floor_;
}

void Entity::sterge() {
    sters = 1;
}

int Entity::status() {
    return sters;
}

void Entity::setEffect(std::shared_ptr<Effect> newef) {
    entef = newef;
}

std::shared_ptr<Effect> Entity::getEffect() {
    return entef;
}

sf::Sprite Entity::getSprite(const std::string &caz) {
    sf::Sprite itemsprite;
    itemsprite.setTexture(ent_txtr);
    if(floor == 0 && caz=="render") poz_y += 2;
    itemsprite.setPosition(poz_x, poz_y);
    return itemsprite;
}
