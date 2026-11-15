# P1 — Le relazioni UML tra le classi

**Informatica, classe quarta — Prototipo `OrarioDocenteCPP_P1`**

## Obiettivo
Capire come le classi del programma sono collegate e perché abbiamo scelto ciascuna relazione. Nel diagramma UML distinguiamo **composizione**, **associazione** e **dipendenza**.

## 1. Composizione: una classe possiede degli oggetti
**Simbolo:** linea continua con **rombo pieno** sul lato del contenitore.

- **`ElencoClassi` ◆— `Classe`**. L'attributo `std::vector<Classe> classi;` contiene gli oggetti per valore. Il vettore può crescere e distrugge automaticamente le classi quando viene distrutto. **Perché va bene:** non occorre usare `new` e `delete`; l'archivio è responsabile dei propri elementi. Molteplicità: **0..*** classi per archivio.
- **`Orario` ◆— `Tessera`**. L'attributo `Tessera* celle[5][8];` conserva fino a 40 puntatori. Se `Set()` riesce, l'orario diventa proprietario della tessera e il distruttore la elimina con `delete`. **Perché va bene nella P1:** permette di riutilizzare il lavoro su puntatori e memoria dinamica svolto nella fase precedente. Molteplicità: **0..40** tessere.
- **`MotoreScambi` ◆— `ValidatoreOrario`**. L'attributo `ValidatoreOrario validatore;` è un oggetto contenuto nel motore. **Perché va bene:** il motore ha bisogno del validatore per controllare gli scambi e, nella P1, basta un solo tipo di controllo.
- **`InterfacciaConsole` ◆— `MotoreScambi`** e **`InterfacciaConsole` ◆— `ValidatoreOrario`**. La console contiene `MotoreScambi motore;` e `ValidatoreOrario validatore;`. **Perché va bene:** delega scambi e verifiche alle classi specializzate; gli oggetti sono creati e distrutti insieme alla console.

**Attenzione:** la composizione non significa necessariamente «usare puntatori». Sia il `vector` di oggetti sia la matrice di puntatori rappresentano qui composizioni, perché conta **chi possiede gli oggetti**.

## 2. Associazione: una classe conserva un collegamento esterno
**Simbolo:** linea continua, eventualmente con freccia verso la classe conosciuta.

- **`InterfacciaConsole` → `Orario`**. L'attributo `Orario& orario;` è un riferimento a un orario creato fuori dalla console. **Perché va bene:** la console usa e modifica l'orario originale, senza copiarlo e senza distruggerlo. Inoltre, la copia di `Orario` è disabilitata.
- **`InterfacciaConsole` → `ElencoClassi`**. L'attributo `ElencoClassi& elencoClassi;` collega la console all'archivio originale. **Perché va bene:** inserimenti e ricerche devono agire sullo stesso archivio usato dal resto del programma.

**Attenzione:** gli oggetti collegati devono rimanere validi finché la console li utilizza. Quando la console viene distrutta, non distrugge gli oggetti a cui si riferisce.

## 3. Dipendenza: una classe usa un'altra per un'operazione
**Simbolo:** linea tratteggiata con freccia verso la classe utilizzata.

- **`Orario` ⇢ `Posizione`**: `Get(Posizione posizione)`, `Set(Posizione posizione, ...)` e `ScambiaCelle(Posizione prima, Posizione seconda)`. **Perché va bene:** giorno e ora servono per individuare le celle, non devono essere conservati come attributi dell'orario.
- **`MotoreScambi` ⇢ `Orario` e `Posizione`**: `Scambia(Orario& orario, Posizione prima, Posizione seconda)`. **Perché va bene:** il motore lavora sull'orario ricevuto e sulle due celle indicate; non deve possedere l'orario.
- **`ValidatoreOrario` ⇢ `Orario`**: `SenzaBuchi(Orario& orario)`. **Perché va bene:** il validatore controlla l'orario passato al metodo, senza conservarlo.
- **`DatiEsempio` ⇢ `Orario` e `ElencoClassi`**: `Carica(Orario& orario)` e `CaricaClassi(ElencoClassi& elenco)`, entrambi statici. **Perché va bene:** servono a caricare i dati iniziali; non è necessario conservare gli oggetti né creare un'istanza di `DatiEsempio`.
- **`InterfacciaConsole` ⇢ `Posizione`**: la console crea e usa posizioni quando l'utente sceglie le celle. **Perché va bene:** sono dati temporanei necessari a una singola operazione.

## 4. Tabella di riepilogo

| Classe di partenza | Classe collegata | Tipo | Motivo |
|---|---|---|---|
| ElencoClassi | Classe | Composizione | Contiene le classi per valore |
| Orario | Tessera | Composizione | Possiede le tessere |
| MotoreScambi | ValidatoreOrario | Composizione | Contiene il validatore |
| InterfacciaConsole | MotoreScambi | Composizione | Contiene il motore |
| InterfacciaConsole | ValidatoreOrario | Composizione | Contiene il validatore |
| InterfacciaConsole | Orario | Associazione | Conserva un riferimento |
| InterfacciaConsole | ElencoClassi | Associazione | Conserva un riferimento |
| Orario | Posizione | Dipendenza | Usa le coordinate |
| MotoreScambi | Orario | Dipendenza | Modifica l'orario ricevuto |
| MotoreScambi | Posizione | Dipendenza | Usa le posizioni |
| ValidatoreOrario | Orario | Dipendenza | Verifica l'orario |
| DatiEsempio | Orario | Dipendenza | Carica le lezioni |
| DatiEsempio | ElencoClassi | Dipendenza | Carica le classi |
| InterfacciaConsole | Posizione | Dipendenza | Usa le coordinate scelte |

## 5. Come scegliere la relazione

1. **Una classe possiede gli oggetti e ne gestisce la durata?** Probabilmente è una **composizione**.
2. **Una classe conserva un collegamento a un oggetto esterno?** Probabilmente è un'**associazione**.
3. **Una classe usa l'altra solo durante un'operazione?** Probabilmente è una **dipendenza**.

Non basta vedere un puntatore o un riferimento nel codice: occorre capire **le responsabilità delle classi**.

Nella P1 non abbiamo ancora introdotto l'**ereditarietà**. La studieremo nella P2 con i vincoli dell'orario.

## 6. Domande di verifica

- Perché `ElencoClassi`–`Classe` e `Orario`–`Tessera` sono entrambe composizioni?
- Chi distrugge le tessere presenti nell'orario?
- Perché `InterfacciaConsole` conserva `Orario&` e non una copia dell'orario?
- Perché `MotoreScambi` riceve `Orario&` come parametro?
- Qual è la differenza tra possedere un oggetto e usarlo temporaneamente?

**Codice di riferimento:** [`OrarioDocenteCPP_P1`](../../OrarioDocenteCPP_P1/). Consultare soprattutto i file `.h` per individuare attributi e parametri.
