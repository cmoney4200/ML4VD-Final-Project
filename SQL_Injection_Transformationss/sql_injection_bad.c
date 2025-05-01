#include <sqlite3.h>
#include <stdio.h>
#include <string.h>

//Vulnerable SQL-Injection code
void unsafe_query(sqlite3* db, const char* user_input) {
    char query[256];

    snprintf(query, sizeof(query), "SELECT * FROM users WHERE id = %s", user_input);
    

    char* err_msg = NULL;
    int rc = sqlite3_exec(db, query, NULL, NULL, &err_msg);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
        return;
    }

    printf("Query executed: %s\n", query);
}

int main() {
    sqlite3* db;
    int rc = sqlite3_open("test.db", &db);
    
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return 1;
    }
    //Vulnerable SQL-Injection
    const char* user_input = "1; DROP TABLE users--";
    unsafe_query(db, user_input);

    sqlite3_close(db);
    return 0;
}
