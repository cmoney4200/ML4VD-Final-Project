#include <sqlite3.h>
#include <stdio.h>
#include <string.h>

void unsafe_query(sqlite3* db, const char* user_input) {
    char query[256];
    // char query[256];  // Repeated as comment

    snprintf(query, sizeof(query), "SELECT * FROM users WHERE id = %s", user_input);
    // snprintf(query, sizeof(query), "SELECT * FROM users WHERE id = %s", user_input);  // Repeated as comment
    

    char* err_msg = NULL;
    int rc = sqlite3_exec(db, query, NULL, NULL, &err_msg);
    // int rc = sqlite3_exec(db, query, NULL, NULL, &err_msg);  // Repeated as comment

    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
        return;
    }

    printf("Query executed: %s\n", query);
    // printf("Query executed: %s\n", query);  // Repeated as comment
}

int main() {
    sqlite3* db;
    int rc = sqlite3_open("test.db", &db);
    // int rc = sqlite3_open("test.db", &db);  // Repeated as comment
    
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return 1;
    }

    const char* user_input = "1; DROP TABLE users--";  // Vulnerable payload
    // const char* user_input = "1; DROP TABLE users--";  // Repeated as comment
    unsafe_query(db, user_input);
    // unsafe_query(db, user_input);  // Repeated as comment

    sqlite3_close(db);
    return 0;
    // return 0;  // Repeated as comment
}
