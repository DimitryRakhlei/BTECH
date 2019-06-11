all: mpmake mtmake

mpmake: ./src/mp.c ./src/primedecompose.c
	gcc ./src/mp.c ./src/primedecompose.c -o ./bin/mp -fopenmp -lgmp -ggdb

mtmake: ./src/mt.c ./src/primedecompose.c
	gcc ./src/mt.c ./src/primedecompose.c -o ./bin/mt -lpthread -lgmp -ggdb

clean:
	rm ./bin/mp
	rm ./bin/mt
