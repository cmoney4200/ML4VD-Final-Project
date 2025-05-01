#include <sqlite3.h>
#include <stdio.h>

//Our own safe version of an SQL Injection vulnerability
void safe_query(sqlite3* db, const char* user_input) {
    sqlite3_stmt* stmt;

    int rc = sqlite3_prepare_v2(db, "SELECT * FROM users WHERE id = ?", -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Prepare failed: %s\n", sqlite3_errmsg(db));
        return;
    }

    rc = sqlite3_bind_text(stmt, 1, user_input, -1, SQLITE_STATIC);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Bind failed: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        return;
    }

    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        const char* name = sqlite3_column_text(stmt, 1);
        printf("ID: %d, Name: %s\n", id, name);
    }

    if (rc != SQLITE_DONE) {
        fprintf(stderr, "Execution failed: %s\n", sqlite3_errmsg(db));
    }
    sqlite3_finalize(stmt);
}
