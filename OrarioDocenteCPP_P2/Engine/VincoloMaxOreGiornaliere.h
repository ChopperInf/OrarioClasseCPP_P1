#pragma once
#include "Vincolo.h"

class VincoloMaxOreGiornaliere : public Vincolo {
private:
    int massimoOre;
public:
    VincoloMaxOreGiornaliere(int massimoOre);
    bool Verifica(Orario& orario) override;
    std::string GetMessaggio() override { return "superato il numero massimo di ore giornaliere"; }
};
