#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_TOURISTS 100
#define MAX_NAME_LEN 250
#define MAX_DEST_LEN 500
#define MAX_DATE_LEN 11
#define MAX_DESTINATIONS 100

// Structure to hold tourist information
typedef struct {
    char name[MAX_NAME_LEN];
    char destination[MAX_DEST_LEN];
    int ticket_number;
    char date[MAX_DATE_LEN];
} Tourist;

// Structure to hold destination information
typedef struct {
    char name[MAX_DEST_LEN];
    int price;
    char facilities[MAX_DEST_LEN];
} Destination;

Destination destinations[MAX_DESTINATIONS] = {
    {"Pokhara", 50000, "Hotel booking for 1 week including tourist guide"},
    {"Mustang", 70000, "Full facility for 1 week"},
    {"Rara", 40000, "Full facility for 3 days"},
    {"Mt. Everest Base camp", 200000, "Full facility for 1 week"},
    {"Lumbini", 200000, "Full facility for 1 week with meditation"},
    {"Chitwan National Park", 400000, "Two days park visit with hotel facility"},
    {"Annapurna base camp trek", 400000, "1 week"},
    {"Chandragiri Cable Car Tour, Kathmandu", 4000, "With ticket and food for 8 hours"}
};
int num_destinations = 8;

// Function to generate a random ticket number
int generate_ticket_number() {
    return rand() % 1000 + 1000;
}

// Function to get the current date in the format YYYY-MM-DD
void get_current_date(char *date) {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(date, "%04d-%02d-%02d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);
}

// Function to delete a record by ticket number/
void delete_record(const char *filename, int ticket_number) {
    FILE *temp_file;
    FILE *file;

    temp_file = fopen("temp.csv", "w"); // Open temporary file for writing
    file = fopen(filename, "r"); // Open original file for reading

    if (file == NULL || temp_file == NULL) {
        printf("Error: Unable to open file.\n");
        exit(1);
    }

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        char *token;
        int temp_ticket_number;
        char temp_name[MAX_NAME_LEN];
        char temp_destination[MAX_DEST_LEN];
        char temp_date[MAX_DATE_LEN];

        token = strtok(line, ",");
        if (token == NULL) {
            continue; // Skip empty lines
        }
        strcpy(temp_name, token);

        token = strtok(NULL, ",");
        if (token == NULL) {
            continue; // Skip lines with missing fields
        }
        strcpy(temp_destination, token);

        token = strtok(NULL, ",");
        if (token == NULL) {
            continue; // Skip lines with missing fields
        }
        temp_ticket_number = atoi(token);

        token = strtok(NULL, ",");
        if (token == NULL) {
            continue; // Skip lines with missing fields
        }
        strcpy(temp_date, token);

        if (temp_ticket_number != ticket_number) {
            fprintf(temp_file, "%s,%s,%d,%s\n", temp_name, temp_destination, temp_ticket_number, temp_date);
        }
    }

    fclose(file);
    fclose(temp_file);

    remove(filename); // Remove original file
    rename("temp.csv", filename); // Rename temporary file to original filename
}


// Function to view tourist data by ticket number
void view_record(const char *filename, int ticket_number) {
    FILE *file;
    file = fopen(filename, "r");

    if (file == NULL) {
        printf("Error: Unable to open file.\n");
        exit(1);
    }

    int found = 0;
    char line[256];
    while (fgets(line, sizeof(line), file)) {
        char *token;
        int temp_ticket_number;
        char temp_name[MAX_NAME_LEN];
        char temp_destination[MAX_DEST_LEN];
        char temp_date[MAX_DATE_LEN];

        token = strtok(line, ",");
        strcpy(temp_name, token);

        token = strtok(NULL, ",");
        strcpy(temp_destination, token);

        token = strtok(NULL, ",");
        temp_ticket_number = atoi(token);

        token = strtok(NULL, ",");
        strcpy(temp_date, token);

        if (temp_ticket_number == ticket_number) {
            found = 1;
            printf("\nTourist Record:\n");
            printf("Name           : %s\n", temp_name);
            printf("Destination    : %s\n", temp_destination);
            printf("Ticket Number  : %d\n", temp_ticket_number);
            printf("Date of entry  : %s\n", temp_date);
            break;
        }
    }

    if (!found) {
        printf("Record not found for ticket number %d.\n", ticket_number);
    }

    fclose(file);
}
// Function to view all tourist data
void view_all_records(const char *filename) {
    FILE *file;
    file = fopen(filename, "r");

    if (file == NULL) {
        printf("Error: Unable to open file.\n");
        exit(1);
    }

    printf("\nAll Tourist Records:\n");
    printf("----------------------------------------------------------------------------------------\n");
    printf("%-25s %-35s %-15s %s\n", "Name", "Destination", "Ticket Number", "Date");
    printf("----------------------------------------------------------------------------------------\n");

    int found = 0; // Flag to check if any records were found
    char line[256];
    while (fgets(line, sizeof(line), file)) {
        // Remove newline character if present
        line[strcspn(line, "\n")] = '\0';

        // Tokenize the line
        char *token = strtok(line, ",");
        if (token == NULL) {
            continue; // Skip empty lines
        }

        found = 1; // Records were found

        // Read fields
        char temp_name[MAX_NAME_LEN];
        char temp_destination[MAX_DEST_LEN];
        int temp_ticket_number;
        char temp_date[MAX_DATE_LEN];

        strcpy(temp_name, token);

        token = strtok(NULL, ",");
        if (token == NULL) {
            continue; // Skip lines with missing fields
        }
        strcpy(temp_destination, token);

        token = strtok(NULL, ",");
        if (token == NULL) {
            continue; // Skip lines with missing fields
        }
        temp_ticket_number = atoi(token);

        token = strtok(NULL, ",");
        if (token == NULL) {
            continue; // Skip lines with missing fields
        }
        strcpy(temp_date, token);

        // Print record
        printf("%-25s %-35s %-15d %s\n", temp_name, temp_destination, temp_ticket_number, temp_date);
    }

    if (!found) {
        printf("No records found.\n");
    }

    printf("----------------------------------------------------------------------------------------\n");

    fclose(file);
}


