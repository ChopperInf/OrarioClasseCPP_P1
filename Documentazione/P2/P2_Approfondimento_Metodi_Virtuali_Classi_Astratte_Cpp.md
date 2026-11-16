# Metodi virtuali, classi astratte e distruttori in C++

**Approfondimento per gli studenti di quarta Informatica**  
**Progetto:** Orario scolastico — P2-A (vincoli) e P2-B (tessere)  
**Prerequisiti:** classi, ereditarietà, puntatori, `new` e `delete`, costruttori e distruttori.

## 1. Il lessico corretto

In C++ non si parla propriamente di *classe virtuale*. È importante distinguere:

- **Classe polimorfica:** una classe che dichiara o eredita almeno un metodo virtuale, compreso eventualmente il solo distruttore.
- **Classe astratta:** una classe che ha almeno un metodo virtuale puro non sostituito da un'implementazione non pura nella classe stessa o lungo la gerarchia. Non è possibile istanziarla direttamente.
- **Classe concreta:** una classe che può essere istanziata, perché non presenta metodi virtuali puri ancora da implementare.

**Un solo metodo virtuale basta a rendere polimorfica una classe, ma non necessariamente astratta.** Per renderla astratta è sufficiente almeno un metodo virtuale puro (`= 0`).

| Dichiarazione | Significato | Effetto |
|---|---|---|
| `void F();` | Metodo ordinario | Non rende la classe polimorfica |
| `virtual void F();` | Metodo virtuale non puro | Classe polimorfica, non per questo astratta |
| `virtual void F() = 0;` | Metodo virtuale puro | Classe astratta finché il metodo rimane puro |
| `virtual ~Base() {}` | Distruttore virtuale con corpo | Classe polimorfica, non per questo astratta |
| `virtual ~Base() = 0;` | Distruttore virtuale puro | Classe astratta; richiede comunque una definizione |

## 2. Le domande da porsi prima di scrivere il codice

1. **Il comportamento deve dipendere dal tipo effettivo dell'oggetto?** Se chiamiamo un metodo attraverso `Base*` o `Base&` e vogliamo comportamenti diversi, usiamo `virtual`.
2. **La classe base può fornire un comportamento generale sensato?** Se sì, un metodo virtuale **non puro** può essere la scelta migliore.
3. **Ogni classe derivata concreta deve essere obbligata a fornire quel comportamento?** Se sì, usiamo un metodo virtuale **puro**, cioè `= 0`.
4. **Un oggetto derivato potrà essere distrutto con `delete` attraverso `Base*`?** In tal caso il distruttore della base deve essere virtuale.

`virtual` e `= 0` rispondono quindi a domande diverse: il primo riguarda la **scelta del metodo a runtime**, il secondo il **contratto imposto alle classi derivate**.

## 3. P2-A: perché `Vincolo` usa metodi virtuali puri

Un vincolo generico non identifica alcuna regola concreta: non sappiamo se deve controllare i buchi o il numero massimo di lezioni. Perciò il progetto richiede che ogni vincolo concreto implementi la propria verifica e il proprio messaggio.

```cpp
class Vincolo {
public:
    virtual bool Verifica(Orario& orario) = 0;
    virtual std::string GetMessaggio() = 0;
    virtual ~Vincolo() {}
};
```

`Vincolo` è **astratta**. `Vincolo v;` è un errore di compilazione, mentre `Vincolo* p;` è consentito. Una derivata concreta, come `VincoloSenzaBuchi`, deve fornire le implementazioni dei due metodi puri. Il distruttore è virtuale ma non puro: la classe è già astratta per gli altri metodi.

```cpp
class VincoloSenzaBuchi : public Vincolo {
public:
    bool Verifica(Orario& orario) override;
    std::string GetMessaggio() override;
};
```

## 4. Alternativa per `Vincolo`: metodi virtuali non puri

Potremmo decidere di fornire una verifica predefinita:

```cpp
class Vincolo {
public:
    virtual bool Verifica(Orario& orario) { return true; }
    virtual std::string GetMessaggio() { return "Nessun errore"; }
    virtual ~Vincolo() {}
};

class VincoloSenzaBuchi : public Vincolo {
public:
    // Per errore dimentichiamo di ridefinire Verifica().
    std::string GetMessaggio() override { return "Buco nell'orario"; }
};
```

