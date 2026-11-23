#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <optional>
#include <ctime>
#include <cstdio> // FIX 1: for sprintf

using namespace std;
using namespace sf;

// ============================================================
//  Pull these globals from your existing files
//  - currentDay/Month/Year  : defined in #includeiostream.txt
//  - productIDs/Names/...   : defined in Inventory.cpp
// ============================================================
extern int currentDay;
extern int currentMonth;
extern int currentYear;
string getCurrentDate();

extern int productCount;
extern int productIDs[];      // FIX 4: size inferred from definition
extern string productNames[]; // FIX 4: size inferred from definition

// ============================================================
//  Fixed-size arrays — same style as your inventory
//  FIX 3: MAX_LINES reduced from 60 to 10 to keep Order small
// ============================================================
const int MAX_ORDERS = 200;
const int MAX_LINES = 10;

struct Order
{
    int day, month, year;
    int total;
    int lineCount;
    string lineNames[MAX_LINES];
    int lineQty[MAX_LINES];
    int lineRevenue[MAX_LINES];
};

// ============================================================
//  Helper: find product ID by name
//  FIX 2: 'inline' added to prevent multiple-definition errors
// ============================================================
inline int findProductIDByName(const string &name)
{
    for (int i = 0; i < productCount; i++)
        if (productNames[i] == name)
            return productIDs[i];
    return 0;
}

// ============================================================
//  Parse sales.txt into a fixed array of Orders
// ============================================================
inline int loadAllOrders(Order orders[])
{
    int orderCount = 0;
    ifstream in("sales.txt");
    if (!in)
        return 0;

    string line;
    bool inOrder = false;
    Order cur;

    while (getline(in, line))
    {
        if (line.rfind("Date: ", 0) == 0)
        {
            if (inOrder && orderCount < MAX_ORDERS)
                orders[orderCount++] = cur;

            cur = Order();
            cur.lineCount = 0;
            inOrder = true;

            string d = line.substr(6);
            int dd, mm, yy;
            char c1, c2;
            stringstream ss(d);
            ss >> dd >> c1 >> mm >> c2 >> yy;
            cur.day = dd;
            cur.month = mm;
            cur.year = yy;
        }
        else if (line.rfind("TOTAL: Rs ", 0) == 0)
        {
            cur.total = stoi(line.substr(10));
        }
        else if (line == "-----------------------")
        {
            if (inOrder && orderCount < MAX_ORDERS)
                orders[orderCount++] = cur;
            inOrder = false;
        }
        else if (!line.empty() && inOrder)
        {
            size_t x = line.rfind(" x");
            size_t eq = line.rfind(" = Rs ");
            if (x != string::npos && eq != string::npos && cur.lineCount < MAX_LINES)
            {
                cur.lineNames[cur.lineCount] = line.substr(0, x);
                cur.lineQty[cur.lineCount] = stoi(line.substr(x + 2, eq - x - 2));
                cur.lineRevenue[cur.lineCount] = stoi(line.substr(eq + 6));
                cur.lineCount++;
            }
        }
    }
    if (inOrder && orderCount < MAX_ORDERS)
        orders[orderCount++] = cur;

    return orderCount;
}

// ============================================================
//  DAILY aggregation
// ============================================================
struct DailyReport
{
    int orders;
    int revenue;
    int bestID;
    int bestQty;
    string bestName;
    int itemCount;
    int itemIDs[50];
    int itemQty[50];
    int itemRevenue[50];
};

