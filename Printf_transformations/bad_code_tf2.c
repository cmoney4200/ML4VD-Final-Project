#include <stdio.h>
#include <string.h>

#define printLine puts
#define SNPRINTF(dest, size, src) snprintf((dest), (size), "%s", (src))

static void beay() {
    char * qa = "dummy string";
    char * ze = qa;
    printf(ze);
}

//Vulnerable printf file, from original researcher dataset
void CWE134_Uncontrolled_Format_String__char_console_snprintf_31_bad() {
    char dataBuffer[100] = "";
    char * data = dataBuffer;
    size_t dataLen;

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

    {
        char * dataCopy = data;
        {
            char dest[100] = "";
            char * data = dataCopy;
            SNPRINTF((dest), (100 - 1), (data));
            printLine(dest);
        }
    }
}

int main(int argc, char * argv[]) {
    CWE134_Uncontrolled_Format_String__char_console_snprintf_31_bad();
    beay();
    return 0;
}
