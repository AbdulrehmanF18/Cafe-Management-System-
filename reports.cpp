#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <optional>
#include <ctime>
#include <cstdio>

using namespace std;
using namespace sf;

extern int current_day;
extern int current_month;
extern int current_year;
string find_currunt_date();

// product data comes from Inventory.cpp
extern int product_counts;
extern int product_ids[];
extern string productNames[];
void loadInventory();

const int MAX_ORDERS = 500;
const int MAX_LINES = 20;
const int MAX_REPORT_ITEMS = 50;

struct Order
{
    int day, month, year;
    int total;
    int lineCount;
    string line_names[MAX_LINES];
    int line_qtt[MAX_LINES];
    int line_revenue[MAX_LINES];
};

inline int find_product_id_by_name(const string &name)
{
    for (int i = 0; i < product_counts; i++)
        if (productNames[i] == name)
            return product_ids[i];
    return 0;
}

// removes a '\r' left at the end of a line (sales.txt saved with Windows line endings)
inline void trim_line_end(string &line)
{
    while (!line.empty() && (line.back() == '\r' || line.back() == ' '))
        line.pop_back();
}

// reads every order saved in sales.txt (written by send_invoice_to_file)
inline int load_all_orders(Order orders[])
{
    int order_count = 0;
    ifstream in("sales.txt");
    if (!in)
    {
        return 0;
    }

    string line;
    bool in_order = false;
    Order cur;

    while (getline(in, line))
    {
        trim_line_end(line);

        if (line.rfind("Date: ", 0) == 0)
        {
            if (in_order && order_count < MAX_ORDERS)
                orders[order_count++] = cur;

            cur = Order();
            cur.total = 0;
            cur.lineCount = 0;
            in_order = true;

            int dd = 0, mm = 0, yy = 0;
            char c1, c2;
            stringstream ss(line.substr(6));
            ss >> dd >> c1 >> mm >> c2 >> yy;
            cur.day = dd;
            cur.month = mm;
            cur.year = yy;
        }
        else if (line.rfind("TOTAL: Rs ", 0) == 0)
        {
            cur.total = atoi(line.substr(10).c_str());
        }
        else if (line.rfind("-----", 0) == 0)
        {
            if (in_order && order_count < MAX_ORDERS)
                orders[order_count++] = cur;
            in_order = false;
        }
        else if (!line.empty() && in_order)
        {
            size_t x = line.rfind(" x");
            size_t eq = line.rfind(" = Rs ");
            if (x != string::npos && eq != string::npos && x < eq && cur.lineCount < MAX_LINES)
            {
                cur.line_names[cur.lineCount] = line.substr(0, x);
                cur.line_qtt[cur.lineCount] = atoi(line.substr(x + 2, eq - x - 2).c_str());
                cur.line_revenue[cur.lineCount] = atoi(line.substr(eq + 6).c_str());
                cur.lineCount++;
            }
        }
    }
    if (in_order && order_count < MAX_ORDERS)
        orders[order_count++] = cur;

    return order_count;
}

// moves a date forward/back by some days (handles month and year changes)
inline void shift_date(int &d, int &m, int &y, int days)
{
    tm t = {};
    t.tm_mday = d + days;
    t.tm_mon = m - 1;
    t.tm_year = y - 1900;
    t.tm_hour = 12;
    mktime(&t);
    d = t.tm_mday;
    m = t.tm_mon + 1;
    y = t.tm_year + 1900;
}

inline string date_text(int d, int m, int y)
{
    char buf[16];
    snprintf(buf, sizeof(buf), "%02d-%02d-%04d", d, m, y);
    return string(buf);
}

//  DAILY aggregation (items grouped by name, so every sold item shows even if not in inventory)
struct DailyReport
{
    int orders;
    int revenue;
    int bestQty;
    string bestName;
    int itemCount;
    string itemNames[MAX_REPORT_ITEMS];
    int itemQty[MAX_REPORT_ITEMS];
    int itemRevenue[MAX_REPORT_ITEMS];
};

