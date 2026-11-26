#include <SFML/Graphics.hpp>
#include <optional>
#include <algorithm>
#include "Inventory.cpp"

using namespace sf;


const float INV_WINDOW_W = 950.f;
const float INV_WINDOW_H = 680.f;
const float INV_TABLE_X = 30.f;
const float INV_TABLE_TOP = 135.f; 
const float INV_TABLE_W = 870.f;
const float INV_TABLE_H = 460.f;   
const float INV_ROW_H = 40.f;
const float INV_SCROLLBAR_X = 906.f;
const float INV_SCROLLBAR_W = 14.f;


void drawInventoryButton(RenderWindow &window, const Font &font, const FloatRect &rect,
                         const string &label, Color fill, Color textColor, unsigned int size = 16)
{
    RectangleShape box(rect.size);
    box.setPosition(rect.position);
    box.setFillColor(fill);
    window.draw(box);

    Text text(font, label, size);
    text.setFillColor(textColor);
    FloatRect b = text.getLocalBounds();
    text.setOrigin({b.position.x + b.size.x / 2.f, b.position.y + b.size.y / 2.f});
    text.setPosition({rect.position.x + rect.size.x / 2.f, rect.position.y + rect.size.y / 2.f});
    window.draw(text);
}


FloatRect inventoryEditRect(int row)
{
    return FloatRect({815.f, INV_TABLE_TOP + row * INV_ROW_H + 6.f}, {75.f, 28.f});
}

