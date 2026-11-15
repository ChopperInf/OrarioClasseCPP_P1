# P2 — Scambi controllati e vincoli polimorfici

**Destinatari:** classe quarta Informatica  
**Prototipo docente di partenza:** `OrarioDocenteCPP_P1`  
**Prototipo docente da realizzare:** `OrarioDocenteCPP_P2`  
**Percorso studenti:** evoluzione dell'orario della classe, con vincoli analoghi ma diversi  
**Documento guida:** `Documentazione/Piano_Didattico_Orario_CPP.md` (P2)  
**Stato:** scheda di progettazione; implementazione da svolgere.

## 1. Obiettivo e problema concreto

Nel prototipo P1, `MotoreScambi` contiene un `ValidatoreOrario` e richiama direttamente `SenzaBuchi()`. Questo funziona, ma rende difficile aggiungere un secondo controllo: il motore dovrebbe conoscere il nome e il funzionamento di ogni nuova verifica.

**Problema da risolvere:** permettere al motore di verificare **più vincoli**, senza dover cambiare l'algoritmo di scambio quando viene aggiunta una nuova classe di vincolo.

**Risultato atteso:** il motore prova lo scambio, esegue tutti i controlli e, se almeno uno fallisce, ripristina esattamente le celle iniziali.

## 2. Nuovi concetti informatici

- **Ereditarietà:** classi specifiche derivate da una classe generale.
- **Classe astratta:** una classe usata come interfaccia comune, dalla quale non si creano direttamente oggetti.
- **Metodo virtuale puro:** `virtual bool Verifica(Orario& orario) = 0;`.
- **Ridefinizione:** `override` nei metodi delle classi derivate.
- **Polimorfismo:** chiamare `Verifica()` tramite un puntatore `Vincolo*` e ottenere il comportamento della classe concreta.
- **`std::vector<Vincolo*>`:** raccolta di puntatori a vincoli diversi, **non proprietari**.
- **Invarianti e ripristino:** una modifica rifiutata non cambia l'orario.

**Non introdurre in P2:** `unique_ptr`, template personalizzati, eccezioni come meccanismo ordinario di validazione, vincoli tra più docenti/classi, pila di annullamento, interfaccia grafica.

## 3. Competenze osservabili

Al termine della P2 lo studente sa:

1. Spiegare perché la dipendenza diretta da un singolo validatore rende poco estensibile il motore.
2. Dichiarare una classe astratta con un metodo virtuale puro e un distruttore virtuale.
3. Realizzare almeno due classi derivate, ridefinendo il metodo con `override`.
4. Distinguere il tipo del puntatore (`Vincolo*`) dal tipo reale dell'oggetto a cui punta.
5. Utilizzare un `std::vector<Vincolo*>` per eseguire più controlli tramite un ciclo.
6. Spiegare chi crea e distrugge i vincoli e perché il vettore del motore non li elimina.
7. Dimostrare con prove ripetibili che uno scambio rifiutato non modifica l'orario.
8. Conservare le funzionalità della P1 senza introdurre regressioni.

## 4. Due problemi analoghi, non identici

| Aspetto | Soluzione guida docente | Compito studenti |
|---|---|---|
| Programma | `OrarioDocenteCPP_P2` | Prototipo P2 dell'orario di una classe |
| Oggetto rappresentato | Orario di un docente | Orario di una classe |
| Classe base | `Vincolo` | `Vincolo` |
| Primo vincolo | `VincoloSenzaBuchi` | `VincoloSenzaBuchi` |
| Secondo vincolo | `VincoloMaxOreGiornaliere` | `VincoloMaxOreMateria` |
| Regola aggiuntiva | Non superare N lezioni totali in un giorno | Non superare N lezioni della **stessa materia** in un giorno |
| Motore | Applica tutti i vincoli registrati | Applica tutti i vincoli registrati |
| Esito negativo | Scambio annullato | Scambio annullato |

