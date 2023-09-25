CC = g++
CFLAGS = -O3 -fomit-frame-pointer -ffast-math -march=native -Wall -Wpedantic 
LDFLAGS = -lm

OBJECT_FILES = utils.o pi.o main.o 
EXE = pi 

$(EXE): $(OBJECT_FILES)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(EXE) $(OBJECT_FILES) *.dat *.bin 
