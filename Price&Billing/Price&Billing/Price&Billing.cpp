#include<iostream>
#include<string>
#include<fstream>
#include<ctime>
#include "GUI_forsales.h"

using namespace std;


//FUNCTION PROTOTYPES 
int displayBurgers();
int displaySandwiches();
int displaySides();
int displayDrinks();
int displayDesserts();

void displayMenu();
void customerPortal();

bool addToInvoice(string name, int price, int qty);
void takeOrder(int category, int pick);
void printInvoice();

//FUNCTION PROTOTYPES for date data .

void loadDate();
void saveDate();
string getCurrentDate();
void saveInvoiceToFile();
void viewSalesHistory();

//FUNCTION PROTOTYPES ( also declared in GUI_forsales.h)
string getCategoryName(int category);
int getItemCount(int category);
string getItemName(int category, int pick);
int getItemPrice(int category, int pick);
bool addItemToOrder(int category, int pick, int qty);
int getInvoiceCount();
string getInvoiceName(int index);
int getInvoiceQty(int index);
int getInvoiceLineTotal(int index);
int getInvoiceTotal();
void clearInvoice();


//Global Variables for storing menu items and their prices

const int MAX_ITEMS = 50;
int currentDay = 1;
int currentMonth = 1;
int currentYear = 2026;

// invoice data (1D arrays)
string invNames[MAX_ITEMS];
int invPrices[MAX_ITEMS];
int invQty[MAX_ITEMS];
int invCount = 0;


//CATEGORY NAMES

string categoryNames[NUM_CATEGORIES] = {
	"Burgers",
	"Sandwiches and Wraps",
	"Sides",
	"Drinks",
	"Desserts"
};


//BURGERS DATA

string burgers[5] = {
		"Classic Cheeseburger",
		"Spicy Zinger Burger",
		"Black Beast Burger",
		"Smoky BBQ Burger",
		"Double Trouble Burger"
};

int burgerPrices[5] = {
	500, // Classic Cheeseburger
	600, // Spicy Zinger Burger
	700, // Black Beast Burger
	650, // Smoky BBQ Burger
	800  // Double Trouble Burger
};


//SANDWICHES DATA

string sandwiches[3] = {
		"Grilled Chicken Sandwich",
		"Crispy Chicken Wrap",
		"Beef Fajita Wrap"
};
int sandwichPrices[3] = {
	550, // Grilled Chicken Sandwich
	500, // Crispy Chicken Wrap
	650  // Beef Fajita Wrap
};

//SIDES DATA

string sides[4] = {
		"Classic Fries",
		"Loaded Fries",
		"Chicken Nuggets",
		"Onion Rings"
};
int sidePrices[4] = {
	250, // Classic Fries
	400, // Loaded Fries
	350, // Chicken Nuggets
	300  // Onion Rings
};

//DRINKS DATA

string drinks[6] = {
		"Blue Lagoon Crusher",
		"Strawberry Crusher",
		"Mango Crusher",
		"Chocolate Milkshake",
		"Oreo Milkshake",
		"Soft Drinks"
};
int drinkPrices[6] = {
	300, // Blue Lagoon Crusher
	300, // Strawberry Crusher
	300, // Mango Crusher
	400, // Chocolate Milkshake
	450, // Oreo Milkshake
	150  // Soft Drinks
};

//DESSERTS DATA

string desserts[3] = {
	"Chocolate Brownie",
	"Molten Lava Cake",
	"Ice Cream"
};


int dessertPrices[3] = {
	300, // Chocolate Brownie
	400, // Molten Lava Cake
	200  // Ice Cream
};




//  FUNCTIONS USED BY THE GUIs

string getCategoryName(int category)
{
	if (category >= 1 && category <= NUM_CATEGORIES)

		return categoryNames[category - 1];
	return "";
}

int getItemCount(int category)
{
	switch (category)
	{
	case 1: return 5;
	case 2: return 3;
	case 3: return 4;
	case 4: return 6;
	case 5: return 3;
	default: return 0;
	}
}

