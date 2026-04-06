#include "input.h"

char *remove_enter(const char *line, char new_line[]) {
    char c = line[0];
    int index = 0;

    while (c != '\n' && c != '\0') {
        new_line[index] = c;
        index++;
        c = line[index];
    }

    new_line[index] = '\0';
    return new_line;
}

static int count_chars(const char *line) {
    int result = 0;
    char c = line[result];

    while (c != '\0') {
        c = line[++result];
    }

    return result;
}

static char *remove_first_spaces(const char *line, char new_line[]) {
    char c = line[0];
    int index_old = 0;
    int index_new = 0;

    enum State { START_INPUT, INPUT };
    enum State s = START_INPUT;

    while (c != '\0') {
        if (s == START_INPUT) {
            while (c == ' ') {
                index_old++;
                c = line[index_old];
            }
            s = INPUT;
        } else {
            new_line[index_new] = c;
            index_new++;
            index_old++;
            c = line[index_old];
        }
    }

    new_line[index_new] = '\0';
    return new_line;
}

static char *remove_last_spaces(const char *line, char new_line[]) {
    int line_len = count_chars(line);

    if (line_len == 0) {
        new_line[0] = '\0';
        return new_line;
    } else if (line_len == 1) {
        if (line[0] != ' ') {
            new_line[0] = line[0];
            new_line[1] = '\0';
        } else {
            new_line[0] = '\0';
        }
        return new_line;
    }

    int index = line_len - 1;
    char c = line[index];
    new_line[0] = '\0';

    enum State { END_INPUT, INPUT };
    enum State s = END_INPUT;

    while (index != 0) {
        if (s == END_INPUT) {
            while (c == ' ' && index > 0) {
                index--;
                c = line[index];
            }
            s = INPUT;
            new_line[index + 1] = '\0';
        } else {
            new_line[index] = c;
            index--;
            c = line[index];
        }
    }

    new_line[0] = line[0];
    return new_line;
}

static char *remove_last_and_first_spaces(const char *line, char new_line[]) {
    char temp[100];
    remove_first_spaces(line, temp);
    remove_last_spaces(temp, new_line);
    return new_line;
}

char *normalize(const char *line, char *normalized_line) {
    char no_enter_line[100];
    remove_enter(line, no_enter_line);
    remove_last_and_first_spaces(no_enter_line, normalized_line);
    return normalized_line;
}

