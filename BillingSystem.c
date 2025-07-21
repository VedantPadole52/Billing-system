#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <windows.h>

#define MAX_ITEMS 100
#define MAX_CUSTOMERS 50
#define MAX_BILLS 100
#define FILENAME_ITEMS "items.dat"
#define FILENAME_CUSTOMERS "customers.dat"
#define FILENAME_BILLS "bills.dat"

// Structures
struct Item {
    int id;
    char name[50];
    float price;
    int stock;
    char category[30];
};

struct Customer {
    int id;
    char name[50];
    char phone[15];
    char email[50];
    float totalPurchases;
};

struct BillItem {
    int itemId;
    char name[50];
    int quantity;
    float price;
    float total;
};

struct Bill {
    int id;
    int customerId;
    char customerName[50];
    char date[20];
    struct BillItem items[MAX_ITEMS];
    int numItems;
    float subtotal;
    float discount;
    float tax;
    float total;
};

// Global variables
struct Item inventory[MAX_ITEMS];
struct Customer customers[MAX_CUSTOMERS];
struct Bill bills[MAX_BILLS];
int numItems = 0;
int numCustomers = 0;
int numBills = 0;

// Function prototypes
void loadData();
void saveData();
void mainMenu();
void inventoryMenu();
void customerMenu();
void billingMenu();
void reportsMenu();
void addItem();
void editItem();
void deleteItem();
void viewItems();
void searchItems();
void addCustomer();
void editCustomer();
void deleteCustomer();
void viewCustomers();
void searchCustomers();
void createBill();
void viewBills();
void searchBills();
void generateReport();
void displayHeader(char *title);
void clearScreen();
void waitForKeypress();

// Utility functions
void getCurrentDate(char *dateStr) {
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    sprintf(dateStr, "%02d/%02d/%d", tm->tm_mday, tm->tm_mon + 1, tm->tm_year + 1900);
}

void clearScreen() {
    system("cls");
}

void waitForKeypress() {
    printf("\nPress any key to continue...");
    getch();
}

void displayHeader(char *title) {
    clearScreen();
    printf("\n==================================================\n");
    printf("\t\t%s\n", title);
    printf("==================================================\n\n");
}

// File operations
void loadData() {
    FILE *fileItems = fopen(FILENAME_ITEMS, "rb");
    if (fileItems != NULL) {
        numItems = fread(inventory, sizeof(struct Item), MAX_ITEMS, fileItems);
        fclose(fileItems);
    }
    
    FILE *fileCustomers = fopen(FILENAME_CUSTOMERS, "rb");
    if (fileCustomers != NULL) {
        numCustomers = fread(customers, sizeof(struct Customer), MAX_CUSTOMERS, fileCustomers);
        fclose(fileCustomers);
    }
    
    FILE *fileBills = fopen(FILENAME_BILLS, "rb");
    if (fileBills != NULL) {
        numBills = fread(bills, sizeof(struct Bill), MAX_BILLS, fileBills);
        fclose(fileBills);
    }
}

void saveData() {
    FILE *fileItems = fopen(FILENAME_ITEMS, "wb");
    if (fileItems != NULL) {
        fwrite(inventory, sizeof(struct Item), numItems, fileItems);
        fclose(fileItems);
    }
    
    FILE *fileCustomers = fopen(FILENAME_CUSTOMERS, "wb");
    if (fileCustomers != NULL) {
        fwrite(customers, sizeof(struct Customer), numCustomers, fileCustomers);
        fclose(fileCustomers);
    }
    
    FILE *fileBills = fopen(FILENAME_BILLS, "wb");
    if (fileBills != NULL) {
        fwrite(bills, sizeof(struct Bill), numBills, fileBills);
        fclose(fileBills);
    }
}