**Nota sulla scelta dei vincoli:** nel progetto del docente, il massimo di ore giornaliere conta tutte le tessere occupate in un giorno; nel progetto degli studenti, il massimo per materia conta separatamente le tessere con la stessa materia in ciascun giorno. Il secondo controllo è quindi diverso, pur richiedendo lo stesso meccanismo polimorfico.

## 5. Soluzione di riferimento del docente

### 5.1 Struttura prevista

Nuovi file nella cartella `Engine`:

- `Vincolo.h` (classe base astratta).
- `VincoloSenzaBuchi.h/.cpp`.
- `VincoloMaxOreGiornaliere.h/.cpp`.

File da modificare:

- `Engine/MotoreScambi.h/.cpp`: il motore conserva una raccolta di vincoli e li verifica in sequenza.
- `InterfacciaConsole.h/.cpp` e, se necessario, `main.cpp`: creazione dei vincoli, registrazione e presentazione degli esiti.
- `OrarioDocenteCPP_P2.sln`, `.vcxproj` e `.vcxproj.filters`: progetto Visual Studio autonomo.

Le classi `Orario`, `Tessera`, `Posizione`, `Classe` ed `ElencoClassi` rimangono sostanzialmente invariate.

### 5.2 Interfaccia proposta

```cpp
// Vincolo.h
#pragma once
#include "../Model/Orario.h"

class Vincolo {
public:
    virtual bool Verifica(Orario& orario) = 0;
    virtual ~Vincolo() {}
};
```

```cpp
// Esempio di classe derivata
class VincoloSenzaBuchi : public Vincolo {
public:
    bool Verifica(Orario& orario) override;
};
```

Per il secondo vincolo, il docente utilizza un attributo `int massimoOre` inizializzato dal costruttore. Il metodo `Verifica()` restituisce `false` se una giornata supera il limite.

Interfaccia essenziale del motore:

```cpp
class MotoreScambi {
private:
    std::vector<Vincolo*> vincoli;
public:
    void AggiungiVincolo(Vincolo* vincolo);
    bool Scambia(Orario& orario, Posizione prima, Posizione seconda);
};
```

Il motore **non possiede** gli oggetti `Vincolo`: sono creati e mantenuti in vita dal codice che li registra. Il motore non chiama `delete` sui puntatori contenuti nel vettore. Non si devono registrare puntatori nulli o oggetti che terminano la propria durata prima del motore o delle sue operazioni.

### 5.3 Algoritmo di scambio

1. Verificare che le posizioni siano valide e diverse.
2. Verificare che l'orario iniziale rispetti tutti i vincoli registrati.
3. Scambiare temporaneamente le due celle.
4. Verificare nuovamente **tutti** i vincoli.
5. Se tutti sono rispettati, confermare lo scambio.
6. Se almeno uno fallisce, scambiare di nuovo le stesse celle per ripristinare lo stato precedente e restituire `false`.

Il controllo dei vincoli deve essere eseguito con un ciclo su `std::vector<Vincolo*>` e una chiamata polimorfica a `Verifica()`, senza controllare il tipo concreto dei vincoli.

**Condizione di configurazione:** il motore deve avere almeno un vincolo registrato prima di autorizzare scambi. Per la soluzione P2 si registrano sempre entrambi i vincoli; la gestione di un motore senza vincoli deve essere definita esplicitamente nell'implementazione, evitando di accettare scambi per errore.

### 5.4 Dati di prova e osservazione importante

L'orario iniziale del docente occupa sette ore dal lunedì al giovedì e sei ore il venerdì. Se il massimo è **7**, l'orario iniziale è valido.

Uno scambio tra una lezione di un giorno da sette ore e una cella vuota di un altro giorno già da sette ore può creare **otto ore occupate** nel secondo giorno: il vincolo deve rifiutarlo, anche se non si crea alcun buco.

Per verificare separatamente i due vincoli occorrono casi costruiti con attenzione: uno scambio che crea un buco ma non supera il limite, e uno che supera il limite senza creare buchi. Il secondo caso può essere ottenuto spostando la prima o l'ultima lezione di una giornata in una cella contigua alle lezioni di un'altra giornata, quando il limite lo permette o lo vieta a seconda della configurazione.

