bin=./bin/
src=./src/


server_src=$(src)server/server.c
#client_src=$(src)client/client.c
thread_src=$(src)pool/

server_bin=$(bin)server
client_bin=$(bin)client


libs= -lpthread -fopenmp -ggdb -lev $(thread_src)libthreadpool.so
flags= -W -Wall


all: $(server_src) $(client_src) $(utils_src)
	$(MAKE) -C $(thread_src)
	$(CC) $(thread_src)libthreadpool.a $(server_src) -o $(server_bin) $(libs) $(flags)
#	$(CC) $(client_src) -o $(client_bin) $(libs) $(flags)


clean:
	rm -f $(bin)*
	rm -f ./logs/*
	$(MAKE) -C $(thread_src) clean
	
