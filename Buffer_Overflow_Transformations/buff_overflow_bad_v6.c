#include <string.h>

void perform_unsafe_copy(char* dest, char* src) {
    strcpy(dest, src);
}

void vulnerable_copy(char* input) {
    char buffer[12];
    //Bounds not checked, vulnerable
    perform_unsafe_copy(buffer, input);
}

int main() {
    char large_input[128] = "This string is definitely longer than 12 characters...";
    vulnerable_copy(large_input);
    return 0;
}
