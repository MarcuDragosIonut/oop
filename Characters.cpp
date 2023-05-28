#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <set>
#include "Characters.h"
#include "GameExceptions.h"
#include "Noncharacters.h"
#include "GameHandler.h"


double Character::getX() const {
    try{
        outofbounds(poz_x);
    }
    catch(eroare_entitate& err){
        std::cout << err.what();
    }
    return poz_x;
}

double Character::getY() const {
    return poz_y;
}

int Character::getMS() const {
    return ms;
}

int Character::getJP() const {
    return jp;
}

int Character::getStatus() const {
    return mort;
}

void Character::Kill() {
    mort = 1;
    char_txtr = char_dead_txtr;
}

int Character::getGravity() {
    return gravity;
}

void Character::setCollisions(int floor_, int up_, int left_, int right_) {
    floor = floor_;
    up = up_;
    left = left_;
    right = right_;
}

sf::Sprite Character::getSprite(const std::string &caz) {
    if(caz=="render"){
        setMovement();
    }
    sf::Sprite sp;
    sp.setTexture(char_txtr);
    sp.setPosition(poz_x, poz_y);
    return sp;
}



void Player::AddEff(Entity &e) {
    if (e.getTip() == 1)
    {
        sf::Clock timpefect;
        if(e.getEffect() == nullptr) std::cout << "efect null\n";
        plref.push_back({*(e.getEffect()), timpefect});
    }
}

void Player::IncreaseScore(int added_score) {
    score += added_score;
}

int Player::getScore() {
    return score;
}

int Player::interaction() const {
    return interact;
}

int Player::getJumpCD() const {
    return jumpcd;
}

void Player::setJumpCD(int jumpcd_) {
    jumpcd = jumpcd_;
}

int Player::getmsmod() {
    int msmod = 0;
    std::set<std::string> efecte_aplicate;
    for(auto& it:plref){
        if(it.second.getElapsedTime() < sf::seconds(30)){
            if(!efecte_aplicate.contains(it.first.getName())){
                msmod += it.first.getM();
                efecte_aplicate.insert(it.first.getName());
            }
        }
    }
    return msmod;
}

int Player::getjpmod() {
    int jpmod = 0;
    std::set<std::string> efecte_aplicate;
    for(auto& it:plref){
        if(it.second.getElapsedTime() < sf::seconds(30)){
            if(!efecte_aplicate.contains(it.first.getName())) {
                jpmod += it.first.getJ();
                efecte_aplicate.insert(it.first.getName());
            }
        }
    }
    return jpmod;
}

int Player::Command(const std::string &c) {
    int miscare = 0;
    if(mort == 0) {
        if(c=="jump" && (floor>0 || (jump < 3 && !floor && !doublejump)))
        {
            if(floor == 0 && up == 0) doublejump = 1;
            jump += 25;
            miscare = 1;
        }
        int msmod = getmsmod();
        if (c == "a") {
            double oldpoz = poz_x;
            poz_x -= ms + msmod - left;
            if (poz_x != oldpoz)miscare = 1;
        }
        if (c == "d") {
            double oldpoz = poz_x;
            poz_x += ms + msmod - right;
            if (poz_x != oldpoz)miscare = 1;
        }
        if (c == "e") interact = 1;
        if (c == "e_release") interact = 0;
    }
    return miscare;
}

void Player::setMovement() {
    int jpmod = getjpmod();
    if(mort) jump = 0;
    if (floor < gravity && jump == 0) poz_y += gravity-floor ;
    if(!mort) {
        if (jump > 5) {
            poz_y -= jp + jpmod - up;
        }
        if (up != 0 && jump > 5) jump = 4; // daca se da cu capu' sa cada
        if (jump > 0) jump--;
        if (floor != 0) doublejump = 0;
    }
}

int Npc::getScoreValue() {
    return score_value;
}

void Npc::setOrder(const std::string &ord) {
    if(ord=="patrol") dir = 1;
    order = ord;
}

