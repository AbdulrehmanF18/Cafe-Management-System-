#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include "inventory_page-gui.cpp"

using namespace std;
using namespace sf;

int openThirdScreen()
{
    RenderWindow window(VideoMode({800, 750}), "Cafe Management System - Dashboard");
    window.setFramerateLimit(60);

    Font font("Arial.ttf");

    // Cafe theme colors
    Color creamBackground(211, 211, 211);
    Color espressoBorder(58, 38, 24);
    Color latteCard(255, 252, 247);
    Color mochaAccent(111, 78, 55);

    // Cafe Title & Smaller Subtitle
    Text cafeTitle(font, "BREW & BITE CAFE", 36);
    cafeTitle.setFillColor(espressoBorder);
    cafeTitle.setStyle(Text::Bold);
    cafeTitle.setPosition({205.f, 38.f});

    Text subTitle(font, "Main Menu of Brew and Bite Cafe", 18);
    subTitle.setFillColor(mochaAccent);
    subTitle.setStyle(Text::Italic);
    subTitle.setPosition({268.f, 92.f});

    // All three buttons
    RectangleShape productButton({340.f, 66.f});
    productButton.setPosition({230.f, 175.f});
    productButton.setFillColor(latteCard);
    productButton.setOutlineThickness(2.5f);
    productButton.setOutlineColor(espressoBorder);

    RectangleShape salesButton({340.f, 66.f});
    salesButton.setPosition({230.f, 280.f});
    salesButton.setFillColor(latteCard);
    salesButton.setOutlineThickness(2.5f);
    salesButton.setOutlineColor(espressoBorder);

    RectangleShape reportButton({340.f, 66.f});
    reportButton.setPosition({230.f, 385.f});
    reportButton.setFillColor(latteCard);
    reportButton.setOutlineThickness(2.5f);
    reportButton.setOutlineColor(espressoBorder);

    Text productText(font, "Product", 26);
    productText.setFillColor(espressoBorder);
    productText.setPosition({352.f, 192.f});

    Text salesText(font, "Sales", 26);
    salesText.setFillColor(espressoBorder);
    salesText.setPosition({368.f, 297.f});

    Text reportText(font, "Report", 26);
    reportText.setFillColor(espressoBorder);
    reportText.setPosition({362.f, 402.f});

    Text statusText(font, "", 20);
    statusText.setFillColor(mochaAccent);
    statusText.setPosition({290.f, 500.f});

    while (window.isOpen())
    {
        while (optional event = window.pollEvent())
        {
            if (event->is<Event::Closed>())
            {
                window.close();
                return 0;
            }

            if (const auto *mouseEvent = event->getIf<Event::MouseButtonPressed>())
            {
                if (mouseEvent->button == Mouse::Button::Left)
                {
                    Vector2f mousePos = window.mapPixelToCoords(mouseEvent->position);

                    if (productButton.getGlobalBounds().contains(mousePos))
                    {
                        statusText.setString("Opening Product page...");
                        openInventoryPage();
                    }
                    else if (salesButton.getGlobalBounds().contains(mousePos))
                    {
                        statusText.setString("Sales button clicked");
                        window.close();
                        return 2;
                    }
                    else if (reportButton.getGlobalBounds().contains(mousePos))
                    {
                        statusText.setString("Report button clicked");
                        window.close();
                        return 3;
                    }
                }
            }
        }

        window.clear(creamBackground);
        window.draw(cafeTitle);
        window.draw(subTitle);
        window.draw(productButton);
        window.draw(salesButton);
        window.draw(reportButton);
        window.draw(productText);
        window.draw(salesText);
        window.draw(reportText);
        window.draw(statusText);
        window.display();
    }

    return 0;
}