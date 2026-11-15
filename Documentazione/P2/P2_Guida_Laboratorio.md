# P2 — Laboratorio: rendere estensibili i controlli dell'orario di classe

**Guida operativa per gli studenti — quarta Informatica**  
**Ambiente:** Visual Studio, C++17, applicazione console  
**Punto di partenza:** il vostro progetto P1 con `Orario`, `Tessera`, `Posizione`, `MotoreScambi` ed `ElencoAule`.  
**Esempio del docente:** [OrarioDocenteCPP_P2](../../OrarioDocenteCPP_P2/).

## Obiettivo

Vogliamo impedire che uno scambio produca un orario di classe non valido. Per farlo useremo **più vincoli**, senza scrivere nel motore una chiamata diversa per ogni tipo di controllo.

Il docente controlla i buchi e il numero massimo di lezioni complessive in un giorno. **Voi controllerete i buchi e il numero massimo di lezioni della stessa materia in un giorno.** Il problema è simile, ma il secondo algoritmo è differente.

## 1. Preparate una nuova versione

1. Sincronizzate il repository con Git, dopo aver salvato le modifiche locali.
2. Conservate la versione P1 senza modificarla.
3. Create una nuova cartella per la versione P2 del vostro orario di classe, copiando i sorgenti della P1.
4. In Visual Studio aprite la nuova soluzione e aggiornate i nomi dei file `.sln` e `.vcxproj`, se necessario.
5. Compilate ed eseguite il programma **prima di introdurre nuovi vincoli**.

**Verifica A:** la P1 e la P2 si aprono come soluzioni indipendenti. Il menu e l'archivio `ElencoAule` continuano a funzionare.

## 2. Individuate il limite del codice P1

Aprite `MotoreScambi.h` e `MotoreScambi.cpp`. Cercate dove viene chiamato il controllo dei buchi.

**Attività:** rispondete sul quaderno:

- Se aggiungessimo un controllo sulle ore della stessa materia, quali metodi del motore dovremmo cambiare?
- Come potremmo fare in modo che il motore conosca soltanto un metodo chiamato `Verifica()`?

Disegnate un semplice schema delle relazioni tra `MotoreScambi`, `Orario` e il validatore attuale.

## 3. Create la classe astratta Vincolo

Nella cartella `Engine` aggiungete `Vincolo.h`.

```cpp
#pragma once
#include "../Model/Orario.h"

class Vincolo {
public:
    virtual bool Verifica(Orario& orario) = 0;
    virtual ~Vincolo() {}
};
```

**Attività:** spiegate con parole vostre il significato di `virtual`, `= 0` e del distruttore virtuale.

**Verifica B:** provate a dichiarare temporaneamente `Vincolo prova;`. Il compilatore deve segnalare un errore: perché? Eliminate la dichiarazione prima di continuare.

## 4. Realizzate VincoloSenzaBuchi

Create `VincoloSenzaBuchi.h` e `VincoloSenzaBuchi.cpp`.

Nell'header completate:

```cpp
#pragma once
#include "Vincolo.h"

class VincoloSenzaBuchi : public Vincolo {
public:
    bool Verifica(Orario& orario) override;
};
```

**Attività:** trasferite nel nuovo metodo la logica che nella P1 controllava i buchi. Un giorno è valido quando **non ci sono celle vuote tra la prima e l'ultima lezione**. Le celle vuote prima della prima lezione o dopo l'ultima non sono buchi.

Per ciascun giorno esaminate le otto ore con `orario.Get(Posizione(giorno, ora))`.

**Verifica C:** costruite un giorno con lezioni alle ore 1, 2 e 3: il risultato deve essere `true`. Con lezioni alle ore 1 e 3 ma non alla 2, deve essere `false`.

## 5. Progettate VincoloMaxOreMateria

Create `VincoloMaxOreMateria.h` e `VincoloMaxOreMateria.cpp`.

Il vincolo deve verificare che **nessuna materia superi un numero massimo di lezioni nello stesso giorno**.

**Esempio:** con `massimoOre = 2`, una giornata con Informatica, Matematica, Informatica è valida rispetto a questo vincolo; una giornata con Informatica, Informatica, Informatica non lo è.

Progettate la classe con:

- un attributo privato `int massimoOre`;
- un costruttore che riceve il limite;
- `bool Verifica(Orario& orario) override;`.

**Indicazione per l'algoritmo:** per ogni giorno, prendete in esame le tessere presenti. Per ciascuna materia incontrata, contate quante tessere dello stesso giorno hanno quella materia. Se il conteggio supera il limite, restituite `false`. Al termine restituite `true`.

Potete usare due cicli sulle ore dello stesso giorno e confrontare le stringhe ottenute da `GetMateria()`. Non servono `map`, `set` o algoritmi STL non ancora studiati. Ignorate le celle vuote prima di accedere ai metodi di `Tessera`.

