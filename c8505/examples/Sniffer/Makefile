
# Makefile for pkt_sniffer

CC=gcc -g
CLIB=-lpcap

pkt_sniffer: pkt_sniffer.o proc_ether.o proc_hdrs.o proc_payload.o
	$(CC) -o pkt_sniffer pkt_sniffer.o proc_ether.o proc_hdrs.o proc_payload.o $(CLIB)

clean:
	rm -f *.o core pkt_sniffer

pkt_sniffer.o:
	$(CC) -c pkt_sniffer.c

proc_ether.o: 
	$(CC) -c proc_ether.c

proc_hdrs.o: 
	$(CC) -c proc_hdrs.c
proc_payload.o: 
	$(CC) -c proc_payload.c
