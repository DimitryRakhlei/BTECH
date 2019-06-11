all: bin bin/client bin/server
	
bin:
	mkdir -p bin
	
bin/util: bin src/util.c include/packet.h
	cc src/util.c -o bin/util -c -fPIC

bin/client: bin src/covert_client.c include/packet.h bin/util
	cc src/covert_client.c bin/util -o bin/client

bin/server: bin src/covert_server.c include/packet.h bin/util
	cc src/covert_server.c bin/util -o bin/server
clean:
	rm bin/*