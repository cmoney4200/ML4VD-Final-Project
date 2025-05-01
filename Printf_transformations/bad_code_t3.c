//Vulnerable printf file, from original researcher dataset
void CWE134_Uncontrolled_Format_String__char_console_snprintf_31_modified()
{
    char * bufferPtr;
    char storageArea[100] = "";
    bufferPtr = storageArea;
    {
        size_t length = strlen(bufferPtr);
        
        if (100-length > 1)
        {
            if (fgets(bufferPtr+length, (int)(100-length), stdin) != NULL)
            {
                length = strlen(bufferPtr);
                if (length > 0 && bufferPtr[length-1] == '\n')
                {
                    bufferPtr[length-1] = '\0';
                }
            }
            else
            {
                printLine("fgets() failed");
                bufferPtr[length] = '\0';
            }
        }
    }
    {
        char * duplicatePtr = bufferPtr;
        char * bufferPtr = duplicatePtr;
        {
            char output[100] = "";
            
            SNPRINTF(output, 100-1, bufferPtr);
            printLine(output);
        }
    }
}

static void helperFunc()  
{
    char * message = source;
    printf(message);
}
