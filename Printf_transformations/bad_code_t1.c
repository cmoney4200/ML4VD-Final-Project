void CWE134_Uncontrolled_Format_String__char_console_snprintf_31_bad()
{
    char * token1;
    char token2Buffer[100] = "";
    token1 = token2Buffer;
    {
        size_t token3Len = strlen(token1);
        if (100-token3Len > 1)
        {
            if (fgets(token1+token3Len, (int)(100-token3Len), stdin) != NULL)
            {
                token3Len = strlen(token1);
                if (token3Len > 0 && token1[token3Len-1] == '\n')
                {
                    token1[token3Len-1] = '\0';
                }
            }
            else
            {
                printLine("fgets() failed");
                token1[token3Len] = '\0';
            }
        }
    }
    {
        char * token4Copy = token1;
        char * token5 = token4Copy;
        {
            char token6[100] = "";
            SNPRINTF(token6, 100-1, token5);
            printLine(token6);
        }
    }
}
static void xqwe()
{
    char * yzrt = pqrs;
    printf(yzrt);
}