Questo codice compila: `Vincolo` è concreta e `VincoloSenzaBuchi` eredita `Verifica()` che restituisce sempre `true`. Il motore continuerebbe a utilizzare `vector<Vincolo*>` e il polimorfismo, ma il controllo dei buchi verrebbe di fatto disattivato senza un errore di compilazione.

Restituire `false` come valore predefinito sarebbe più prudente dal punto di vista dell'accettazione degli scambi, ma nasconderebbe comunque una verifica mancante. **Per questo progetto i metodi puri sono preferibili:** il compilatore impone alle classi concrete di implementare la regola.

## 5. Alternativa intermedia: un solo metodo puro

```cpp
class Vincolo {
public:
    virtual bool Verifica(Orario& orario) = 0;
    virtual std::string GetMessaggio() {
        return "Vincolo non rispettato";
    }
    virtual ~Vincolo() {}
};
```

La classe rimane **astratta**, perché `Verifica()` è puro. Le derivate devono implementare `Verifica()`, ma possono ereditare il messaggio generico. Questa soluzione è utile se il messaggio predefinito è accettabile; nel progetto P2-A preferiamo messaggi specifici.

## 6. P2-B: perché `Tessera` usa un metodo virtuale non puro

Una tessera generica contiene già i dati necessari per una descrizione: **materia, classe e aula**. La base può quindi fornire un comportamento utile, lasciando alle derivate la possibilità di specializzarlo.

**Tessera.h**

```cpp
#pragma once
#include <string>

class Tessera {
private:
    std::string materia;
    std::string classe;
    std::string aula;

public:
    Tessera(std::string materia, std::string classe, std::string aula);
    std::string GetMateria();
    std::string GetClasse();
    std::string GetAula();
    virtual std::string GetDescrizione();
    virtual ~Tessera();
};
```

**Tessera.cpp** (parti significative)

```cpp
Tessera::Tessera(std::string materia, std::string classe, std::string aula)
    : materia(materia), classe(classe), aula(aula) {}

std::string Tessera::GetDescrizione() {
    return materia + " - " + classe + " - " + aula;
}

Tessera::~Tessera() {}
```

`GetDescrizione()` è **virtuale, ma non puro**. Di conseguenza `Tessera` è polimorfica e concreta:

```cpp
Tessera t("Matematica", "4AI", "A12");  // Valido
std::cout << t.GetDescrizione();         // Matematica - 4AI - A12
```

Una derivata può ridefinire `GetDescrizione()`; se non lo fa, eredita la descrizione generale. La scelta è coerente con il fatto che anche una tessera generica possiede un significato autonomo.

## 7. Alternativa per `Tessera`: `GetDescrizione() = 0`, anche con un corpo nella base

Supponiamo di cambiare il requisito: **non devono esistere tessere generiche**, ma soltanto tessere di aula e di laboratorio. In questo caso possiamo rendere puro `GetDescrizione()`.

**Tessera.h**

```cpp
#pragma once
#include <string>

class Tessera {
private:
    std::string materia;
    std::string classe;
    std::string aula;

public:
    Tessera(std::string materia, std::string classe, std::string aula);
    std::string GetMateria();
    std::string GetClasse();
    std::string GetAula();
    virtual std::string GetDescrizione() = 0;
    virtual ~Tessera();
};
```

Il costruttore e i getter restano utilizzabili dalle classi derivate. La differenza è che ora **`Tessera` è astratta**:

```cpp
// Tessera t("Matematica", "4AI", "A12");  // ERRORE
Tessera* p = nullptr;                     // Valido
```

### 7.1 Un metodo virtuale puro può avere un'implementazione?

**Sì.** `= 0` non significa che il metodo non possa avere un corpo: significa che la classe è astratta e che una derivata deve fornire un **override non puro** per diventare concreta.

Possiamo mantenere una definizione fuori dalla classe:

**Tessera.cpp**

```cpp
Tessera::Tessera(std::string materia, std::string classe, std::string aula)
    : materia(materia), classe(classe), aula(aula) {}

std::string Tessera::GetDescrizione() {
    return materia + " - " + classe + " - " + aula;
}

Tessera::~Tessera() {}
```

