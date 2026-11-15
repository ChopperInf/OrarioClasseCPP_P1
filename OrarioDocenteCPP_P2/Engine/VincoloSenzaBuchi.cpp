#include "VincoloSenzaBuchi.h"

bool VincoloSenzaBuchi::Verifica(Orario& orario) {
    for (int giorno = 0; giorno < 5; giorno++) {
        bool lezioneIniziata = false;
        bool bucoTrovato = false;

        for (int ora = 0; ora < 8; ora++) {
            if (orario.Get(Posizione(giorno, ora)) != nullptr) {
                // Una lezione dopo un'ora vuota interna crea un buco.
                if (bucoTrovato) return false;
                lezioneIniziata = true;
            }
            else if (lezioneIniziata) {
                // Il vuoto e' un buco solo se seguono altre lezioni.
                bucoTrovato = true;
            }
        }
    }
    return true;
}
