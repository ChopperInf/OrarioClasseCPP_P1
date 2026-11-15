# P2 — Esercizi autonomi: ereditarietà e vincoli polimorfici

**Informatica — classe quarta**  
**Progetto:** orario di classe, prototipo P2  
**Prerequisiti:** [dispensa teorica](P2_Dispensa_Teorica.md) e [guida di laboratorio](P2_Guida_Laboratorio.md).  
**Stato:** esercizi predisposti sul codice P2 pubblicato, non ancora collaudato in Visual Studio.

## Obiettivo

Applicare classi astratte, ereditarietà, metodi virtuali, puntatori alla classe base e `std::vector` per controllare la validità di un orario di classe.

Gli esercizi sono progressivi. I primi possono essere svolti su carta; quelli successivi richiedono di modificare il proprio progetto C++. **Non modificare la P1:** lavorare soltanto sulla copia P2.

## Esercizio A — Riconoscere le responsabilità

Considera le classi `Vincolo`, `VincoloSenzaBuchi`, `VincoloMaxOreMateria`, `MotoreScambi` e `Orario`.

1. Quale classe rappresenta il comportamento comune a tutti i controlli?
2. Quali classi devono conoscere l'algoritmo di verifica specifico?
3. Quale classe deve decidere se confermare o annullare uno scambio?
4. Perché non conviene inserire in `MotoreScambi` un metodo diverso per ogni vincolo?
5. Disegna le relazioni UML tra le cinque classi, distinguendo ereditarietà e dipendenze.

**Consegna:** risposte sintetiche e diagramma UML.

---

## Esercizio B — Individuare gli errori in una classe astratta

Esamina il codice:

```cpp
class Vincolo {
public:
    bool Verifica(Orario& orario) = 0;
    ~Vincolo() {}
};
```

1. Individua gli errori o le omissioni.
2. Riscrivi la classe in modo corretto.
3. Spiega perché non è possibile dichiarare `Vincolo v;`.
4. Spiega a cosa serve un distruttore virtuale.

**Consegna:** header corretto e spiegazioni.

---

## Esercizio C — Interpretare il polimorfismo

Supponi che `VincoloSenzaBuchi` e `VincoloMaxOreMateria` derivino da `Vincolo` e implementino `Verifica()`.

```cpp
VincoloSenzaBuchi a;
VincoloMaxOreMateria b(2);

Vincolo* primo = &a;
Vincolo* secondo = &b;

bool x = primo->Verifica(orario);
bool y = secondo->Verifica(orario);
```

1. Qual è il tipo dichiarato di `primo` e `secondo`?
2. Qual è il tipo effettivo degli oggetti indicati?
3. Quale versione di `Verifica()` viene chiamata nelle ultime due istruzioni?
4. I valori `x` e `y` sono necessariamente uguali? Motiva la risposta.

**Consegna:** tabella con tipi e metodi eseguiti.

---

## Esercizio D — Progettare il vincolo sulle materie

Nel programma degli studenti, una tessera contiene la materia della lezione. Realizza `VincoloMaxOreMateria`, che impedisce di superare un numero massimo di ore della **stessa materia nello stesso giorno**.

Interfaccia richiesta:

```cpp
class VincoloMaxOreMateria : public Vincolo {
private:
    int massimoOre;
public:
    explicit VincoloMaxOreMateria(int massimoOre);
    bool Verifica(Orario& orario) override;
};
```

**Requisiti:**

- Esaminare 5 giorni e 8 ore al giorno.
- Ignorare le celle vuote (`nullptr`).
- Confrontare le materie mediante `GetMateria()`.
- Restituire `false` se una materia supera il limite in un giorno.
- Considerare non valido un limite minore di 0 o maggiore di 8.
- Non utilizzare `map`, `set` o algoritmi STL avanzati.

**Consegna:** file `.h` e `.cpp`, con commenti ai cicli.

---

## Esercizio E — Prevedere l'esito del vincolo

Per il solo `VincoloMaxOreMateria(2)`, considera le seguenti giornate; `—` indica una cella vuota.

| Caso | Ora 1 | Ora 2 | Ora 3 | Ora 4 | Ora 5 | Esito atteso? |
|---|---|---|---|---|---|---|
| A | INF | MAT | INF | — | — | ? |
| B | INF | INF | INF | — | — | ? |
| C | INF | MAT | ING | INF | MAT | ? |
| D | INF | INF | MAT | MAT | MAT | ? |
| E | — | INF | — | INF | — | ? |

