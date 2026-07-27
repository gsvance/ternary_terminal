all: malbolge original

.PHONY: clean malbolge original

malbolge: malbolge.c
	gcc -Wall -Wextra -Werror -ggdb malbolge.c -o malbolge

original: original.c
	gcc -ggdb original.c -o original

clean:
	rm -rf malbolge original
