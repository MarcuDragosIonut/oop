#pragma once
#include <stdexcept>
#include "Noncharacters.h"

class eroare_joc : public std::runtime_error{
    using std::runtime_error::runtime_error;
};

class eroare_entitate : public eroare_joc{
public:
    explicit eroare_entitate(const std::string& m ) : eroare_joc(m){};
};

void pointerentitate(std::shared_ptr<Effect> ent);

void outofbounds(double x);

class eroare_textura : public eroare_joc{
public:
    explicit eroare_textura(const std::string& m) : eroare_joc(m){};
};

void loadtxtr(bool ret);
void loadfont(bool ret);
