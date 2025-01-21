compile: game.o player.o
	gcc -o game game.o
	gcc -o player player.o
game.o: game.c
	gcc -c game.c
player.o: player.c
	gcc -c player.c
games:
	./game
play:
	./player
clean:
	rm -f *.o
	rm -f game
	rm -f player
	rm -f gameProgress.txt 
	rm -f shuffledArray.txt
