CC = clang++
CFLAGS = -O3 -std=c++11
SRCPATH = src
BUILDPATH = build

all: project2

project2: 
	$(CC) $(CFLAGS) $(SRCPATH)/main.cpp -o $(BUILDPATH)/fingerprinting

run: project2
	$(BUILDPATH)/fingerprinting

clean:
	rm -rf $(BUILDPATH)
	mkdir $(BUILDPATH)


