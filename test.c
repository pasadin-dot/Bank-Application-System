#include <stdio.h>
#include <string.h>

int main() {
    char accNum[20];          // To store account number input
    char filename[64];        // To store the full filename

    // Ask user for account number
    printf("Enter account number: ");
    if (fgets(accNum, sizeof(accNum), stdin) != NULL) {
        // Remove the newline character if present
        size_t len = strlen(accNum);
        if (len > 0 && accNum[len - 1] == '\n') {
            accNum[len - 1] = '\0';
        }
    }

    // Construct the full file path
    snprintf(filename, sizeof(filename), "database/%s.txt", accNum);

    // Delete the file
    if (remove(filename) == 0) {
        printf("File '%s' deleted successfully.\n", filename);
    } else {
        perror("Error deleting file");
    }

    return 0;
}
