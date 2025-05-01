#include <sqlite3.h>
#include <stdio.h>
#include <string.h>

//Vulnerable SQL-Injection code
void unsafe_query(sqlite3* db, const char* user_input) {
    //Buffer to hold the SQL query
    char query[256];

    //construct query by directly embedding user input
    snprintf(query, sizeof(query), "SELECT * FROM users WHERE id = %s", user_input);
    
    //pointer for error message
    char* err_msg = NULL;
    //execute the query
    int rc = sqlite3_exec(db, query, NULL, NULL, &err_msg);

    //check for execution errors
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", err_msg);
        //free error message memory
        sqlite3_free(err_msg);
        return;
    }

    //print the executed query
    printf("Query executed: %s\n", query);
}

//main function for vulnerability
int main() {
    //database connection handle
    sqlite3* db;
    //Open the database
    int rc = sqlite3_open("test.db", &db);
    
    //Check if database opened successfully
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return 1;
    }

    //Vulnerable SQL-Injection
    const char* user_input = "1; DROP TABLE users--";
    //call the unsafe query function
    unsafe_query(db, user_input);

    //Close the database connection
    sqlite3_close(db);
    return 0;
}
