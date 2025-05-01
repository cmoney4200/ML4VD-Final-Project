#include <stdio.h>
#include <string.h>

#define printLine puts
#define SNPRINTF(dest, size, src) snprintf((dest), (size), "%s", (src))

/* Reordered function definitions */
static void beay() {
    char * qa = "dummy string";
    char * ze = qa;
    printf(ze);  // Still vulnerable format string
}

void CWE134_Uncontrolled_Format_String__char_console_snprintf_31_bad() {
    char dataBuffer[100] = "";
    char * data = dataBuffer;
    size_t dataLen;

    /* Reordered while keeping same logic */
    dataLen = strlen(data);
    if ((100 - dataLen) > 1) {
        if ((fgets((data + dataLen), stdin, (int)(100 - dataLen)) != NULL) {
            dataLen = strlen(data);
            if ((data[dataLen - 1] == '\n') && (dataLen > 0)) {
                data[dataLen - 1] = '\0';
            }
        } else {
            printLine("fgets() failed");
            data[dataLen] = '\0';
        }
    }

    /* Reordered but equivalent scope blocks */
    {
        char * dataCopy = data;
        {
            char dest[100] = "";
            char * data = dataCopy;
            SNPRINTF((dest), (100 - 1), (data));  // Still vulnerable
            printLine(dest);
        }
    }
}

/* Main with reordered declaration */
int main(int argc, char * argv[]) {
    CWE134_Uncontrolled_Format_String__char_console_snprintf_31_bad();
    beay();
    return 0;
}