// Inventory management functions
void addItem() {
    displayHeader("Add New Item");
    
    if (numItems >= MAX_ITEMS) {
        printf("Inventory is full. Cannot add more items.\n");
        waitForKeypress();
        return;
    }
    
    struct Item newItem;
    
    // Generate ID (simple incremental ID)
    newItem.id = numItems > 0 ? inventory[numItems-1].id + 1 : 1001;
    
    printf("Item ID: %d (auto-generated)\n", newItem.id);
    
    printf("Enter item name: ");
    scanf(" %[^\n]", newItem.name);
    
    printf("Enter category: ");
    scanf(" %[^\n]", newItem.category);
    
    printf("Enter price: ");
    scanf("%f", &newItem.price);
    
    printf("Enter initial stock: ");
    scanf("%d", &newItem.stock);
    
    inventory[numItems++] = newItem;
    
    printf("\nItem added successfully!\n");
    saveData();
    waitForKeypress();
}

void editItem() {
    int id, found = 0, index;
    
    displayHeader("Edit Item");
    
    printf("Enter item ID to edit: ");
    scanf("%d", &id);
    
    for (int i = 0; i < numItems; i++) {
        if (inventory[i].id == id) {
            found = 1;
            index = i;
            break;
        }
    }
    
    if (!found) {
        printf("Item with ID %d not found!\n", id);
        waitForKeypress();
        return;
    }
    
    printf("\nCurrent details:\n");
    printf("ID: %d\n", inventory[index].id);
    printf("Name: %s\n", inventory[index].name);
    printf("Category: %s\n", inventory[index].category);
    printf("Price: %.2f\n", inventory[index].price);
    printf("Stock: %d\n\n", inventory[index].stock);
    
    printf("Enter new name (or press enter to keep current): ");
    char buffer[50];
    getchar(); // Clear input buffer
    fgets(buffer, sizeof(buffer), stdin);
    if (buffer[0] != '\n') {
        buffer[strcspn(buffer, "\n")] = 0; // Remove newline
        strcpy(inventory[index].name, buffer);
    }
    
    printf("Enter new category (or press enter to keep current): ");
    fgets(buffer, sizeof(buffer), stdin);
    if (buffer[0] != '\n') {
        buffer[strcspn(buffer, "\n")] = 0;
        strcpy(inventory[index].category, buffer);
    }
    
    printf("Enter new price (or -1 to keep current): ");
    float newPrice;
    scanf("%f", &newPrice);
    if (newPrice >= 0) {
        inventory[index].price = newPrice;
    }
    
    printf("Enter new stock (or -1 to keep current): ");
    int newStock;
    scanf("%d", &newStock);
    if (newStock >= 0) {
        inventory[index].stock = newStock;
    }
    
    printf("\nItem updated successfully!\n");
    saveData();
    waitForKeypress();
}

void deleteItem() {
    int id, found = 0, index;
    
    displayHeader("Delete Item");
    
    printf("Enter item ID to delete: ");
    scanf("%d", &id);
    
    for (int i = 0; i < numItems; i++) {
        if (inventory[i].id == id) {
            found = 1;
            index = i;
            break;
        }
    }
    
    if (!found) {
        printf("Item with ID %d not found!\n", id);
        waitForKeypress();
        return;
    }
    
    char confirm;
    printf("\nAre you sure you want to delete '%s'? (y/n): ", inventory[index].name);
    scanf(" %c", &confirm);
    
    if (confirm == 'y' || confirm == 'Y') {
        // Shift items to fill the gap
        for (int i = index; i < numItems - 1; i++) {
            inventory[i] = inventory[i + 1];
        }
        numItems--;
        
        printf("\nItem deleted successfully!\n");
        saveData();
    } else {
        printf("\nDeletion cancelled.\n");
    }
    
    waitForKeypress();
}

void viewItems() {
    displayHeader("Inventory List");
    
    if (numItems == 0) {
        printf("No items in inventory.\n");
        waitForKeypress();
        return;
    }
    
    printf("%-6s %-25s %-15s %-10s %-10s\n", "ID", "Name", "Category", "Price", "Stock");
    printf("--------------------------------------------------------------\n");
    
    for (int i = 0; i < numItems; i++) {
        printf("%-6d %-25s %-15s %-10.2f %-10d\n", 
               inventory[i].id, 
               inventory[i].name, 
               inventory[i].category, 
               inventory[i].price, 
               inventory[i].stock);
    }
    
    waitForKeypress();
}

