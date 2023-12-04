#include <stdio.h>
#include <ctype.h>
#include <string.h>

#define BUFFER_SIZE     256

const char *digit_words[] = {"one", "two", "three", "four", "five", "six", "seven", "eight", "nine"};


static int parse_line(const char *line);
static int try_digit(char digit);
static int try_word(const char* line);


int main(void)
{
    FILE *input = fopen("input.txt", "r");
    char line[BUFFER_SIZE];

    int total = 0;
    for (;;)
    {
        if (fgets(line, BUFFER_SIZE, input) == NULL) break; 
        total += parse_line(line);
    }

    //Expected: 54450
    printf("%d\n", total);

    fclose(input);
    return 0;
}


static int parse_line(const char *line)
{
    int first = 0, last = 0;
    for (const char *p = line; *p; ++p)
    {
        int num = try_digit(*p);

        if (!num)
            num = try_word(p);

        if (num)
        {
            if (!first) first = num;
            last = num;
        }
    }


    printf("%d%d        %s", first, last, line);

    return first * 10 + last;
}

static int try_digit(char digit)
{
    return isdigit(digit) ? digit - '0' : 0;
}

static int try_word(const char *line)
{
    for (int i = 0; i < 9; ++i)
    {
        if (strstr(line, digit_words[i]) == line)
            return i + 1;
    }

    return 0;
}