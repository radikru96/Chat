CS=g++ -g -Wall -lmysqlclient -pthread
RS=g++ -Wall -lmysqlclient -pthread
CC=g++ -g -Wall -lncurses -pthread
RC=g++ -Wall -lncurses -pthread

test:bin/server_test
debug: bin/server_debug bin/client_debug
release:bin/server bin/client


bin/server_test: obj/server_test.o obj/libserver.o obj/dbaccess.o
	$(CS) obj/server_test.o obj/dbaccess.o obj/libserver.o -o bin/server_test

obj/server_test.o: src/server_test.cpp src/dbaccess.h src/libserver.h src/structures.h
	$(CS) -c src/server_test.cpp -o obj/server_test.o


bin/server_debug: obj/server.o obj/libserver.o obj/dbaccess.o
	$(CS) obj/server.o obj/libserver.o obj/dbaccess.o -o bin/server_debug

bin/server: obj/server.o obj/libserver.o obj/dbaccess.o
	$(RS) obj/server.o obj/dbaccess.o -o bin/server

obj/server.o: src/server.cpp src/dbaccess.h src/libserver.h src/structures.h
	$(CS) -c src/server.cpp -o obj/server.o


bin/client_debug: obj/client.o
	$(CC) obj/client.o -o bin/client_debug

bin/client: obj/client.o
	$(RC) obj/client.o -o bin/client

obj/client.o: src/client.cpp src/structures.h
	$(CC) -c src/client.cpp -o obj/client.o


obj/libserver.o: src/libserver.cpp src/libserver.h src/dbaccess.h src/dbaccess.cpp src/structures.h
	$(CS) -c src/libserver.cpp -o obj/libserver.o

obj/dbaccess.o: src/dbaccess.cpp src/dbaccess.h src/structures.h
	$(CS) -c src/dbaccess.cpp -o obj/dbaccess.o

clean:
	rm -rf obj/*o bin/*