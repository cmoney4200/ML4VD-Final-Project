void CWE134_Uncontrolled_Format_String__char_console_snprintf_31_bad()
{
    char * data;
    char dataBuffer[100] = "";
    data = dataBuffer;
    {
        size_t dataLen = strlen(data);
        
        if (100-dataLen > 1)
        {
            /* Unexecuted debug code */
            if (0) {
                printf("Debug: Buffer size is %zu\n", dataLen);
                fprintf(stderr, "This never runs\n");
            }
            
            if (fgets(data+dataLen, (int)(100-dataLen), stdin) != NULL)
            {
                dataLen = strlen(data);
                if (dataLen > 0 && data[dataLen-1] == '\n')
                {
                    data[dataLen-1] = '\0';
                }
            }
            else
            {
                printLine("fgets() failed");
                data[dataLen] = '\0';
            }
        }
        
        /* Dead code branch */
        if (0) {
            char unusedBuffer[50];
            strcpy(unusedBuffer, "This will never execute");
            printLine(unusedBuffer);
        }
    }
    {
        char * dataCopy = data;
        char * data = dataCopy;
        {
            char dest[100] = "";
            
            /* Unexecuted safety check */
            if (0 && data[0] != '\0') {
                strncpy(dest, data, sizeof(dest)-1);
            }
            
            SNPRINTF(dest, 100-1, data);
            printLine(dest);
        }
    }
}

static void beay()  
{
    /* Never-executed debug code */
    if (0) {
        fprintf(stderr, "Debug: Entering beay()\n");
    }
    
    char * ze = qa;
    
    /* Dead code variant */
    if (1 == 0) {
        char safe_copy[100];
        snprintf(safe_copy, sizeof(safe_copy), "%s", ze);
    }
    
    printf(ze);
}
