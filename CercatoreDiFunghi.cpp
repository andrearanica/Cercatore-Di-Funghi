#include <iostream>
#include <Windows.h>
#include <ctime>
#include <conio.h>
using namespace std;

/*
ANALISI:
Innanzitutto dichiaro delle costani per le dimensioni della mappa (x, y)
Due funzioni/procedure:
    -settextcolor: cambia il colore del testo;
    -insegna: stampa a schermo un insegna con il titolo del gioco;
    -generaMappa: genera all'interno della mappa dei funghi;
    -stampaMappa: stampa a schermo la mappa con i suoi elementi;
    -movimento: riceve in input i movimenti e cambia i valori di yP e xP e il numero dei passi;
    -scambia: serve per l'ordinamento del vettore;
    -ordinamento: ordina il vettore;
Ad ogni turno, viene chiesto al giocatore di inserire la direzione in cui camminare (W, A, S, D) e viene spostato il personaggio verso quella direzione della matrice.
Ad ogni turno viene decrementato il numero di passi rimanente.
Se il giocatore si trova su una cella in cui è presente un fungo, viene incrementato il punteggio e diminuito il numero di funghi rimasti.
Alla fine della partita, se il giocatore ha vinto il suo punteggio viene salvato in un vettore classifica, che viene poi ordinato ad ogni turno.

Differenza tra le difficoltà:
    -facile: inizialmente 200 passi, sono presenti bonus che aumentano i passi e anche i funghi velenosi, che diminuiscono i passi;
    -difficile: i passi possibili sono 50 in meno, non ci sono possibilità di prendere i funghi speciali che aumentano i passi, e non si può tornare per i propri passi già compiuti, contrassegnati da una X;
    -impossibile: i passi possibili sono sempre come la modalità difficile, ma aumentano le possibilità di raccogliere funghi allucinogeni e bombe;
*/

const int y = 11, x = 31; 

void insegna() {
    Sleep(100);
    cout << "###### ##### ##### ##### ##### ##### ##### ##### #    ####  #    ##### #   # #   # ##### #   # #" << endl;
    Sleep(100);
    cout << "#      #     #   # #     #   #   #   #   # #   # #    #   # #    #     #   # ##  # #     #   # #" << endl;
    Sleep(100);
    cout << "#      ##### ##### #     #####   #   #   # ##### #    #   # #    ##### #   # # # # # ### ##### #" << endl;
    Sleep(100);
    cout << "#      #     #  #  #     #   #   #   #   # #  #  #    #   # #    #     #   # #  ## #   # #   # #"  << endl;
    Sleep(100);
    cout << "###### ##### #   # ##### #   #   #   ##### #   # #    ####  #    #     ##### #   # ##### #   # #" << endl;
    Sleep(100);
}

static int current_text_colors = 0;

void settextcolor(int color)
{
    current_text_colors = (current_text_colors & 0xF0) | (color & 0x0F);
    SetConsoleTextAttribute(
        GetStdHandle(STD_OUTPUT_HANDLE),
        current_text_colors
    );
}

void generaMappa(char mappa[y][x], int &nF) {                               //FUNZIONE PER GENERARE LA MAPPA
    int random;
    nF = 0;
    for (int i = 0; i < y; i++) {
        for (int j = 0; j < x; j++) {
            random = rand() % 80;                                           //Se un numero randomizzato vale più di 76
            if (random >= 77) {
                mappa[i][j] = 'f';                                          //Riempie la mappa di un fungo
                nF++;
            }
            else {
                mappa[i][j] = ' ';                                          //Altrimenti si riempie con uno spazio
            }
        }
    }
    mappa[10][15] = ("%C", 2);
}

void stampaMappa(char mappa[y][x]) {                                        //Stampa a schermo il contenuto della matrice
    for (int i = 0; i < x; i++) {
        cout << "-";
    }
    cout << "--";
    cout << endl;
    for (int i = 0; i < y; i++) {
        cout << "|";
        for (int j = 0; j < x; j++) {
            if (mappa[i][j] == 'f') {
                settextcolor(2);
            }
            else if (mappa[i][j] == 'X') {
                settextcolor(6);
            }
            cout << mappa[i][j];
            settextcolor(15);
        }
        cout << "|" << endl;
    }
    for (int i = 0; i < x; i++) {
        cout << "-";
    }
    cout << "--" << endl;
}