**Verifica D:** preparate tre casi con massimo 2: due lezioni della stessa materia (valido), tre lezioni della stessa materia (non valido), tre lezioni di materie tutte diverse (valido).

## 6. Modificate MotoreScambi

Nel file `MotoreScambi.h` sostituite l'attributo che contiene il vecchio validatore con:

```cpp
#include <vector>
#include "Vincolo.h"

// Nella classe:
std::vector<Vincolo*> vincoli;
```

Aggiungete le dichiarazioni:

```cpp
void AggiungiVincolo(Vincolo* vincolo);
bool VerificaVincoli(Orario& orario);
```

`VerificaVincoli` può essere un metodo privato.

**Attività:** implementate `AggiungiVincolo` in modo da non registrare `nullptr` e da evitare di inserire due volte lo stesso puntatore.

In `VerificaVincoli`:

1. Se il vettore è vuoto, restituite `false`.
2. Scorrete il vettore con un ciclo.
3. Per ogni elemento chiamate `vincoli[i]->Verifica(orario)`.
4. Se una verifica fallisce, restituite subito `false`; altrimenti, dopo il ciclo, `true`.

**Domanda:** perché il motore non deve sapere se il puntatore indica un `VincoloSenzaBuchi` o un `VincoloMaxOreMateria`?

## 7. Conservate il ripristino dello scambio

Modificate `Scambia()` affinché:

1. rifiuti posizioni non valide o uguali;
2. controlli tutti i vincoli sull'orario iniziale;
3. scambi le due celle;
4. ricontrolli tutti i vincoli;
5. se almeno un controllo fallisce, **ripristini le celle** e restituisca `false`.

**Verifica E:** dopo uno scambio rifiutato, stampate l'orario e confrontatelo con quello precedente. Nessuna cella deve essere cambiata.

## 8. Registrate i vincoli nell'interfaccia

La console può contenere i due oggetti concreti e il motore:

```cpp
VincoloSenzaBuchi vincoloSenzaBuchi;
VincoloMaxOreMateria vincoloMaxOreMateria;
MotoreScambi motore;
```

Nel costruttore inizializzate `vincoloMaxOreMateria` con un limite scelto per i test (per esempio 2), poi registrate gli indirizzi dei vincoli:

```cpp
motore.AggiungiVincolo(&vincoloSenzaBuchi);
motore.AggiungiVincolo(&vincoloMaxOreMateria);
```

**Attenzione:** il motore conserva puntatori **non proprietari**. Non deve eseguire `delete` sui vincoli. Gli oggetti registrati devono esistere per tutto il tempo in cui vengono utilizzati.

Aggiornate l'opzione del menu che verifica l'orario per mostrare l'esito di entrambi i controlli.

**Verifica F:** il programma deve continuare a visualizzare l'orario e a gestire l'archivio delle aule della P1.

## 9. Piano di collaudo

| ID | Prova | Risultato atteso |
|---|---|---|
| T01 | Compilazione P2 | Nessun errore |
| T02 | Esecuzione P1 conservata | Funziona ancora |
| T03 | Giorno senza buchi | `VincoloSenzaBuchi` restituisce `true` |
| T04 | Giorno con un buco interno | `VincoloSenzaBuchi` restituisce `false` |
| T05 | Massimo 2, due ore di Informatica | `VincoloMaxOreMateria` restituisce `true` |
| T06 | Massimo 2, tre ore di Informatica | `VincoloMaxOreMateria` restituisce `false` |
| T07 | Scambio che rispetta entrambi i vincoli | Accettato |
| T08 | Scambio che crea un buco | Rifiutato, orario invariato |
| T09 | Scambio che supera il massimo per materia | Rifiutato, orario invariato |
| T10 | Motore senza vincoli | Scambio rifiutato |
| T11 | Aggiunta di un nuovo vincolo concreto | Nessuna modifica all'algoritmo di `Scambia()` |
| T12 | Menu e `ElencoAule` | Funzioni P1 ancora operative |

Per T08 e T09 costruite **orari iniziali validi**. Se necessario preparate dati di prova separati: un test non è significativo se l'orario di partenza viola già un vincolo.

### Scheda risultati

| Test | Superato (S/N) | Osservazioni |
|---|---|---|
| T01–T04 | | |
| T05–T08 | | |
| T09–T12 | | |

## 10. Cosa consegnare

- La soluzione Visual Studio P2 con tutti i file `.h` e `.cpp`.
- Un diagramma UML con `Vincolo`, le due classi derivate e `MotoreScambi`.
- La tabella dei test compilata, con almeno un esempio di scambio accettato e uno rifiutato.
- Una breve spiegazione: **come funziona il polimorfismo nel ciclo di verifica e chi possiede i vincoli?**

Non è richiesto introdurre `unique_ptr`, `map`, `set`, template personalizzati o un'interfaccia grafica.

**Riferimenti:** [dispensa teorica P2](P2_Dispensa_Teorica.md) · [scheda didattica P2](P2_Scheda_Didattica.md).