## 6. Sequenza didattica suggerita

| Passo | Attività guidata docente | Attività studenti | Evidenza |
|---|---|---|---|
| A | Analisi del limite del validatore P1 | Individuare le dipendenze nel diagramma UML | Schema prima/dopo |
| B | Definizione di `Vincolo` astratta | Scrivere una classe base equivalente | Header compilabile |
| C | Trasferimento di `SenzaBuchi` nella classe derivata | Realizzare il primo vincolo | Prove su orari con/senza buchi |
| D | Implementazione massimo ore del docente | Implementare massimo ore per materia | Prove con limite superato |
| E | `vector<Vincolo*>` e ciclo polimorfico | Integrare i vincoli nel proprio motore | Scambio verificato da entrambi |
| F | Registrazione e durata dei vincoli | Spiegare chi possiede gli oggetti | Diagramma e spiegazione |
| G | Collaudo e confronto con P1 | Eseguire casi di prova e controlli di regressione | Tabella risultati |

## 7. Criteri oggettivi di verifica

| ID | Prova | Risultato atteso |
|---|---|---|
| T01 | Compilazione della soluzione P2 in Visual Studio | Nessun errore |
| T02 | Esecuzione della soluzione P1 conservata | P1 continua a funzionare indipendentemente |
| T03 | Orario iniziale senza buchi, massimo docente = 7 | Entrambi i vincoli restituiscono `true` |
| T04 | Orario con una cella vuota fra due lezioni | `VincoloSenzaBuchi::Verifica` restituisce `false` |
| T05 | Giornata con otto lezioni e massimo docente = 7 | `VincoloMaxOreGiornaliere::Verifica` restituisce `false` |
| T06 | Scambio valido rispetto a entrambi i vincoli | Scambio confermato; celle invertite |
| T07 | Scambio che crea un buco | Scambio rifiutato; celle identiche a prima |
| T08 | Scambio che supera il limite giornaliero | Scambio rifiutato; celle identiche a prima |
| T09 | Aggiunta di un vincolo concreto al vettore | Il motore lo applica senza modificare l'algoritmo di scambio |
| T10 | Revisione della gestione memoria | Nessun `delete` sui vincoli nel motore; puntatori validi durante l'uso |
| T11 | Menu P1: visualizzazione, ricerca e aggiunta classi | Nessuna regressione |
| T12 | Motore senza vincoli | Nessuno scambio accettato accidentalmente |

**Prove studenti:** replicare T01–T12 sul proprio orario, sostituendo T05 e T08 con il limite di ore della stessa materia in un giorno. Per esempio, con massimo 2 ore di Informatica, tre tessere di Informatica nello stesso giorno devono produrre `false`.

**Precisazione:** se si verifica un vincolo su un orario già non valido, il motore deve rifiutare lo scambio prima di modificarlo. I test del singolo vincolo possono invece utilizzare orari appositamente non validi per verificarne il funzionamento.

## 8. Materiali e prodotti della fase

- **Scheda docente:** questo documento.
- **Codice di riferimento:** `OrarioDocenteCPP_P2`, da creare copiando `OrarioDocenteCPP_P1` e adattando i file della soluzione Visual Studio.
- **Guida di laboratorio studenti:** `Documentazione/P2/P2_Guida_Laboratorio.md` (da preparare).
- **Dispensa teorica:** `Documentazione/P2/P2_Dispensa_Teorica.md` (da preparare).
- **Esercizi e soluzioni:** da preparare separatamente.
- **Piano di test dettagliato:** da preparare prima della consegna finale.

**Output P2:** due applicazioni console che applicano più vincoli mediante polimorfismo e ripristinano l'orario dopo uno scambio rifiutato.

**Continuità P3:** lo scambio confermato potrà essere registrato in una pila per consentirne l'annullamento.
