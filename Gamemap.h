#include <SFML/Graphics.hpp>

class Harta{
    int lvlwon = 0;
    std::vector< std::pair< Entity, std::pair<double,double> > > Obj;
    std::map<std::string, sf::Texture > Textures;
public:
    friend std::ostream& operator<<(std::ostream& os, const Harta& h) {
        os << " Nr obiecte: " << h.Obj.size() << '\n';
        return os;
    }

    void addObj(const Entity& obj, std::pair<double,double> p){
        Obj.emplace_back(obj,p);
    }

    void addTexture(const std::string& str, const sf::Texture& txtr){
        Textures[str]=txtr;
    }

    void drawMap(sf::RenderWindow& window_, Player& plr, std::vector<Npc*> npcvect, std::vector<Entity*> itemvect){
        if(lvlwon == 1){
            sf::Sprite spfinmesaj;
            spfinmesaj.setTexture(Textures["finmesaj"]);
            spfinmesaj.setPosition(window_.getView().getCenter().x - Textures["finmesaj"].getSize().x, window_.getView().getCenter().y - Textures["finmesaj"].getSize().y);
            window_.draw(spfinmesaj);
        }
        sf::Sprite ps = plr.getSprite("collision");

        // items
        for(auto item=itemvect.begin(); item!=itemvect.end();item++){
            if((*item)->status() == 0) {
                sf::Sprite spitem = (*item)->getSprite();
                if (ps.getGlobalBounds().intersects(spitem.getGlobalBounds())) {
                    sf::Sprite spbuton;
                    spbuton.setTexture(Textures["e"]);
                    spbuton.setPosition(spitem.getPosition().x, spitem.getPosition().y - Textures["e"].getSize().y - 5);
                    window_.draw(spbuton);
                    if (plr.interaction()) {
                        Entity itemaux = **item;
                        plr.AddEff(itemaux);
                        plr.Command("e_release");
                        (*item)->sterge();
                    }
                }
            }
        }
        // items

        ps.setPosition(ps.getPosition().x, ps.getPosition().y+2);
        sf::FloatRect pb_low = ps.getGlobalBounds();
        ps.setPosition(ps.getPosition().x, ps.getPosition().y-2-plr.getJP()-plr.getjpmod());
        sf::FloatRect pb_high = ps.getGlobalBounds();
        ps.setPosition(ps.getPosition().x-plr.getMS()-plr.getmsmod(), ps.getPosition().y+plr.getJP()+plr.getjpmod());
        sf::FloatRect pb_left = ps.getGlobalBounds();
        ps.setPosition(ps.getPosition().x+2*plr.getMS()+2*plr.getmsmod(), ps.getPosition().y);
        sf::FloatRect pb_right = ps.getGlobalBounds();

        int podea_ = 0, sus_ = 0, dreapta_ = 0, stanga_ = 0;
        std::vector < std::vector<int> > npccollisions(npcvect.size(), {0, 0, 0, 0});
        std::vector < int > itemcollisions(itemvect.size(), 0);
        int xi = 0;
        for(auto& it:Obj){
            xi++;
            sf::Sprite sp;
            sp.setTexture(it.first.getTexture());
            sp.setPosition(it.second.first, it.second.second); //second.first = x second.second = y

            //player
            if (it.second.second >= plr.getlowHeight()) {
                if (sp.getGlobalBounds().intersects(pb_low)){
                    if(it.first.getTip() == 3) lvlwon = 1;
                    podea_ = 1;
                }
            }
            if (sp.getGlobalBounds().intersects(pb_high)) {
                if(sus_ == 0) sus_ =  plr.getJP()+plr.getjpmod() - (plr.getY()- (sp.getPosition().y + sp.getTexture()->getSize().y));
            }
            if (sp.getGlobalBounds().intersects(pb_right))
            {
                if(it.first.getTip() == 3) lvlwon = 1;
                if(dreapta_ == 0)dreapta_ = plr.getMS()+plr.getmsmod() - (sp.getPosition().x-plr.getX()-pb_right.getSize().x);
            }
            if (sp.getGlobalBounds().intersects(pb_left))
            {
                if(it.first.getTip() == 3) lvlwon = 1;
                if(stanga_ == 0 ) stanga_ = plr.getMS()+plr.getmsmod() - (plr.getX()-sp.getPosition().x-sp.getTexture()->getSize().x);
            }
            //player

            //items
            for(unsigned int itemindex = 0; itemindex < itemvect.size(); itemindex++){
                sf::Sprite itemsp = itemvect[itemindex]->getSprite("collision");
                itemsp.setPosition(itemsp.getPosition().x, itemsp.getPosition().y + 2);
                if(itemsp.getGlobalBounds().intersects(sp.getGlobalBounds())) itemcollisions[itemindex] = 1;
            }

            for(unsigned int itemindex = 0; itemindex < itemvect.size(); itemindex++){
                itemvect[itemindex]->setPodea(itemcollisions[itemindex]);
            }
            //items

            //npcs
            for(unsigned int npcindex = 0; npcindex < npcvect.size(); npcindex++) {
                sf::Sprite npcs = npcvect[npcindex]->getSprite("collision");
                //if(xi == 1)std::cout << npcs.getPosition().x << ' ';
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
            //if(xi == 1)std::cout << '\n';
            for(unsigned int npcindex = 0; npcindex < npcvect.size(); npcindex++){
                npcvect[npcindex]->setCollisions(npccollisions[npcindex][0], npccollisions[npcindex][1], npccollisions[npcindex][2], npccollisions[npcindex][3]);
            }
            //npcs

            window_.draw(sp);
        }
        plr.setCollisions(podea_, sus_, stanga_, dreapta_);
    }
};