#include "GameExceptions.h"

void outofbounds(double x) {
    if(x < -300){
        throw eroare_entitate("entitatea/caracterul nu este unde ar trebui\n");
    }
}

void loadtxtr(bool ret) {
    if(!ret){
        throw eroare_resursa("nu s-a incarcat textura");
    }
}

void loadfont(bool ret) {
    if(!ret){
        throw eroare_resursa("nu s-a incarcat fontul");
    }
}
