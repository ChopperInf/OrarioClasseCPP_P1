#include "InterfacciaConsole.h"
#include <iostream>
#include <iomanip>
#include <limits>
using namespace std;
InterfacciaConsole::InterfacciaConsole(Orario& orario) : orario(orario) {}
int InterfacciaConsole::LeggiIntero(string messaggio, int minimo, int massimo) {
    int valore;
    while (true) {
        cout << messaggio;
        if (cin >> valore && valore >= minimo && valore <= massimo) return valore;
        cout << "Valore non valido. Inserisci un numero tra " << minimo
             << " e " << massimo << ".\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}
Posizione InterfacciaConsole::LeggiPosizione() {
    int g = LeggiIntero("Giorno (1=Lun, 2=Mar, 3=Mer, 4=Gio, 5=Ven): ", 1, 5);
    int o = LeggiIntero("Ora (1-8): ", 1, 8);
    return Posizione(g - 1, o - 1);
}
void InterfacciaConsole::VisualizzaOrario() {
    string giorni[5] = {"Lun", "Mar", "Mer", "Gio", "Ven"};
    cout << "\nORARIO SETTIMANALE DELLA CLASSE\n" << left << setw(6) << "Ora";
    for (int g = 0; g < 5; g++) cout << setw(18) << giorni[g];
    cout << "\n";
    for (int o = 0; o < 8; o++) {
        cout << left << setw(6) << o + 1;
        for (int g = 0; g < 5; g++) {
            Tessera* t = orario.Get(Posizione(g, o));
            cout << setw(18) << (t == nullptr ? "---" : t->GetMateria());
        }
        cout << "\n";
    }
    cout << "\nPer visualizzare docente e aula, scegli l'opzione 2.\n";
}
void InterfacciaConsole::VisualizzaTessera() {
    cout << "\nCONSULTA UNA CELLA\n";
    Tessera* t = orario.Get(LeggiPosizione());
    if (t == nullptr) cout << "La cella e' vuota.\n";
    else {
        cout << "Materia: " << t->GetMateria() << "\n";
        cout << "Docente: " << t->GetDocente() << "\n";
        cout << "Aula: " << t->GetAula() << "\n";
    }
}
void InterfacciaConsole::RichiediScambio() {
    cout << "\nSCAMBIO DI DUE CELLE\nPrima posizione:\n";
    Posizione a = LeggiPosizione();
    cout << "Seconda posizione:\n";
    Posizione b = LeggiPosizione();
    if (a.GetGiorno() == b.GetGiorno() && a.GetOra() == b.GetOra()) {
        cout << "Hai selezionato due volte la stessa cella.\n";
        return;
    }
    if (motore.Scambia(orario, a, b)) cout << "Scambio autorizzato.\n";
    else cout << "Scambio rifiutato: si creerebbe un buco o l'orario non e' valido.\n";
}
void InterfacciaConsole::VerificaOrario() {
    cout << "\nVERIFICA DELL'ORARIO\n";
    if (validatore.SenzaBuchi(orario)) cout << "Orario valido: non ci sono buchi.\n";
    else cout << "Orario non valido: ci sono ore vuote prima di altre lezioni.\n";
}
void InterfacciaConsole::MostraMenu() {
    cout << "\n================================\n       ORARIO CLASSE\n"
         << "================================\n"
         << "1 - Visualizza orario\n2 - Visualizza una tessera\n"
         << "3 - Scambia due tessere\n4 - Verifica validita' dell'orario\n0 - Esci\n";
}
void InterfacciaConsole::Esegui() {
    int scelta;
    do {
        MostraMenu();
        scelta = LeggiIntero("Scelta: ", 0, 4);
        switch (scelta) {
        case 1: VisualizzaOrario(); break;
        case 2: VisualizzaTessera(); break;
        case 3: RichiediScambio(); break;
        case 4: VerificaOrario(); break;
        case 0: cout << "Chiusura del programma.\n"; break;
        }
    } while (scelta != 0);
}
