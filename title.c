#include "define.h"
#include "title.h"

//stampa la schermata iniziale, chiama il menu, restituisce la scelta selezionata dal menu
int title(){
	int choice;	//variabile da restituire
	int maxcol, maxrow;
	char titolo[8][70] = {"                            _                       _               ",
						  "                           (_)                     | |              ",
						  " ___ _ __   __ _  ___ ___   _ _ __ __   __ __ _  __| | ___ _ __ ___ ",
						  "/ __| '_ \\ / _` |/ __/ _ \\ | | '_  \\ \\ / // _` |/ _` |/ _ \\ '__| __|",
						  "\\__ \\ |_) | (_| | (_|  __/ | | | | |\\ v /| (_| | (_| |  __/ |  \\__ \\",
						  "|___/ .__/ \\__,_|\\___\\___| |_|_| |_| \\_/  \\__,_|\\__._|\\___|_|  |___/",
						  "    | |                                                             ",
						  "    |_|                                                             "};
	
	/*
                            _                       _                
                           (_)                     | |              
 ___ _ __   __ _  ___ ___   _ _ __ __   __ __ _  __| | ___ _ __ ___ 
/ __| '_ \ / _` |/ __/ _ \ | | '_ \\ \ / // _` |/ _` |/ _ \ '__| __|
\__ \ |_) | (_| | (_|  __/ | | | | |\ v /| (_| | (_| |  __/ |  \__ \
|___/ .__/ \__,_|\___\___| |_|_| |_| \_/  \__,_|\__._|\___|_|  |___/
    | |                                                             
    |_|                                                             

	*/

	getmaxyx(stdscr, maxrow, maxcol);
	
	//stampa il titolo riga per riga, con un pausa per farlo comparire dall'alto verso il basso
	for(int j = 0; j < 8; j++){
		attron(COLOR_PAIR(2));
		mvprintw(maxrow/5+j, maxcol/2 -34, "%s", titolo[j]);
		attroff(COLOR_PAIR(2));

		refresh();
		usleep(100000);
	}
	
	//stampa dettagli 
	attron(COLOR_PAIR(3));
	mvprintw(maxrow-3, maxcol-19, "Creato da:");
	mvprintw(maxrow-2, maxcol-30, "Cristina Pinna, Elena Puggioni");
	attroff(COLOR_PAIR(3));
	refresh();
	getchar();
	
	//chiama il menu
	choice = menu();
	
	clear();
	return choice;
}

//menu per selezionare start o quit, restituisce la scelta
int menu(){
	WINDOW *menu_win; //finestra del menu
 	int maxrow, maxcol;
 	getmaxyx(stdscr, maxrow, maxcol);
	int startx = maxcol/2 -18; //x dell'angolo in alto a sinistra del menu
	int starty = maxrow/5 +10; //y dell'angolo in alto a sinistra del menu
	int n_choices = 2;
	int highlight = 1; 
	
	int choice = 0;	//variabile da restituire
	int c;
	char *choices[] = {
                        "           S t a r t           ",
                        "            Q u i t            ",
						};
	//inizializza finestra del menu di altezza 6 e larghezza 36 con l'angolo in alto a sx nel punto (startx, starty)
	menu_win = newwin(6, 35, starty, startx);
	keypad(menu_win, TRUE);

	//stampa menu
	print_menu(choices, menu_win, highlight);
 	refresh();
	
	while(1){
		c = wgetch(menu_win); //scelta dell'utente
		
		switch(c){
			case KEY_UP:	//muove il cursore in alto
				if(highlight == 1)	//se premi su ma siamo all'inizio della lista, torna alla fine 
					highlight = n_choices;
				else
					--highlight;
				break;
			case KEY_DOWN:	//muove il cursore in basso
				if(highlight == n_choices) //se premi giù ma siamo alla fine della lista, torna all'inizio
					highlight = 1;
				else 
					++highlight;
				break;
			case 10:	//se premi enter, choice prende il valore attualmente evidenziato
				choice = highlight;
				break;
			default:
				break;
		}
		//stampa menu ad ogni ciclo
		print_menu(choices, menu_win, highlight);
		
		if(choice != 0)	//se choice != 0 l'utente ha selezionato qualcosa e si esce dal loop
			break;
	}

	//pulisce il buffer da tastiera
	clrtoeol();
	refresh();
	
	return choice; 
}

//stampa il menu
void print_menu(char *choices[], WINDOW *menu_win, int highlight){
	int x, y, i;	

	x = 2;
	y = 2;
	
	box(menu_win, 0, 0);
	
	for(i = 0; i < 2; ++i)
	{	if(highlight == i + 1) //se siamo all'elemento da evidenziare lo evidenzia
		{	wattron(menu_win, A_REVERSE); //A_REVERSE scambia i colori di carattere e sfondo
			mvwprintw(menu_win, y, x, "%s", choices[i]);	//stampa la stringa choices[i] nel punto x,y (la y si incrementa ogni ciclo)
			wattroff(menu_win, A_REVERSE);
		}
		else	//stampa elemento non evidenziato
			mvwprintw(menu_win, y, x, "%s", choices[i]);
		++y;
	}
	wrefresh(menu_win);	//refresh della finestra
}

