#pragma once

#include "Noncharacters.h"
#include "Gamemap.h"
#include "Characters.h"

class GameState{
    inline static int state = 0;
public:
    GameState(const GameState& other) = delete;
    GameState& operator=(const GameState& other) = delete;
    static void RunGame(sf::RenderWindow &window, sf::Event &event, sf::RectangleShape& bg,
                        Player &p, sf::Text &score,
                        Harta& h, std::vector<Entity*> &itemvect, std::map<std::string, sf::Texture>& Textures) ;

    static void ViewAndMovement(sf::RenderWindow &window, Player &p, sf::Text &score);

    static void EventHandling1(sf::RenderWindow &window, const sf::Event &event, Player &p);

};


template <typename T>
class ScoreCalculator{
public:
    static int CalculateScore(const T& target){
        return 1*target.getMS()+1*target.getJP();
    }
};
