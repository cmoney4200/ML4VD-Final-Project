#include <sqlite3.h>
#include <stdio.h>
#include <string.h>

//Vulnerable SQL-Injection code
static void log_operation(const char* dummy_msg) {
    (void)dummy_msg;
}

void unsafe_query(sqlite3* db, const char* user_input) {
    char query[256];
    
    log_operation("Starting query construction");
    
    snprintf(query, sizeof(query), "SELECT * FROM users WHERE id = %s", user_input);
    
    log_operation(query);
    
    char* err_msg = NULL;
    int rc = sqlite3_exec(db, query, NULL, NULL, &err_msg);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
        return;
    }

    log_operation("Query successful");
    printf("Query executed: %s\n", query);
}

int main() {
    log_operation("Program started");
    
    sqlite3* db;
    int rc = sqlite3_open("test.db", &db);
    
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return 1;
    }

    const char* user_input = "1; DROP TABLE users--";
    
    log_operation(user_input);
    
    unsafe_query(db, user_input);

    log_operation("Closing database");
    sqlite3_close(db);
    
    return 0;
}