int openInventoryPage()
{
    loadInventory();

    RenderWindow window(VideoMode({(unsigned int)INV_WINDOW_W, (unsigned int)INV_WINDOW_H}),
                        "Cafe Management System - Inventory");
    window.setFramerateLimit(60);

    Font font("Arial.ttf");
    int selectedIndex = -1;

    // ---------- scrolling ----------
    float scrollY = 0.f;          // how many pixels the table is scrolled down
    bool draggingThumb = false;   // dragging the scrollbar with the mouse
    float dragStartMouseY = 0.f;
    float dragStartScrollY = 0.f;

    auto maxScroll = [&]()
    {
        return max(0.f, product_counts * INV_ROW_H - INV_TABLE_H);
    };
    auto clampScroll = [&]()
    {
        if (scrollY > maxScroll())
            scrollY = maxScroll();
        if (scrollY < 0.f)
            scrollY = 0.f;
    };

    auto thumbRect = [&]()
    {
        float contentH = max(product_counts * INV_ROW_H, INV_TABLE_H);
        float thumbH = max(40.f, INV_TABLE_H * INV_TABLE_H / contentH);
        float thumbY = INV_TABLE_TOP;
        if (maxScroll() > 0.f)
            thumbY += (INV_TABLE_H - thumbH) * (scrollY / maxScroll());
        return FloatRect({INV_SCROLLBAR_X, thumbY}, {INV_SCROLLBAR_W, thumbH});
    };

    const FloatRect tableRect({INV_TABLE_X, INV_TABLE_TOP}, {INV_TABLE_W, INV_TABLE_H});


    const FloatRect backRect({30.f, 25.f}, {120.f, 45.f});
    const FloatRect addProductRect({160.f, 25.f}, {150.f, 45.f});
    const FloatRect availableRect({580.f, 25.f}, {170.f, 45.f});
    const FloatRect unavailableRect({760.f, 25.f}, {170.f, 45.f});

    // ---------- Add / Edit product form ----------
    bool formOpen = false;
    int editIndex = -1; // -1 = adding a new product, otherwise the product being edited
    string newName = "";
    string newPrice = "";
    int newCategory = 0; // 1-5, 0 = not chosen
    int activeField = 0; // 1 = name, 2 = price
    string formMessage = "";
    string pageMessage = "";

    const FloatRect formPanel({175.f, 100.f}, {600.f, 480.f});
    const FloatRect nameRect({215.f, 185.f}, {520.f, 42.f});
    const FloatRect priceRect({215.f, 275.f}, {520.f, 42.f});
    FloatRect categoryRects[5];
    for (int c = 0; c < 5; c++)
        categoryRects[c] = FloatRect({215.f + (c % 3) * 175.f, 365.f + (c / 3) * 50.f}, {165.f, 40.f});
    const FloatRect saveRect({235.f, 510.f}, {220.f, 48.f});
    const FloatRect cancelRect({495.f, 510.f}, {220.f, 48.f});

    RectangleShape cursor({2.f, 24.f});
    cursor.setFillColor(Color::Black);
    Clock blinkClock;
    bool showCursor = true;

    while (window.isOpen())
    {
        while (optional<Event> event = window.pollEvent())
        {
            if (event->is<Event::Closed>())
            {
                window.close();
                return 0;
            }

            // mouse wheel scrolls the product table
            if (const auto *wheel = event->getIf<Event::MouseWheelScrolled>())
            {
                if (!formOpen)
                {
                    scrollY -= wheel->delta * INV_ROW_H;
                    clampScroll();
                }
            }

            // dragging the scrollbar
            if (const auto *moved = event->getIf<Event::MouseMoved>())
            {
                if (draggingThumb && maxScroll() > 0.f)
                {
                    float thumbH = thumbRect().size.y;
                    float dy = static_cast<float>(moved->position.y) - dragStartMouseY;
                    scrollY = dragStartScrollY + dy * maxScroll() / (INV_TABLE_H - thumbH);
                    clampScroll();
                }
            }
            if (event->is<Event::MouseButtonReleased>())
                draggingThumb = false;

            // typing into the form
            if (const auto *textEvent = event->getIf<Event::TextEntered>())
            {
                if (formOpen && activeField != 0)
                {
                    formMessage = "";
                    char32_t ch = textEvent->unicode;

                    if (ch == 8) // backspace
                    {
                        if (activeField == 1 && !newName.empty())
                            newName.pop_back();
                        else if (activeField == 2 && !newPrice.empty())
                            newPrice.pop_back();
                    }
                    else if (ch == 9 || ch == 13) // tab / enter moves to next field
                    {
                        activeField = (activeField == 1) ? 2 : 0;
                    }
                    else if (activeField == 1 && ch >= 32 && ch < 128 && newName.size() < 30)
                    {
                        newName += static_cast<char>(ch);
                    }
                    else if (activeField == 2 && ch >= '0' && ch <= '9' && newPrice.size() < 6)
                    {
                        newPrice += static_cast<char>(ch);
                    }

                    showCursor = true;
                    blinkClock.restart();
                }
            }

            if (const auto *mouseEvent = event->getIf<Event::MouseButtonPressed>())
            {
                if (mouseEvent->button != Mouse::Button::Left)
                    continue;

                Vector2f mousePos = window.mapPixelToCoords(mouseEvent->position);

                // ----- clicks inside the form -----
                if (formOpen)
                {
                    if (nameRect.contains(mousePos))
                        activeField = 1;
                    else if (priceRect.contains(mousePos))
                        activeField = 2;
                    else if (cancelRect.contains(mousePos))
                        formOpen = false;
                    else if (saveRect.contains(mousePos))
                    {
                        int price = newPrice.empty() ? 0 : stoi(newPrice);

                        // another product with the same name?
                        bool duplicate = false;
                        for (int i = 0; i < product_counts; i++)
                            if (i != editIndex && productNames[i] == newName)
                                duplicate = true;

                        if (newName.empty())
                            formMessage = "Please enter a product name.";
                        else if (price <= 0)
                            formMessage = "Please enter a price greater than 0.";
                        else if (newCategory == 0)
                            formMessage = "Please choose a category.";
                        else if (duplicate)
                            formMessage = "A product with this name already exists.";
                        else if (editIndex == -1) // ADD
                        {
                            int id = generateProductID(newCategory);
                            if (addProduct(id, newName, price, newCategory))
                            {
                                saveInventory();
                                pageMessage = "Added " + newName + " (ID " + to_string(id) + ")";
                                formOpen = false;

                                // scroll to the bottom so the new product is visible, and select it
                                selectedIndex = product_counts - 1;
                                scrollY = maxScroll();
                            }
                            else
                            {
                                formMessage = "Inventory is full!";
                            }
                        }
                        else // EDIT
                        {
                            // a new category gets a new ID in that category's range (e.g. 2xx)
                            if (productCategories[editIndex] != newCategory)
                                product_ids[editIndex] = generateProductID(newCategory);

                            productNames[editIndex] = newName;
                            productPrices[editIndex] = price;
                            productCategories[editIndex] = newCategory;
                            saveInventory();

                            pageMessage = "Updated " + newName + " (ID " + to_string(product_ids[editIndex]) + ")";
                            selectedIndex = editIndex;
                            formOpen = false;
                        }
                    }
                    else
                    {
                        activeField = 0;
                        for (int c = 0; c < 5; c++)
                            if (categoryRects[c].contains(mousePos))
                                newCategory = c + 1;
                    }
                    continue;
                }

                // ----- clicks on the page -----
                if (backRect.contains(mousePos))
                {
                    window.close();
                    return 1;
                }
                else if (addProductRect.contains(mousePos))
                {
                    pageMessage = "";
                    formOpen = true;
                    editIndex = -1;
                    newName = "";
                    newPrice = "";
                    newCategory = 0;
                    activeField = 1;
                    formMessage = "";
                    showCursor = true;
                    blinkClock.restart();
                }
                else if (selectedIndex >= 0 && availableRect.contains(mousePos))
                {
                    productAvailable[selectedIndex] = true;
                    saveInventory();
                    pageMessage = productNames[selectedIndex] + " is now Available";
                }
                else if (selectedIndex >= 0 && unavailableRect.contains(mousePos))
                {
                    productAvailable[selectedIndex] = false;
                    saveInventory();
                    pageMessage = productNames[selectedIndex] + " is now Unavailable";
                }
                else if (thumbRect().contains(mousePos))
                {
                    draggingThumb = true;
                    dragStartMouseY = mousePos.y;
                    dragStartScrollY = scrollY;
                }
                else if (mousePos.x >= INV_SCROLLBAR_X && mousePos.x <= INV_SCROLLBAR_X + INV_SCROLLBAR_W &&
                         mousePos.y >= INV_TABLE_TOP && mousePos.y <= INV_TABLE_TOP + INV_TABLE_H)
                {
                    // click on the scrollbar track: jump one screen up or down
                    scrollY += (mousePos.y < thumbRect().position.y) ? -INV_TABLE_H : INV_TABLE_H;
                    clampScroll();
                }
                else if (tableRect.contains(mousePos))
                {
                    // which row was clicked (taking the scroll into account)
                    Vector2f tablePos(mousePos.x, mousePos.y + scrollY);
                    int row = static_cast<int>((tablePos.y - INV_TABLE_TOP) / INV_ROW_H);

                    if (row >= 0 && row < product_counts)
                    {
                        selectedIndex = row;
                        pageMessage = "";

                        if (inventoryEditRect(row).contains(tablePos)) // Edit button of this row
                        {
                            formOpen = true;
                            editIndex = row;
                            newName = productNames[row];
                            newPrice = to_string(productPrices[row]);
                            newCategory = productCategories[row];
                            activeField = 1;
                            formMessage = "";
                            showCursor = true;
                            blinkClock.restart();
                        }
                    }
                    else
                    {
                        selectedIndex = -1;
                    }
                }
                else
                {
                    selectedIndex = -1;
                }
            }
        }

        if (blinkClock.getElapsedTime().asSeconds() >= 0.5f)
        {
            showCursor = !showCursor;
            blinkClock.restart();
        }

        window.clear(Color(211, 211, 211)); // light gray background

        Text title(font, "Product Inventory", 30);
        title.setFillColor(Color(44, 62, 80));
        title.setPosition({328.f, 28.f});
        window.draw(title);

        drawInventoryButton(window, font, backRect, "Back", Color(189, 195, 199), Color::Black, 18);
        drawInventoryButton(window, font, addProductRect, "+ Add Product", Color(41, 128, 185), Color::White);
        drawInventoryButton(window, font, availableRect, "Available", Color(39, 174, 96), Color::White);
        drawInventoryButton(window, font, unavailableRect, "Unavailable", Color(192, 57, 43), Color::White);

        // table header
        RectangleShape header({INV_TABLE_W, 40.f});
        header.setPosition({INV_TABLE_X, 90.f});
        header.setFillColor(Color(52, 73, 94));
        window.draw(header);

        const string headers[6] = {"ID", "Product Name", "Price", "Category", "Status", "Action"};
        const float headerX[6] = {45.f, 130.f, 430.f, 530.f, 715.f, 825.f};
        for (int h = 0; h < 6; h++)
        {
            Text ht(font, headers[h], 16);
            ht.setFillColor(Color::White);
            ht.setPosition({headerX[h], 101.f});
            window.draw(ht);
        }

        // ----- rows: drawn through a view so they scroll and are cut off at the table edges -----
        View tableView(FloatRect({INV_TABLE_X, INV_TABLE_TOP + scrollY}, {INV_TABLE_W, INV_TABLE_H}));
        tableView.setViewport(FloatRect({INV_TABLE_X / INV_WINDOW_W, INV_TABLE_TOP / INV_WINDOW_H},
                                        {INV_TABLE_W / INV_WINDOW_W, INV_TABLE_H / INV_WINDOW_H}));
        window.setView(tableView);

        RectangleShape tableBg({INV_TABLE_W, max(INV_TABLE_H, product_counts * INV_ROW_H)});
        tableBg.setPosition({INV_TABLE_X, INV_TABLE_TOP});
        tableBg.setFillColor(Color(245, 247, 250));
        window.draw(tableBg);

        for (int i = 0; i < product_counts; i++)
        {
            float y = INV_TABLE_TOP + i * INV_ROW_H;

            // skip rows that are outside the visible part
            if (y + INV_ROW_H < INV_TABLE_TOP + scrollY || y > INV_TABLE_TOP + scrollY + INV_TABLE_H)
                continue;

            RectangleShape rowBox({INV_TABLE_W, INV_ROW_H - 2.f});
            rowBox.setPosition({INV_TABLE_X, y});
            rowBox.setFillColor(selectedIndex == i
                                    ? Color(174, 214, 241)
                                    : (i % 2 == 0 ? Color::White : Color(236, 240, 241)));
            window.draw(rowBox);

            Text idText(font, to_string(product_ids[i]), 15);
            idText.setFillColor(Color::Black);
            idText.setPosition({45.f, y + 10.f});
            window.draw(idText);

            Text nameText(font, productNames[i], 15);
            nameText.setFillColor(Color::Black);
            nameText.setPosition({130.f, y + 10.f});
            window.draw(nameText);

            Text priceText(font, "Rs. " + to_string(productPrices[i]), 15);
            priceText.setFillColor(Color::Black);
            priceText.setPosition({430.f, y + 10.f});
            window.draw(priceText);

            Text categoryText(font, getCategoryName(productCategories[i]), 15);
            categoryText.setFillColor(Color::Black);
            categoryText.setPosition({530.f, y + 10.f});
            window.draw(categoryText);

            Text statusText(font, productAvailable[i] ? "Available" : "Unavailable", 15);
            statusText.setFillColor(productAvailable[i] ? Color(39, 174, 96) : Color(192, 57, 43));
            statusText.setPosition({715.f, y + 10.f});
            window.draw(statusText);

            drawInventoryButton(window, font, inventoryEditRect(i), "Edit", Color(41, 128, 185), Color::White, 14);
        }

        window.setView(window.getDefaultView());

        // scrollbar (only when there are more products than fit)
        if (maxScroll() > 0.f)
        {
            RectangleShape track({INV_SCROLLBAR_W, INV_TABLE_H});
            track.setPosition({INV_SCROLLBAR_X, INV_TABLE_TOP});
            track.setFillColor(Color(236, 240, 241));
            window.draw(track);

            FloatRect t = thumbRect();
            RectangleShape thumb(t.size);
            thumb.setPosition(t.position);
            thumb.setFillColor(draggingThumb ? Color(52, 73, 94) : Color(127, 140, 141));
            window.draw(thumb);
        }

        Text footerText(font, "Products: " + to_string(product_counts) +
                                  "   (click a row to select it, use the mouse wheel or the bar to scroll)", 14);
        footerText.setFillColor(Color(80, 80, 80));
        footerText.setPosition({30.f, 605.f});
        window.draw(footerText);

        if (!pageMessage.empty())
        {
            Text msg(font, pageMessage, 15);
            msg.setFillColor(Color(30, 130, 76));
            msg.setPosition({30.f, 630.f});
            window.draw(msg);
        }

        if (product_counts == 0)
        {
            Text emptyText(font, "No products found", 20);
            emptyText.setFillColor(Color(127, 140, 141));
            emptyText.setPosition({390.f, 180.f});
            window.draw(emptyText);
        }

        // ---------- draw the Add / Edit form on top ----------
        if (formOpen)
        {
            RectangleShape shade({INV_WINDOW_W, INV_WINDOW_H});
            shade.setFillColor(Color(0, 0, 0, 120));
            window.draw(shade);

            RectangleShape panel(formPanel.size);
            panel.setPosition(formPanel.position);
            panel.setFillColor(Color::White);
            panel.setOutlineThickness(2.f);
            panel.setOutlineColor(Color(52, 73, 94));
            window.draw(panel);

            Text formTitle(font, editIndex == -1 ? "Add New Product" : "Edit Product", 24);
            formTitle.setFillColor(Color(44, 62, 80));
            formTitle.setStyle(Text::Bold);
            FloatRect tb = formTitle.getLocalBounds();
            formTitle.setPosition({475.f - tb.size.x / 2.f, 115.f});
            window.draw(formTitle);

            Text nameLabel(font, "Product Name:", 16);
            nameLabel.setFillColor(Color::Black);
            nameLabel.setPosition({215.f, 160.f});
            window.draw(nameLabel);

            Text priceLabel(font, "Price (Rs.):", 16);
            priceLabel.setFillColor(Color::Black);
            priceLabel.setPosition({215.f, 250.f});
            window.draw(priceLabel);

            Text categoryLabel(font, "Category:", 16);
            categoryLabel.setFillColor(Color::Black);
            categoryLabel.setPosition({215.f, 340.f});
            window.draw(categoryLabel);

            const FloatRect fieldRects[2] = {nameRect, priceRect};
            const string fieldValues[2] = {newName, newPrice};
            for (int f = 0; f < 2; f++)
            {
                RectangleShape field(fieldRects[f].size);
                field.setPosition(fieldRects[f].position);
                field.setFillColor(Color::White);
                field.setOutlineThickness(2.f);
                field.setOutlineColor(activeField == f + 1 ? Color(41, 128, 185) : Color::Black);
                window.draw(field);

                Text value(font, fieldValues[f], 18);
                value.setFillColor(Color::Black);
                value.setPosition({fieldRects[f].position.x + 10.f, fieldRects[f].position.y + 10.f});
                window.draw(value);

                if (activeField == f + 1 && showCursor)
                {
                    float x = fieldValues[f].empty()
                                  ? value.getPosition().x
                                  : value.getGlobalBounds().position.x + value.getGlobalBounds().size.x + 2.f;
                    cursor.setPosition({x, fieldRects[f].position.y + 9.f});
                    window.draw(cursor);
                }
            }

            for (int c = 0; c < 5; c++)
                drawInventoryButton(window, font, categoryRects[c], getCategoryName(c + 1),
                                    newCategory == c + 1 ? Color(41, 128, 185) : Color(236, 240, 241),
                                    newCategory == c + 1 ? Color::White : Color::Black, 14);

            // show which ID the product will have
            if (newCategory != 0)
            {
                string idInfo;
                if (editIndex == -1)
                    idInfo = "New ID will be: " + to_string(generateProductID(newCategory));
                else if (productCategories[editIndex] == newCategory)
                    idInfo = "Product ID: " + to_string(product_ids[editIndex]);
                else
                    idInfo = "Category changed - new ID will be: " + to_string(generateProductID(newCategory));

                Text idPreview(font, idInfo, 14);
                idPreview.setFillColor(Color(127, 140, 141));
                idPreview.setPosition({215.f, 462.f});
                window.draw(idPreview);
            }

            if (!formMessage.empty())
            {
                Text msg(font, formMessage, 15);
                msg.setFillColor(Color(192, 57, 43));
                msg.setPosition({215.f, 482.f});
                window.draw(msg);
            }

            drawInventoryButton(window, font, saveRect, editIndex == -1 ? "Save Product" : "Save Changes",
                                Color(39, 174, 96), Color::White, 18);
            drawInventoryButton(window, font, cancelRect, "Cancel", Color(189, 195, 199), Color::Black, 18);
        }

        window.display();
    }

    return 0;
}
