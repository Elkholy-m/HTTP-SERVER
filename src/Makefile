CC = gcc
CFLAGS = -Wall -Wextra
OUTDIR = out
ODIR = obj
INCLUDE = common.c

build_all: tcp_server tcp_client

run_all: run_server run_client

run_server: tcp_server
	@$(OUTDIR)/tcp_server

tcp_server: tcp_server.o
	@$(CC) -o $(OUTDIR)/$@ tcp_server.c $(INCLUDE) $(CFLAGS)

tcp_server.o: common.o
	@$(CC) -c -o $(ODIR)/$@ tcp_server.c $(CFLAGS)

run_client: tcp_client
	@$(OUTDIR)/tcp_client

tcp_client: tcp_client.o
	@$(CC) -o $(OUTDIR)/$@ tcp_client.c $(INCLUDE) $(CFLAGS)

tcp_client.o: common.o
	@$(CC) -c -o $(ODIR)/$@ tcp_client.c $(CFLAGS)

ftp_client:
	@gcc -o out/ftp_client ftp_client.c common.c && out/ftp_client

ftp_server:
	@gcc -o out/ftp_server ftp_server.c common.c && out/ftp_server

common.o: 
	@$(CC) -c -o $(ODIR)/$@ common.c $(CFLAGS)

.PHONY: clean

clean:
	@rm -f $(ODIR)/*.o
	@rm -f $(OUTDIR)/*