**Attenzione:** questa definizione **non rende concreto `Tessera`**. Il metodo continua a essere puro perché la sua dichiarazione contiene `= 0`.

### 7.2 Come si utilizza allora il corpo della classe base?

La derivata può richiamarlo **esplicitamente**, per riutilizzare la parte comune della descrizione:

```cpp
class TesseraAula : public Tessera {
public:
    TesseraAula(std::string materia, std::string classe, std::string aula)
        : Tessera(materia, classe, aula) {}

    std::string GetDescrizione() override {
        return "AULA - " + Tessera::GetDescrizione();
    }
};
```

Esempio d'uso:

```cpp
TesseraAula aula("Matematica", "4AI", "A12");
Tessera* p = &aula;

std::cout << p->GetDescrizione() << "\n";
// AULA - Matematica - 4AI - A12

std::cout << aula.Tessera::GetDescrizione() << "\n";
// Matematica - 4AI - A12
```

Le due chiamate sono diverse:

- `p->GetDescrizione()` usa il **dispatch virtuale** e seleziona il metodo di `TesseraAula`.
- `aula.Tessera::GetDescrizione()` usa il nome qualificato `Tessera::` e richiama **direttamente** l'implementazione della base, senza dispatch virtuale.

Nel metodo della derivata, l'espressione `Tessera::GetDescrizione()` serve a costruire la descrizione comune senza duplicare codice. **Non è una chiamata virtuale dinamica**: la qualificazione esplicita seleziona la base.

### 7.3 Che cosa succede se una derivata non ridefinisce il metodo puro?

```cpp
class TesseraProvvisoria : public Tessera {
public:
    TesseraProvvisoria(std::string materia, std::string classe, std::string aula)
        : Tessera(materia, classe, aula) {}
    // Nessun override di GetDescrizione()
};

// TesseraProvvisoria x("Fisica", "4AI", "A3");  // ERRORE
```

`TesseraProvvisoria` **rimane astratta**, anche se esiste un corpo per `Tessera::GetDescrizione()`. Il compilatore non considera quel corpo un sostituto dell'override richiesto.

### 7.4 E se la base non avesse alcuna definizione?

La derivata potrebbe comunque implementare autonomamente il metodo puro:

```cpp
std::string TesseraAula::GetDescrizione() {
    return "AULA - " + GetMateria() + " - " + GetClasse() + " - " + GetAula();
}
```

Questo esempio è **alternativo** alla definizione inline precedente: non vanno usate entrambe contemporaneamente. Se invece la derivata prova a chiamare `Tessera::GetDescrizione()` e la base non ha una definizione, il programma non può essere collegato correttamente (errore del linker).

**Confronto:**

| `GetDescrizione()` non puro | `GetDescrizione() = 0` |
|---|---|
| `Tessera` può essere concreta | `Tessera` è astratta |
| La derivata può ereditare il comportamento | La derivata concreta deve fornire un override |
| La base deve avere una definizione utilizzabile | La base può avere una definizione facoltativa |
| La definizione comune può essere riutilizzata | Anche qui la definizione comune può essere richiamata esplicitamente |
| Il polimorfismo funziona | Il polimorfismo funziona ugualmente |

**Conclusione:** il metodo puro con corpo nella base è utile quando vogliamo contemporaneamente **imporre una personalizzazione** e **offrire codice comune riutilizzabile**. È però una soluzione meno immediata, da adottare solo quando il requisito lo giustifica.

## 8. `TesseraLaboratorio`: che cosa significa `override` nel distruttore?

Una tessera di laboratorio aggiunge informazioni e possiede facoltativamente una dotazione informatica allocata dinamicamente:

```cpp
class TesseraLaboratorio : public Tessera {
private:
    std::string tipoLaboratorio;
    std::string docenteITP;
    DotazioneLabInformatica* dotazione;

public:
    // Costruttore omesso per concentrarsi sui metodi virtuali.
    std::string GetDescrizione() override;
    ~TesseraLaboratorio() override;
};
```

### 8.1 Perché il distruttore della base è virtuale?

`Orario` conserva puntatori di tipo `Tessera*`, ma gli oggetti effettivi possono essere `TesseraAula` o `TesseraLaboratorio`:

```cpp
Tessera* p = new TesseraLaboratorio(/* argomenti del costruttore */);
delete p;
```

