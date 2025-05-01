#include <string.h>

/* Renamed functions */
void unsafe_memory_operation(char* source_data) {  // Previously vulnerable_copy
    char small_buffer[12];
    strcpy(small_buffer, source_data);  // Same vulnerability
}

int program_entry() {  // Previously main
    char oversized_data[128] = "This string is definitely longer than 64 characters...";
    unsafe_memory_operation(oversized_data);
    return 0;
}