void searchItems() {
    char keyword[50];
    int found = 0;
    
    displayHeader("Search Items");
    
    printf("Enter search keyword: ");
    scanf(" %[^\n]", keyword);
    
    printf("\nSearch results for '%s':\n\n", keyword);
    printf("%-6s %-25s %-15s %-10s %-10s\n", "ID", "Name", "Category", "Price", "Stock");
    printf("--------------------------------------------------------------\n");
    
    for (int i = 0; i < numItems; i++) {
        // Search in name or category
        if (strstr(inventory[i].name, keyword) != NULL || 
            strstr(inventory[i].category, keyword) != NULL) {
            printf("%-6d %-25s %-15s %-10.2f %-10d\n", 
                   inventory[i].id, 
                   inventory[i].name, 
                   inventory[i].category, 
                   inventory[i].price, 
                   inventory[i].stock);
            found = 1;
        }
    }
    
    if (!found) {
        printf("No items found matching '%s'.\n", keyword);
    }
    
    waitForKeypress();
}

// Customer management functions
void addCustomer() {
    displayHeader("Add New Customer");
    
    if (numCustomers >= MAX_CUSTOMERS) {
        printf("Customer database is full. Cannot add more customers.\n");
        waitForKeypress();
        return;
    }
    
    struct Customer newCustomer;
    
    // Generate ID (simple incremental ID)
    newCustomer.id = numCustomers > 0 ? customers[numCustomers-1].id + 1 : 2001;
    newCustomer.totalPurchases = 0.0;
    
    printf("Customer ID: %d (auto-generated)\n", newCustomer.id);
    
    printf("Enter customer name: ");
    scanf(" %[^\n]", newCustomer.name);
    
    printf("Enter phone number: ");
    scanf(" %[^\n]", newCustomer.phone);
    
    printf("Enter email: ");
    scanf(" %[^\n]", newCustomer.email);
    
    customers[numCustomers++] = newCustomer;
    
    printf("\nCustomer added successfully!\n");
    saveData();
    waitForKeypress();
}

void editCustomer() {
    int id, found = 0, index;
    
    displayHeader("Edit Customer");
    
    printf("Enter customer ID to edit: ");
    scanf("%d", &id);
    
    for (int i = 0; i < numCustomers; i++) {
        if (customers[i].id == id) {
            found = 1;
            index = i;
            break;
        }
    }
    
    if (!found) {
        printf("Customer with ID %d not found!\n", id);
        waitForKeypress();
        return;
    }
    
    printf("\nCurrent details:\n");
    printf("ID: %d\n", customers[index].id);
    printf("Name: %s\n", customers[index].name);
    printf("Phone: %s\n", customers[index].phone);
    printf("Email: %s\n", customers[index].email);
    printf("Total Purchases: %.2f\n\n", customers[index].totalPurchases);
    
    printf("Enter new name (or press enter to keep current): ");
    char buffer[50];
    getchar(); // Clear input buffer
    fgets(buffer, sizeof(buffer), stdin);
    if (buffer[0] != '\n') {
        buffer[strcspn(buffer, "\n")] = 0; // Remove newline
        strcpy(customers[index].name, buffer);
    }
    
    printf("Enter new phone (or press enter to keep current): ");
    fgets(buffer, sizeof(buffer), stdin);
    if (buffer[0] != '\n') {
        buffer[strcspn(buffer, "\n")] = 0;
        strcpy(customers[index].phone, buffer);
    }
    
    printf("Enter new email (or press enter to keep current): ");
    fgets(buffer, sizeof(buffer), stdin);
    if (buffer[0] != '\n') {
        buffer[strcspn(buffer, "\n")] = 0;
        strcpy(customers[index].email, buffer);
    }
    
    printf("\nCustomer updated successfully!\n");
    saveData();
    waitForKeypress();
}

