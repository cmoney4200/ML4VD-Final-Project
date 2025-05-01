#include <stdio.h>
#include <string.h>

#define SNPRINTF snprintf

void CWE134_Uncontrolled_Format_String__char_console_snprintf_31_good()
{
    char * data;
    char dataBuffer[100] = "";
    data = dataBuffer;
    
    //Safe input reading w/ format string validation
    {
        size_t dataLen = strlen(data);
        if (100-dataLen > 1)
        {
            if (fgets(data+dataLen, (int)(100-dataLen), stdin) != NULL)
            {
                dataLen = strlen(data);
                if (dataLen > 0 && data[dataLen-1] == '\n')
                {
                    data[dataLen-1] = '\0';
                }
            }
        }
    }
    
    //Safely handled printf
    {
        char dest[100] = "";
        SNPRINTF(dest, sizeof(dest), "%s", data);
        printf("%s\n", dest);
    }
}

static void safe_print()  
{
    const char * static_msg = "Safe message";
    printf("%s\n", static_msg);  // Always use format specifiers
}

int main() {
    CWE134_Uncontrolled_Format_String__char_console_snprintf_31_good();
    safe_print();
    return 0;
}
