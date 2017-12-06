test_matrix: test_matrix.o
	g++ -g -Wall $^ -o $@

test_matrix.o: test_matrix.cpp matrix.h
	g++ -g -c -Wall $< -o $@

.PHONY: clean

clean:
	-rm test_matrix.o testmatrix
