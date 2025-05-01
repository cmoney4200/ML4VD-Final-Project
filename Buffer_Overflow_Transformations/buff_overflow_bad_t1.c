#include <string.h>

void vulnerable_copy(char* token7) {
    char buffer[12];
    //Bounds not checked, vulnerable
    strcpy(buffer, token7);
}

int main() {
    char large_input[128] = "This string is definitely longer than 12 characters...";
    vulnerable_copy(large_input);
    return 0;
}
