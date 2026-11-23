#include <iostream>
#include <string>
#include <fstream>
#include <ctime>

using namespace std;

// ============================================================
// GLOBAL VARIABLES (used by reports.cpp and the GUI)
// ============================================================
int currentDay = 1;
int currentMonth = 1;
int currentYear = 2026;

// ============================================================
// INVOICE DATA (used by your sales GUI)
// ============================================================
const int MAX_ITEMS = 50;
string invNames[MAX_ITEMS];
int invPrices[MAX_ITEMS];
int invQty[MAX_ITEMS];
int invCount = 0;

// ============================================================
// DATE FUNCTIONS
// ============================================================
void saveDate()
{
    ofstream outFile("date.txt");
    outFile << currentDay << " " << currentMonth << " " << currentYear;
    outFile.close();
}

void loadDate()
{
    ifstream inFile("date.txt");

    if (inFile >> currentDay >> currentMonth >> currentYear)
    {
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
        time_t now = time(0);
        tm t;
        localtime_s(&t, &now);

        currentDay = t.tm_mday;
        currentMonth = t.tm_mon + 1;
        currentYear = t.tm_year + 1900;
    }

    saveDate();
}

string getCurrentDate()
{
    string d = to_string(currentDay);
    string m = to_string(currentMonth);
    if (currentDay < 10)
        d = "0" + d;
    if (currentMonth < 10)
        m = "0" + m;
    return d + "-" + m + "-" + to_string(currentYear);
}

// ============================================================
// INVOICE FUNCTIONS (Used by your GUI to save sales)
// ============================================================
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

void saveInvoiceToFile()
{
    if (invCount == 0)
        return;

    ofstream outFile("sales.txt", ios::app); // app = append, keeps old data
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