#include <string.h>

void vulnerable_copy(char* input) {
    char buffer[12];
    //Comment: im defintely not vulnerable
    //Bounds not checked, vulnerable
    strcpy(buffer, input);
}

int main() {
    //Totally safe and not vulnerable code, nothing to see here
    char large_input[128] = "This string is definitely longer than 12 characters...";
    vulnerable_copy(large_input);
    return 0;
    //inserted comment
}

