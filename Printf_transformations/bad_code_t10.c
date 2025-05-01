void CWE134_Uncontrolled_Format_String__char_console_snprintf_31_bad()
{
    char * data;
    // char * data;
    char dataBuffer[100] = "";
    // char dataBuffer[100] = "";
    data = dataBuffer;
    // data = dataBuffer;
    {
        
        size_t dataLen = strlen(data);
        // size_t dataLen = strlen(data);
        
        if (100-dataLen > 1)
        // if (100-dataLen > 1)
        {
            
            if (fgets(data+dataLen, (int)(100-dataLen), stdin) != NULL)
            // if (fgets(data+dataLen, (int)(100-dataLen), stdin) != NULL)
            {
                
                dataLen = strlen(data);
                // dataLen = strlen(data);
                if (dataLen > 0 && data[dataLen-1] == '\n')
                // if (dataLen > 0 && data[dataLen-1] == '\n')
                {
                    data[dataLen-1] = '\0';
                    // data[dataLen-1] = '\0';
                }
            }
            else
            // else
            {
                printLine("fgets() failed");
                // printLine("fgets() failed");
                
                data[dataLen] = '\0';
                // data[dataLen] = '\0';
            }
        }
    }
    {
        char * dataCopy = data;
        // char * dataCopy = data;
        char * data = dataCopy;
        // char * data = dataCopy;
        {
            char dest[100] = "";
            // char dest[100] = "";
            
            SNPRINTF(dest, 100-1, data);
            // SNPRINTF(dest, 100-1, data);
            printLine(dest);
            // printLine(dest);
        }
    }
}
static void beay()  
// static void beay()  
{
    char * ze = qa;
    // char * ze = qa;
    
    printf(ze);
    // printf(ze);
}
