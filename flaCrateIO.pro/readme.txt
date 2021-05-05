Per compilare, usare "make mWarp". In caso di problemi, provare a rimuovere tutti i .o e rilanciare il make.

******************************** Descrizione **********************************

La gestione del nodo di acquisizione dati e` eseguita dal programma mWarp.cpp 
che si basa sulla classe WarpCrate.                                           
 
La gestione dell'I/O e` affidata alla classe PhysCrate, che si appoggia alla  
classe AcqBoard: queste sono le sole classi che fanno uso delle funzioni di I/O
per della libreria Acqiris.

La gestione dei comandi e` eseguita dalla classe/thread CmdListener che eredita
dalle classi icathr e tcpcom.

La gestione della comunicazione con il Writer e` svolta dalla classe WrHandler.
La classe WrManager si occupa di rimanere in ascolto sulla porta a cui si
collegano i writer e di istanziare un WrHandler ad ogni nuova connessione di un
Writer.

L'interfacciamento tra il WrHandler e il PhysCrate e` svolto dalla classe
DataManager che dispone di funzioni membro usate dal WrHandler per richiedere 
i dati. La classe, a sua volta, richiede i dati al PhysCrate, in maniera
sincrona. 

Vi e` inoltre una classe ConnectionHandler usata quando necessario che si
occupa di attendere ed ottenere una connessione su una determinata porta.

L'architettura per la gestione del nodo che legge dal crate funziona nel modo
seguente:


mWarp.cpp:
La funzione main() istanzia la classe WarpCrate.
	Il costruttore di WarpCrate istanzia e inizializza il PhysCrate e
	istanzia il DataManager. Per la configurazione costruisce inoltre la 
	struttura BoardConf contenente i parametri di configurazione delle 
	board.
		L'inizializzazione del PhysCrate consiste nel rilevare il 
		numero di schede, effettuare l'inizializzazione hardware e 
		costruire l'array di oggetti AcqBoard. 
		
La funzione main() richiede di ottenere la connessione con l'EVM all'oggetto
WarpCrate, con la funzione membro getEvmConnection().
	La funzione getEvmConnection() istanzia un oggetto di classe 
	ConnectionHandler passando come parametro la porta di connessione 
	dell'EVM.
	Ottenuta la connessione, la assegna alla variabile d'istanza evmConn.

La funzione main() richiede lo spawn del gestore dei comandi mediante la 
funzione membro spawnCmdListener().
	La funzione membro spawnCmdListener() istanzia un'oggetto di classe 
	CmdListener passando come parametri l'oggetto WarpCrate stesso (this) 
	e la connessione appena ottenuta. 
		Il costruttore di CmdListener da` lo start al thread.

La funzione main() si mette in attesa della terminazione del CmdListener 
mediante la funzione membro waitStopCmd(), che attende il join del  thread.


CmdListener:
Una volta iniziato, il thread inizia il loop della gestione comandi chiamando 
ciclicamente il metodo parseCommand() finche` la variabile alive e` true. 
I comandi attualmente gestiti sono:
INIT: esegue lo spawn del WrManager
TRGP: configura i paramtri del trigger.
DEDP: configura le board chiamando la funzione del WarpCrate che richiama a 
sua volta quella del PhysCrate, che configura tutte le schede. In piu` attiva
l'acquisizione sulle board chiamando il metodo start()
**Rimane da implementare la lettura dei parametri di configurazione passati 
dall'EVM. 
La configurazione del PhysCrate consiste nel configurare ciascuna board, 
passando la struttura BoardConf alla funzione configure() di AcqBoard.
MODE, STRT, FLSH, STOP: stampano solo un messaggio di conferma.
QUIT: porta la variabile alive a false determinando la terminazione del thread


WrManager:
Il costruttore di WrManager fa partire il thread.
Il thread entra in un loop in cui si mette in ascolto attendendo una 
connessione da un writer e quando la ottiene istanzia un oggetto/thread
WrHandler.

WrHandler:
Il thread esegue un loop in cui:
	Il metodo waitRequest() si mette in attesa di ricezione del messaggio
	"getevent" sulla connessione stabilita.
	Il metodo sendEvent():
		Richiama la funzione getData() del DataManager la quale:
			Chiama la funzione waitData() del PhysCrate che esegue
			un'attesa del dati su ciascuna scheda. 
			Chiama la funzione getData() del PhysCrate che alloca 
			il buffer per i dati e lo fa riempire dalle schede, 
			ritornando il puntatore.
			Imposta l'header.
			Invia i dati al Writer tramite la connessione TCP.
	Il metodo sendStat:
		Richiede le statistiche al DataManager e le invia.


 
	