string getItemName(int category, int pick)
{
	if (pick < 1 || pick > getItemCount(category))
		return "";

	switch (category)
	{
	case 1: return burgers[pick - 1];
	case 2: return sandwiches[pick - 1];
	case 3: return sides[pick - 1];
	case 4: return drinks[pick - 1];
	case 5: return desserts[pick - 1];
	default: return "";
	}
}

int getItemPrice(int category, int pick)
{
	if (pick < 1 || pick > getItemCount(category))
		return 0;

	switch (category)
	{
	case 1: return burgerPrices[pick - 1];
	case 2: return sandwichPrices[pick - 1];
	case 3: return sidePrices[pick - 1];
	case 4: return drinkPrices[pick - 1];
	case 5: return dessertPrices[pick - 1];
	default: return 0;
	}
}

// checks the choice and adds the item to the invoice; returns true if it was added
bool addItemToOrder(int category, int pick, int qty)
{
	if (qty <= 0)
		return false;

	if (pick < 1 || pick > getItemCount(category))
		return false;

	return addToInvoice(getItemName(category, pick), getItemPrice(category, pick), qty);
}

int getInvoiceCount()
{
	return invCount;
}

string getInvoiceName(int index)
{
	if (index < 0 || index >= invCount)
		return "";
	return invNames[index];
}

int getInvoiceQty(int index)
{
	if (index < 0 || index >= invCount)
		return 0;
	return invQty[index];
}

int getInvoiceLineTotal(int index)
{
	if (index < 0 || index >= invCount)
		return 0;
	return invPrices[index] * invQty[index];
}

int getInvoiceTotal()
{
	int total = 0;
	for (int i = 0; i < invCount; i++)
		total += invPrices[i] * invQty[i];
	return total;
}

void clearInvoice()
{
	invCount = 0;
}




int displayBurgers()
{
	cout << "\n--- BURGERS ---\n";
	for (int i = 0; i < 5; i++)
		cout << i + 1 << ". " << burgers[i] << " - Rs " << burgerPrices[i] << endl;
	cout << "0. Back\n";

	int choice = 0;
	cout << "Select burger: ";
	cin >> choice;
	return choice;       // 0 = back, 1-5 = burger number
}

int displaySandwiches()
{
	cout << endl << "\n----- SANDWICHES AND WRAPS -----\n" << endl;
	for (int i = 0; i < 3; i++)
		cout << i + 1 << ". " << sandwiches[i] << " - Rs " << sandwichPrices[i] << endl;
	cout << "0. Back\n";

	int choice = 0;
	cout << "Select sandwich: ";
	cin >> choice;
	return choice;          // 0 = back, 1-3 = sandwich number

}

int displaySides()
{
	cout << endl << "\n----- SIDES -----\n" << endl;

	for (int i = 0; i < 4; i++)
	{
		cout << i + 1 << ". " << sides[i] << " - Rs " << sidePrices[i] << endl;
	}
	cout << "0. Back\n";

	int choice = 0;
	cout << "Select side: ";
	cin >> choice;
	return choice;          // 0 = back, 1-4= side number

}

int  displayDrinks()
{
	cout << endl << "\n----- DRINKS -----\n" << endl;


	for (int i = 0; i < 6; i++)
	{
		cout << i + 1 << ". " << drinks[i] << " - Rs " << drinkPrices[i] << endl;
	}
	cout << "0. Back\n";

	int choice = 0;
	cout << "Select drink: ";
	cin >> choice;
	return choice;          // 0 = back, 1-6 = drink number

}

int displayDesserts()
{
	cout << endl << "\n----- DESSERTS -----\n" << endl;

	for (int i = 0; i < 3; i++)
	{
		cout << i + 1 << ". " << desserts[i] << " - Rs " << dessertPrices[i] << endl;
	}
	cout << "0. Back\n";


	int choice = 0;
	cout << "Select dessert: ";
	cin >> choice;
	return choice;          // 0 = back, 1-3 = dessert number
}