L'esempio illustra il tipo del puntatore; per compilarlo occorre sostituire il commento con gli argomenti reali del costruttore. Poiché la distruzione avviene tramite `Tessera*`, **`~Tessera()` deve essere virtuale**. In caso contrario, eliminare così l'oggetto derivato comporterebbe comportamento indefinito.

### 8.2 Che cosa fa `override` su un distruttore?

Quando una classe base dichiara un distruttore virtuale, anche il distruttore della derivata è virtuale **automaticamente**. Possiamo quindi scrivere:

```cpp
~TesseraLaboratorio();             // Corretto: resta virtuale
~TesseraLaboratorio() override;    // Corretto e più esplicito
```

Queste sono **due alternative**, non due dichiarazioni da inserire insieme. La seconda è preferibile nel progetto perché `override` chiede al compilatore di verificare che il distruttore stia davvero ridefinendo un distruttore virtuale ereditato.

`override` **non**:

- rende puro il distruttore;
- crea automaticamente il corpo del distruttore;
- provoca da solo la chiamata al distruttore della base;
- è indispensabile per ottenere la distruzione polimorfica (lo è invece `virtual` nella base).

Se per errore la base non avesse un distruttore virtuale:

```cpp
class Base {
public:
    ~Base() {}
};

class Derivata : public Base {
public:
    ~Derivata() override {}  // ERRORE di compilazione
};
```

Il compilatore segnala l'errore: il distruttore della base non è virtuale e quindi non può essere ridefinito con `override`. **Questo controllo è il vantaggio concreto della parola `override`.** Senza `override`, la dichiarazione del distruttore derivato potrebbe compilare, ma non renderebbe sicuro il `delete` tramite `Base*`.

### 8.3 Perché in `TesseraLaboratorio` serve un corpo specifico?

La classe possiede `dotazione`: quando l'oggetto viene distrutto, deve liberare la risorsa.

```cpp
TesseraLaboratorio::~TesseraLaboratorio() {
    delete dotazione;
}
```

`delete nullptr` è lecito e non ha effetto, quindi il distruttore funziona anche per laboratori senza dotazione informatica.

L'ordine concettuale della distruzione è:

1. `delete` su `Tessera*` individua il distruttore del **tipo effettivo** grazie al distruttore virtuale della base.
2. Si esegue il corpo di `~TesseraLaboratorio()`, che libera la dotazione.
3. Vengono distrutti automaticamente gli attributi della derivata (per esempio le stringhe).
4. Viene eseguito `~Tessera()` e vengono distrutti automaticamente i suoi attributi.

Il distruttore della base **non deve essere chiamato manualmente** dalla derivata. Il linguaggio C++ garantisce questa sequenza.

### 8.4 E se `TesseraLaboratorio` non dichiarasse alcun distruttore?

Il compilatore potrebbe generarne uno automaticamente, ma quel distruttore **non eseguirebbe `delete dotazione`**: distruggerebbe il puntatore come semplice attributo, senza liberare l'oggetto a cui punta. Nel nostro modello si avrebbe una **perdita di memoria** quando `dotazione` non è nulla.

Per questo il distruttore specifico è necessario. `override` è invece una scelta di chiarezza e controllo del compilatore, non la causa della deallocazione.

**Attenzione alla copia:** dato che `TesseraLaboratorio` possiede il puntatore, la copia superficiale potrebbe produrre due oggetti che tentano di liberare la stessa dotazione. Nel progetto la copia e l'assegnazione sono disabilitate con `= delete`. Qui `= delete` ha un significato diverso da `= 0`.

## 9. Distruttori: `virtual` e `= 0` rispondono a domande diverse

- **`virtual` sul distruttore:** permette di distruggere correttamente un oggetto derivato tramite un puntatore alla base.
- **`= 0` sul distruttore:** rende il distruttore *puro* e la classe astratta; non evita di doverlo definire.

Nel nostro progetto `Orario` possiede le tessere e le elimina tramite `Tessera*`: per questo `~Tessera()` è virtuale. Non è puro, perché nella soluzione adottata `Tessera` è concreta.

## 10. Distruttore virtuale puro: il caso particolare

```cpp
class Base {
public:
    virtual ~Base() = 0;
};

Base::~Base() {}  // Definizione OBBLIGATORIA

class Derivata : public Base {
public:
    ~Derivata() override {}
};
```

