#include <iostream>
#include <string>
#include <fstream>
#include <ctime>

using namespace std;

int current_day = 1;
int current_month = 1;
int current_year = 2026;

const int MAX_ITEMS = 50;
string inventory_name[MAX_ITEMS];
int inventory_price[MAX_ITEMS];
int inventory_qut[MAX_ITEMS];
int inventory_count = 0;

void saveDate()
{
    ofstream outFile("date.txt");
    outFile << current_day << " " << current_month << " " << current_year;
    outFile.close();
}

void load_date()
{
    ifstream inFile("date.txt");

    if (inFile >> current_day >> current_month >> current_year)
    {
        inFile.close();

        tm t = {};
        t.tm_mday = current_day + 1;
        t.tm_mon = current_month - 1;
        t.tm_year = current_year - 1900;
        t.tm_hour = 12;
        mktime(&t);

        current_day = t.tm_mday;
        current_month = t.tm_mon + 1;
        current_year = t.tm_year + 1900;
    }
    else
    {
        time_t now = time(0);
        tm t;
        localtime_s(&t, &now);

        current_day = t.tm_mday;
        current_month = t.tm_mon + 1;
        current_year = t.tm_year + 1900;
    }

    saveDate();
}

string find_currunt_date()
{
    string d = to_string(current_day);
    string m = to_string(current_month);
    if (current_day < 10)
        d = "0" + d;
    if (current_month < 10)
        m = "0" + m;
    return d + "-" + m + "-" + to_string(current_year);
}

bool add_this_to_invoice(string name, int price, int qty)
{
    for (int i = 0; i < inventory_count; i++)
    {
        if (inventory_name[i] == name)
        {
            inventory_qut[i] += qty;
            return true;
        }
    }

    if (inventory_count >= MAX_ITEMS)
    {
        return false;
    }

    inventory_name[inventory_count] = name;
    inventory_price[inventory_count] = price;
    inventory_qut[inventory_count] = qty;
    inventory_count++;
    return true;
}

int invoice_total_is()
{
    int total = 0;
    for (int i = 0; i < inventory_count; i++)
        total += inventory_price[i] * inventory_qut[i];
    return total;
}

void clear_invoice()
{
    inventory_count = 0;
}

void send_invoice_to_file()
{
    if (inventory_count == 0)
        return;

    ofstream outFile("sales.txt", ios::app); // app = append while keeping old data
    if (!outFile)
    {
        cout << "Could not open sales file." << endl;
        return;
    }

    outFile << "Date: " << find_currunt_date() << endl;
    for (int i = 0; i < inventory_count; i++)
        outFile << inventory_name[i] << " x" << inventory_qut[i] << " = Rs " << inventory_price[i] * inventory_qut[i] << endl;
    outFile << "TOTAL: Rs " << invoice_total_is() << endl;
    outFile << "-----------------------" << endl;

    outFile.close();
}
