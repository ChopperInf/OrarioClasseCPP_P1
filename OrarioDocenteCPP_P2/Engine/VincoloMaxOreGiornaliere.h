#pragma once
#include "Vincolo.h"

class VincoloMaxOreGiornaliere : public Vincolo {
private:
    int massimoOre;
public:
    explicit VincoloMaxOreGiornaliere(int massimoOre);
    bool Verifica(Orario& orario) override;
};