`Base` è astratta, `Derivata` può essere concreta. Il distruttore della base viene comunque eseguito al termine della distruzione della derivata: ecco perché **un distruttore virtuale puro deve avere una definizione**.

Nel nostro progetto non occorre un distruttore puro: `Vincolo` è già astratta per `Verifica()` e `GetMessaggio()`, mentre `Tessera` deve restare concreta.

## 11. Il caso che spesso genera confusione

| Domanda | Risposta |
|---|---|
| Basta un metodo virtuale per rendere astratta una classe? | No: la rende polimorfica. |
| Basta un metodo virtuale puro per rendere astratta una classe? | Sì, se resta puro nella classe considerata. |
| Un metodo virtuale non puro deve avere una definizione? | Sì, se viene utilizzato o è comunque richiesto dal programma. |
| Un metodo virtuale puro può avere un corpo? | Sì, ma resta puro. |
| Un distruttore virtuale puro deve avere un corpo? | Sì, obbligatoriamente. |
| La derivata deve ripetere `virtual`? | No: la virtualità è ereditata. |
| Che cosa controlla `override`? | Che il metodo, compreso un distruttore, ridefinisca un metodo virtuale della base. |
| Una classe astratta può avere attributi e costruttori? | Sì: vengono usati per costruire la parte base degli oggetti derivati. |

## 12. Confronto finale delle alternative

| Scelta | `Vincolo` | `Tessera` |
|---|---|---|
| Metodi virtuali puri | **Scelta adottata:** ogni vincolo concreto deve definire regola e messaggio. | **Alternativa:** impedisce tessere generiche; le derivate concrete devono definire la descrizione. |
| Metodi virtuali non puri | **Alternativa:** consente comportamenti predefiniti, ma può nascondere implementazioni mancanti. | **Scelta adottata:** descrizione generale già utile, specializzabile. |
| Distruttore virtuale non puro | Scelta adottata: sicurezza per eventuale distruzione tramite `Vincolo*`. | Scelta adottata: necessario per `delete` tramite `Tessera*`. |
| Distruttore virtuale puro | Non necessario: la classe è già astratta. | Renderebbe astratta `Tessera`, senza vantaggio nel modello scelto. |

## 13. Attività di verifica

1. Se togliamo `= 0` da `Vincolo::Verifica()` e forniamo un'implementazione che restituisce `true`, che cosa accade se una derivata dimentica di ridefinirlo?
2. Se soltanto `GetMessaggio()` fosse puro, `Vincolo` sarebbe ancora astratta? Perché?
3. Se `Tessera::GetDescrizione()` è puro **ma definito**, possiamo creare un oggetto `Tessera`? Possiamo richiamare esplicitamente il corpo della base da `TesseraAula`?
4. Che differenza c'è tra `p->GetDescrizione()` e `aula.Tessera::GetDescrizione()`?
5. Perché `delete` su `Tessera*` che punta a `TesseraLaboratorio` richiede un distruttore virtuale nella base?
6. Che cosa succede se scriviamo `~TesseraLaboratorio() override` ma `~Tessera()` non è virtuale?
7. Se omettiamo completamente il distruttore di `TesseraLaboratorio`, che cosa succede all'oggetto `dotazione`?
8. Perché `= delete` usato per vietare la copia di `TesseraLaboratorio` non equivale a `= 0`?

## 14. Il criterio progettuale da ricordare

**Prima decidiamo il contratto della gerarchia, poi scegliamo la sintassi.** Se la base possiede un comportamento generale sensato, un metodo virtuale non puro può bastare. Se vogliamo obbligare le derivate concrete a personalizzare un comportamento, scegliamo `= 0`. In entrambi i casi possiamo ottenere il polimorfismo: è `virtual` a renderlo possibile. Infine, se prevediamo di distruggere oggetti derivati attraverso puntatori alla base, il distruttore della base deve essere virtuale.

**Riferimenti per il ripasso:** `Documentazione/P2/P2_Dispensa_Teorica.md`, `Documentazione/P2/P2_Dispensa_Teorica_Tessere_Polimorfiche.md` e sorgenti `OrarioDocenteCPP_P2` del repository `giodabg/orario_scolastico`.
