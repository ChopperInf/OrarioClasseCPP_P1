#pragma once
#include "Tessera.h"
class TesseraAula : public Tessera {
public:
    TesseraAula(std::string materia, std::string classe, std::string aula);
    std::string GetDescrizione() override;
};
