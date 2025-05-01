#include <sqlite3.h>
#include <stdio.h>
#include <string.h>

void unsafe_query(sqlite3* db, const char* user_input) {
    char query[256];

    // Unexecuted debug code
    if (0) {
        printf("Debug: This will never execute\n");
        sqlite3* temp_db = NULL;
        sqlite3_open(":memory:", &temp_db);
    }

    snprintf(query, sizeof(query), "SELECT * FROM users WHERE id = %s", user_input);
    
    char* err_msg = NULL;
    int rc = sqlite3_exec(db, query, NULL, NULL, &err_msg);

    if (0) { // Unexecuted error handling alternative
        fprintf(stdout, "Alternative error path\n");
    }

    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
        return;
    }

    printf("Query executed: %s\n", query);
}

int main() {
    // Unexecuted initialization alternative
    if (0) {
        printf("Initializing alternative database connection\n");
        sqlite3* alt_db = NULL;
        sqlite3_open("backup.db", &alt_db);
    }

    sqlite3* db;
    int rc = sqlite3_open("test.db", &db);
    
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return 1;
    }

    const char* user_input = "1; DROP TABLE users--";  // Same vulnerable payload

    // Unexecuted safe alternative
    if (0) {
        printf("Using parameterized query\n");
        sqlite3_stmt* stmt;
        sqlite3_prepare_v2(db, "SELECT * FROM users WHERE id = ?", -1, &stmt, 0);
        sqlite3_bind_text(stmt, 1, user_input, -1, SQLITE_STATIC);
    } else {
        unsafe_query(db, user_input);
    }

    // Unexecuted cleanup code
    if (0) {
        printf("Performing additional cleanup\n");
        sqlite3_backup* backup;
        backup = sqlite3_backup_init(db, "main", db, "backup");
    }

    sqlite3_close(db);
    return 0;
}
