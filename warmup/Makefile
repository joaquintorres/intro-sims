CC = g++
CFLAGS = -O3 -fomit-frame-pointer -ffast-math -march=ivybridge -Wall -Wpedantic 
LDFLAGS = -lm

OBJECT_FILES = main.o warmup.o
EXE = warmup

$(EXE): $(OBJECT_FILES)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $< -o $@

readbin: readbin.cpp warmup.h
	$(CC) $(CFLAGS) -o readbin readbin.cpp
	
clean:
	rm -f $(EXE) $(OBJECT_FILES) *.dat *.bin readbin
