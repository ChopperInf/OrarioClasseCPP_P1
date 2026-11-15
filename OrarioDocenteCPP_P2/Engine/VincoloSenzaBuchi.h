#pragma once
#include "Vincolo.h"

class VincoloSenzaBuchi : public Vincolo {
public:
    bool Verifica(Orario& orario) override;
};
