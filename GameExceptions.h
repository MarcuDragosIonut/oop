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


class eroare_resursa : public eroare_joc{
public:
    explicit eroare_resursa(const std::string& m) : eroare_joc(m){};
};

void outofbounds(double x);
void loadtxtr(bool ret);
void loadfont(bool ret);
