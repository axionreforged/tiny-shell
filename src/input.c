#include "input.h"
#include <string.h>

char *normalize(const char *line, char *normalized_line) {
    int enter_index = strcspn(line, "\n");
    int left_no_space_index = 0;
    int right_no_space_index = enter_index - 1;

    if(right_no_space_index == -1){
        normalized_line[0] = '\0';
        return normalized_line;
    }

    while (line[left_no_space_index] == ' ' && line[left_no_space_index] != '\0') left_no_space_index++;
    if(line[left_no_space_index] == '\0'){
        normalized_line[0] = '\0';
        return normalized_line;
    }

    while(line[right_no_space_index] == ' ' && right_no_space_index > 0) right_no_space_index--;
    if(line[right_no_space_index] == ' '){
        normalized_line[0] = '\0';
        return normalized_line;
    }

    int count = right_no_space_index - left_no_space_index + 1;
    strncpy(normalized_line, line + left_no_space_index, count);
    normalized_line[count] = '\0';
    
    return normalized_line;
}

