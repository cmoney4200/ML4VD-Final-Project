#include <sqlite3.h>
#include <stdio.h>
#include <string.h>

void unsafe_query(const char* user_input, sqlite3* db) {  // Parameters reordered
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

    const char* user_input = "1; DROP TABLE users--";  // Same vulnerable payload
    unsafe_query(user_input, db);  // Arguments reordered to match new parameter order

    sqlite3_close(db);
    return 0;
}
