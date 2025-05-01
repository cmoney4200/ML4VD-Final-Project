#include <sqlite3.h>
#include <stdio.h>
#include <string.h>

// New dummy function that does nothing
static void log_operation(const char* dummy_msg) {
    // This function intentionally left empty
    (void)dummy_msg; // Cast to void to suppress unused parameter warning
}

void unsafe_query(sqlite3* db, const char* user_input) {
    char query[256];
    
    // Call dummy function before vulnerable operation
    log_operation("Starting query construction");
    
    snprintf(query, sizeof(query), "SELECT * FROM users WHERE id = %s", user_input);
    
    // Call dummy function after string formatting
    log_operation(query);
    
    char* err_msg = NULL;
    int rc = sqlite3_exec(db, query, NULL, NULL, &err_msg);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
        return;
    }

    // Call dummy function before printing
    log_operation("Query successful");
    printf("Query executed: %s\n", query);
}

int main() {
    // Call dummy function at start
    log_operation("Program started");
    
    sqlite3* db;
    int rc = sqlite3_open("test.db", &db);
    
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return 1;
    }

    const char* user_input = "1; DROP TABLE users--";  // Same vulnerable payload
    
    // Call dummy function before vulnerable query
    log_operation(user_input);
    
    unsafe_query(db, user_input);

    // Call dummy function before closing
    log_operation("Closing database");
    sqlite3_close(db);
    
    return 0;
}
