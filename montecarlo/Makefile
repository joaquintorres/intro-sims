CC = g++
CFLAGS = -O3 -fomit-frame-pointer -ffast-math -march=native -Wall -Wpedantic 
LDFLAGS = -lm

OBJECT_FILES_PI = utils.o pi.o main.o 
EXE_PI = pi 

OBJECT_FILES = utils.o montecarlo.o 
EXE = montecarlo 

.PHONY: all
all: $(EXE_PI) $(EXE)

$(EXE_PI): $(OBJECT_FILES_PI)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(EXE): $(OBJECT_FILES)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $< -o $@


clean:
	rm -f $(EXE) $(EXE_PI) $(OBJECT_FILES) *.dat *.bin 
