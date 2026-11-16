# OrarioDocenteCPP_P2 — vincoli polimorfici

Prototipo console C++17 dell'orario di un docente, ottenuto dalla P1 senza modificarne i sorgenti.

## Aprire il progetto

Aprire `OrarioDocenteCPP_P2.sln` in Visual Studio 2022, selezionare **Debug | x64** e compilare. Il progetto usa MSVC v143 e C++17; non richiede CMake.

## Novità rispetto alla P1

- `Engine/Vincolo.h`: classe astratta con metodo virtuale puro `Verifica()`.
- `Engine/VincoloSenzaBuchi`: impedisce ore vuote tra lezioni dello stesso giorno.
- `Engine/VincoloMaxOreGiornaliere`: impedisce di superare **4 lezioni (valore iniziale, modificabile dal menu)** nello stesso giorno.
- `Engine/MotoreScambi`: conserva `std::vector<Vincolo*>`, verifica tutti i vincoli prima e dopo lo scambio e annulla gli scambi non validi.
- `InterfacciaConsole`: possiede i due vincoli e li registra nel motore. L'opzione 4 controlla entrambe le regole.

Il motore **non possiede** i vincoli registrati e non li elimina con `delete`. Gli oggetti devono rimanere vivi durante il loro utilizzo.

## Casi di prova manuali

L'orario iniziale contiene 7 lezioni dal lunedì al giovedì e 6 il venerdì.

1. **Scambio valido:** dopo aver impostato il limite a 7, lunedì ora 1 con lunedì ora 2; deve essere accettato.
2. **Buco:** con limite 7, lunedì ora 2 con venerdì ora 8; deve essere rifiutato e annullato.
3. **Superamento del massimo senza buchi:** con limite 7, venerdì ora 6 con lunedì ora 8. Il lunedì passa a 8 lezioni consecutive; lo scambio deve essere rifiutato per il limite.
4. **Verifica iniziale:** con limite 4 l'orario non rispetta il massimo giornaliero; dopo aver impostato 7 entrambi i vincoli devono essere rispettati.

**Nota:** il punto 3 utilizza uno scambio tra una tessera e una cella vuota; gli orari delle altre giornate rimangono invariati.

## Documentazione

Vedere [scheda didattica P2](../Documentazione/P2/P2_Scheda_Didattica.md).

Le classi del modello, l'archivio `ElencoClassi` e le funzioni della console P1 sono mantenuti.

## Modifica del limite giornaliero

Il limite iniziale è **4 ore**. L'opzione **8 – Modifica massimo ore giornaliere** permette di impostare un valore tra **1 e 8**, visualizza il limite attuale e segnala se l'orario supera il nuovo valore. La modifica ha effetto immediato e resta valida durante l'esecuzione del programma; non viene salvata su file. Con il limite iniziale di 4, l'orario di esempio può essere già non valido: per effettuare gli scambi della precedente prova manuale occorre prima impostare un limite compatibile, per esempio 7 o 8. I messaggi di verifica riportano il limite effettivo, non un numero fisso.

## P2-B — Tessere polimorfiche

La P2 include anche `TesseraAula` e `TesseraLaboratorio`, derivate da `Tessera`, e `DotazioneLabInformatica`. Le lezioni in laboratorio contengono `tipoLaboratorio`, `docenteITP` e un puntatore opzionale alla dotazione. La dotazione contiene `vector<string> postazioni` e `bool presenzaLIM`; nei laboratori non informatici il puntatore è `nullptr`.

`Tessera` dispone di `GetDescrizione()` e di un distruttore virtuale. `TesseraLaboratorio` libera la dotazione che possiede nel proprio distruttore; non è copiabile. Il programma di esempio crea tessere in aula e in laboratorio informatico e la consultazione della cella mostra la descrizione polimorfica. La matrice `Tessera* celle[5][8]` e il motore degli scambi non cambiano.

**Documenti:** [P2-A — vincoli](../Documentazione/P2/P2_Dispensa_Teorica.md) · [P2-B — tessere](../Documentazione/P2/P2_Dispensa_Teorica_Tessere_Polimorfiche.md) · [guida laboratorio](../Documentazione/P2/P2_Guida_Laboratorio.md).
