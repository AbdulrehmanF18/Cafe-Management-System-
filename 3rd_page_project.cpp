#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>

using namespace std;
using namespace sf;

int openThirdScreen()
{
    RenderWindow window(VideoMode({600, 665}), "Cafe Management System - Dashboard");
    window.setFramerateLimit(60);

    Font font("Arial.ttf");

    Text title(font, "Dashboard", 32);
    title.setFillColor(Color::Black);
    title.setPosition({220.f, 40.f});

    RectangleShape productButton({300.f, 60.f});
    productButton.setPosition({150.f, 140.f});
    productButton.setFillColor(Color::White);
    productButton.setOutlineThickness(2.f);
    productButton.setOutlineColor(Color::Black);

    RectangleShape salesButton({300.f, 60.f});
    salesButton.setPosition({150.f, 250.f});
    salesButton.setFillColor(Color::White);
    salesButton.setOutlineThickness(2.f);
    salesButton.setOutlineColor(Color::Black);

    RectangleShape reportButton({300.f, 60.f});
    reportButton.setPosition({150.f, 360.f});
    reportButton.setFillColor(Color::White);
    reportButton.setOutlineThickness(2.f);
    reportButton.setOutlineColor(Color::Black);

    Text productText(font, "Product", 26);
    productText.setFillColor(Color::Black);
    productText.setPosition({245.f, 155.f});

    Text salesText(font, "Sales", 26);
    salesText.setFillColor(Color::Black);
    salesText.setPosition({255.f, 265.f});

    Text reportText(font, "Report", 26);
    reportText.setFillColor(Color::Black);
    reportText.setPosition({250.f, 375.f});

    Text statusText(font, "", 20);
    statusText.setFillColor(Color::Black);
    statusText.setPosition({170.f, 500.f});

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
                        statusText.setString("Product button clicked");
                    }
                    else if (salesButton.getGlobalBounds().contains(mousePos))
                    {
                        statusText.setString("Sales button clicked");
                    }
                    else if (reportButton.getGlobalBounds().contains(mousePos))
                    {
                        window.close();
                        return 3;
                    }
                }
            }
        }

        window.clear(Color::Cyan);
        window.draw(title);
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