void Npc::setPosition(double x, double y) {
    poz_x = x;
    poz_y = y;
}

void Orange::setMovement() {
    if(floor < gravity) poz_y += gravity - floor;
    if(!mort) {
        if (order == "patrol") {
            if (dir == 1) {
                if (right > 0) dir = -1;
                if (floor > 0)poz_x += ms - right;
            }
            if (dir == -1) {
                if (left > 0) dir = 1;
                if (floor > 0)poz_x -= ms - left;
            }
        }
    }
}

void Verde::setMovement() {
    if(mort == 1 && floor < gravity) poz_y += gravity - floor;
    if(mort == 0) {
        if (order == "patrol") {
            if (dir == 1) {
                if (right > 0) dir = -1;
                poz_x += ms - right;
            }
            if (dir == -1) {
                if (left > 0) dir = 1;
                poz_x -= ms - left;
            }
        }
        if(order=="fly"){
            if(dir == 1){
                if(!up)poz_y -= jp;
                if(poz_y < -100 || up > 0) dir = -1;
            }
            else {
                poz_y += gravity - floor;
                if(floor > 0 || poz_y > 350) dir = 1;
            }
        }
    }
}

Verde::Verde(const sf::Texture &t, const sf::Texture &tdead, double x, double y) : Npc(t, tdead, x, y){
    ms = 5;
    dir = -1;
    score_value = ScoreCalculator<Verde>::CalculateScore(*this);
}

void Mov::setMovement() {
    if(floor < gravity && !(order=="jump" && dir == 1 && mort == 0)) poz_y += gravity - floor;
    if(mort == 0) {
        if (order == "patrol") {
            if (dir == 1) {
                if (right > 0) dir = -1;
                poz_x += ms - right;
            }
            if (dir == -1) {
                if (left > 0) dir = 1;
                poz_x -= ms - left;
            }
        }
        if(order=="jump"){
            if(dir == 1){
                if(airtime == 10 || up) dir = -1;
                if(!up) {
                    airtime++;
                    poz_y -= jp;
                }
            }
            else {
                if(floor) {
                    dir = 1;
                    airtime = 0;
                }
            }
        }
    }
}

sf::Sprite Mov::getSprite(const std::string &caz) {
    sf::Sprite sp;
    sp.setTexture(char_txtr);
    if(caz=="render"){
        setMovement();
    }
    if(movc.getElapsedTime().asSeconds() > sf::seconds(10).asSeconds()){
        transparent = transparent ^ 1;
        movc.restart();
    }
    if(transparent)
    {
        sf::Color spcolor = sp.getColor();
        spcolor.a = 100;
        sp.setColor(spcolor);
    }
    sp.setPosition(poz_x, poz_y);
    return sp;
}

Mov::Mov(const sf::Texture &t, const sf::Texture &tdead, double x, double y) : Npc(t, tdead, x, y){
    jp = 3;
    score_value = ScoreCalculator<Mov>::CalculateScore(*this);
}

void Fantoma::setMovement() {
    if(mort == 1 && floor < gravity) poz_y += gravity - floor;
    if(mort == 0) {
        if (order == "patrol") {
            if (dir == 1) {
                poz_x += ms - right;
                poz_y -= jp - up;
                movlen++;
            }
            if (dir == -1) {
                poz_x -= ms - left;
                poz_y += jp - floor;
                movlen++;
            }
            if(movlen == 35){
                dir *= -1;
                movlen = 0;
            }
        }
    }
}

Fantoma::Fantoma(const sf::Texture &t, const sf::Texture &tmort, double x, double y) : Npc(t, tmort, x, y){
    dir = 1;
    jp = 3;
    ms = 3;
   score_value = ScoreCalculator<Fantoma>::CalculateScore(*this);
}

void CharacterTexturesHolder::addTexture(std::string nume, const sf::Texture &textura) {
    CharacterTextures[nume] = textura;
}

sf::Texture CharacterTexturesHolder::getCharTexture(const std::string& charname){
    return CharacterTextures[charname];
}
