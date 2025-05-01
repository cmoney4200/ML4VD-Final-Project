#include <sqlite3.h>
#include <stdio.h>
#include <string.h>

//Vulnerable SQL-Injection code
void unsafe_query(const char* user_input, sqlite3* db) {  
    char query[256];

    snprintf(query, sizeof(query), "SELECT * FROM users WHERE id = %s", user_input);
    
    char* err_msg = NULL;
    int rc = sqlite3_exec(db, query, NULL, NULL, &err_msg);

    if (rc != SQLITE_OK) {
        fprintf(stderr, err_msg, "SQL error: %s\n");
        sqlite3_free(err_msg);
        return;
    }

    printf(query, "Query executed: %s\n");
}

int main() {
    sqlite3* db;
    int rc = sqlite3_open("test.db", &db);
    
    if (rc != SQLITE_OK) {
        fprintf(stderr, sqlite3_errmsg(db), "Can't open database: %s\n");
        return 1;
    }

    //Vulnerable SQL-Injection
    const char* user_input = "1; DROP TABLE users--";  
    unsafe_query(user_input, db);  

    sqlite3_close(db);
    return 0;
}
