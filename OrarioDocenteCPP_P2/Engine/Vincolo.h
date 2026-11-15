#pragma once
#include "../Model/Orario.h"
#include <string>

// Interfaccia comune a tutti i controlli dell'orario.
class Vincolo {
public:
    virtual bool Verifica(Orario& orario) = 0;
    virtual std::string GetMessaggio() = 0;
    virtual ~Vincolo() {}
};