void deleteCustomer() {
    int id, found = 0, index;
    
    displayHeader("Delete Customer");
    
    printf("Enter customer ID to delete: ");
    scanf("%d", &id);
    
    for (int i = 0; i < numCustomers; i++) {
        if (customers[i].id == id) {
            found = 1;
            index = i;
            break;
        }
    }
    
    if (!found) {
        printf("Customer with ID %d not found!\n", id);
        waitForKeypress();
        return;
    }
    
    char confirm;
    printf("\nAre you sure you want to delete customer '%s'? (y/n): ", customers[index].name);
    scanf(" %c", &confirm);
    
    if (confirm == 'y' || confirm == 'Y') {
        // Shift customers to fill the gap
        for (int i = index; i < numCustomers - 1; i++) {
            customers[i] = customers[i + 1];
        }
        numCustomers--;
        
        printf("\nCustomer deleted successfully!\n");
        saveData();
    } else {
        printf("\nDeletion cancelled.\n");
    }
    
    waitForKeypress();
}

void viewCustomers() {
    displayHeader("Customer List");
    
    if (numCustomers == 0) {
        printf("No customers in database.\n");
        waitForKeypress();
        return;
    }
    
    printf("%-6s %-25s %-15s %-25s %-15s\n", "ID", "Name", "Phone", "Email", "Total Purchases");
    printf("-------------------------------------------------------------------------\n");
    
    for (int i = 0; i < numCustomers; i++) {
        printf("%-6d %-25s %-15s %-25s %-15.2f\n", 
               customers[i].id, 
               customers[i].name, 
               customers[i].phone, 
               customers[i].email, 
               customers[i].totalPurchases);
    }
    
    waitForKeypress();
}

void searchCustomers() {
    char keyword[50];
    int found = 0;
    
    displayHeader("Search Customers");
    
    printf("Enter search keyword: ");
    scanf(" %[^\n]", keyword);
    
    printf("\nSearch results for '%s':\n\n", keyword);
    printf("%-6s %-25s %-15s %-25s %-15s\n", "ID", "Name", "Phone", "Email", "Total Purchases");
    printf("-------------------------------------------------------------------------\n");
    
    for (int i = 0; i < numCustomers; i++) {
        // Search in name, phone or email
        if (strstr(customers[i].name, keyword) != NULL || 
            strstr(customers[i].phone, keyword) != NULL || 
            strstr(customers[i].email, keyword) != NULL) {
            printf("%-6d %-25s %-15s %-25s %-15.2f\n", 
                   customers[i].id, 
                   customers[i].name, 
                   customers[i].phone, 
                   customers[i].email, 
                   customers[i].totalPurchases);
            found = 1;
        }
    }
    
    if (!found) {
        printf("No customers found matching '%s'.\n", keyword);
    }
    
    waitForKeypress();
}

