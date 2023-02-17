CPP_FLAGS='-std=c++11'
perceptron:perceptron_algorithm.o
	g++ ${CPP_FLAGS} perceptron_algorithm.o -o perceptron
perceptron_algorithm.o: perceptron_algorithm.cpp
	g++ ${CPP_FLAGS} -c perceptron_algorithm.cpp 
clean:
	rm -f perceptron perceptron_algorithm.o