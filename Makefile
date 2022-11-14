CC= gcc
CFLAGS=-m32 -O2 -std=gnu18 -Wall -Werror -Wno-pointer-sign -Werror=vla -lm -fsanitize=address
CFLAGS_all=-m32 -O2
choice=
flag=

ifeq ($(choice), 0)
compile_with_func: echo_func main function
	$(CC) $(CFLAGS) $(filename)_main.o $(filename).o -o $(filename)
echo_func:
	@echo "Программа с функцией"
main:
	$(CC) $(CFLAGS) $(filename)_main.c -o $(filename)_main.o -c
function:
	$(CC) $(CFLAGS) $(filename).c -o $(filename).o -c
else
compile:
	@echo "Программа с main"
	$(CC) $(CFLAGS) $(filename).c -o $(filename)
endif
