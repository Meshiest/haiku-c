.PHONY: all clean build run

all: clean build

build: haiku

clean:
	rm -f haiku

run:
	./haiku
