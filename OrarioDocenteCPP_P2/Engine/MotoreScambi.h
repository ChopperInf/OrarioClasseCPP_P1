#pragma once
#include <vector>
#include <string>
#include "../Model/Orario.h"
#include "Vincolo.h"

class MotoreScambi {
private:
    // I puntatori non sono proprietari: i vincoli vivono altrove.
    std::vector<Vincolo*> vincoli;
    bool VerificaVincoli(Orario& orario, std::string& messaggio);
public:
    void AggiungiVincolo(Vincolo* vincolo);
    bool Scambia(Orario& orario, Posizione prima, Posizione seconda, std::string& messaggio);
};