// Billing functions
void createBill() {
    displayHeader("Create New Bill");
    
    if (numBills >= MAX_BILLS) {
        printf("Maximum number of bills reached!\n");
        waitForKeypress();
        return;
    }
    
    struct Bill newBill;
    newBill.numItems = 0;
    newBill.subtotal = 0;
    newBill.discount = 0;
    newBill.tax = 0;
    newBill.total = 0;
    
    // Generate bill ID
    newBill.id = numBills > 0 ? bills[numBills-1].id + 1 : 3001;
    
    // Get current date
    getCurrentDate(newBill.date);
    
    // Customer selection
    int customerChoice;
    printf("Select customer:\n");
    printf("1. Existing customer\n");
    printf("2. New customer\n");
    printf("3. Guest customer\n");
    printf("Enter choice: ");
    scanf("%d", &customerChoice);
    
    if (customerChoice == 1) {
        if (numCustomers == 0) {
            printf("No customers in database. Creating bill for guest.\n");
            newBill.customerId = 0;
            strcpy(newBill.customerName, "Guest");
        } else {
            int customerId, found = 0;
            
            // Display customer list
            printf("\nCustomer List:\n");
            printf("%-6s %-25s\n", "ID", "Name");
            printf("--------------------------------\n");
            
            for (int i = 0; i < numCustomers; i++) {
                printf("%-6d %-25s\n", customers[i].id, customers[i].name);
            }
            
            printf("\nEnter customer ID: ");
            scanf("%d", &customerId);
            
            for (int i = 0; i < numCustomers; i++) {
                if (customers[i].id == customerId) {
                    found = 1;
                    newBill.customerId = customerId;
                    strcpy(newBill.customerName, customers[i].name);
                    break;
                }
            }
            
            if (!found) {
                printf("Customer not found. Creating bill for guest.\n");
                newBill.customerId = 0;
                strcpy(newBill.customerName, "Guest");
            }
        }
    } else if (customerChoice == 2) {
        // Add new customer first
        addCustomer();
        newBill.customerId = customers[numCustomers-1].id;
        strcpy(newBill.customerName, customers[numCustomers-1].name);
    } else {
        newBill.customerId = 0;
        strcpy(newBill.customerName, "Guest");
    }
    
    // Add items to bill
    char addMore = 'y';
    while (addMore == 'y' || addMore == 'Y') {
        if (newBill.numItems >= MAX_ITEMS) {
            printf("Maximum number of items in bill reached!\n");
            break;
        }
        
        // Display available items
        printf("\nAvailable Items:\n");
        printf("%-6s %-25s %-15s %-10s %-10s\n", "ID", "Name", "Category", "Price", "Stock");
        printf("--------------------------------------------------------------\n");
        
        for (int i = 0; i < numItems; i++) {
            if (inventory[i].stock > 0) {
                printf("%-6d %-25s %-15s %-10.2f %-10d\n", 
                       inventory[i].id, 
                       inventory[i].name, 
                       inventory[i].category, 
                       inventory[i].price, 
                       inventory[i].stock);
            }
        }
        
        int itemId, found = 0, index;
        printf("\nEnter item ID: ");
        scanf("%d", &itemId);
        
        for (int i = 0; i < numItems; i++) {
            if (inventory[i].id == itemId) {
                found = 1;
                index = i;
                break;
            }
        }
        
        if (!found) {
            printf("Item not found!\n");
        } else if (inventory[index].stock <= 0) {
            printf("Item out of stock!\n");
        } else {
            int quantity;
            printf("Enter quantity (available: %d): ", inventory[index].stock);
            scanf("%d", &quantity);
            
            if (quantity <= 0) {
                printf("Invalid quantity!\n");
            } else if (quantity > inventory[index].stock) {
                printf("Not enough stock available!\n");
            } else {
                // Add item to bill
                struct BillItem billItem;
                billItem.itemId = inventory[index].id;
                strcpy(billItem.name, inventory[index].name);
                billItem.quantity = quantity;
                billItem.price = inventory[index].price;
                billItem.total = quantity * inventory[index].price;
                
                newBill.items[newBill.numItems++] = billItem;
                newBill.subtotal += billItem.total;
                
                // Update inventory
                inventory[index].stock -= quantity;
                
                printf("Item added to bill!\n");
            }
        }
        
        printf("\nAdd more items? (y/n): ");
        scanf(" %c", &addMore);
    }
    
    if (newBill.numItems == 0) {
        printf("\nNo items added to bill. Operation cancelled.\n");
        waitForKeypress();
        return;
    }
    
    // Apply discount if applicable
    float discountPercent = 0;
    printf("\nEnter discount percentage (0-100): ");
    scanf("%f", &discountPercent);
    
    if (discountPercent > 0 && discountPercent <= 100) {
        newBill.discount = newBill.subtotal * (discountPercent / 100);
    }
    
    // Calculate tax and total
    newBill.tax = (newBill.subtotal - newBill.discount) * 0.08; // 8% tax
    newBill.total = newBill.subtotal - newBill.discount + newBill.tax;
    
    // Update customer's total purchases
    if (newBill.customerId != 0) {
        for (int i = 0; i < numCustomers; i++) {
            if (customers[i].id == newBill.customerId) {
                customers[i].totalPurchases += newBill.total;
                break;
            }
        }
    }
    
    // Save bill
    bills[numBills++] = newBill;
    saveData();
    
    // Print bill
    displayHeader("Bill Receipt");
    printf("Bill ID: %d\n", newBill.id);
    printf("Date: %s\n", newBill.date);
    printf("Customer: %s\n\n", newBill.customerName);
    
    printf("%-30s %-10s %-10s %-10s\n", "Item", "Quantity", "Price", "Total");
    printf("--------------------------------------------------------------\n");
    
    for (int i = 0; i < newBill.numItems; i++) {
        printf("%-30s %-10d %-10.2f %-10.2f\n", 
               newBill.items[i].name, 
               newBill.items[i].quantity, 
               newBill.items[i].price, 
               newBill.items[i].total);
    }
    
    printf("\nSubtotal: %.2f\n", newBill.subtotal);
    if (newBill.discount > 0) {
        printf("Discount: %.2f\n", newBill.discount);
    }
    printf("Tax (8%%): %.2f\n", newBill.tax);
    printf("Total: %.2f\n", newBill.total);
    
    printf("\nThank you for your purchase!\n");
    waitForKeypress();
}

