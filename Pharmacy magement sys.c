#include <stdio.h>
#include <string.h>
#include <stdlib.h> //for standard function like (cls)
#include <conio.h> //for getch() pass hiding..
#include <ctype.h> //for char handeling
#include <time.h> //date and time function

#define MAX_MEDICINES 100 //maximum medicine to store
#define DATA_FILE "pharmacy_data.txt" //file name where be stored
#define ADMIN_FILE "admin_data.txt"

typedef struct {
    char name[100];
    char manufacturer[100];
    float price;
    int quantity;
    char expiry_date[11];
} Medicine;

Medicine medicines[MAX_MEDICINES]; //ary to store medicine
int medicineCount = 0; //counter for medi

// Function prototypes
int verifyAdmin(); //check admin details
void adminSection(); //admin control panel
void userSection(); //user panel
void addMedicine();
void showMedicines();
void deleteMedicine();
void modifyMedicine();
void buyMedicines();
void searchMedicineByName();
void saveToFile();
void loadFromFile();
int isExpired(const char* expiryDate); //check expiry
int validateDate(const char* date); //validate date format

//main funct starts from here


int main() {
    loadFromFile();//laod medicine data when progrm starts

    int choice;
    do {
        system("cls"); //clear screen
        printf("\n\n\t\tPHARMACY MANAGEMENT SYSTEM");
        printf("\n\n\t\t1. Admin Section");
        printf("\n\t\t2. User Section");
        printf("\n\t\t3. Exit");
        printf("\n\n\t\tEnter your choice: ");
        scanf("%d", &choice);

        //handle user choice
        switch(choice) {
            case 1:
                if(verifyAdmin()) {  //check admin details
                    adminSection(); //go to admin panel
                } else {
                    printf("\n\t\tLogin failed! Wrong username or password.");
                    getch(); //wait for key press
                }
                break;
            case 2: userSection(); break; //go to user panel
            case 3:
                saveToFile(); //save data before exiting
                printf("\n\t\tData saved successfully. Exiting...\n");
                break;
            default:
                printf("\n\t\tInvalid choice. Please try again.");
                getch();
        }
    } while(choice != 3); //repeat until user chooses to exit

    return 0;
}

int verifyAdmin() { //verify admin,Returns 1 if correct,0 if wrong
    system("cls");
    printf("\n\n\t\tADMIN LOGIN");

    char username[100], password[100];

    printf("\n\t\tEnter Username: ");
    getchar(); //clear input buffer
    gets(username);

    //get pass with hiding
    printf("\t\tEnter Password: ");
    int i = 0;//number of char typed
    char ch;
    while(1) {
        ch = getch();//get charecter without showing it
        if(ch == '\r') break;//stop when enter pressed
        if(ch == '\b' && i > 0) { //handle back space
            i--;
            printf("\b \b");//erase star
        } else {
            password[i++] = ch;
            printf("*");//show star instead of charcter
        }
    }
    password[i] = '\0';//end string
   //check if details match(user name,pass)
    return (strcmp(username, "admin") == 0 && strcmp(password, "1234") == 0);
}

//Save medicine data to file
void saveToFile() {
    FILE *fp = fopen(DATA_FILE, "w");  //Open file for writing
    if(fp == NULL) {
        printf("\n\t\tError: Could not save data to file!");
        return;
    }

    // Write medicine count first
    fprintf(fp, "%d\n", medicineCount);

    // Write each medicine record
    for(int i = 0; i < medicineCount; i++) {
        fprintf(fp, "%s\n%s\n%.2f\n%d\n%s\n",
               medicines[i].name,
               medicines[i].manufacturer,
               medicines[i].price,
               medicines[i].quantity,
               medicines[i].expiry_date);
    }
    fclose(fp);// Close the file
}

