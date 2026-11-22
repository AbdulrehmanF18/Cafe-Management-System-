#pragma once
#include <string>
using namespace std;

void runGUI();  

const int NUM_CATEGORIES = 5;   

string getCategoryName(int category);
int getItemCount(int category);
string getItemName(int category, int pick); 
int getItemPrice(int category, int pick);          

// ---------- Defined in GUI.cpp ----------
void runGUI();   // opens the window with buttons, runs until the window is closed

// ---------- Defined in main.cpp (the GUI calls these) ----------
const int NUM_CATEGORIES = 5;   // 1 = Burgers, 2 = Sandwiches and Wraps, 3 = Sides, 4 = Drinks, 5 = Desserts

std::string getCategoryName(int category);
int getItemCount(int category);
std::string getItemName(int category, int pick);   // pick starts from 1
int getItemPrice(int category, int pick);          // pick starts from 1

bool addItemToOrder(int category, int pick, int qty);

int getInvoiceCount();
std::string getInvoiceName(int index);
int getInvoiceQty(int index);
int getInvoiceLineTotal(int index);
int getInvoiceTotal();
void saveInvoiceToFile();
string getCurrentDate();
void clearInvoice();