void viewBills() {
    displayHeader("View Bills");
    
    if (numBills == 0) {
        printf("No bills in database.\n");
        waitForKeypress();
        return;
    }
    
    printf("%-6s %-12s %-25s %-15s\n", "ID", "Date", "Customer", "Total");
    printf("--------------------------------------------------\n");
    
    for (int i = 0; i < numBills; i++) {
        printf("%-6d %-12s %-25s %-15.2f\n", 
               bills[i].id, 
               bills[i].date, 
               bills[i].customerName, 
               bills[i].total);
    }
    
    int billId, found = 0, index;
    printf("\nEnter bill ID to view details (or 0 to return): ");
    scanf("%d", &billId);
    
    if (billId == 0) {
        return;
    }
    
    for (int i = 0; i < numBills; i++) {
        if (bills[i].id == billId) {
            found = 1;
            index = i;
            break;
        }
    }
    
    if (!found) {
        printf("Bill with ID %d not found!\n", billId);
        waitForKeypress();
        return;
    }
    
    // Display bill details
    displayHeader("Bill Details");
    printf("Bill ID: %d\n", bills[index].id);
    printf("Date: %s\n", bills[index].date);
    printf("Customer: %s\n\n", bills[index].customerName);
    
    printf("%-30s %-10s %-10s %-10s\n", "Item", "Quantity", "Price", "Total");
    printf("--------------------------------------------------------------\n");
    
    for (int i = 0; i < bills[index].numItems; i++) {
        printf("%-30s %-10d %-10.2f %-10.2f\n", 
               bills[index].items[i].name, 
               bills[index].items[i].quantity, 
               bills[index].items[i].price, 
               bills[index].items[i].total);
    }
    
    printf("\nSubtotal: %.2f\n", bills[index].subtotal);
    if (bills[index].discount > 0) {
        printf("Discount: %.2f\n", bills[index].discount);
    }
    printf("Tax (8%%): %.2f\n", bills[index].tax);
    printf("Total: %.2f\n", bills[index].total);
    
    waitForKeypress();
}

void searchBills() {
    char keyword[50];
    int found = 0;
    
    displayHeader("Search Bills");
    
    printf("Enter search keyword (customer name or date): ");
    scanf(" %[^\n]", keyword);
    
    printf("\nSearch results for '%s':\n\n", keyword);
    printf("%-6s %-12s %-25s %-15s\n", "ID", "Date", "Customer", "Total");
    printf("--------------------------------------------------\n");
    
    for (int i = 0; i < numBills; i++) {
        // Search in customer name or date
        if (strstr(bills[i].customerName, keyword) != NULL || 
            strstr(bills[i].date, keyword) != NULL) {
            printf("%-6d %-12s %-25s %-15.2f\n", 
                   bills[i].id, 
                   bills[i].date, 
                   bills[i].customerName, 
                   bills[i].total);
            found = 1;
        }
    }
    
    if (!found) {
        printf("No bills found matching '%s'.\n", keyword);
    }
    
    waitForKeypress();
}

