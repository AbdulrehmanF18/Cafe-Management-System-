#include <iostream>
#include <SFML/Graphics.hpp>
#include <string>
using namespace std;
using namespace sf;

int main()
{
    RenderWindow window(VideoMode({600, 665}), "cafe management system "); // creating window
    window.setFramerateLimit(60);

    // input boxes
    RectangleShape emailbox({400.f, 45.f});
    emailbox.setPosition({100.f, 110.f});
    emailbox.setFillColor(Color::White);
    emailbox.setOutlineThickness(2.f);
    emailbox.setOutlineColor(Color::Black);

    RectangleShape passwordbox({400.f, 45.f});
    passwordbox.setPosition({100.f, 220.f});
    passwordbox.setFillColor(Color::White);
    passwordbox.setOutlineThickness(2.f);
    passwordbox.setOutlineColor(Color::Black);

    Font font("C:\\Windows\\Fonts\\Arial.ttf");

    // input text
    Text emailtext(font, "", 20);
    emailtext.setPosition({112.f, 108.f});
    emailtext.setFillColor(Color::White);
    emailtext.setOutlineColor(Color::Black);
    emailtext.setOutlineThickness(2.f);

    Text passwordtext(font, "", 20);
    passwordtext.setPosition({112.f, 218.f});
    passwordtext.setFillColor(Color::White);
    passwordtext.setOutlineColor(Color::Black);
    passwordtext.setOutlineThickness(2.f);

    // input labels
    Text emaillable(font, "E-MAIL:", 20);
    emaillable.setPosition({100.f, 65.f});
    emaillable.setFillColor(Color::Black);

    Text passwordlable(font, "Password:", 20);
    passwordlable.setPosition({100.f, 165.f});
    passwordlable.setFillColor(Color::Black);

    string email = "";
    string password = "";

    int input_fields = 0;

    while (window.isOpen())
    {
        while (optional event = window.pollEvent())
        {
            if (event->is<Event::Closed>()) // manage events like closing of window
            {
                window.close();
            }
            if (const auto *mouseEvent = event->getIf<Event::MouseButtonPressed>())
            {
                if (mouseEvent->button == Mouse::Button::Left)
                {
                    Vector2f mousePos = window.mapPixelToCoords(mouseEvent->position);

                    if (emailbox.getGlobalBounds().contains(mousePos))
                    {
                        input_fields = 1;
                    }

                    else if (passwordbox.getGlobalBounds().contains(mousePos))
                    {
                        input_fields = 2;
                    }

                    else
                    {
                        input_fields = 0;
                    }
                }
            }
            if (const auto *textEvent = event->getIf<Event::TextEntered>())
            {
                if (input_fields != 0)
                {
                    if (textEvent->unicode == 8)
                    {
                        if (input_fields == 1 && !email.empty())
                        {
                            email.pop_back();
                        }
                        if (input_fields == 2 && !password.empty())
                        {
                            password.pop_back();
                        }
                    }
                    else if (textEvent->unicode == 13) // enter key
                    {
                        input_fields = 0;
                    }
                    else if (textEvent->unicode >= 32 && textEvent->unicode < 128)
                    {
                        char character = static_cast<char>(textEvent->unicode);

                        if (input_fields == 1)
                        {
                            email += character;
                        }
                        else if (input_fields == 2)
                        {
                            password += character;
                        }
                    }
                    emailtext.setString(email);
                    passwordtext.setString(password);
                }
            }
        }
        window.clear(Color::Cyan);

        window.draw(emailbox);
         window.draw(emaillable);
        window.draw(emailtext);

        window.draw(passwordbox);
         window.draw(passwordlable);
        window.draw(passwordtext);

        window.display();
    }

    return 0;
}