void displayMenu()
{
	int choice;

	do
	{
		cout << endl << "========== BREW & BITE CAFE ==========" << endl;
		cout << "1. Burgers" << endl;
		cout << "2. Sandwiches and Wraps" << endl;
		cout << "3. Sides" << endl;
		cout << "4. Drinks" << endl;
		cout << "5. Desserts" << endl;
		cout << "6. Back to Customer Portal" << endl;
		cout << "Enter your choice: ";
		cin >> choice;

		int pick = 0;

		switch (choice)
		{
		case 1:
			pick = displayBurgers();
			break;
		case 2:
			pick = displaySandwiches();
			break;
		case 3:
			pick = displaySides();
			break;
		case 4:
			pick = displayDrinks();
			break;
		case 5:
			pick = displayDesserts();
			break;
		case 6:
			break;                      // loop ends because choice == 6
		default:
			cout << "invalid choice" << endl;
		}

		// a category was opened and the user didn't press 0 (Back)
		if (choice >= 1 && choice <= 5 && pick != 0)
			takeOrder(choice, pick);

	} while (choice != 6);
}


void customerPortal()
{
	int choice;

	do
	{
		cout << endl << "========== CUSTOMER PORTAL ==========" << endl;
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


// adds an item to the invoice; if the same item is already there, its quantity is increased
// returns false if the invoice is full
bool addToInvoice(string name, int price, int qty)
{
	for (int i = 0; i < invCount; i++)
	{
		if (invNames[i] == name)
		{
			invQty[i] += qty;
			return true;
		}
	}

	if (invCount >= MAX_ITEMS)
		return false;

	invNames[invCount] = name;
	invPrices[invCount] = price;
	invQty[invCount] = qty;
	invCount++;
	return true;
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
	for (int i = 0; i < invCount; i++)
	{
		int lineTotal = invPrices[i] * invQty[i];
		cout << invNames[i] << " x" << invQty[i] << " = Rs " << lineTotal << endl;
		total += lineTotal;
	}
	cout << "TOTAL: Rs " << total << endl;
}




// Sets the date.
void loadDate()
{
	ifstream inFile("date.txt");

	if (inFile >> currentDay >> currentMonth >> currentYear)
	{
		// This if condition will only run after the first date is stored .
		inFile.close();

		tm t = {};
		t.tm_mday = currentDay + 1;
		t.tm_mon = currentMonth - 1;
		t.tm_year = currentYear - 1900;
		t.tm_hour = 12;
		mktime(&t);

		currentDay = t.tm_mday;
		currentMonth = t.tm_mon + 1;
		currentYear = t.tm_year + 1900;
	}
	else
	{
		// first run .
		time_t now = time(0);
		tm t;
		localtime_s(&t, &now);

		currentDay = t.tm_mday;
		currentMonth = t.tm_mon + 1;
		currentYear = t.tm_year + 1900;
	}

	saveDate();
}

void saveDate()
{
	ofstream outFile("date.txt");
	outFile << currentDay << " " << currentMonth << " " << currentYear;
	outFile.close();
}

// returns the date.
string getCurrentDate()
{
	string d = to_string(currentDay);
	string m = to_string(currentMonth);
	if (currentDay < 10)   d = "0" + d;
	if (currentMonth < 10) m = "0" + m;
	return d + "-" + m + "-" + to_string(currentYear);
}

// adds the current invoice to the end of sales.txt
void saveInvoiceToFile()
{
	if (invCount == 0)
		return;

	ofstream outFile("sales.txt", ios::app);   // app = append, keeps old data
	if (!outFile)
	{
		cout << "Could not open sales file." << endl;
		return;
	}

	outFile << "Date: " << getCurrentDate() << endl;
	for (int i = 0; i < invCount; i++)
		outFile << invNames[i] << " x" << invQty[i] << " = Rs " << invPrices[i] * invQty[i] << endl;
	outFile << "TOTAL: Rs " << getInvoiceTotal() << endl;
	outFile << "-----------------------" << endl;

	outFile.close();
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


//PROGRAM EXECUTION STARTS FROM HERE.

const bool USE_GUI = true;

int main()
{
	loadDate();
	if (USE_GUI)
	{
		runGUI();
	}
	else
	{
		customerPortal();
		printInvoice();
	}

	return 0;
}