// Reports function
void generateReport() {
    int reportType;
    
    displayHeader("Generate Reports");
    
    printf("Select report type:\n");
    printf("1. Sales Summary\n");
    printf("2. Inventory Status\n");
    printf("3. Top Customers\n");
    printf("Enter choice: ");
    scanf("%d", &reportType);
    
    switch (reportType) {
        case 1: {
            // Sales Summary
            displayHeader("Sales Summary Report");
            
            if (numBills == 0) {
                printf("No sales data available.\n");
                waitForKeypress();
                return;
            }
            
            float totalSales = 0, totalTax = 0, totalDiscount = 0;
            int totalItems = 0;
            
            for (int i = 0; i < numBills; i++) {
                totalSales += bills[i].total;
                totalTax += bills[i].tax;
                totalDiscount += bills[i].discount;
                totalItems += bills[i].numItems;
            }
            
            printf("Total Bills: %d\n", numBills);
            printf("Total Items Sold: %d\n", totalItems);
            printf("Total Sales: %.2f\n", totalSales);
            printf("Total Discounts: %.2f\n", totalDiscount);
            printf("Total Tax Collected: %.2f\n", totalTax);
            printf("Average Bill Amount: %.2f\n", totalSales / numBills);
            
            break;
        }
        case 2: {
            // Inventory Status
            displayHeader("Inventory Status Report");
            
            if (numItems == 0) {
                printf("No inventory data available.\n");
                waitForKeypress();
                return;
            }
            
            int lowStockCount = 0;
            float inventoryValue = 0;
            
            printf("Low Stock Items (5 or fewer):\n\n");
            printf("%-6s %-25s %-15s %-10s\n", "ID", "Name", "Category", "Stock");
            printf("--------------------------------------------------\n");
            
            for (int i = 0; i < numItems; i++) {
                inventoryValue += inventory[i].price * inventory[i].stock;
                
                if (inventory[i].stock <= 5) {
                    printf("%-6d %-25s %-15s %-10d\n", 
                           inventory[i].id, 
                           inventory[i].name, 
                           inventory[i].category, 
                           inventory[i].stock);
                    lowStockCount++;
                }
            }
            
            if (lowStockCount == 0) {
                printf("No items with low stock.\n");
            }
            
            printf("\nTotal Inventory Value: %.2f\n", inventoryValue);
            printf("Total Items: %d\n", numItems);
            printf("Low Stock Items: %d\n", lowStockCount);
            
            break;
        }
        case 3: {
            // Top Customers
            displayHeader("Top Customers Report");
            
            if (numCustomers == 0) {
                printf("No customer data available.\n");
                waitForKeypress();
                return;
            }
            
            // Create a copy of customers array for sorting
            struct Customer sortedCustomers[MAX_CUSTOMERS];
            for (int i = 0; i < numCustomers; i++) {
                sortedCustomers[i] = customers[i];
            }
            
            // Sort by total purchases (simple bubble sort)
            for (int i = 0; i < numCustomers - 1; i++) {
                for (int j = 0; j < numCustomers - i - 1; j++) {
                    if (sortedCustomers[j].totalPurchases < sortedCustomers[j + 1].totalPurchases) {
                        struct Customer temp = sortedCustomers[j];
                        sortedCustomers[j] = sortedCustomers[j + 1];
                        sortedCustomers[j + 1] = temp;
                    }
                }
            }
            
            printf("Top Customers by Purchase Amount:\n\n");
            printf("%-6s %-25s %-15s %-15s\n", "Rank", "Name", "Phone", "Total Purchases");
            printf("--------------------------------------------------\n");
            
            int limit = numCustomers > 10 ? 10 : numCustomers;
            
            for (int i = 0; i < limit; i++) {
                if (sortedCustomers[i].totalPurchases > 0) {
                    printf("%-6d %-25s %-15s %-15.2f\n", 
                           i + 1, 
                           sortedCustomers[i].name, 
                           sortedCustomers[i].phone, 
                           sortedCustomers[i].totalPurchases);
                }
            }
            
            break;
        }
        default:
            printf("Invalid report type!\n");
    }
    
    waitForKeypress();
}