//stampa schermata hai vinto
void hai_vinto(){
	int maxcol, maxrow;
	char vinto[6][70] = {" _    _          _____     __      _______ _   _ _______ ____  _ ",
				"| |  | |   /\\   |_   _|    \\ \\    / /_   _| \\ | |__   __/ __ \\| |",
				"| |__| |  /  \\    | |       \\ \\  / /  | | |  \\| |  | | | |  | | |",
				"|  __  | / /\\ \\   | |        \\ \\/ /   | | | . ` |  | | | |  | | |",
				"| |  | |/ ____ \\ _| |_        \\  /   _| |_| |\\  |  | | | |__| |_|",
				"|_|  |_/_/    \\_\\_____|        \\/   |_____|_| \\_|  |_|  \\____/(_)"};
	
	char congrats[3][40] = {" _ _  _  _  _ _ _|_  | _ _ . _  _ .|",
				   "(_(_)| |(_|| (_| ||_||(_|/_|(_)| ||.",
				   "         _|                         "};
	/*
 _    _          _____     __      _______ _   _ _______ ____  _ 
| |  | |   /\   |_   _|    \ \    / /_   _| \ | |__   __/ __ \| |
| |__| |  /  \    | |       \ \  / /  | | |  \| |  | | | |  | | |
|  __  | / /\ \   | |        \ \/ /   | | | . ` |  | | | |  | | |
| |  | |/ ____ \ _| |_        \  /   _| |_| |\  |  | | | |__| |_|
|_|  |_/_/    \_\_____|        \/   |_____|_| \_|  |_|  \____/(_)


 _ _  _  _  _ _ _|_  | _ _ . _  _ .|
(_(_)| |(_|| (_| ||_||(_|/_|(_)| ||.
         _|                         

	*/
	
	getmaxyx(stdscr, maxrow, maxcol);
	
	//stampa hai vinto
	for(int j = 0; j < 6; j++){
		attron(COLOR_PAIR(4));
		mvprintw(maxrow/5+j, maxcol/2 -33, "%s", vinto[j]);
		attroff(COLOR_PAIR(4));

		refresh();
		usleep(100000);
	}
	//stampa congratulazioni
	for(int j = 0; j < 3; j++){
		attron(COLOR_PAIR(5));
		mvprintw(maxrow/5+j+7, maxcol/2 -17, "%s", congrats[j]);
		attroff(COLOR_PAIR(5));

		refresh();
		usleep(100000);
	}
	//stampa premi F1 per uscire
	do{
		attron(COLOR_PAIR(3));
		mvprintw(maxrow-2, maxcol-22, "premi F1 per uscire...");
		attron(COLOR_PAIR(3));
		refresh();
	}while(getch() != KEY_F(1));
}

//stampa schermata hai perso
void hai_perso(){
int maxcol, maxrow;
	char perso[6][70] = {" _    _          _____      _____  ______ _____   _____  ____  ",
						"| |  | |   /\\   |_   _|    |  __ \\|  ____|  __ \\ / ____|/ __ \\ ",
						"| |__| |  /  \\    | |      | |__) | |__  | |__) | (___ | |  | |",
						"|  __  | / /\\ \\   | |      |  ___/|  __| |  _  / \\___ \\| |  | |",
						"| |  | |/ ____ \\ _| |_     | |    | |____| | \\ \\ ____) | |__| |",
						"|_|  |_/_/    \\_\\_____|    |_|    |______|_|  \\_\\_____/ \\____/ ",};
	
	char peccato[3][40] = {" _  _  _ _ _ _|_ _    ",
						  "|_)(/_(_(_(_| | (_)...",
						  "|                     "};
	/*
 _    _          _____      _____  ______ _____   _____  ____  
| |  | |   /\   |_   _|    |  __ \|  ____|  __ \ / ____|/ __ \ 
| |__| |  /  \    | |      | |__) | |__  | |__) | (___ | |  | |
|  __  | / /\ \   | |      |  ___/|  __| |  _  / \___ \| |  | |
| |  | |/ ____ \ _| |_     | |    | |____| | \ \ ____) | |__| |
|_|  |_/_/    \_\_____|    |_|    |______|_|  \_\_____/ \____/ 

 _  _  _ _ _ _|_ _    
|_)(/_(_(_(_| | (_)...
|                     
	*/
	
	getmaxyx(stdscr, maxrow, maxcol);
	
	//stampa hai perso
	for(int j = 0; j < 6; j++){
		attron(COLOR_PAIR(2));
		mvprintw(maxrow/5+j, maxcol/2 -33, "%s", perso[j]);
		attroff(COLOR_PAIR(2));

		refresh();
		usleep(100000);
	}
	//stampa peccato
	for(int j = 0; j < 3; j++){
		attron(COLOR_PAIR(7));
		mvprintw(maxrow/5+j+7, maxcol/2 -13, "%s", peccato[j]);
		attroff(COLOR_PAIR(7));

		refresh();
		usleep(100000);
	}
	//stampa premi F1 per uscire
	do{
		attron(COLOR_PAIR(3));
		mvprintw(maxrow-2, maxcol-22, "premi F1 per uscire...");
		attroff(COLOR_PAIR(3));
		refresh();
	}while(getch() != KEY_F(1));
}