void movimento(char mossa, int &yP, int &xP, char mappa[y][x], int &nPassi) {               //Elabora le coordinate del giocatore in base alla mossa data in input
    if ((mossa == 'w' || mossa == 'W') && yP >= 0 && mappa[yP - 1][xP] != 'X') {
        yP--;
        nPassi--;
    }
    else if ((mossa == 's' || mossa == 'S') && yP < y && mappa[yP + 1][xP] != 'X') {
        yP++;
        nPassi--;
    }
    else if ((mossa == 'a' || mossa == 'A') && xP >= 0 && mappa[yP][xP - 1] != 'X') {
        xP--;
        nPassi--;
    }
    else if ((mossa == 'd' || mossa == 'D') && xP <= x && mappa[yP][xP + 1] != 'X') {
        xP++;
        nPassi--;
    }
    else if (mossa == 'x' || mossa == 'X') {
        nPassi = 0;
        system("cls");
    }
}

void scambia(float& a, float& b) {                          
    int n = b;
    b = a;
    a = n;
}

void ordinamento(float v[], int dim) {                                        //FUNZIONE PER ORDINARE UN VETTORE
    for (int i = 0; i < dim; i++) {
        for (int j = i + 1; j < dim; j++) {
            if (v[i] < v[j]) {
                scambia(v[i], v[j]);
            }
        }
    }
}

HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

