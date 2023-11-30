all: main multi_threaded

main: main.cpp
	g++ -O1 main.cpp -o main
	g++ -g main.cpp -o mainDebug

multi_threaded: multi_threaded.cpp
	g++ -O1 multi_threaded.cpp -o multi_threaded
	g++ -g multi_threaded.cpp -o multi_threadedDebug

clean:
	rm -f main multi_threaded mainDebug multi_threadedDebug