// Menu functions
void inventoryMenu() {
    char choice;
    
    do {
        displayHeader("Inventory Management");
        
        printf("1. Add New Item\n");
        printf("2. Edit Item\n");
        printf("3. Delete Item\n");
        printf("4. View All Items\n");
        printf("5. Search Items\n");
        printf("0. Back to Main Menu\n\n");
        
        printf("Enter your choice: ");
        scanf(" %c", &choice);
        
        switch (choice) {
            case '1':
                addItem();
                break;
            case '2':
                editItem();
                break;
            case '3':
                deleteItem();
                break;
            case '4':
                viewItems();
                break;
            case '5':
                searchItems();
                break;
            case '0':
                break;
            default:
                printf("Invalid choice!\n");
                waitForKeypress();
        }
    } while (choice != '0');
}

void customerMenu() {
    char choice;
    
    do {
        displayHeader("Customer Management");
        
        printf("1. Add New Customer\n");
        printf("2. Edit Customer\n");
        printf("3. Delete Customer\n");
        printf("4. View All Customers\n");
        printf("5. Search Customers\n");
        printf("0. Back to Main Menu\n\n");
        
        printf("Enter your choice: ");
        scanf(" %c", &choice);
        
        switch (choice) {
            case '1':
                addCustomer();
                break;
            case '2':
                editCustomer();
                break;
            case '3':
                deleteCustomer();
                break;
            case '4':
                viewCustomers();
                break;
            case '5':
                searchCustomers();
                break;
            case '0':
                break;
            default:
                printf("Invalid choice!\n");
                waitForKeypress();
        }
    } while (choice != '0');
}

void billingMenu() {
    char choice;
    
    do {
        displayHeader("Billing Management");
        
        printf("1. Create New Bill\n");
        printf("2. View Bills\n");
        printf("3. Search Bills\n");
        printf("0. Back to Main Menu\n\n");
        
        printf("Enter your choice: ");
        scanf(" %c", &choice);
        
        switch (choice) {
            case '1':
                createBill();
                break;
            case '2':
                viewBills();
                break;
            case '3':
                searchBills();
                break;
            case '0':
                break;
            default:
                printf("Invalid choice!\n");
                waitForKeypress();
        }
    } while (choice != '0');
}

void reportsMenu() {
    char choice;
    
    do {
        displayHeader("Reports");
        
        printf("1. Generate Reports\n");
        printf("0. Back to Main Menu\n\n");
        
        printf("Enter your choice: ");
        scanf(" %c", &choice);
        
        switch (choice) {
            case '1':
                generateReport();
                break;
            case '0':
                break;
            default:
                printf("Invalid choice!\n");
                waitForKeypress();
        }
    } while (choice != '0');
}

void mainMenu() {
    char choice;
    
    do {
        displayHeader("Billing System - Main Menu");
        
        printf("1. Inventory Management\n");
        printf("2. Customer Management\n");
        printf("3. Billing\n");
        printf("4. Reports\n");
        printf("0. Exit\n\n");
        
        printf("Enter your choice: ");
        scanf(" %c", &choice);
        
        switch (choice) {
            case '1':
                inventoryMenu();
                break;
            case '2':
                customerMenu();
                break;
            case '3':
                billingMenu();
                break;
            case '4':
                reportsMenu();
                break;
            case '0':
                printf("\nThank you for using the Billing System!\n");
                break;
            default:
                printf("Invalid choice!\n");
                waitForKeypress();
        }
    } while (choice != '0');
}

int main() {
    // Set console title
    SetConsoleTitle("Advanced Billing System");
    
    // Load data from files
    loadData();
    
    // Display welcome message
    clearScreen();
    printf("\n\n\n\n");
    printf("\t\t================================================\n");
    printf("\t\t|                                              |\n");
    printf("\t\t|        WELCOME TO THE BILLING SYSTEM        |\n");
    printf("\t\t|                                              |\n");
    printf("\t\t================================================\n");
    printf("\n\n\t\tPress any key to continue...");
    getch();
    
    // Show main menu
    mainMenu();
    
    return 0;
}
