NAME=navigate

all: $(NAME) bruteforce

$(NAME): $(NAME).c
	clang -Wall -Wextra $(NAME).c -o $(NAME)

bruteforce: bruteforce.c
	clang -O3 -Wall -Wextra bruteforce.c -o bruteforce

clean:
	rm -rf *.o $(NAME) bruteforce