inline DailyReport get_daily_report(int d, int m, int y)
{
    DailyReport r;
    r.orders = 0;
    r.revenue = 0;
    r.bestQty = 0;
    r.bestName = "";
    r.itemCount = 0;
    for (int i = 0; i < MAX_REPORT_ITEMS; i++)
    {
        r.itemQty[i] = 0;
        r.itemRevenue[i] = 0;
    }

    static Order orders[MAX_ORDERS];
    int order_count = load_all_orders(orders);

    for (int i = 0; i < order_count; i++)
    {
        if (orders[i].day != d || orders[i].month != m || orders[i].year != y)
            continue;

        r.orders++;
        r.revenue += orders[i].total;

        for (int j = 0; j < orders[i].lineCount; j++)
        {
            const string &name = orders[i].line_names[j];

            int slot = -1;
            for (int k = 0; k < r.itemCount; k++)
                if (r.itemNames[k] == name)
                {
                    slot = k;
                    break;
                }

            if (slot == -1 && r.itemCount < MAX_REPORT_ITEMS)
            {
                slot = r.itemCount;
                r.itemNames[slot] = name;
                r.itemCount++;
            }

            if (slot != -1)
            {
                r.itemQty[slot] += orders[i].line_qtt[j];
                r.itemRevenue[slot] += orders[i].line_revenue[j];
            }
        }
    }

    for (int i = 0; i < r.itemCount; i++) // most selling item
        if (r.itemQty[i] > r.bestQty)
        {
            r.bestQty = r.itemQty[i];
            r.bestName = r.itemNames[i];
        }

    return r;
}

//  WEEKLY aggregation (the 7 days ending on the chosen date)
struct weekly_reportts
{
    string dayLabels[7];
    int ordersPerDay[7];
    int revenuePerDay[7];
    int totalOrders;
    int totalRevenue;
    int topQty;
    string topName;
    int itemCount;
    string itemNames[MAX_REPORT_ITEMS];
    int itemQty[MAX_REPORT_ITEMS];
};

inline weekly_reportts get_weekly_reports(int endD, int endM, int endY)
{
    weekly_reportts r;
    r.totalOrders = 0;
    r.totalRevenue = 0;
    r.topQty = 0;
    r.topName = "";
    r.itemCount = 0;
    for (int i = 0; i < MAX_REPORT_ITEMS; i++)
        r.itemQty[i] = 0;

    int dd[7], mm[7], yy[7];
    const char *weekDays[7] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
    for (int i = 0; i < 7; i++)
    {
        dd[i] = endD;
        mm[i] = endM;
        yy[i] = endY;
        shift_date(dd[i], mm[i], yy[i], -(6 - i));

        tm t = {};
        t.tm_mday = dd[i];
        t.tm_mon = mm[i] - 1;
        t.tm_year = yy[i] - 1900;
        t.tm_hour = 12;
        mktime(&t);

        r.dayLabels[i] = string(weekDays[t.tm_wday]) + "  " + date_text(dd[i], mm[i], yy[i]);
        r.ordersPerDay[i] = 0;
        r.revenuePerDay[i] = 0;
    }

    static Order orders[MAX_ORDERS];
    int order_count = load_all_orders(orders);

    for (int i = 0; i < order_count; i++)
    {
        for (int d = 0; d < 7; d++)
        {
            if (orders[i].day != dd[d] || orders[i].month != mm[d] || orders[i].year != yy[d])
                continue;

            r.ordersPerDay[d] += 1;
            r.revenuePerDay[d] += orders[i].total;
            r.totalOrders += 1;
            r.totalRevenue += orders[i].total;

            for (int j = 0; j < orders[i].lineCount; j++)
            {
                const string &name = orders[i].line_names[j];

                int slot = -1;
                for (int k = 0; k < r.itemCount; k++)
                    if (r.itemNames[k] == name)
                    {
                        slot = k;
                        break;
                    }

                if (slot == -1 && r.itemCount < MAX_REPORT_ITEMS)
                {
                    slot = r.itemCount;
                    r.itemNames[slot] = name;
                    r.itemCount++;
                }

                if (slot != -1)
                    r.itemQty[slot] += orders[i].line_qtt[j];
            }
            break;
        }
    }

    for (int i = 0; i < r.itemCount; i++) // top selling
        if (r.itemQty[i] > r.topQty)
        {
            r.topQty = r.itemQty[i];
            r.topName = r.itemNames[i];
        }

    return r;
}

