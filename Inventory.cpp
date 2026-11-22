
#include <iostream>
#include <string>
#include <fstream>
using namespace std;

const int MAX_PRODUCTS = 100;

int productIDs[MAX_PRODUCTS];
string productNames[MAX_PRODUCTS];
int productPrices[MAX_PRODUCTS];
int productCategories[MAX_PRODUCTS];
bool productAvailable[MAX_PRODUCTS];

int productCount = 0;

string getCategoryName(int category)
{
    if (category == 1)
        return "Burgers";
    else if (category == 2)
        return "Sandwiches and Wraps";
    else if (category == 3)
        return "Sides";
    else if (category == 4)
        return "Drinks";
    else if (category == 5)
        return "Desserts";

    return "Unknown";
}

void displayCategories()
{
    cout << "1. Burgers" << endl;
    cout << "2. Sandwiches and Wraps" << endl;
    cout << "3. Sides" << endl;
    cout << "4. Drinks" << endl;
    cout << "5. Desserts" << endl;
}

bool addProduct(int id, string name, int price, int category)
{
    if (productCount >= MAX_PRODUCTS)
        return false;

    productIDs[productCount] = id;
    productNames[productCount] = name;
    productPrices[productCount] = price;
    productCategories[productCount] = category;
    productAvailable[productCount] = true;

    productCount++;

    return true;
}

int generateProductID(int category)
{
    int largestID = category * 100;

    for (int i = 0; i < productCount; i++)
    {
        if (productCategories[i] == category &&
            productIDs[i] > largestID)
        {
            largestID = productIDs[i];
        }
    }

    return largestID + 1;
}

void saveInventory()
{
    ofstream file("inventory.txt");

    if (!file)
    {
        cout << "Unable to save inventory!" << endl;
        return;
    }

    for (int i = 0; i < productCount; i++)
    {
        file << productIDs[i] << " "
             << productCategories[i] << " "
             << productPrices[i] << " "
             << productAvailable[i] << " "
             << productNames[i] << endl;
    }

    file.close();
}

void loadInventory()
{
    ifstream file("inventory.txt");
    productCount = 0;
    if (file)
    {
        int id, category, price;
        bool available;
        string name;

        while (file >> id >> category >> price >> available)
        {
            file.ignore();

            getline(file, name);

            if (productCount < MAX_PRODUCTS)
            {
                productIDs[productCount] = id;
                productCategories[productCount] = category;
                productPrices[productCount] = price;
                productAvailable[productCount] = available;
                productNames[productCount] = name;

                productCount++;
            }
        }

        file.close();
    }

    if (productCount == 0)
    {
        addProduct(101, "Classic Cheeseburger", 500, 1);
        addProduct(102, "Spicy Zinger Burger", 600, 1);
        addProduct(103, "Black Beast Burger", 700, 1);
        addProduct(104, "Smoky BBQ Burger", 650, 1);
        addProduct(105, "Double Trouble Burger", 800, 1);

        addProduct(201, "Grilled Chicken Sandwich", 550, 2);
        addProduct(202, "Crispy Chicken Wrap", 500, 2);
        addProduct(203, "Beef Fajita Wrap", 650, 2);

        addProduct(301, "Classic Fries", 250, 3);
        addProduct(302, "Loaded Fries", 400, 3);
        addProduct(303, "Chicken Nuggets", 350, 3);
        addProduct(304, "Onion Rings", 300, 3);

        addProduct(401, "Blue Lagoon Crusher", 300, 4);
        addProduct(402, "Strawberry Crusher", 300, 4);
        addProduct(403, "Mango Crusher", 300, 4);
        addProduct(404, "Chocolate Milkshake", 400, 4);
        addProduct(405, "Oreo Milkshake", 450, 4);
        addProduct(406, "Soft Drinks", 150, 4);

        addProduct(501, "Chocolate Brownie", 300, 5);
        addProduct(502, "Molten Lava Cake", 400, 5);
        addProduct(503, "Ice Cream", 200, 5);

        saveInventory();
    }
}

void displayAllProducts()
{
    cout << endl
         << "========== PRODUCT INVENTORY ==========" << endl;

    for (int i = 0; i < productCount; i++)
    {
        cout << endl
             << "ID: " << productIDs[i];
        cout << endl
             << "Name: " << productNames[i];
        cout << endl
             << "Price: Rs. " << productPrices[i];
        cout << endl
             << "Category: " << getCategoryName(productCategories[i]);

        if (productAvailable[i])
            cout << endl
                 << "Status: Available" << endl;
        else
            cout << endl
                 << "Status: Unavailable" << endl;

        cout << "----------------------------------" << endl;
    }
}

void displayProductIDs()
{
    cout << endl
         << "========== PRODUCT IDs ==========" << endl;

    for (int i = 0; i < productCount; i++)
    {
        cout << productIDs[i] << " - "
             << productNames[i] << " ("
             << getCategoryName(productCategories[i]) << ")";

        if (!productAvailable[i])
            cout << " - Unavailable";

        cout << endl;
    }
}

void addNewProduct()
{
    if (productCount >= MAX_PRODUCTS)
    {
        cout << "Inventory is full!" << endl;
        return;
    }

    string name;
    int price;
    int category;

    cout << endl
         << "========== ADD NEW PRODUCT ==========" << endl;

    cin.ignore();

    cout << "Enter product name: ";
    getline(cin, name);

    if (name.empty())
    {
        cout << "Product name cannot be empty!" << endl;
        return;
    }

    cout << "Enter product price: ";
    cin >> price;

    if (price <= 0)
    {
        cout << "Invalid price!" << endl;
        return;
    }

    cout << endl
         << "Select product category:" << endl;
    displayCategories();

    cout << "Enter category number: ";
    cin >> category;

    if (category < 1 || category > 5)
    {
        cout << "Invalid category!" << endl;
        return;
    }

    int id = generateProductID(category);

    if (addProduct(id, name, price, category))
    {
        saveInventory();

        cout << endl
             << "Product successfully added!" << endl;
        cout << "Generated Product ID: " << id << endl;
    }
    else
    {
        cout << "Unable to add product!" << endl;
    }
}

void changeProductAvailability(bool status)
{
    if (productCount == 0)
    {
        cout << "No products in inventory!" << endl;
        return;
    }

    displayProductIDs();

    int id;
    bool found = false;

    cout << endl
         << "Enter product ID: ";
    cin >> id;

    for (int i = 0; i < productCount; i++)
    {
        if (productIDs[i] == id)
        {
            productAvailable[i] = status;
            found = true;
            break;
        }
    }

    if (found)
    {
        saveInventory();

        if (status)
            cout << "Product is now Available!" << endl;
        else
            cout << "Product is now Unavailable!" << endl;
    }
    else
    {
        cout << "Invalid product ID!" << endl;
    }
}

int runInventoryConsole()
{
    int choice;

    loadInventory();

    do
    {
        cout << "========== BREW & BITE ==========" << endl;
        cout << "1. Display all products" << endl;
        cout << "2. Add a new product" << endl;
        cout << "3. Remove product from inventory" << endl;
        cout << "4. Make product available" << endl;
        cout << "5. Exit" << endl;

        cout << "Enter your choice: ";
        cin >> choice;

        if (choice == 1)
            displayAllProducts();
        else if (choice == 2)
            addNewProduct();
        else if (choice == 3)
            changeProductAvailability(false);
        else if (choice == 4)
            changeProductAvailability(true);
        else if (choice == 5)
            cout << "Exiting inventory system..." << endl;
        else
            cout << "Invalid choice! Try again." << endl;

    } while (choice != 5);

    return 0;
}