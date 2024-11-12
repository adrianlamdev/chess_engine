CXX = g++
CXXFLAGS = -std=c++11
EXES = main

main: main.o chess_board.o magics.o
	$(CXX) $(CXXFLAGS) -o main main.o chess_board.o magics.o

main.o: main.cpp
	$(CXX) $(CXXFLAGS) -c main.cpp

chess_board.o: ./src/chess_board/chess_board.cpp ./src/chess_board/chess_board.h
	$(CXX) $(CXXFLAGS) -c ./src/chess_board/chess_board.cpp

magics.o: ./src/magics/magics.cpp ./src/magics/magics.h
	$(CXX) $(CXXFLAGS) -c ./src/magics/magics.cpp

all: $(EXES)

clean:
	rm -f $(EXES) *.o

.PHONY: all clean
