#include<iostream>
#include<string>
using namespace std;


//FUNCTION PROTOTYPES
int displayBurgers();
int displaySandwiches();
int displaySides();
int displayDrinks();
int displayDesserts();

void displayMenu();
void customerPortal();

void addToInvoice(string name, int price, int qty);
void takeOrder(int category, int pick);
void printInvoice();
//Global Variables for storing menu items and their prices

const int MAX_ITEMS = 50;

// invoice data (1D arrays)
string invNames[MAX_ITEMS];
int invPrices[MAX_ITEMS];
int invQty[MAX_ITEMS];
int invCount = 0;


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
        else if (choice > 2)
        {
            cout << "invalid option please enter you choice again" << endl;
        }

    } while (choice != 2);
}


void addToInvoice(string name, int price, int qty)
{
	invNames[invCount] = name;
	invPrices[invCount] = price;
	invQty[invCount] = qty;
	invCount++;
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

	switch (category)
	{
	case 1:
		if (pick >= 1 && pick <= 5)
			addToInvoice(burgers[pick - 1], burgerPrices[pick - 1], qty);
		else
			cout << "Invalid item selected." << endl;
		break;

	case 2:
		if (pick >= 1 && pick <= 3)
			addToInvoice(sandwiches[pick - 1], sandwichPrices[pick - 1], qty);
		else
			cout << "Invalid item selected." << endl;
		break;

	case 3:
		if (pick >= 1 && pick <= 4)
			addToInvoice(sides[pick - 1], sidePrices[pick - 1], qty);
		else
			cout << "Invalid item selected." << endl;
		break;

	case 4:
		if (pick >= 1 && pick <= 6)
			addToInvoice(drinks[pick - 1], drinkPrices[pick - 1], qty);
		else
			cout << "Invalid item selected." << endl;
		break;

	case 5:
		if (pick >= 1 && pick <= 3)
			addToInvoice(desserts[pick - 1], dessertPrices[pick - 1], qty);
		else
			cout << "Invalid item selected." << endl;
		break;

	default:
		cout << "Invalid category." << endl;
	}
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




//PROGRAM EXECUTION STARTS FROM HERE.

int main()
{
	customerPortal();
	printInvoice();
	return 0;
}

