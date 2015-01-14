


all:
	g++  -c dem_stats.cpp
	g++ -L/usr/lib  dem_stats.o -o dem_stats -lgdal
	g++ -c convert.cpp
	g++ -L/usr/lib convert.o -o convert -lgdal

CXXFLAGS=-std=c++11 -I../SRombauts-SQLiteCpp-91a98f8/include/

polygon:
	g++ -c  polygontest.cpp
	g++ -o polygontest polygontest.o

polygon1:
	g++ $(CXXFLAGS) -c  polygontest1.cpp
	g++ -o polygontest1 polygontest1.o

sqlite: sqlitetest.o
	g++ -o sqlitetest sqlitetest.o -L../SRombauts-SQLiteCpp-91a98f8/build -l SQLiteCpp -l sqlite3 -lboost_regex

