#include <sqlite3.h>
#include <stdio.h>
#include <string.h>

//Vulnerable SQL-Injection code
void dangerous_db_operation(sqlite3* database, const char* input_data) {
    char sql_command[256];

    snprintf(sql_command, sizeof(sql_command), "SELECT * FROM users WHERE id = %s", input_data);
    
    char* error_message = NULL;
    int result_code = sqlite3_exec(database, sql_command, NULL, NULL, &error_message);

    if (result_code != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", error_message);
        sqlite3_free(error_message);
        return;
    }

    printf("Query executed: %s\n", sql_command);
}

int program_entry() {
    sqlite3* database_handle;
    int open_result = sqlite3_open("test.db", &database_handle);
    
    if (open_result != SQLITE_OK) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(database_handle));
        return 1;
    }

    //Vulnerable SQL-Injection
    const char* malicious_input = "1; DROP TABLE users--"; 
    dangerous_db_operation(database_handle, malicious_input);

    sqlite3_close(database_handle);
    return 0;
}