inline DailyReport getDailyReport(int d, int m, int y)
{
    DailyReport r;
    r.orders = 0;
    r.revenue = 0;
    r.bestID = 0;
    r.bestQty = 0;
    r.bestName = "-";
    r.itemCount = 0;
    for (int i = 0; i < 50; i++)
    {
        r.itemIDs[i] = 0;
        r.itemQty[i] = 0;
        r.itemRevenue[i] = 0;
    }

    // FIX 3 (continued): 'static' moves the array off the stack
    static Order orders[MAX_ORDERS];
    int orderCount = loadAllOrders(orders);

    for (int i = 0; i < orderCount; i++)
    {
        if (orders[i].day != d || orders[i].month != m || orders[i].year != y)
            continue;

        r.orders++;
        r.revenue += orders[i].total;

        for (int j = 0; j < orders[i].lineCount; j++)
        {
            int pid = findProductIDByName(orders[i].lineNames[j]);

            int slot = -1;
            for (int k = 0; k < r.itemCount; k++)
                if (r.itemIDs[k] == pid)
                {
                    slot = k;
                    break;
                }

            if (slot == -1)
            {
                if (r.itemCount < 50)
                {
                    slot = r.itemCount;
                    r.itemIDs[slot] = pid;
                    r.itemCount++;
                }
            }

            if (slot != -1)
            {
                r.itemQty[slot] += orders[i].lineQty[j];
                r.itemRevenue[slot] += orders[i].lineRevenue[j];

                if (r.itemQty[slot] > r.bestQty)
                {
                    r.bestQty = r.itemQty[slot];
                    r.bestID = pid;
                    r.bestName = orders[i].lineNames[j];
                }
            }
        }
    }

    return r;
}

// ============================================================
//  WEEKLY aggregation
// ============================================================
struct WeeklyReport
{
    int ordersPerDay[7];
    int revenuePerDay[7];
    int totalOrders;
    int totalRevenue;
    int topID;
    int topQty;
    int itemCount;
    int itemIDs[50];
    int itemQty[50];
};

inline WeeklyReport getWeeklyReport(int endD, int endM, int endY)
{
    WeeklyReport r;
    for (int i = 0; i < 7; i++)
    {
        r.ordersPerDay[i] = 0;
        r.revenuePerDay[i] = 0;
    }
    r.totalOrders = 0;
    r.totalRevenue = 0;
    r.topID = 0;
    r.topQty = 0;
    r.itemCount = 0;
    for (int i = 0; i < 50; i++)
    {
        r.itemIDs[i] = 0;
        r.itemQty[i] = 0;
    }

    // build 7 day keys (YYYY-MM-DD strings) for matching
    string keys[7];
    tm base = {};
    base.tm_mday = endD;
    base.tm_mon = endM - 1;
    base.tm_year = endY - 1900;
    for (int i = 0; i < 7; i++)
    {
        tm c = base;
        c.tm_mday -= (6 - i);
        mktime(&c);
        char buf[16];
        sprintf(buf, "%04d-%02d-%02d",
                c.tm_year + 1900, c.tm_mon + 1, c.tm_mday);
        keys[i] = string(buf);
    }

    // FIX 3 (continued): 'static' moves the array off the stack
    static Order orders[MAX_ORDERS];
    int orderCount = loadAllOrders(orders);

    for (int i = 0; i < orderCount; i++)
    {
        char key[16];
        sprintf(key, "%04d-%02d-%02d",
                orders[i].year, orders[i].month, orders[i].day);
        string orderKey = key;

        for (int d = 0; d < 7; d++)
        {
            if (keys[d] == orderKey)
            {
                r.ordersPerDay[d] += 1;
                r.revenuePerDay[d] += orders[i].total;
                r.totalOrders += 1;
                r.totalRevenue += orders[i].total;

                for (int j = 0; j < orders[i].lineCount; j++)
                {
                    int pid = findProductIDByName(orders[i].lineNames[j]);

                    int slot = -1;
                    for (int k = 0; k < r.itemCount; k++)
                        if (r.itemIDs[k] == pid)
                        {
                            slot = k;
                            break;
                        }

                    if (slot == -1 && r.itemCount < 50)
                    {
                        slot = r.itemCount;
                        r.itemIDs[slot] = pid;
                        r.itemCount++;
                    }

                    if (slot != -1)
                        r.itemQty[slot] += orders[i].lineQty[j];
                }
                break;
            }
        }
    }

    // find top ID
    for (int i = 0; i < r.itemCount; i++)
        if (r.itemQty[i] > r.topQty)
        {
            r.topQty = r.itemQty[i];
            r.topID = r.itemIDs[i];
        }

    return r;
}

// ============================================================
//  Name lookup by ID for display
// ============================================================
inline string productNameByID(int id)
{
    for (int i = 0; i < productCount; i++)
        if (productIDs[i] == id)
            return productNames[i];
    return "(unknown)";
}

