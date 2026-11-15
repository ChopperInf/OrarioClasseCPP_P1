#pragma once
#include "Vincolo.h"

class VincoloSenzaBuchi : public Vincolo {
public:
    bool Verifica(Orario& orario) override;
    std::string GetMessaggio() override { return "sono presenti buchi nell\u0027orario"; }
};
