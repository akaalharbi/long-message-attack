LDLIBS  = -lm
CFLAGS  = -O3 -Wall -march=native -msse4 -msha -DNPROBES_COUNT -std=c11 -fopenmp
LDFLAGS = -msse4 -msha -fopenmp
objects = benchmark.o sha256-x86.o vsha256.o dict.o util/memory.o util/util_char_arrays.o 



all: benchmark

benchmark: $(objects)



benchmark.o: types.h util/util_char_arrays.h shared.h sha256-x86.h
sha256-x86.o: sha256-x86.h
dict.o: dict.h types.h shared.h
util/memory.o: util/memory.h
util/util_char_arrays.o: util/util_char_arrays.h
vsha256.o: vsha256.h

.PHONY: clean
clean:
	rm -f $(objects)
