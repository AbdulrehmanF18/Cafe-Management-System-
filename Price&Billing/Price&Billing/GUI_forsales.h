#pragma once
#include <string>
using namespace std;

void runGUI();

const int NUM_CATEGORIES = 5;

string getCategoryName(int category);
int getItemCount(int category);
string getItemName(int category, int pick);
int getItemPrice(int category, int pick);

bool addItemToOrder(int category, int pick, int qty);

int getInvoiceCount();
std::string getInvoiceName(int index);
int getInvoiceQty(int index);
int getInvoiceLineTotal(int index);
int invoice_total_is();
void send_invoice_to_file();
string find_currunt_date();
void clear_invoice();
