#include <string.h>

void vulnerable_copy(char* input, int unused_param) 
{
    char buffer[12];
    strcpy(buffer, input);  // Original parameter order (required for vulnerability)
}

int main(int argc, char** argv) 
{
    char large_input[128] = "This string is definitely longer than 12 characters";
    vulnerable_copy(large_input, 0);  // Added unused parameter
    return 0;
}