int main()
{
    srand(time(NULL));

    char faccina = ("%C", 2);

    int xP = 15, yP = 10;                           //Coordinate del personaggio
    char mappa[y][x] = { 0 };                       //Matrice char per la mappa (char così possono essere inseriti caratteri). La mappa viene inizializzata a 0, quindi senza giocatore e funghi
    mappa[yP][xP] = faccina;                        //Viene posizionato il simbolo del giocatore


    int scelta, nF = 0, nPassi, pos = 0, bomba = 0, fG = 0;                             //Variabili di gioco
    float classifica[100] = { 0 };
    char mossa, continua = 'S', bonus;
    bool primaPartita = true;                                                       //Variabile per controllare se è la prima partita in corso

    do {
        insegna();                                                                  //Stampo a schermo il "logo" della schermata iniziale
        cout << endl;
        nF = 0;                                                                     //Inizializzo il numero dei funghi presenti all'inizio in modo che poi venga modificato dalla procedura

        cout << "Scegli il livello di difficolta':\n1) Facile: il personaggio puo' tornare sui suoi passi e sono presenti solo funghi bonus e malus \n2) Difficile: il personaggio NON puo' tornare sui suoi passi, i passi sono meno e non ci sono funghi bonus\n3) Impossibile: numero ancora minore di passi, piu' possibilita' di prendere bombe e funghi avvelenati" << endl;
        if (classifica[0] != 0 && !primaPartita) {
            cout << "Il punteggio piu' alto fino ad ora e' di " << classifica[0] << " punti " << endl;
        }
        do {
            cin >> scelta;
        } while (scelta < 0 || scelta > 3);

        if (primaPartita) {                                                         //Se è la prima partita che viene giocata scrive le regole del gioco 
            system("cls");
            cout << "----Come si gioca?----\nPer muovere il tuo personaggio, usa i tasti W A S D (su, giu, destra, sinistra).\nPremi uno di questi tasti e poi premi invio: il giocatore si muovera'.\nMuoviti nella mappa e cerca di prendere tutti i funghi!\nRICORDA! Se rimani bloccato, specialmente nella modalita' difficile, puoi digitare X come tuo movimento per terminare la partita\nNon puoi uscire dalla mappa: se lo farai, terminera' la partita. " << endl;
            system("pause");
            primaPartita = false;
        }
        
        system("cls");

        switch (scelta) {
        case 1: {
            //MODALITA' FACILE
            generaMappa(mappa, nF);                                                                                                 //Viene generata la mappa con i funghi e gli spazi
            fG = nF;
            xP = 15, yP = 10;                                                                                                       //Le coordinate iniziali sono 15, 10 centrate orizzontalmente                                                                                                       
            nPassi = 200;
            if (nF >= 15) {                                                                                                         //Se sono stati generati tanti funghi, offro al giocatore la possibilità di avere un bonus di 20 passi
                cout << "Hai avuto sfortuna, in questa mappa ci sono parecchi funghi... vuoi un bonus di 20 passi? (S/N) ";
                cin >> bonus;
                if (bonus == 'S' || bonus == 's') {                                                                                 //Può decidere se riscattarlo o no
                    nPassi = nPassi + 20;
                    cout << "I tuoi passi sono aumentati a " << nPassi << ", buona partita!" << endl;
                    system("pause");
                    system("cls");
                }
                else{
                    cout << "Va bene, buona fortuna!" << endl;
                    system("pause");
                    system("cls");
                }
            }
            do {
                cout << "DIFFICOLTA' FACILE | Passi rimasti: " << nPassi << " | Funghi rimasti: " << nF << " | Premi X per uscire " << endl;       //Stampa a schermo dei parametri di gioco
                stampaMappa(mappa);                                                                                                                                             
                mappa[yP][xP] = ' ';                                                                                                               //La cella della matrice contenente la vecchia posizione viene modificata in ' '
                cin >> mossa;
                movimento(mossa, yP, xP, mappa, nPassi);
                if (mappa[yP][xP] == 'f') {                                                                                                        //Quando il giocatore raccoglie il fungo
                    bomba = rand() % 30;
                    if (bomba == 9) {                                                                                             //Una probabilita' su 20 che sia un fungo velenoso
                        settextcolor(12);
                        cout << endl << "Hai raccolto un fungo velenoso, hai perso 10 passi" << endl;
                        Sleep(1000);
                        system("pause");
                        system("cls");
                        nPassi = nPassi - 10;                                                                                                      //Diminuisce il numero di passi disponibili di 10
                        settextcolor(15);
                    }
                    else if (bomba == 1) {                                                                                                         //Una probabilità su 20 che sia un super fungo, che incrementa i passi disponibili di 10
                        settextcolor(14);
                        cout << endl << "Hai raccolto un SUPER FUNGO, hai guadagnato 10 passi!" << endl;
                        Sleep(1000);
                        system("pause");
                        system("cls");
                        nPassi = nPassi + 10;
                    }
                    nF--;                                                                                                                          //Il fungo viene raccolto, e quindi diminuisce il numero dei funghi da raccogliere 
                }
                system("cls");
                mappa[yP][xP] = faccina;                                                                                                           //La nuova posizione del giocatore verra' visualizzata come una faccina (dichiarata a riga 78)                                                                                                
                if (yP >= y || xP >= x || yP < 0 || xP < 0) {
                    cout << "Sei uscito dalla mappa!" << endl;
                    system("pause");
                    nPassi = 0;
                }
            } while (nPassi > 0 && nF > 0);                                                                                                        //Il ciclo si ripete fino a quando il giocatore ha ancora dei passi da poter fare e i funghi sono ancora presenti sulla mappa 

            system("cls");
        }break;
        case 2: {
            //MODALITA' DIFFICILE
            generaMappa(mappa, nF);
            fG = nF;
            xP = 15, yP = 10;
            nPassi = 150;
            do {
                cout << "DIFFICOLTA' DIFFICILE | Passi rimasti: " << nPassi << " | Funghi rimasti: " << nF << " | Premi X per uscire " << endl;                                 
                stampaMappa(mappa);                                                                                                                                          
                cin >> mossa;
                mappa[yP][xP] = 'X';                                                                                                               //La vecchia posizione dei giocatore viene modificata con X, a significare che il giocatore è già passato di lì 
                movimento(mossa, yP, xP, mappa, nPassi);

                if (mappa[yP][xP] == 'f') {                                                                                                                                     
                    bomba = rand() % 20;
                    if (bomba == 9) {                                                                                                               //Non c'è più possibilità che il giocatore raccolga un super fungo 
                        settextcolor(12);
                        cout << endl << "Hai raccolto un fungo velenoso, hai perso 10 passi" << endl;
                        Sleep(1000);
                        system("pause");
                        system("cls");
                        nPassi = nPassi - 10;                                                                                                                                   
                        settextcolor(15);
                    }
                    else if (bomba == 1) {                                                                                                         //Una probabilità su 20 che sia un super fungo, che incrementa i passi disponibili di 10
                        settextcolor(14);
                        cout << endl << "Hai raccolto un SUPER FUNGO, hai guadagnato 10 passi!" << endl;
                        Sleep(1000);
                        system("pause");
                        system("cls");
                        nPassi = nPassi + 10;
                        settextcolor(15);
                    }
                    nF--;
                }
                if (yP >= y || xP >= x || yP < 0 || xP < 0) {
                    cout << "Sei uscito dalla mappa!" << endl;
                    system("pause");
                    nPassi = 0;
                }
                system("cls");
                mappa[yP][xP] = faccina;                                                                                                           //La nuova posizione del giocatore verra' visualizzata come una faccina (dichiarata a riga 78)                                                                                                
            } while (nPassi > 0 && nF > 0);

            system("cls");
        }break;
        case 3: {                                                                                                                                                           
            //MODALITA' IMPOSSIBILE
            generaMappa(mappa, nF);
            fG = nF;
            xP = 15, yP = 10;
            nPassi = 150;
            do {
                cout << "DIFFICOLTA' IMPOSSIBILE | Passi rimasti: " << nPassi << " | Funghi rimasti: " << nF << " | Premi X per uscire " << endl;                            
                stampaMappa(mappa);                                                                                                                                          
                cin >> mossa;
                mappa[yP][xP] = 'X';
                movimento(mossa, yP, xP, mappa, nPassi);

                if (mappa[yP][xP] == 'f') {                                                                                                                                     //lE probabilità di prendere un fungo velenoso o una bomba sono aumentate
                    bomba = rand() % 10;
                    if (bomba <= 2) {                                                                                                                                           
                        settextcolor(12);
                        cout << endl << "Hai raccolto un fungo velenoso, hai perso 10 passi" << endl;
                        Sleep(1000);
                        system("pause");
                        system("cls");
                        nPassi = nPassi - 10;  
                        settextcolor(15);
                    }
                    else if (bomba == 9) {
                        cout << endl << "Non hai raccolto un fungo, ma una bomba! Il gioco finisce adesso" << endl;
                        Sleep(1000);
                        system("pause");
                        system("cls");
                        nPassi = 0;
                    }
                    nF--;
                }
                system("cls");
                mappa[yP][xP] = faccina;                                                                                                                                        //La nuova posizione del giocatore verra' visualizzata come una faccina (dichiarata a riga 78)                                                                                                
                if (yP >= y || xP >= x || yP < 0 || xP < 0) {
                    cout << "Sei uscito dalla mappa!" << endl;
                    system("pause");
                    nPassi = 0;
                }
            } while (nPassi > 0 && nF > 0);

            system("cls");
        }break;
        }
        
        if (nPassi == 0) {
            cout << "HAI PERSO!\nIl risotto sara' senza i funghi :(" << endl;
        }
        else {
            cout << "HAI VINTO! Ti mancavano ancora " << nPassi << " passi\n" << endl;
            cout << "Il tuo punteggio e' " << nPassi * fG << endl;
            classifica[pos] = nPassi * fG;
            ordinamento(classifica, 100);
        }
        cout << "Vuoi fare un'altra partita? (S/N) ";                                                                                                                           //Chiedo al giocatore se vuole giocare di nuovo
        cin >> continua;
        system("cls");
        pos++;
    } while (continua == 'S' || continua == 's');

    if (classifica[0] != 0) {                                                                                                                                                   //Se la classifica non è vuota la stampa
        cout << "CLASSIFICA FINALE\n---------------------------------------------------" << endl;

        for (int i = 0; i < 100; i++) {
            if (classifica[i] != 0) {
                cout << i + 1 << ". " << classifica[i] << " pt " << endl;
            }
        }
    }
    else {
        cout << "Nessun salvataggio sulla classifica, devi fare almeno una partita!" << endl;                                                                                  //Altrimenti scrive che non ci sono partite salvate
    }
    return 0;
}