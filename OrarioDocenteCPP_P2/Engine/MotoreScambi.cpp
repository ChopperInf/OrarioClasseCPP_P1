#include "MotoreScambi.h"

void MotoreScambi::AggiungiVincolo(Vincolo* vincolo) {
    if (vincolo == nullptr) return;
    for (int i = 0; i < static_cast<int>(vincoli.size()); i++)
        if (vincoli[i] == vincolo) return;
    vincoli.push_back(vincolo);
}

bool MotoreScambi::VerificaVincoli(Orario& orario) {
    // Senza vincoli il motore non autorizza alcuno scambio.
    if (vincoli.empty()) return false;
    for (int i = 0; i < static_cast<int>(vincoli.size()); i++)
        if (!vincoli[i]->Verifica(orario)) return false;
    return true;
}

bool MotoreScambi::Scambia(Orario& orario, Posizione prima, Posizione seconda) {
    if (prima.GetGiorno() < 0 || prima.GetGiorno() >= 5 ||
        prima.GetOra() < 0 || prima.GetOra() >= 8 ||
        seconda.GetGiorno() < 0 || seconda.GetGiorno() >= 5 ||
        seconda.GetOra() < 0 || seconda.GetOra() >= 8) return false;

    if (prima.GetGiorno() == seconda.GetGiorno() &&
        prima.GetOra() == seconda.GetOra()) return false;

    if (!VerificaVincoli(orario)) return false;

    orario.ScambiaCelle(prima, seconda);
    if (VerificaVincoli(orario)) return true;

    // Ripristina le celle se almeno un vincolo non e' rispettato.
    orario.ScambiaCelle(prima, seconda);
    return false;
}