void loadFromFile() {
    FILE *fp = fopen(DATA_FILE, "r");  // Open file for reading
    if(fp == NULL) {
        medicineCount = 0;// If file doesn't exist, start with empty inventory
        return;
    }

    // Read medicine count
    fscanf(fp, "%d\n", &medicineCount);

    // Read each medicine record
    for(int i = 0; i < medicineCount; i++) {
        fgets(medicines[i].name, 100, fp);
        medicines[i].name[strlen(medicines[i].name)-1] = '\0'; // Remove newline

        fgets(medicines[i].manufacturer, 100, fp);
        medicines[i].manufacturer[strlen(medicines[i].manufacturer)-1] = '\0';

        fscanf(fp, "%f\n", &medicines[i].price);
        fscanf(fp, "%d\n", &medicines[i].quantity);

        // Read exactly 10 characters for expiry date (DD/MM/YYYY format)
        fscanf(fp, "%10s", medicines[i].expiry_date);
        fgetc(fp); // Read and discard the newline character
    }
    fclose(fp);
}

void adminSection() {
    int choice;
    do {
        system("cls");
        //dispaly admin panel
        printf("\n\n\t\tADMIN SECTION");
        printf("\n\n\t\t1. Add Medicine");
        printf("\n\t\t2. View All Medicines");
        printf("\n\t\t3. Search Medicine by Name");
        printf("\n\t\t4. Delete Medicine");
        printf("\n\t\t5. Modify Medicine");
        printf("\n\t\t6. Back to Main Menu");
        printf("\n\n\t\tEnter your choice: ");
        scanf("%d", &choice);
        //handle admin choice

        switch(choice) {
            case 1: addMedicine(); break;
            case 2: showMedicines(); break;
            case 3: searchMedicineByName(); break;
            case 4: deleteMedicine(); break;
            case 5: modifyMedicine(); break;
            case 6:
                saveToFile(); //save before exit
                printf("\n\t\tReturning to main menu...");
                break;
            default:
                printf("\n\t\tInvalid choice. Please try again.");
                getch();//wait for press key
        }
    } while(choice != 6);//reapeat until admin chooses to exit
}

void userSection() {
    int choice;
    do {
        system("cls");
        printf("\n\n\t\tUSER SECTION");
        printf("\n\n\t\t1. View Available Medicines");
        printf("\n\t\t2. Search Medicine by Name");
        printf("\n\t\t3. Buy Medicines");
        printf("\n\t\t4. Back to Main Menu");
        printf("\n\n\t\tEnter your choice: ");
        scanf("%d", &choice);
        //handle user choice

        switch(choice) {
            case 1: showMedicines(); break;
            case 2: searchMedicineByName(); break;
            case 3: buyMedicines(); break;
            case 4: printf("\n\t\tReturning to main menu..."); break;
            default:
                printf("\n\t\tInvalid choice. Please try again.");
                getch();
        }
    } while(choice != 4);//repeat until exit
}

void addMedicine() {
    system("cls");
    printf("\n\n\t\tADD NEW MEDICINE");

    if(medicineCount >= MAX_MEDICINES) { //check if inventory is full
        printf("\n\t\tInventory full! Cannot add more medicines.");
        getch();
        return;
    }

    Medicine m;//temporary medic variable/structure

    //get medic details from admin
    printf("\n\t\tEnter Medicine Name: ");
    getchar();//clear input buffer
    gets(m.name);

    printf("\t\tEnter Manufacturer: ");
    gets(m.manufacturer);

    printf("\t\tEnter Price: ");
    scanf("%f", &m.price);

    printf("\t\tEnter Quantity: ");
    scanf("%d", &m.quantity);

//get and validate expiry date
    do {
        printf("\t\tEnter Expiry Date (DD/MM/YYYY): ");
        getchar();
        gets(m.expiry_date);
    } while(!validateDate(m.expiry_date));

//add medicine to arry and increase count
    medicines[medicineCount++] = m;
    printf("\n\t\tMedicine added successfully!");
    saveToFile();//save changes
    getch();
}

void showMedicines() {
    system("cls");
    printf("\n\n\t\tMEDICINE INVENTORY\n");
    printf("\n\t%-5s %-30s %-25s %-10s %-10s %-12s", "ID", "Name", "Manufacturer", "Price", "Quantity", "Expiry");
    printf("\n\t-------------------------------------------------------------------------------");

    //check if inventory is empty
    if(medicineCount == 0) {
        printf("\n\t\tNo medicines in inventory!");
    } else {
        // Display all medicines
        for(int i = 0; i < medicineCount; i++) {
            printf("\n\t%-5d %-30s %-25s $%-9.2f %-10d %-12s",
                  i+1, medicines[i].name, medicines[i].manufacturer,
                  medicines[i].price, medicines[i].quantity, medicines[i].expiry_date);
            //mark if expired
            if(isExpired(medicines[i].expiry_date)) {
                printf(" (EXPIRED)");
            }
        }
    }
    getch();//wait for key press
}

