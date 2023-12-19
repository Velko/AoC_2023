NAME=lagoon

all: $(NAME) $(NAME)-floodfill

$(NAME): $(NAME).c
	clang -Wall -Wextra $(NAME).c -o $(NAME)

$(NAME)-floodfill: $(NAME)-floodfill.c
	clang -Wall -Wextra $(NAME)-floodfill.c -o $(NAME)-floodfill

clean:
	rm -rf *.o $(NAME) $(NAME)-floodfill