// Function to display available destinations
void display_destinations() {
    printf("\nAvailable Destinations:\n");
    for (int i = 0; i < num_destinations; i++) {
        printf("%d. %s - %s, Price: %d\n", i + 1, destinations[i].name, destinations[i].facilities, destinations[i].price);
    }
}

// Function to add a new tourist
void add_tourist() {
    Tourist tourists[MAX_TOURISTS];
    FILE *file = fopen("tourists.csv", "a");

    // Get tourist information
    int num_tourists = 0;
    printf("\nEnter the number of new tourists (or enter 0 to skip): ");
    scanf("%d", &num_tourists);
    if (num_tourists > MAX_TOURISTS) {
        printf("Error: Too many tourists\n");
        fclose(file);
        return;
    }

    // Populate tourist information and write to file
    for (int i = 0; i < num_tourists; i++) {
        printf("\nTourist %d:\n", i + 1);
        
        printf("Enter full name: ");
        while (getchar() != '\n'); // clear input buffer
        fgets(tourists[i].name, MAX_NAME_LEN, stdin);
        tourists[i].name[strcspn(tourists[i].name, "\n")] = 0; // remove newline character

        // Display and select destination
        display_destinations();
        int destination_choice;
        printf("Select destination (1-%d): ", num_destinations);
        scanf("%d", &destination_choice);
        if (destination_choice < 1 || destination_choice > num_destinations) {
            printf("Invalid choice\n");
            fclose(file);
            return;
        }
        strcpy(tourists[i].destination, destinations[destination_choice - 1].name);

        tourists[i].ticket_number = generate_ticket_number();
        get_current_date(tourists[i].date);
        printf("Ticket number: %d\n", tourists[i].ticket_number);

        // Write tourist information to file
        fprintf(file, "%s,%s,%d,%s\n", tourists[i].name, tourists[i].destination, tourists[i].ticket_number, tourists[i].date);
    }

    fclose(file);
}

// Function to add a new destination
void add_destination() {
    if (num_destinations >= MAX_DESTINATIONS) {
        printf("Error: Cannot add more destinations. Maximum limit reached.\n");
        return;
    }

    Destination new_destination;
    printf("\nEnter the name of the new destination: ");
    while (getchar() != '\n'); // clear input buffer
    fgets(new_destination.name, MAX_DEST_LEN, stdin);
    new_destination.name[strcspn(new_destination.name, "\n")] = 0; // remove newline character

    printf("Enter the price of the new destination: ");
    scanf("%d", &new_destination.price);

    printf("Enter the facilities of the new destination: ");
    while (getchar() != '\n'); // clear input buffer
    fgets(new_destination.facilities, MAX_DEST_LEN, stdin);
    new_destination.facilities[strcspn(new_destination.facilities, "\n")] = 0; // remove newline character

    destinations[num_destinations++] = new_destination;
    printf("New destination added successfully.\n");
}

int main() {
    // Initialize 
    srand(time(NULL));

    // File pointer
    FILE *file;

    // Check if file exists
    file = fopen("tourists.csv", "r");
    if (file == NULL) {
        // Open file in write mode and create it
        file = fopen("tourists.csv", "w");
        if (file == NULL) {
            printf("Error: Unable to create file.\n");
            return 1;
        }
        // Write headers
        fprintf(file, "Name,Destination,Ticket Number,Date\n");
        printf("Created file 'tourists.csv'.\n");
    } else {
        fclose(file);
    }

    printf("\nWelcome to the tourist management system\n");
    printf("\n....................TMS....................\n");

    while (1) {
        char choice[20];
        int ticket_number;

        printf("\nEnter your choice:\n");
        printf("1. Add new tourists\n");
        printf("2. Delete a record by ticket number\n");
        printf("3. View tourist data by ticket number\n");
        printf("4. View all tourist data\n");
        printf("5. Add a new destination\n");
        printf("6. Exit\n");
        printf("Choice: ");
        scanf("%s", choice);

        if (strcmp(choice, "1") == 0) {
            add_tourist();
        } else if (strcmp(choice, "2") == 0) {
            printf("Enter the ticket number to delete the record: ");
            scanf("%d", &ticket_number);
            delete_record("tourists.csv", ticket_number);
            printf("Record with ticket number %d deleted successfully.\n", ticket_number);
        } else if (strcmp(choice, "3") == 0) {
            printf("Enter the ticket number to view data: ");
            scanf("%d", &ticket_number);
            view_record("tourists.csv", ticket_number);
        } else if (strcmp(choice, "4") == 0) {
            view_all_records("tourists.csv");
        } else if (strcmp(choice, "5") == 0) {
            char password[20];
            printf("Enter the key to add a new destination: ");
            scanf("%s", password);
            if (strcmp(password, "Desti111") == 0) {
                add_destination();
            } else {
                printf("Invalid key. Access denied.\n");
            }
        } else if (strcmp(choice, "6") == 0) {
            printf("Exiting the program. Goodbye!\n");
            break;
        } else {
            printf("Invalid choice. Please try again.\n");
        }
    }

    return 0;
}
