OBJECT=basedb.o inifile.o for_my_occi.o MyDate.o my_occi.o LoadForNoTem.o LoadDay.o
ORA_PATH=/usr/local/oracle_client_12_1
LDFLAGS=-L $(ORA_PATH)/lib  -lclntsh
CFLAGS=-g -I $(ORA_PATH)/sdk/include
CC=g++
for_my_occi:$(OBJECT)
	$(CC) -o for_my_occi $(OBJECT) $(LDFLAGS)
for_my_occi.o:for_my_occi.cpp inifile.h LoadForNoTem.h MyDate.h my_occi.h
	$(CC) -o for_my_occi.o -c for_my_occi.cpp $(CFLAGS)
my_occi.o:my_occi.cpp my_occi.h
	$(CC) -o my_occi.o -c my_occi.cpp $(CFLAGS)
LoadForNoTem.o:LoadForNoTem.cpp LoadForNoTem.h LoadDay.h MyDate.h
	$(CC) -o LoadForNoTem.o -c LoadForNoTem.cpp $(CFLAGS)
LoadDay.o:LoadDay.cpp LoadDay.h
	$(CC) -o LoadDay.o -c LoadDay.cpp $(CFLAGS)
MyDate.o:MyDate.cpp MyDate.h
	$(CC) -o MyDate.o -c MyDate.cpp $(CFLAGS)
inifile.o:inifile.cpp inifile.h
	$(CC) -o inifile.o -c inifile.cpp $(CFLAGS)
basedb.o:basedb.cpp basedb.h
	$(CC) -o basedb.o -c basedb.cpp $(CFLAGS)
clean:
	-rm *.o forcast
release:
	-rm $(OBJECT)
