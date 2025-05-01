#include <sqlite3.h>
#include <stdio.h>
#include <string.h>

// Function to execute an unsafe SQL query, vulnerable to SQL injection
void unsafe_query(sqlite3* db, const char* user_input) {
    // Buffer to hold the SQL query
    char query[256];

    // Construct query by directly embedding user input
    snprintf(query, sizeof(query), "SELECT * FROM users WHERE id = %s", user_input);
    
    // Pointer for error message
    char* err_msg = NULL;
    // Execute the query
    int rc = sqlite3_exec(db, query, NULL, NULL, &err_msg);

    // Check for execution errors
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", err_msg);
        // Free error message memory
        sqlite3_free(err_msg);
        return;
    }

    // Print the executed query
    printf("Query executed: %s\n", query);
}

// Main function to demonstrate the vulnerability
int main() {
    // Database connection handle
    sqlite3* db;
    // Open the database
    int rc = sqlite3_open("test.db", &db);
    
    // Check if database opened successfully
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return 1;
    }

    // Example user input containing malicious SQL
    const char* user_input = "1; DROP TABLE users--";  // Vulnerable payload
    // Call the unsafe query function
    unsafe_query(db, user_input);

    // Close the database connection
    sqlite3_close(db);
    return 0;
}
