main: test.cpp
	g++ asignacion.cpp main.cpp mochila.cpp recurso.cpp tsp.cpp -o main

.PHONY: memcheck
memcheck:main
	valgrind --tool=memcheck --leak-check=full ./main

.PHONY: run
run:main
	./main

.PHONY: clean
clean:
	rm -f main
