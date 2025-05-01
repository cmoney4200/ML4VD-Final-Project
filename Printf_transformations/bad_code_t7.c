void CWE134_Uncontrolled_Format_String__char_console_snprintf_31_bad( )
{
    char * data;
    
    
    char dataBuffer[ 100 ] = "";
    
    data = dataBuffer;
    
    
    {
        
        
        size_t dataLen = strlen( data );
        
        
        if ( 100 - dataLen > 1 )
        {
            
            
            if ( fgets( data + dataLen, ( int ) ( 100 - dataLen ), stdin ) != NULL )
            {
                
                
                dataLen = strlen( data );
                
                if ( dataLen > 0 && data[ dataLen - 1 ] == '\n' )
                {
                    data[ dataLen - 1 ] = '\0';
                    
                }
            }
            else
            {
                printLine( "fgets() failed" );
                
                
                data[ dataLen ] = '\0';
                
            }
        }
    }
    
    
    {
        char * dataCopy = data;
        
        char * data = dataCopy;
        
        
        {
            char dest[ 100 ] = "";
            
            
            SNPRINTF( dest, 100 - 1, data );
            
            printLine( dest );
            
        }
    }
}


static void beay( )  
{
    char * ze = qa;
    
    
    printf( ze );
    
}
