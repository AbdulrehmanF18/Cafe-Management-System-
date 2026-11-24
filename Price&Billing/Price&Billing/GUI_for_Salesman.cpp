#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <optional>
#include <string>
#include "GUI_forsales.h"

//FUNCTION PROTOTYPES
void runGUI();



string getCategoryName(int category);
int getItemCount(int category);
string getItemName(int category, int pick);
int getItemPrice(int category, int pick);

bool addItemToOrder(int category, int pick, int qty);

int getInvoiceCount();
std::string getInvoiceName(int index);
int getInvoiceQty(int index);
int getInvoiceLineTotal(int index);
int getInvoiceTotal();
void saveInvoiceToFile();
string getCurrentDate();
void clearInvoice();

using namespace std;

// The four screens of the program
enum Screen { PORTAL, CATEGORIES, ITEMS, INVOICE };

const unsigned int WINDOW_W = 1000;
const unsigned int WINDOW_H = 750;
const int MAX_BUTTONS = 16;

// ================= BUTTON HELPERS =================

struct Button
{
	float x, y, w, h;
	string label;
};

Button makeButton(float x, float y, float w, float h, string label)
{
	Button b;
	b.x = x;
	b.y = y;
	b.w = w;
	b.h = h;
	b.label = label;
	return b;
}

bool isInside(const Button& b, sf::Vector2f p)
{
	return p.x >= b.x && p.x <= b.x + b.w &&
		p.y >= b.y && p.y <= b.y + b.h;
}

void drawButton(sf::RenderWindow& window, const sf::Font& font,const Button& b, bool hovered, bool selected)
{
	sf::RectangleShape box(sf::Vector2f(b.w, b.h));
	box.setPosition(sf::Vector2f(b.x, b.y));
	box.setFillColor(hovered ? sf::Color(225, 225, 225) : sf::Color::White);
	box.setOutlineColor(sf::Color::Black);
	box.setOutlineThickness(selected ? 5.f : 2.f);
	window.draw(box);

	sf::Text text(font, b.label, 24);
	text.setFillColor(sf::Color::Black);

	// center the text inside the button
	sf::FloatRect bounds = text.getLocalBounds();
	text.setOrigin(sf::Vector2f(bounds.position.x + bounds.size.x / 2.f,
		bounds.position.y + bounds.size.y / 2.f));
	text.setPosition(sf::Vector2f(b.x + b.w / 2.f, b.y + b.h / 2.f));
	window.draw(text);
}

void drawText(sf::RenderWindow& window, const sf::Font& font, const string& str,
	float x, float y, unsigned int size, bool centered = false)
{
	sf::Text text(font, str, size);
	text.setFillColor(sf::Color::Black);

	if (centered)
	{
		sf::FloatRect bounds = text.getLocalBounds();
		text.setOrigin(sf::Vector2f(bounds.position.x + bounds.size.x / 2.f, 0.f));
	}

	text.setPosition(sf::Vector2f(x, y));
	window.draw(text);
}

// BUTTON LAYOUT FOR EACH SCREEN
// Fills the buttons array for the current screen and returns how many buttons there are.
// The order of buttons here is the order used when handling clicks.

int buildButtons(Screen screen, int category, Button buttons[])
{
	int n = 0;

	if (screen == PORTAL)
	{
		buttons[n++] = makeButton(350, 300, 300, 70, "View Menu");        // 0
		buttons[n++] = makeButton(350, 400, 300, 70, "Checkout / Exit");  // 1
	}
	else if (screen == CATEGORIES)
	{
		for (int i = 0; i < NUM_CATEGORIES; i++)                           // 0 to 4
			buttons[n++] = makeButton(350, 120.f + i * 72.f, 300, 58, getCategoryName(i + 1));

		buttons[n++] = makeButton(350, 500, 300, 58, "View Invoice");      // 5
		buttons[n++] = makeButton(350, 570, 300, 58, "Back to Portal");    // 6
	}
	else if (screen == ITEMS)
	{
		int count = getItemCount(category);

		for (int i = 0; i < count; i++)                                    // 0 to count-1
		{
			string label = to_string(i + 1) + ". " + getItemName(category, i + 1) +
				" - Rs " + to_string(getItemPrice(category, i + 1));
			buttons[n++] = makeButton(150, 100.f + i * 58.f, 700, 50, label);
		}

		buttons[n++] = makeButton(300, 500, 60, 50, "-");                  // count
		buttons[n++] = makeButton(470, 500, 60, 50, "+");                  // count + 1
		buttons[n++] = makeButton(600, 500, 250, 50, "Add to Order");      // count + 2
		buttons[n++] = makeButton(150, 620, 300, 55, "Back");              // count + 3
		buttons[n++] = makeButton(550, 620, 300, 55, "View Invoice");      // count + 4
	}
	else if (screen == INVOICE)
	{
		buttons[n++] = makeButton(60, 670, 280, 55, "Back to Portal");     // 0
		buttons[n++] = makeButton(360, 670, 280, 55, "New Order");         // 1
		buttons[n++] = makeButton(660, 670, 280, 55, "Close");             // 2
	}

	return n;
}

//  MAIN GUI FUNCTION 