1. Completa la colonna degli esiti.
2. Per i casi non validi, indica la materia che supera il limite.
3. Spiega perché la presenza di buchi nel caso E non determina da sola l'esito di **questo** vincolo.

**Consegna:** tabella completata e spiegazione.

---

## Esercizio F — Completare la verifica polimorfica

Completa il metodo seguente, sostituendo i commenti `TODO` con istruzioni C++:

```cpp
bool MotoreScambi::VerificaVincoli(Orario& orario) {
    // TODO 1: se non ci sono vincoli, rifiuta.

    for (int i = 0; i < static_cast<int>(vincoli.size()); i++) {
        // TODO 2: chiama il metodo virtuale del vincolo.
        // TODO 3: se fallisce, restituisci false.
    }

    // TODO 4: tutti i vincoli sono rispettati.
}
```

1. Perché il vettore contiene `Vincolo*` anziché oggetti `Vincolo`?
2. Perché non serve un `if` per riconoscere il tipo concreto del vincolo?

**Consegna:** metodo completo e due spiegazioni.

---

## Esercizio G — Proprietà e durata dei vincoli

Considera:

```cpp
void Configura(MotoreScambi& motore) {
    VincoloMaxOreMateria limite(2);
    motore.AggiungiVincolo(&limite);
}
```

1. Che cosa succede all'oggetto `limite` quando `Configura` termina?
2. Il puntatore conservato nel motore resta utilizzabile?
3. Proponi una soluzione in cui l'oggetto resti valido per tutto il periodo di utilizzo del motore.
4. Spiega perché il motore non deve chiamare `delete` sul puntatore ricevuto.

**Consegna:** analisi dell'errore e codice corretto.

---

## Esercizio H — Scambio e ripristino

Un orario inizialmente valido contiene:

| Giorno | Ora 1 | Ora 2 | Ora 3 | Ora 4 |
|---|---|---|---|---|
| Lunedì | INF | MAT | ING | — |
| Martedì | MAT | INF | — | — |

Sono attivi `VincoloSenzaBuchi` e `VincoloMaxOreMateria(2)`. Le altre ore e giornate sono vuote.

Valuta separatamente, **ripartendo ogni volta dall'orario iniziale**, questi scambi:

1. Lunedì ora 1 ↔ lunedì ora 2.
2. Lunedì ora 2 ↔ martedì ora 3.
3. Lunedì ora 3 ↔ martedì ora 3.

Per ciascuno indica se viene accettato o rifiutato, quale vincolo eventualmente fallisce e lo stato finale delle due celle scambiate.

**Consegna:** tabella degli esiti con motivazioni.

---

## Esercizio I — Aggiungere un nuovo vincolo

Progetta `VincoloPrimaOraLibera`, che richiede che **l'ora 1 di ogni giorno sia vuota**.

1. Scrivi la dichiarazione della classe derivata da `Vincolo`.
2. Implementa `Verifica()`.
3. Spiega come registrare un'istanza nel motore.
4. Indica quali metodi di `MotoreScambi` devono essere modificati per supportare questo nuovo vincolo.

**Attenzione:** per provare il nuovo vincolo, costruisci un orario iniziale che lo rispetti; l'orario di esempio del docente potrebbe non rispettarlo.

**Consegna:** due file C++ e spiegazione.

---

## Esercizio L — Piano di collaudo

Prepara almeno sei test indipendenti:

- un orario valido per entrambi i vincoli;
- un orario con un buco;
- un orario con tre ore della stessa materia e limite 2;
- uno scambio accettato;
- uno scambio rifiutato con ripristino;
- un motore senza vincoli registrati.

Per ogni test specifica **stato iniziale, operazione, risultato atteso, risultato osservato**. Non dichiarare superato un test che non hai eseguito.

**Consegna:** tabella di collaudo compilata per quanto effettivamente verificato.

## Criteri di valutazione

| Aspetto | Punti |
|---|---:|
| Comprensione di ereditarietà e polimorfismo (A–C) | 6 |
| Algoritmo del vincolo e analisi dei casi (D–E) | 6 |
| Ciclo polimorfico e gestione della memoria (F–G) | 4 |
| Scambi, estensibilità e collaudo (H–L) | 4 |
| **Totale** | **20** |

**Riferimento per il docente:** [soluzioni commentate](P2_Soluzioni_Esercizi.md). Le soluzioni non sostituiscono la verifica della compilazione del progetto.
