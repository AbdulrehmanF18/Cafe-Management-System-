#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <optional>

using namespace std;
using namespace sf;

int openMainControlBox()
{
    RenderWindow window(VideoMode({950, 700}), "Cafe Management System");
    window.setFramerateLimit(60);

    Font font("Arial.ttf");

    const string mainTabs[3] = {"Menu", "Sales Grid", "Reports"};
    const string productNames[3] = {"pasta", "fries", "capachino"};
    const int productPrices[3] = {250, 450, 180};
    const string salesItems[3] = {"pasta", "fries", "capachino"};
    const int salesAmounts[3] = {250, 900, 180};
    const string weeklyDays[7] = {"Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun"};

    int main_tab = 0;
    int sub_tab = 0;

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

                    if (my >= 20 && my <= 70)
                    {
                        if (mx >= 50 && mx < 300)
                        {
                            main_tab = 0;
                        }
                        else if (mx >= 300 && mx < 550)
                        {
                            main_tab = 1;
                        }
                        else if (mx >= 550 && mx < 900)
                        {
                            main_tab = 2;
                        }
                    }

                    if (main_tab == 2 && my >= 90 && my <= 130)
                    {
                        if (mx >= 100 && mx < 250)
                            sub_tab = 0;
                        else if (mx >= 260 && mx < 410)
                            sub_tab = 1;
                    }
                }
            }
        }

        window.clear(Color(211, 211, 211)); 

        for (int i = 0; i < 3; i++)
        {
            RectangleShape tabBox({240.f, 50.f});
            tabBox.setPosition({50.f + i * 250.f, 20.f});
            tabBox.setFillColor(main_tab == i ? Color(41, 128, 185) : Color(189, 195, 199));
            window.draw(tabBox);

            Text tabText(font, mainTabs[i], 18);
            tabText.setFillColor(Color::White);
            tabText.setPosition({tabBox.getPosition().x + 15.f, tabBox.getPosition().y + 12.f});
            window.draw(tabText);
        }

        if (main_tab == 0)
        {
            Text title(font, "Menu page", 22);
            title.setFillColor(Color(44, 62, 80));
            title.setPosition({50.f, 90.f});
            window.draw(title);

            RectangleShape tableHeader({840.f, 40.f});
            tableHeader.setPosition({50.f, 130.f});
            tableHeader.setFillColor(Color(52, 73, 94));
            window.draw(tableHeader);

            Text h1(font, "Pro ID", 16);
            h1.setPosition({70.f, 140.f});
            h1.setFillColor(Color::White);
            window.draw(h1);

            Text h2(font, "Product Name", 16);
            h2.setPosition({250.f, 140.f});
            h2.setFillColor(Color::White);
            window.draw(h2);

            Text h3(font, "Price (Rs.)", 16);
            h3.setPosition({650.f, 140.f});
            h3.setFillColor(Color::White);
            window.draw(h3);

            for (int i = 0; i < 3; i++)
            {
                float yPos = 180.f + (i * 45.f);
                RectangleShape rowBox({840.f, 40.f});
                rowBox.setPosition({50.f, yPos});
                rowBox.setFillColor(i % 2 == 0 ? Color(255, 255, 255) : Color(236, 240, 241));
                window.draw(rowBox);

                Text idText(font, to_string(i + 1), 16);
                idText.setPosition({70.f, yPos + 8.f});
                idText.setFillColor(Color::Black);
                window.draw(idText);

                Text nameText(font, productNames[i], 16);
                nameText.setPosition({250.f, yPos + 8.f});
                nameText.setFillColor(Color::Black);
                window.draw(nameText);

                Text priceText(font, to_string(productPrices[i]), 16);
                priceText.setPosition({650.f, yPos + 8.f});
                priceText.setFillColor(Color::Black);
                window.draw(priceText);
            }
        }
        else if (main_tab == 1)
        {
            Text sectionTitle(font, "Sales page", 22);
            sectionTitle.setFillColor(Color(44, 62, 80));
            sectionTitle.setPosition({50.f, 90.f});
            window.draw(sectionTitle);

            RectangleShape inputBox({840.f, 60.f});
            inputBox.setPosition({50.f, 130.f});
            inputBox.setFillColor(Color(255, 255, 255));
            inputBox.setOutlineThickness(1.f);
            inputBox.setOutlineColor(Color(189, 195, 199));
            window.draw(inputBox);

            Text inputLabel(font, "Enter Item ID & Qty here... [Simulated Input]", 16);
            inputLabel.setPosition({70.f, 148.f});
            inputLabel.setFillColor(Color(127, 140, 141));
            window.draw(inputLabel);

            RectangleShape salesHeader({840.f, 40.f});
            salesHeader.setPosition({50.f, 210.f});
            salesHeader.setFillColor(Color(39, 174, 96));
            window.draw(salesHeader);

            Text sh1(font, "Sale Item / Description", 16);
            sh1.setPosition({70.f, 220.f});
            sh1.setFillColor(Color::White);
            window.draw(sh1);

            Text sh2(font, "Amount (Rs.)", 16);
            sh2.setPosition({650.f, 220.f});
            sh2.setFillColor(Color::White);
            window.draw(sh2);

            float totalSum = 0;
            for (int i = 0; i < 3; ++i)
            {
                float yPos = 260.f + (i * 45.f);
                RectangleShape sRow({840.f, 40.f});
                sRow.setPosition({50.f, yPos});
                sRow.setFillColor(Color::White);
                window.draw(sRow);

                Text itemText(font, salesItems[i], 16);
                itemText.setPosition({70.f, yPos + 8.f});
                itemText.setFillColor(Color::Black);
                window.draw(itemText);

                Text amtText(font, to_string(salesAmounts[i]), 16);
                amtText.setPosition({650.f, yPos + 8.f});
                amtText.setFillColor(Color::Black);
                window.draw(amtText);

                totalSum += salesAmounts[i];
            }

            Text sumText(font, "Total Revenue: Rs. " + to_string(static_cast<int>(totalSum)), 20);
            sumText.setFillColor(Color(192, 57, 43));
            sumText.setPosition({550.f, 410.f});
            window.draw(sumText);
        }
        else if (main_tab == 2)
        {
            Text sectionTitle(font, "Reports Dashboard", 22);
            sectionTitle.setFillColor(Color(44, 62, 80));
            sectionTitle.setPosition({50.f, 90.f});
            window.draw(sectionTitle);

            const string subTabs[2] = {"Today", "Weekly (Last 7 Days)"};
            for (int j = 0; j < 2; ++j)
            {
                RectangleShape subBox({140.f, 35.f});
                subBox.setPosition({50.f + j * 160.f, 135.f});
                subBox.setFillColor(sub_tab == j ? Color(52, 152, 219) : Color(218, 225, 231));
                window.draw(subBox);

                Text subText(font, subTabs[j], 14);
                subText.setFillColor(sub_tab == j ? Color::White : Color::Black);
                subText.setPosition({subBox.getPosition().x + 10.f, subBox.getPosition().y + 8.f});
                window.draw(subText);
            }

            RectangleShape reportBox({840.f, 420.f});
            reportBox.setPosition({50.f, 190.f});
            reportBox.setFillColor(Color::White);
            reportBox.setOutlineThickness(1.f);
            reportBox.setOutlineColor(Color(189, 195, 199));
            window.draw(reportBox);

            if (sub_tab == 0)
            {
                Text rTitle(font, "Today's Summary Report", 18);
                rTitle.setPosition({70.f, 210.f});
                rTitle.setFillColor(Color(41, 128, 185));
                window.draw(rTitle);

                Text r1(font, "- Total Transactions: 3", 16);
                r1.setPosition({70.f, 250.f});
                r1.setFillColor(Color::Black);
                window.draw(r1);

                Text r2(font, "- Total Cash Collected: Rs. 2,250", 16);
                r2.setPosition({70.f, 280.f});
                r2.setFillColor(Color::Black);
                window.draw(r2);

                Text r3(font, "- Most Selling Item: Club Sandwich", 16);
                r3.setPosition({70.f, 310.f});
                r3.setFillColor(Color::Black);
                window.draw(r3);
            }
            else
            {
                Text rTitle(font, "Weekly Performance", 18);
                rTitle.setPosition({70.f, 210.f});
                rTitle.setFillColor(Color(41, 128, 185));
                window.draw(rTitle);

                for (int d = 0; d < 7; ++d)
                {
                    Text dayText(font, weeklyDays[d], 15);
                    dayText.setPosition({70.f, 250.f + (d * 30.f)});
                    dayText.setFillColor(Color(52, 73, 94));
                    window.draw(dayText);
                }
            }
        }

        window.display();
    }

    return 0;
}