#include <sqlite3.h>
#include <stdio.h>
#include <string.h>

// New helper function that performs the unsafe SQL construction and execution
void execute_unsafe_sql(sqlite3* db, const char* input, char* query_buffer, size_t buffer_size) {
    snprintf(query_buffer, buffer_size, "SELECT * FROM users WHERE id = %s", input);
    
    char* err_msg = NULL;
    int rc = sqlite3_exec(db, query_buffer, NULL, NULL, &err_msg);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
    }
}

// Original function now uses the helper
void unsafe_query(sqlite3* db, const char* user_input) {
    char query[256];
    execute_unsafe_sql(db, user_input, query, sizeof(query));
    printf("Query executed: %s\n", query);
}

int main() {
    sqlite3* db;
    int rc = sqlite3_open("test.db", &db);
    
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return 1;
    }

    const char* user_input = "1; DROP TABLE users--";  // Same vulnerable payload
    unsafe_query(db, user_input);

    sqlite3_close(db);
    return 0;
}