void deleteMedicine() {
    system("cls");
    printf("\n\n\t\tDELETE MEDICINE");

    // Check if inventory is empty
    if(medicineCount == 0) {
        printf("\n\t\tNo medicines to delete!");
        getch();
        return;
    }

    showMedicines();//show all medic 1st

    int id;
    printf("\n\n\t\tEnter Medicine ID to delete: ");
    scanf("%d", &id);


    //validate id
    if(id < 1 || id > medicineCount) {
        printf("\n\t\tInvalid ID!");
        getch();
        return;
    }

    //confirm delletion
    printf("\n\t\tConfirm delete '%s'? (Y/N): ", medicines[id-1].name);
    char confirm = getche();//get single char input
    confirm = toupper(confirm);//convert to upper case

    if(confirm == 'Y') {
            //shift all medic after the delleted one
        for(int i = id-1; i < medicineCount-1; i++) {
            medicines[i] = medicines[i+1];
        }
        medicineCount--;//decrease count
        printf("\n\t\tMedicine deleted successfully!");
        saveToFile();
    } else {
        printf("\n\t\tDeletion cancelled.");
    }
    getch();
}

void modifyMedicine() {
    system("cls");
    printf("\n\n\t\tMODIFY MEDICINE");

    if(medicineCount == 0) {
        printf("\n\t\tNo medicines to modify!");
        getch();
        return;
    }

    showMedicines();//show all medic

    int id;
    printf("\n\n\t\tEnter Medicine ID to modify: ");
    scanf("%d", &id);

    //validate id
    if(id < 1 || id > medicineCount) {
        printf("\n\t\tInvalid ID!");
        getch();
        return;
    }

    //Pointer to the medicine we're modifying
    Medicine *m = &medicines[id-1];

    //show current details
    printf("\n\t\tCurrent Details:");
    printf("\n\t\tName: %s", m->name);
    printf("\n\t\tManufacturer: %s", m->manufacturer);
    printf("\n\t\tPrice: %.2f", m->price);
    printf("\n\t\tQuantity: %d", m->quantity);
    printf("\n\t\tExpiry: %s", m->expiry_date);

    // Get new details (user can leave blank to keep current value)
    printf("\n\n\t\tEnter new details (leave blank to keep current):");

    printf("\n\t\tNew Name: ");
    getchar();
    char newName[100];
    gets(newName);
    if(strlen(newName) > 0) strcpy(m->name, newName);

    printf("\t\tNew Manufacturer: ");
    gets(newName);
    if(strlen(newName) > 0) strcpy(m->manufacturer, newName);

    printf("\t\tNew Price: ");
    char priceStr[20];
    gets(priceStr);
    if(strlen(priceStr) > 0) m->price = atof(priceStr);

    printf("\t\tNew Quantity: ");
    char qtyStr[20];
    gets(qtyStr);
    if(strlen(qtyStr) > 0) m->quantity = atoi(qtyStr);

    printf("\t\tNew Expiry (DD/MM/YYYY): ");
    char expiryStr[20];
    gets(expiryStr);
    if(strlen(expiryStr) > 0 && validateDate(expiryStr)) {
        strcpy(m->expiry_date, expiryStr);
    }

    printf("\n\t\tMedicine updated successfully!");
    saveToFile();
    getch();
}

