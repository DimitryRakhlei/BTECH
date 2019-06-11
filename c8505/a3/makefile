CC          =g++
CLIB        =-std=c++11 -lpcap -lboost_system -lpthread -lsodium
#
CFLAG       =-Wall
PROG_NAME   =NotABackdoor

all: $(PROG_NAME)

$(PROG_NAME): main.o pkt_sniffer.o proc_ether.o proc_hdrs.o proc_payload.o encrypt.o
	$(CC) $(CFLAGS) -o bin/$(PROG_NAME) build/main.o build/pkt_sniffer.o build/proc_ether.o \
	build/proc_hdrs.o build/proc_payload.o build/simpleencrypt.o  $(CLIB)

#
#
#	sudo chown root bin/$(PROG_NAME)
#	sudo chmod +s bin/$(PROG_NAME)

main.o: src/main.cpp
	$(CC) $(CFLAGS) -c src/main.cpp -o build/main.o $(CLIB)

encrypt.o: src/encrypt.cpp src/checkentropy.cpp
	#$(CC) $(CFLAGS) -c src/checkentropy.cpp -o build/checkentropy.o $(CLIB)
	#$(CC) $(CFLAGS) -c src/encrypt.cpp -o build/encrypt.o $(CLIB)
	$(CC) $(CFLAGS) -c src/simpleencrypt.cpp -o build/simpleencrypt.o $(CLIB)

pkt_sniffer.o: proc_ether.o
	$(CC) -c src/libpcap/pkt_sniffer.cpp -o build/pkt_sniffer.o 
proc_ether.o: 
	$(CC) -c src/libpcap/proc_ether.cpp -o build/proc_ether.o
proc_hdrs.o: 
	$(CC) -c src/libpcap/proc_hdrs.cpp -o build/proc_hdrs.o
proc_payload.o: 
	$(CC) -c src/libpcap/proc_payload.cpp -o build/proc_payload.o


clean:
	rm -f bin/$(PROG_NAME) build/*.o