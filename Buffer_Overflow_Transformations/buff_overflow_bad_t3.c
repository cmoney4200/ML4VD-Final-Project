#include <string.h>

//Our own buffer overflow vulnerability code
void unsafe_memory_operation(char* source_data) { 
    char small_buffer[12];
    //Bounds not checked, vulnerable
    strcpy(small_buffer, source_data);
}

int program_entry() {
    char oversized_data[128] = "This string is definitely longer than 12 characters...";
    unsafe_memory_operation(oversized_data);
    return 0;
}
