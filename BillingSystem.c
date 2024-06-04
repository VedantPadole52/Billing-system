#include <stdio.h>
#include <string.h>

#define MAX_ITEMS 50

struct Item {
  char name[50];
  int quantity;
  float price;
};

void calculate_bill(struct Item items[], int num_items, float *subtotal, float *tax, float *total) {
  *subtotal = 0;
  for (int i = 0; i < num_items; i++) {
    *subtotal += items[i].quantity * items[i].price;
  }
  *tax = *subtotal * 0.08; // Assuming 8% tax
  *total = *subtotal + *tax;
}

void print_bill(struct Item items[], int num_items, float subtotal, float tax, float total) {
  printf("** Bill **\n\n");
  printf("Items\t\tQuantity\tPrice\tTotal Price\n");
  printf("------\t\t--------\t-------\t------------\n");
  for (int i = 0; i < num_items; i++) {
    printf("%-25s\t%d\t%.2f\t%.2f\n", items[i].name, items[i].quantity, items[i].price, items[i].quantity * items[i].price);
  }
  printf("Subtotal:\t\t\t%.2f\n", subtotal);
  printf("Tax (8%%):\t\t\t%.2f\n", tax);
  printf("Total:\t\t\t\t%.2f\n", total);
}

int main() {
  struct Item items[MAX_ITEMS];
  int num_items = 0;
  char choice;

  do {
    printf("\nBilling System\n");
    printf("1. Add Item\n");
    printf("2. Generate Bill\n");
    printf("3. Exit\n");
    printf("Enter your choice: ");
    scanf(" %c", &choice);

    switch (choice) {
      case '1':
        if (num_items == MAX_ITEMS) {
          printf("Maximum number of items reached!\n");
        } else {
          printf("Enter item name: ");
          scanf(" %s", items[num_items].name);
          printf("Enter quantity: ");
          scanf("%d", &items[num_items].quantity);
          printf("Enter price: ");
          scanf("%f", &items[num_items].price);
          num_items++;
        }
        break;
      case '2':
        if (num_items == 0) {
          printf("No items added yet!\n");
        } else {
          float subtotal, tax, total;
          calculate_bill(items, num_items, &subtotal, &tax, &total);
          print_bill(items, num_items, subtotal, tax, total);
        }
        break;
      case '3':
        printf("Exiting...\n");
        break;
      default:
        printf("Invalid choice!\n");
    }
  } while (choice != '3');

  return 0;
}
