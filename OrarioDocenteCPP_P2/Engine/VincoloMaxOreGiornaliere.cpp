#include "VincoloMaxOreGiornaliere.h"

VincoloMaxOreGiornaliere::VincoloMaxOreGiornaliere(int massimoOre)
    : massimoOre(massimoOre) {}

bool VincoloMaxOreGiornaliere::Verifica(Orario& orario) {
    if (massimoOre < 0 || massimoOre > 8) return false;

    for (int giorno = 0; giorno < 5; giorno++) {
        int numeroOre = 0;
        for (int ora = 0; ora < 8; ora++) {
            if (orario.Get(Posizione(giorno, ora)) != nullptr)
                numeroOre++;
        }
        if (numeroOre > massimoOre) return false;
    }
    return true;
}
