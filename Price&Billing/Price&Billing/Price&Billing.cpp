#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <ctime>
#include "GUI_forsales.h"

using namespace std;

// FUNCTION PROTOTYPES
int displayCategoryItems(int category);
void displayMenu();
void customerPortal();

bool add_this_to_invoice(string name, int price, int qty);
void takeOrder(int category, int pick);
void printInvoice();
void SalesSection();

// FUNCTION PROTOTYPES for date data .

void load_date();
void saveDate();
string find_currunt_date();
void send_invoice_to_file();
void viewSalesHistory();

// FUNCTION PROTOTYPES ( also declared in GUI_forsales.h)
string getCategoryName(int category);
int getItemCount(int category);
string getItemName(int category, int pick);
int getItemPrice(int category, int pick);
bool addItemToOrder(int category, int pick, int qty);
int getInvoiceCount();
string getInvoiceName(int index);
int getInvoiceQty(int index);
int getInvoiceLineTotal(int index);
int invoice_total_is();
void clear_invoice();

// These are defined once in shared_data.cpp (and getCategoryName in Inventory.cpp)
extern int current_day;
extern int current_month;
extern int current_year;
extern string inventory_name[];
extern int inventory_price[];
extern int inventory_qut[];
extern int inventory_count;

// The menu comes from the product inventory (Inventory.cpp / inventory.txt),
// so products added on the Product screen show up here, and
// products marked "Unavailable" are hidden from the menu.
extern int product_counts;
extern string productNames[];
extern int productPrices[];
extern int productCategories[];
extern bool productAvailable[];
void loadInventory();

// most items one category can show on the sales screen
const int MAX_ITEMS_PER_CATEGORY = 14;

// returns the inventory index of the pick-th (1-based) available product in a category, or -1
int findMenuProduct(int category, int pick)
{
	int found = 0;
	for (int i = 0; i < product_counts; i++)
	{
		if (productCategories[i] == category && productAvailable[i])
		{
			found++;
			if (found == pick)
				return i;
		}
	}
	return -1;
}

//  FUNCTIONS USED BY THE GUIs

int getItemCount(int category)
{
	int count = 0;
	for (int i = 0; i < product_counts; i++)
		if (productCategories[i] == category && productAvailable[i])
			count++;

	if (count > MAX_ITEMS_PER_CATEGORY)
		count = MAX_ITEMS_PER_CATEGORY;
	return count;
}

string getItemName(int category, int pick)
{
	if (pick < 1 || pick > getItemCount(category))
		return "";

	int index = findMenuProduct(category, pick);
	return index == -1 ? "" : productNames[index];
}

int getItemPrice(int category, int pick)
{
	if (pick < 1 || pick > getItemCount(category))
		return 0;

	int index = findMenuProduct(category, pick);
	return index == -1 ? 0 : productPrices[index];
}

// checks the choice and adds the item to the invoice; returns true if it was added
bool addItemToOrder(int category, int pick, int qty)
{
	if (qty <= 0)
		return false;

	if (pick < 1 || pick > getItemCount(category))
		return false;

	return add_this_to_invoice(getItemName(category, pick), getItemPrice(category, pick), qty);
}

int getInvoiceCount()
{
	return inventory_count;
}

string getInvoiceName(int index)
{
	if (index < 0 || index >= inventory_count)
		return "";
	return inventory_name[index];
}

int getInvoiceQty(int index)
{
	if (index < 0 || index >= inventory_count)
		return 0;
	return inventory_qut[index];
}

int getInvoiceLineTotal(int index)
{
	if (index < 0 || index >= inventory_count)
		return 0;
	return inventory_price[index] * inventory_qut[index];
}

// console version: shows one category's items and returns the picked number (0 = back)
int displayCategoryItems(int category)
{
	cout << endl
		 << "----- " << getCategoryName(category) << " -----" << endl;

	int count = getItemCount(category);
	if (count == 0)
		cout << "(no items available)" << endl;

	for (int i = 1; i <= count; i++)
		cout << i << ". " << getItemName(category, i) << " - Rs " << getItemPrice(category, i) << endl;
	cout << "0. Back\n";

	int choice = 0;
	cout << "Select item: ";
	cin >> choice;
	return choice;
}

void displayMenu()
{
	int choice;

	do
	{
		cout << endl
			 << "========== BREW & BITE CAFE ==========" << endl;
		for (int c = 1; c <= NUM_CATEGORIES; c++)
			cout << c << ". " << getCategoryName(c) << endl;
		cout << "6. Back to Customer Portal" << endl;
		cout << "Enter your choice: ";
		cin >> choice;

		int pick = 0;

		if (choice >= 1 && choice <= NUM_CATEGORIES)
			pick = displayCategoryItems(choice);
		else if (choice != 6)
			cout << "invalid choice" << endl;

		// a category was opened and the user didn't press 0 (Back)
		if (choice >= 1 && choice <= NUM_CATEGORIES && pick != 0)
			takeOrder(choice, pick);

	} while (choice != 6);
}

void customerPortal()
{
	int choice;

	do
	{
		cout << endl
			 << "========== CUSTOMER PORTAL ==========" << endl;
		cout << "1. View Menu" << endl;
		cout << "2. Exit" << endl;
		cout << "Enter your choice: ";
		cin >> choice;

		if (choice == 1)
		{
			displayMenu();
		}
		else if (choice == 2)
		{
			cout << "thankyou for your visit we hope to serve you again" << endl;
		}
		else
		{
			cout << "invalid option please enter you choice again" << endl;
		}

	} while (choice != 2);
}

void takeOrder(int category, int pick)
{
	int qty;
	cout << "Quantity: ";
	cin >> qty;

	if (qty <= 0)
	{
		cout << "Invalid quantity." << endl;
		return;
	}

	if (!addItemToOrder(category, pick, qty))
		cout << "Invalid item selected or invoice is full." << endl;
}

void printInvoice()
{
	int total = 0;
	cout << "\n===== INVOICE =====\n";
	for (int i = 0; i < inventory_count; i++)
	{
		int lineTotal = inventory_price[i] * inventory_qut[i];
		cout << inventory_name[i] << " x" << inventory_qut[i] << " = Rs " << lineTotal << endl;
		total += lineTotal;
	}
	cout << "TOTAL: Rs " << total << endl;
}

// prints everything saved in sales.txt
void viewSalesHistory()
{
	ifstream inFile("sales.txt");
	if (!inFile)
	{
		cout << "No sales recorded yet." << endl;
		return;
	}

	string line;
	while (getline(inFile, line))
		cout << line << endl;

	inFile.close();
}

void SalesSection()
{
	const bool USE_GUI = true;
	// load_date() is already called once in main(); calling it again moved the date one more day

	// read the latest products (new products / availability changes from the Product screen)
	loadInventory();

	if (USE_GUI)
	{
		runGUI();
	}
	else
	{
		customerPortal();
		printInvoice();
	}
}
