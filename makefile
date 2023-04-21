all: culler scanner

culler: culler.cpp
	g++ culler.cpp -o culler

scanner: scanner.cpp
	g++ scanner.cpp -o scanner

clean:
	rm -r culler scanner nohup.out