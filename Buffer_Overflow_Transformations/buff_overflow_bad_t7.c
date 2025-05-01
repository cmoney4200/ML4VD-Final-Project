#include <string.h>


void vulnerable_copy( char * input ) 
{
    char buffer[ 12 ];
    
    strcpy( buffer , input );  // No bounds checking
    
}


int main( ) 
{
    char large_input[ 128 ] = "This string is definitely longer than 64 characters...";
    
    vulnerable_copy( large_input );
    
    return 0;
    
}