// ============================================================
//  Draw the whole Reports page
// ============================================================
inline int openReportsPage()
{
    RenderWindow window(VideoMode({950, 700}), "Cafe Management System - Reports");
    window.setFramerateLimit(60);

    Font font("Arial.ttf");

    const string buttons[2] = {"Daily Report", "Weekly Report"};
    int activeReport = 0;

    DailyReport daily = getDailyReport(currentDay, currentMonth, currentYear);
    WeeklyReport weekly = getWeeklyReport(currentDay, currentMonth, currentYear);
    string dateStr = getCurrentDate();

    while (window.isOpen())
    {
        while (const optional event = window.pollEvent())
        {
            if (event->is<Event::Closed>())
            {
                window.close();
                return 0;
            }
            else if (const auto *mousePress = event->getIf<Event::MouseButtonPressed>())
            {
                if (mousePress->button == Mouse::Button::Left)
                {
                    float mx = static_cast<float>(mousePress->position.x);
                    float my = static_cast<float>(mousePress->position.y);

                    if (my >= 25 && my <= 70)
                    {
                        if (mx >= 50 && mx < 290)
                            activeReport = 0;
                        else if (mx >= 310 && mx < 550)
                            activeReport = 1;
                    }
                }
            }
        }

        window.clear(Color(245, 247, 250));

        // ---------- top buttons ----------
        for (int i = 0; i < 2; i++)
        {
            RectangleShape btnBox({240.f, 45.f});
            btnBox.setPosition({50.f + i * 260.f, 25.f});
            btnBox.setFillColor(activeReport == i ? Color(41, 128, 185)
                                                  : Color(189, 195, 199));
            window.draw(btnBox);

            Text btnText(font, buttons[i], 18);
            btnText.setFillColor(Color::White);
            btnText.setPosition({btnBox.getPosition().x + 55.f,
                                 btnBox.getPosition().y + 11.f});
            window.draw(btnText);
        }

        // ---------- report panel ----------
        RectangleShape reportBox({840.f, 560.f});
        reportBox.setPosition({50.f, 100.f});
        reportBox.setFillColor(Color::White);
        reportBox.setOutlineThickness(1.f);
        reportBox.setOutlineColor(Color(189, 195, 199));
        window.draw(reportBox);

        Text dateText(font, "Report date: " + dateStr, 14);
        dateText.setFillColor(Color(127, 140, 141));
        dateText.setPosition({70.f, 110.f});
        window.draw(dateText);

        if (activeReport == 0)
        {
            // ===================== DAILY =====================
            Text title(font, "Daily Summary Report", 20);
            title.setFillColor(Color(41, 128, 185));
            title.setPosition({70.f, 140.f});
            window.draw(title);

            Text r1(font, "- Total Transactions: " + to_string(daily.orders), 16);
            r1.setFillColor(Color::Black);
            r1.setPosition({70.f, 185.f});
            window.draw(r1);

            Text r2(font, "- Total Cash Collected: Rs. " + to_string(daily.revenue), 16);
            r2.setFillColor(Color::Black);
            r2.setPosition({70.f, 220.f});
            window.draw(r2);

            string bestLine = "- Most Selling Item: ";
            if (daily.bestID == 0)
                bestLine += "(no sales)";
            else
                bestLine += to_string(daily.bestID) + "  " + daily.bestName +
                            "  (" + to_string(daily.bestQty) + " sold)";

            Text r3(font, bestLine, 16);
            r3.setFillColor(Color::Black);
            r3.setPosition({70.f, 255.f});
            window.draw(r3);

            // header row
            Text h1(font, "ID", 15);
            h1.setFillColor(Color(52, 73, 94));
            h1.setPosition({70.f, 320.f});
            window.draw(h1);

            Text h2(font, "Item Name", 15);
            h2.setFillColor(Color(52, 73, 94));
            h2.setPosition({160.f, 320.f});
            window.draw(h2);

            Text h3(font, "Qty", 15);
            h3.setFillColor(Color(52, 73, 94));
            h3.setPosition({560.f, 320.f});
            window.draw(h3);

            Text h4(font, "Revenue (Rs.)", 15);
            h4.setFillColor(Color(52, 73, 94));
            h4.setPosition({650.f, 320.f});
            window.draw(h4);

            int shown = 0;
            for (int i = 0; i < daily.itemCount && shown < 7; i++)
            {
                float y = 355.f + shown * 32.f;

                RectangleShape bg({800.f, 28.f});
                bg.setPosition({70.f, y - 5.f});
                bg.setFillColor(shown % 2 == 0 ? Color(255, 255, 255)
                                               : Color(236, 240, 241));
                window.draw(bg);

                Text idT(font, to_string(daily.itemIDs[i]), 15);
                idT.setFillColor(Color::Black);
                idT.setPosition({75.f, y});
                window.draw(idT);

                Text nmT(font, productNameByID(daily.itemIDs[i]), 15);
                nmT.setFillColor(Color::Black);
                nmT.setPosition({170.f, y});
                window.draw(nmT);

                Text qT(font, to_string(daily.itemQty[i]), 15);
                qT.setFillColor(Color::Black);
                qT.setPosition({570.f, y});
                window.draw(qT);

                Text rT(font, to_string(daily.itemRevenue[i]), 15);
                rT.setFillColor(Color(192, 57, 43));
                rT.setPosition({660.f, y});
                window.draw(rT);

                shown++;
            }

            if (shown == 0)
            {
                Text empty(font, "No sales recorded for this date.", 16);
                empty.setFillColor(Color(127, 140, 141));
                empty.setPosition({70.f, 365.f});
                window.draw(empty);
            }
        }
        else
        {
            // ===================== WEEKLY =====================
            Text title(font, "Weekly Performance (Last 7 Days)", 20);
            title.setFillColor(Color(41, 128, 185));
            title.setPosition({70.f, 140.f});
            window.draw(title);

            Text hDay(font, "Day", 15);
            hDay.setFillColor(Color(52, 73, 94));
            hDay.setPosition({70.f, 185.f});
            window.draw(hDay);

            Text hOrd(font, "Orders", 15);
            hOrd.setFillColor(Color(52, 73, 94));
            hOrd.setPosition({250.f, 185.f});
            window.draw(hOrd);

            Text hRev(font, "Revenue (Rs.)", 15);
            hRev.setFillColor(Color(52, 73, 94));
            hRev.setPosition({420.f, 185.f});
            window.draw(hRev);

            const string days[7] = {"Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun"};
            for (int d = 0; d < 7; d++)
            {
                float y = 220.f + d * 32.f;

                RectangleShape bg({800.f, 28.f});
                bg.setPosition({70.f, y - 5.f});
                bg.setFillColor(d % 2 == 0 ? Color(255, 255, 255)
                                           : Color(236, 240, 241));
                window.draw(bg);

                Text dayT(font, days[d], 15);
                dayT.setFillColor(Color::Black);
                dayT.setPosition({80.f, y});
                window.draw(dayT);

                Text ordT(font, to_string(weekly.ordersPerDay[d]), 15);
                ordT.setFillColor(Color::Black);
                ordT.setPosition({260.f, y});
                window.draw(ordT);

                Text revT(font, to_string(weekly.revenuePerDay[d]), 15);
                revT.setFillColor(Color(192, 57, 43));
                revT.setPosition({430.f, y});
                window.draw(revT);
            }

            Text totLine(font,
                         "Week Total: " + to_string(weekly.totalOrders) +
                             " orders  |  Rs. " + to_string(weekly.totalRevenue),
                         16);
            totLine.setFillColor(Color(44, 62, 80));
            totLine.setPosition({70.f, 465.f});
            window.draw(totLine);

            string bestLine = "Top seller this week: ";
            if (weekly.topID == 0)
                bestLine += "(no sales)";
            else
                bestLine += to_string(weekly.topID) + "  " +
                            productNameByID(weekly.topID) +
                            "  (" + to_string(weekly.topQty) + " sold)";

            Text bestT(font, bestLine, 16);
            bestT.setFillColor(Color(44, 62, 80));
            bestT.setPosition({70.f, 500.f});
            window.draw(bestT);
        }

        window.display();
    }

    return 0;
}