void buyMedicines() {
    system("cls");
    printf("\n\n\t\tBUY MEDICINES");

    if(medicineCount == 0) {
        printf("\n\t\tNo medicines available!");
        getch();
        return;
    }

    showMedicines();

    int id, qty;
    printf("\n\n\t\tEnter Medicine ID to buy: ");
    scanf("%d", &id);

    //validate id
    if(id < 1 || id > medicineCount) {
        printf("\n\t\tInvalid ID!");
        getch();
        return;
    }
//check if expired
    if(isExpired(medicines[id-1].expiry_date)) {
        printf("\n\t\tCannot buy expired medicine!");
        getch();
        return;
    }

    printf("\t\tEnter Quantity: ");
    scanf("%d", &qty);

    //validate quantity
    if(qty <= 0) {
        printf("\n\t\tInvalid quantity!");
        getch();
        return;
    }

    // Check if enough stock available
    if(qty > medicines[id-1].quantity) {
        printf("\n\t\tNot enough stock! Available: %d", medicines[id-1].quantity);
        getch();
        return;
    }

    // Calculate total price
    float total = qty * medicines[id-1].price;

    // Reduce stock quantity
    medicines[id-1].quantity -= qty;

     // Print receipt
    system("cls");
    printf("\n\n\t\tPHARMACY RECEIPT");
    printf("\n\t\t=================================");
    printf("\n\t\tMedicine: %s", medicines[id-1].name);
    printf("\n\t\tManufacturer: %s", medicines[id-1].manufacturer);
    printf("\n\t\tPrice per unit: $%.2f", medicines[id-1].price);
    printf("\n\t\tQuantity sold: %d", qty);
    printf("\n\t\tExpiry Date: %s", medicines[id-1].expiry_date);
    printf("\n\t\t---------------------------------");
    printf("\n\t\tTOTAL: $%.2f", total);
    printf("\n\t\t=================================");
    printf("\n\t\tThank you for your purchase!");

    saveToFile();
    getch();
}

//Search medicines by name (full or partial match)
void searchMedicineByName() {
    system("cls");
    printf("\n\n\t\tSEARCH MEDICINE BY NAME");

    if (medicineCount == 0) {
        printf("\n\t\tNo medicines in inventory!");
        getch();
        return;
    }

    char searchTerm[100];
    printf("\n\t\tEnter medicine name (or part of name): ");
    getchar();
    gets(searchTerm);

    // Display search results header
    printf("\n\n\t\tSEARCH RESULTS\n");
    printf("\n\t%-5s %-30s %-25s %-10s %-10s %-12s", "ID", "Name", "Manufacturer", "Price", "Quantity", "Expiry");
    printf("\n\t-------------------------------------------------------------------------------");

    int found = 0;// Flag to check if any matches found

    // Search through all medicines
    for (int i = 0; i < medicineCount; i++) {
        // Check if medicine name contains search term
        if (strstr(medicines[i].name, searchTerm) != NULL) {
             // Display medicine details
            printf("\n\t%-5d %-30s %-25s $%-9.2f %-10d %-12s",
                  i+1, medicines[i].name, medicines[i].manufacturer,
                  medicines[i].price, medicines[i].quantity, medicines[i].expiry_date);
            if (isExpired(medicines[i].expiry_date)) {
                printf(" (EXPIRED)");
            }
            found = 1;// Set flag since we found a match
        }
    }

    // If no matches found
    if (!found) {
        printf("\n\t\tNo medicines found matching '%s'", searchTerm);
    }
    getch();
}

//Check if medicine is expired
//Returns 1 if expired, 0 if not

int isExpired(const char* expiryDate) {
    // Get current date
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    int current_year = tm.tm_year + 1900;
    int current_month = tm.tm_mon + 1;
    int current_day = tm.tm_mday;

    // Extract expiry date components
    int day, month, year;
    sscanf(expiryDate, "%d/%d/%d", &day, &month, &year);

    // Compare dates
    if(year < current_year) return 1;
    if(year == current_year && month < current_month) return 1;
    if(year == current_year && month == current_month && day < current_day) return 1;

    return 0;// Not expired
}

//Validate date format (DD/MM/YYYY)
//Returns 1 if valid, 0 if invalid
int validateDate(const char* date) {
    // Check length and slash positions
    if(strlen(date) != 10) return 0;
    if(date[2] != '/' || date[5] != '/') return 0;

    int day, month, year;
    // Extract day, month, year
    if(sscanf(date, "%d/%d/%d", &day, &month, &year) != 3)
        return 0;

        // Validate ranges
    if(day < 1 || day > 31) return 0;
    if(month < 1 || month > 12) return 0;
    if(year < 2020 || year > 2100) return 0;

    return 1;// Valid date
}

