/*============================================================================
| Assignment: pa02 -
|
| Author: Mizbah Ateeq
| Language: c
| Class: CIS3360 - Security in Computing - Summer 2024
| Instructor: McAlpin
| Due Date: July 7, 2024
===========================================================================*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// calculate 8 bit check sum
int 8bit_checksum(char *input) {
    int 8bit=0;
    int len =(int)strlen(input);
    for (int i=0; i<len; i++) {
        8bit+=input[i]
    }
    return 8bit;
}

int 16bit_checksum (char *input) {
    int 16bit=0;
    int len =(int)strlen(input);
    for (int i=0; i<len; i+=2) {
        16bit+=input[i]<<8;
        16bit += (input[i + 1]);
    }



}
void print_error(const char *message) {
    fprintf(stderr, "%s\n", message);
    exit(1);
}

void echo_input_file(const char *data) {
    size_t len = strlen(data);
    for (size_t i = 0; i < len; i += 80) {
        printf("%.80s\n", data + i);
    }
}

unsigned long calculate_checksum(const char *data, int checksum_size) {
    unsigned long checksum = 0;
    size_t len = strlen(data);

    if (checksum_size == 8) {
        for (size_t i = 0; i < len; ++i) {
            checksum = (checksum + (unsigned char)data[i]) % 256;
        }
    } else if (checksum_size == 16) {
        for (size_t i = 0; i < len; i += 2) {
            unsigned short word = (data[i] << 8) + (i + 1 < len ? data[i + 1] : 'X');
            checksum = (checksum + word) % 65536;
        }
    } else if (checksum_size == 32) {
        for (size_t i = 0; i < len; i += 4) {
            unsigned long dword = (data[i] << 24) +
                                  ((i + 1 < len ? data[i + 1] : 'X') << 16) +
                                  ((i + 2 < len ? data[i + 2] : 'X') << 8) +
                                  (i + 3 < len ? data[i + 3] : 'X');
            checksum = (checksum + dword) % 4294967296;
        }
    }

    return checksum;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <filename> <checksum size>\n", argv[0]);
        return 1;
    }

    const char *filename = argv[1];
    int checksum_size = atoi(argv[2]);

    if (checksum_size != 8 && checksum_size != 16 && checksum_size != 32) {
        print_error("Valid checksum sizes are 8, 16, or 32");
    }

    FILE *file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Error: Could not open file %s\n", filename);
        return 1;
    }

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *data = (char *)malloc(file_size + 1);
    if (!data) {
        fclose(file);
        print_error("Memory allocation error");
    }

    fread(data, 1, file_size, file);
    data[file_size] = '\0';
    fclose(file);

    echo_input_file(data);

    unsigned long checksum = calculate_checksum(data, checksum_size);
    size_t character_count = strlen(data);

    printf("%2d bit checksum is %08lx for all %4lu chars\n", checksum_size, checksum, character_count);

    return 0;
}


/*=============================================================================
| I Mizbah Ateeq [mi780306] affirm that this program is
| entirely my own work and that I have neither developed my code together with
| any another person, nor copied any code from any other person, nor permitted
| my code to be copied or otherwise used by any other person, nor have I
| copied, modified, or otherwise used programs created by others. I acknowledge
| that any violation of the above terms will be treated as academic dishonesty.
+=============================================================================*/
