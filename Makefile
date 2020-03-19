name= pthreader
links= -pthread
flags= -Wall -Wextra
cc= g++ -Wall

build:
	$(cc) $(flags) $(links) -o $(name) src/main.cpp

test:
	$(cc) $(flags) $(links) -B src test/main.cpp

