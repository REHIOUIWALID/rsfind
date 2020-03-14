
rsfind: rsfind.c rechercherec.o Affichage_ls.o tirett.o tiretG.o expe.o image.o 
	gcc -Wall -I. -Werror -Wextra -Wformat -o rsfind rsfind.c rechercherec.o Affichage_ls.o expe.o tirett.o tiretGrandT.o image.o -ldl -pthread -lpcre

rechercherec.o: rechercherec.c
	gcc -Wall -c rechercherec.c

Affichage_ls.o: Affichage_ls.c
	gcc -Wall -c Affichage_ls.c

tirett.o: tirett.c
	gcc -Wall -c tirett.c

tiretG.o: tiretGrandT.c
	gcc -Wall -c tiretGrandT.c

expe.o: expe.c
	gcc -Wall -c expe.c

image.o: image.c
	gcc -Wall -c image.c

test: test.sh
	bash ./test.sh

clean:
	rm *.o
	rm rsfind