inline string id_text_for(const string &name)
{
    int id = find_product_id_by_name(name);
    return id == 0 ? "-" : to_string(id);
}

// draws a simple button with centered text
inline void draw_report_button(RenderWindow &window, const Font &font, const FloatRect &rect,
                               const string &label, Color fill, Color textColor)
{
    RectangleShape box(rect.size);
    box.setPosition(rect.position);
    box.setFillColor(fill);
    window.draw(box);

    Text text(font, label, 18);
    text.setFillColor(textColor);
    FloatRect b = text.getLocalBounds();
    text.setOrigin({b.position.x + b.size.x / 2.f, b.position.y + b.size.y / 2.f});
    text.setPosition({rect.position.x + rect.size.x / 2.f, rect.position.y + rect.size.y / 2.f});
    window.draw(text);
}

// returns 1 when Back is clicked, 0 when the window is closed
inline int openReportsPage() // gui of reports
{
    RenderWindow window(VideoMode({950, 700}), "Cafe Management System - Reports");
    window.setFramerateLimit(60);

    Font font("Arial.ttf");

    // product IDs come from the inventory, so load it before building the reports
    loadInventory();

    const string buttons[2] = {"Daily Report", "Weekly Report"};
    int activeReport = 0;

    const FloatRect tabRects[2] = {FloatRect({50.f, 25.f}, {240.f, 45.f}),
                                   FloatRect({310.f, 25.f}, {240.f, 45.f})};
    const FloatRect backRect({760.f, 25.f}, {130.f, 45.f});
    // reports are always for today (daily) and the last 7 days ending today (weekly)
    int viewD = current_day, viewM = current_month, viewY = current_year;

    DailyReport daily = get_daily_report(viewD, viewM, viewY);
    weekly_reportts weekly = get_weekly_reports(viewD, viewM, viewY);

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
                if (mousePress->button != Mouse::Button::Left)
                    continue;

                Vector2f mouse = window.mapPixelToCoords(mousePress->position);

                if (backRect.contains(mouse))
                {
                    window.close();
                    return 1;
                }
                else if (tabRects[0].contains(mouse))
                    activeReport = 0;
                else if (tabRects[1].contains(mouse))
                    activeReport = 1;
            }
        }

        window.clear(Color(211, 211, 211)); // light gray background

        for (int i = 0; i < 2; i++)
            draw_report_button(window, font, tabRects[i], buttons[i],
                               activeReport == i ? Color(41, 128, 185) : Color(189, 195, 199),
                               Color::White);

        draw_report_button(window, font, backRect, "Back", Color(52, 73, 94), Color::White);

        RectangleShape reportBox({840.f, 560.f});
        reportBox.setPosition({50.f, 100.f});
        reportBox.setFillColor(Color::White);
        reportBox.setOutlineThickness(1.f);
        reportBox.setOutlineColor(Color(189, 195, 199));
        window.draw(reportBox);

        string dateLabel = activeReport == 0 ? "Today: " + date_text(viewD, viewM, viewY)
                                             : "Week ending today: " + date_text(viewD, viewM, viewY);
        Text dateText(font, dateLabel, 15);
        dateText.setFillColor(Color(127, 140, 141));
        dateText.setPosition({70.f, 114.f});
        window.draw(dateText);

        if (activeReport == 0)
        {
            Text title(font, "Daily Summary Report", 20);
            title.setFillColor(Color(41, 128, 185));
            title.setPosition({70.f, 150.f});
            window.draw(title);

            Text r1(font, "- Total Transactions: " + to_string(daily.orders), 16);
            r1.setFillColor(Color::Black);
            r1.setPosition({70.f, 190.f});
            window.draw(r1);

            Text r2(font, "- Total Cash Collected: Rs. " + to_string(daily.revenue), 16);
            r2.setFillColor(Color::Black);
            r2.setPosition({70.f, 222.f});
            window.draw(r2);

            string bestLine = "- Most Selling Item: ";
            if (daily.bestQty == 0)
                bestLine += "(no sales)";
            else
                bestLine += daily.bestName + "  (" + to_string(daily.bestQty) + " sold)";

            Text r3(font, bestLine, 16);
            r3.setFillColor(Color::Black);
            r3.setPosition({70.f, 254.f});
            window.draw(r3);

            const string headers[4] = {"ID", "Item Name", "Qty", "Revenue (Rs.)"};
            const float headerX[4] = {70.f, 160.f, 560.f, 650.f};
            for (int h = 0; h < 4; h++)
            {
                Text ht(font, headers[h], 15);
                ht.setFillColor(Color(52, 73, 94));
                ht.setPosition({headerX[h], 300.f});
                window.draw(ht);
            }

            int shown = 0;
            for (int i = 0; i < daily.itemCount && shown < 9; i++)
            {
                float y = 332.f + shown * 32.f;

                RectangleShape bg({800.f, 28.f});
                bg.setPosition({70.f, y - 5.f});
                bg.setFillColor(shown % 2 == 0 ? Color(255, 255, 255) : Color(236, 240, 241));
                window.draw(bg);

                Text idT(font, id_text_for(daily.itemNames[i]), 15);
                idT.setFillColor(Color::Black);
                idT.setPosition({75.f, y});
                window.draw(idT);

                Text nmT(font, daily.itemNames[i], 15);
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
                empty.setPosition({70.f, 340.f});
                window.draw(empty);
            }
        }
        else
        {
            Text title(font, "Weekly Performance (Last 7 Days)", 20);
            title.setFillColor(Color(41, 128, 185));
            title.setPosition({70.f, 150.f});
            window.draw(title);

            Text hDay(font, "Day", 15);
            hDay.setFillColor(Color(52, 73, 94));
            hDay.setPosition({70.f, 192.f});
            window.draw(hDay);

            Text hOrd(font, "Orders", 15);
            hOrd.setFillColor(Color(52, 73, 94));
            hOrd.setPosition({300.f, 192.f});
            window.draw(hOrd);

            Text hRev(font, "Revenue (Rs.)", 15);
            hRev.setFillColor(Color(52, 73, 94));
            hRev.setPosition({450.f, 192.f});
            window.draw(hRev);

            for (int d = 0; d < 7; d++)
            {
                float y = 225.f + d * 32.f;

                RectangleShape bg({800.f, 28.f});
                bg.setPosition({70.f, y - 5.f});
                bg.setFillColor(d % 2 == 0 ? Color(255, 255, 255) : Color(236, 240, 241));
                window.draw(bg);

                Text dayT(font, weekly.dayLabels[d], 15);
                dayT.setFillColor(Color::Black);
                dayT.setPosition({80.f, y});
                window.draw(dayT);

                Text ordT(font, to_string(weekly.ordersPerDay[d]), 15);
                ordT.setFillColor(Color::Black);
                ordT.setPosition({310.f, y});
                window.draw(ordT);

                Text revT(font, to_string(weekly.revenuePerDay[d]), 15);
                revT.setFillColor(Color(192, 57, 43));
                revT.setPosition({460.f, y});
                window.draw(revT);
            }

            Text totLine(font,
                         "Week Total: " + to_string(weekly.totalOrders) +
                             " orders  |  Rs. " + to_string(weekly.totalRevenue),
                         16);
            totLine.setFillColor(Color(44, 62, 80));
            totLine.setPosition({70.f, 470.f});
            window.draw(totLine);

            string bestLine = "Top seller this week: ";
            if (weekly.topQty == 0)
                bestLine += "(no sales)";
            else
                bestLine += id_text_for(weekly.topName) + "  " + weekly.topName +
                            "  (" + to_string(weekly.topQty) + " sold)";

            Text bestT(font, bestLine, 16);
            bestT.setFillColor(Color(44, 62, 80));
            bestT.setPosition({70.f, 505.f});
            window.draw(bestT);
        }

        window.display();
    }

    return 0;
}
