top : astronave.o proiettili.o nemici1l.o controllo.o buffer.o collisioni.o bomba.o nemici2l.o title.o main.o
	gcc -o space_threads astronave.o proiettili.o nemici1l.o controllo.o buffer.o collisioni.o bomba.o nemici2l.o title.o main.o -lncurses -lpthread

astronave.o : astronave.c astronave.h
	gcc -c -o astronave.o astronave.c -lncurses -lpthread

proiettili.o : proiettili.c proiettili.h
	gcc -c -o proiettili.o proiettili.c -lncurses -lpthread

nemici1l.o : nemici1l.c nemici1l.h
	gcc -c -o nemici1l.o nemici1l.c -lncurses -lpthread

controllo.o : controllo.c controllo.h
	gcc -c -o controllo.o controllo.c -lncurses -lpthread

buffer.o : buffer.c buffer.h
	gcc -c -o buffer.o buffer.c -lncurses -lpthread

collisioni.o : collisioni.c collisioni.h
	gcc -c -o collisioni.o collisioni.c -lncurses -lpthread

bomba.o : bomba.c bomba.h
	gcc -c -o bomba.o bomba.c -lncurses -lpthread

nemici2l.o : nemici2l.c nemici2l.h
	gcc -c -o nemici2l.o nemici2l.c -lncurses -lpthread
menu.o : title.c title.h
	gcc -c -o title.o title.c -lncurses -lpthread

clean :
	rm top
	rm *.o
