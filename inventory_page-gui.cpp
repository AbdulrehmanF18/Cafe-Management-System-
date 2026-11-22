#include <SFML/Graphics.hpp>
#include <optional>
#include "Inventory.cpp"

using namespace sf;

int openInventoryPage()
{
    loadInventory();

    RenderWindow window(VideoMode({950, 700}), "Cafe Management System - Inventory");
    window.setFramerateLimit(60);

    Font font("Arial.ttf");
    int selectedIndex = -1;

    RectangleShape backButton({120.f, 45.f});
    backButton.setPosition({30.f, 25.f});
    backButton.setFillColor(Color(189, 195, 199));

    RectangleShape availableButton({180.f, 45.f});
    availableButton.setPosition({620.f, 25.f});
    availableButton.setFillColor(Color(39, 174, 96));

    RectangleShape unavailableButton({180.f, 45.f});
    unavailableButton.setPosition({810.f, 25.f});
    unavailableButton.setFillColor(Color(192, 57, 43));

    while (window.isOpen())
    {
        while (optional<Event> event = window.pollEvent())
        {
            if (event->is<Event::Closed>())
            {
                window.close();
                return 0;
            }

            if (const auto *mouseEvent = event->getIf<Event::MouseButtonPressed>())
            {
                if (mouseEvent->button != Mouse::Button::Left)
                    continue;

                Vector2f mousePos = window.mapPixelToCoords(mouseEvent->position);

                if (backButton.getGlobalBounds().contains(mousePos))
                {
                    window.close();
                    return 1;
                }
                else if (selectedIndex >= 0 &&
                         availableButton.getGlobalBounds().contains(mousePos))
                {
                    productAvailable[selectedIndex] = true;
                    saveInventory();
                }
                else if (selectedIndex >= 0 &&
                         unavailableButton.getGlobalBounds().contains(mousePos))
                {
                    productAvailable[selectedIndex] = false;
                    saveInventory();
                }
                else
                {
                    selectedIndex = -1;

                    for (int i = 0; i < productCount; i++)
                    {
                        float y = 155.f + i * 40.f;

                        if (mousePos.y >= y &&
                            mousePos.y < y + 38.f &&
                            mousePos.x >= 30.f &&
                            mousePos.x <= 920.f)
                        {
                            selectedIndex = i;
                            break;
                        }
                    }
                }
            }
        }

        window.clear(Color(245, 247, 250));

        Text title(font, "Product Inventory", 30);
        title.setFillColor(Color(44, 62, 80));
        title.setPosition({290.f, 25.f});
        window.draw(title);

        window.draw(backButton);
        Text backText(font, "Back", 18);
        backText.setFillColor(Color::Black);
        backText.setPosition({55.f, 35.f});
        window.draw(backText);

        window.draw(availableButton);
        Text availableText(font, "Available", 16);
        availableText.setFillColor(Color::White);
        availableText.setPosition({658.f, 37.f});
        window.draw(availableText);

        window.draw(unavailableButton);
        Text unavailableText(font, "Unavailable", 16);
        unavailableText.setFillColor(Color::White);
        unavailableText.setPosition({840.f, 37.f});
        window.draw(unavailableText);

        RectangleShape header({890.f, 40.f});
        header.setPosition({30.f, 100.f});
        header.setFillColor(Color(52, 73, 94));
        window.draw(header);

        Text idHeader(font, "ID", 16);
        idHeader.setFillColor(Color::White);
        idHeader.setPosition({45.f, 112.f});
        window.draw(idHeader);

        Text nameHeader(font, "Product Name", 16);
        nameHeader.setFillColor(Color::White);
        nameHeader.setPosition({150.f, 112.f});
        window.draw(nameHeader);

        Text priceHeader(font, "Price", 16);
        priceHeader.setFillColor(Color::White);
        priceHeader.setPosition({520.f, 112.f});
        window.draw(priceHeader);

        Text categoryHeader(font, "Category", 16);
        categoryHeader.setFillColor(Color::White);
        categoryHeader.setPosition({650.f, 112.f});
        window.draw(categoryHeader);

        Text statusHeader(font, "Status", 16);
        statusHeader.setFillColor(Color::White);
        statusHeader.setPosition({820.f, 112.f});
        window.draw(statusHeader);

        for (int i = 0; i < productCount && i < 13; i++)
        {
            float y = 155.f + i * 40.f;
            RectangleShape row({890.f, 38.f});
            row.setPosition({30.f, y});
            row.setFillColor(selectedIndex == i
                                 ? Color(174, 214, 241)
                                 : (i % 2 == 0 ? Color::White : Color(236, 240, 241)));
            window.draw(row);

            Text idText(font, to_string(productIDs[i]), 15);
            idText.setFillColor(Color::Black);
            idText.setPosition({45.f, y + 8.f});
            window.draw(idText);

            Text nameText(font, productNames[i], 15);
            nameText.setFillColor(Color::Black);
            nameText.setPosition({150.f, y + 8.f});
            window.draw(nameText);

            Text priceText(font, "Rs. " + to_string(productPrices[i]), 15);
            priceText.setFillColor(Color::Black);
            priceText.setPosition({520.f, y + 8.f});
            window.draw(priceText);

            Text categoryText(font, getCategoryName(productCategories[i]), 15);
            categoryText.setFillColor(Color::Black);
            categoryText.setPosition({650.f, y + 8.f});
            window.draw(categoryText);

            Text statusText(font, productAvailable[i] ? "Available" : "Unavailable", 15);
            statusText.setFillColor(productAvailable[i] ? Color(39, 174, 96) : Color(192, 57, 43));
            statusText.setPosition({820.f, y + 8.f});
            window.draw(statusText);
        }

        if (productCount == 0)
        {
            Text emptyText(font, "No products found", 20);
            emptyText.setFillColor(Color(127, 140, 141));
            emptyText.setPosition({390.f, 180.f});
            window.draw(emptyText);
        }

        window.display();
    }

    return 0;
}
