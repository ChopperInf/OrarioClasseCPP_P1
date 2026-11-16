#include "TesseraAula.h"
TesseraAula::TesseraAula(std::string materia, std::string classe, std::string aula)
    : Tessera(materia, classe, aula) {}
std::string TesseraAula::GetDescrizione() {
    return "AULA - " + Tessera::GetDescrizione();
}
