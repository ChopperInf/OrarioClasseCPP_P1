# OrarioDocenteCPP_P2 — vincoli polimorfici

Prototipo console C++17 dell'orario di un docente, ottenuto dalla P1 senza modificarne i sorgenti.

## Aprire il progetto

Aprire `OrarioDocenteCPP_P2.sln` in Visual Studio 2022, selezionare **Debug | x64** e compilare. Il progetto usa MSVC v143 e C++17; non richiede CMake.

## Novità rispetto alla P1

- `Engine/Vincolo.h`: classe astratta con metodo virtuale puro `Verifica()`.
- `Engine/VincoloSenzaBuchi`: impedisce ore vuote tra lezioni dello stesso giorno.
- `Engine/VincoloMaxOreGiornaliere`: impedisce di superare **7 lezioni** nello stesso giorno.
- `Engine/MotoreScambi`: conserva `std::vector<Vincolo*>`, verifica tutti i vincoli prima e dopo lo scambio e annulla gli scambi non validi.
- `InterfacciaConsole`: possiede i due vincoli e li registra nel motore. L'opzione 4 controlla entrambe le regole.

Il motore **non possiede** i vincoli registrati e non li elimina con `delete`. Gli oggetti devono rimanere vivi durante il loro utilizzo.

## Casi di prova manuali

L'orario iniziale contiene 7 lezioni dal lunedì al giovedì e 6 il venerdì.

1. **Scambio valido:** lunedì ora 1 con lunedì ora 2; deve essere accettato.
2. **Buco:** lunedì ora 2 con venerdì ora 8; deve essere rifiutato e annullato.
3. **Superamento del massimo senza buchi:** venerdì ora 6 con lunedì ora 8. Venerdì passa a 5 lezioni consecutive, lunedì a 8 consecutive; lo scambio deve essere rifiutato per il limite di 7 ore.
4. **Verifica iniziale:** opzione 4; entrambi i vincoli devono essere rispettati.

**Nota:** il punto 3 utilizza uno scambio tra una tessera e una cella vuota; gli orari delle altre giornate rimangono invariati.

## Documentazione

Vedere [scheda didattica P2](../Documentazione/P2/P2_Scheda_Didattica.md).

Le classi del modello, l'archivio `ElencoClassi` e le funzioni della console P1 sono mantenuti.
