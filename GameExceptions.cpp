#include <memory>
#include "GameExceptions.h"
#include "Noncharacters.h"

void outofbounds(double x) {
    if(x < -300){
        throw eroare_entitate("entitatea/caracterul nu este unde ar trebui\n");
    }
}

void pointerentitate(std::shared_ptr<Effect> ent) {
    if(ent == nullptr) {
        throw eroare_entitate("pointerul de entitate este nullptr");
    }
}

void loadtxtr(bool ret) {
    if(!ret){
        throw eroare_textura("nu s-a incarcat textura");
    }
}

void loadfont(bool ret) {
    if(!ret){
        throw eroare_textura("nu s-a incarcat fontul");
    }
}
