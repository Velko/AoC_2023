NAME=boats

all: $(NAME) equations.pdf

$(NAME): $(NAME).c
	clang -Wall -Wextra -lm $(NAME).c -o $(NAME)

equations.pdf: equations.tex
	pdflatex equations.tex

clean:
	rm -rf *.o $(NAME) *.aux *.log *.pdf