void runGUI()
{
	// Load the Calibri font (Windows font folder, or next to the program)
	sf::Font font;
	if (!font.openFromFile("C:/Windows/Fonts/calibri.ttf"))
	{
		if (!font.openFromFile("calibri.ttf"))
		{
			cerr << "Could not load the Calibri font." << endl;
			return;
		}
	}

	sf::RenderWindow window(sf::VideoMode({ WINDOW_W, WINDOW_H }), "Brew & Bite Cafe");
	window.setFramerateLimit(60);

	// Program state
	Screen screen = PORTAL;
	int category = 0;       // 1 to 5 once a category is opened
	int selectedItem = 0;   // 0 = nothing selected
	int qty = 1;
	string message = "";

	while (window.isOpen())
	{
		Button buttons[MAX_BUTTONS];
		int buttonCount = buildButtons(screen, category, buttons);

		// EVENTS
		while (const optional<sf::Event> event = window.pollEvent())
		{
			if (event->is<sf::Event::Closed>())
			{
				window.close();
			}
			else if (const auto* click = event->getIf<sf::Event::MouseButtonPressed>())
			{
				if (click->button == sf::Mouse::Button::Left)
				{
					sf::Vector2f mouse(click->position);

					int clicked = -1;
					for (int i = 0; i < buttonCount; i++)
					{
						if (isInside(buttons[i], mouse))
							clicked = i;
					}

					if (clicked != -1)
					{
						if (screen == PORTAL)
						{
							if (clicked == 0)
							{
								screen = CATEGORIES;
							}
							else if (clicked == 1)
							{
								screen = INVOICE;
							}
						}
						else if (screen == CATEGORIES)
						{
							if (clicked >= 0 && clicked < NUM_CATEGORIES)
							{
								category = clicked + 1;
								selectedItem = 0;
								qty = 1;
								message = "";
								screen = ITEMS;
							}
							else if (clicked == NUM_CATEGORIES)
							{
								screen = INVOICE;
							}
							else
							{
								screen = PORTAL;
							}
						}
						else if (screen == ITEMS)
						{
							int count = getItemCount(category);

							if (clicked < count)
							{
								selectedItem = clicked + 1;
								message = "";
							}
							else if (clicked == count)          // minus
							{
								if (qty > 1)
									qty--;
							}
							else if (clicked == count + 1)      // plus
							{
								if (qty < 99)
									qty++;
							}
							else if (clicked == count + 2)      // add to order
							{
								if (selectedItem == 0)
								{
									message = "Please select an item first.";
								}
								else if (addItemToOrder(category, selectedItem, qty))
								{
									message = "Added " + to_string(qty) + " x " +
										getItemName(category, selectedItem);
									qty = 1;
								}
								else
								{
									message = "Could not add the item.";
								}
							}
							else if (clicked == count + 3)      // back
							{
								selectedItem = 0;
								message = "";
								screen = CATEGORIES;
							}
							else                                // view invoice
							{
								screen = INVOICE;
							}
						}
						else if (screen == INVOICE)
						{
							if (clicked == 0)
							{
								screen = PORTAL;
							}
							else if (clicked == 1)
							{
								saveInvoiceToFile();
								clearInvoice();
								message = "";
								screen = PORTAL;
							}
							else
							{
								saveInvoiceToFile();
								window.close();
							}
						}

						// the screen may have changed, so rebuild the buttons
						buttonCount = buildButtons(screen, category, buttons);
					}
				}
			}
		}

		if (!window.isOpen())
			break;

		//  DRAWING
		window.clear(sf::Color::Cyan);

		sf::Vector2f mouse(sf::Mouse::getPosition(window));

		if (screen == PORTAL)
		{
			drawText(window, font, "BREW & BITE CAFE", 500, 90, 60, true);
			drawText(window, font, "CUSTOMER PORTAL", 500, 190, 34, true);
			drawText(window, font, "Date: " + getCurrentDate(), 500, 245, 28, true);
		}
		else if (screen == CATEGORIES)
		{
			drawText(window, font, "MENU", 500, 35, 48, true);
		}
		else if (screen == ITEMS)
		{
			drawText(window, font, getCategoryName(category), 500, 25, 44, true);
			drawText(window, font, "Quantity:", 150, 505, 28);
			drawText(window, font, to_string(qty), 415, 503, 32, true);
			drawText(window, font, message, 500, 565, 24, true);
		}
		else if (screen == INVOICE)
		{
			drawText(window, font, "INVOICE", 500, 20, 44, true);
			drawText(window, font, "Date: " + getCurrentDate(), 700, 35, 24);
			int count = getInvoiceCount();

			if (count == 0)
			{
				drawText(window, font, "No items ordered yet.", 500, 300, 30, true);
			}

			for (int i = 0; i < count; i++)
			{
				float y = 85.f + i * 24.f;
				drawText(window, font, getInvoiceName(i) + " x" + to_string(getInvoiceQty(i)),
					150, y, 20);
				drawText(window, font, "Rs " + to_string(getInvoiceLineTotal(i)),
					700, y, 20);
			}

			drawText(window, font, "TOTAL: Rs " + to_string(getInvoiceTotal()), 500, 615, 32, true);
		}

		// buttons are drawn last so they sit on top
		for (int i = 0; i < buttonCount; i++)
		{
			bool hovered = isInside(buttons[i], mouse);
			bool selected = (screen == ITEMS && i < getItemCount(category) && i + 1 == selectedItem);
			drawButton(window, font, buttons[i], hovered, selected);
		}

		window.display();
	}
}
