#include "Model/Orario.h"
#include "DatiEsempio.h"
#include "InterfacciaConsole.h"
int main() {
    Orario orario;
    if (!DatiEsempio::Carica(orario)) return 1;
    InterfacciaConsole interfaccia(orario);
    interfaccia.Esegui();
    return 0;
}
