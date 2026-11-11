#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
using namespace std;
using namespace sf;

int main()
{
    RenderWindow window(VideoMode({600, 665}), "cafe management system "); // creating window
    window.setFramerateLimit(60);

    // input boxes and their inside colors
    RectangleShape namebox({400.f, 45.f});
    RectangleShape emailbox({400.f, 45.f});
    RectangleShape rolebox({400.f, 45.f});
    RectangleShape passwordbox({400.f, 45.f});

    namebox.setFillColor(Color::White);
    emailbox.setFillColor(Color::White);
    rolebox.setFillColor(Color::White);
    passwordbox.setFillColor(Color::White);

    // set position of rectangle
    namebox.setPosition({100.f, 110.f});
    emailbox.setPosition({100.f, 220.f});
    rolebox.setPosition({100.f, 330.f});
    passwordbox.setPosition({100.f, 520.f});

    // set outline thickness and color of rectangle
    namebox.setOutlineThickness(2.f);
    emailbox.setOutlineThickness(2.f);
    rolebox.setOutlineThickness(2.f);
    passwordbox.setOutlineThickness(2.f);

    namebox.setOutlineColor(Color::Black);
    emailbox.setOutlineColor(Color::Black);
    rolebox.setOutlineColor(Color::Black);
    passwordbox.setOutlineColor(Color::Black);

    // input text
    Font font("C:\\Windows\\Fonts\\Arial.ttf");

    Text nametext(font, "", 20);
    nametext.setFillColor(Color::Black);
    nametext.setPosition({112.f, 118.f}); // set position and colors of text

    Text emailtext(font, "", 20);
    emailtext.setFillColor(Color::Black);
    emailtext.setPosition({112.f, 228.f});

    Text roletext(font, "Select Role", 20);
    roletext.setFillColor(Color::Black);
    roletext.setPosition({112.f, 338.f});

    // dropdown for role

    RectangleShape adminoption({400.f, 45.f});
    RectangleShape salesmanoption({400.f, 45.f});

    adminoption.setPosition({100.f, 375.f});
    salesmanoption.setPosition({100.f, 415.f});

    adminoption.setOutlineThickness(1.f);
    salesmanoption.setOutlineThickness(1.f);

    adminoption.setOutlineColor(Color::Black);
    salesmanoption.setOutlineColor(Color::Black);

    // dropdown text
    Text admintext(font, "Admin", 20);
    admintext.setFillColor(Color::Black);
    admintext.setPosition({112.f, 383.f});

    Text salesmantext(font, "Salesman", 20);
    salesmantext.setFillColor(Color::Black);
    salesmantext.setPosition({112.f, 423.f}); //

    Text passwordtext(font, "", 20);
    passwordtext.setFillColor(Color::Black);
    passwordtext.setPosition({112.f, 528.f});

    // lable
    Text namelabel(font, "Name:", 20);
    namelabel.setFillColor(Color::Black);
    namelabel.setPosition({100.f, 65.f});

    Text emaillabel(font, "E-MAIL:", 20);
    emaillabel.setFillColor(Color::Black);
    emaillabel.setPosition({100.f, 175.f});

    Text rolelabel(font, "Role:", 20);
    rolelabel.setFillColor(Color::Black);
    rolelabel.setPosition({100.f, 285.f});

    Text passwordlabel(font, "Password:", 20);
    passwordlabel.setFillColor(Color::Black);
    passwordlabel.setPosition({100.f, 475.f});

    Text adminText(font, "Admin", 20);
    adminText.setPosition({112.f, 382.f});
    adminText.setFillColor(Color::Black);

    Text salesmanText(font, "Salesman", 20);
    salesmanText.setPosition({112.f, 422.f});
    salesmanText.setFillColor(Color::Black);

    string name = "";
    string email = "";
    string password = "";
    string role = "";

    int input_fields = 0;
    bool drop = false;

    while (window.isOpen())
    {
        while (optional event = window.pollEvent())
        {
            if (event->is<Event::Closed>()) // manage events like closing of window
            {
                window.close();
            }

            if (const auto *textEvent = event->getIf<Event::TextEntered>())
            {
                if (!drop && input_fields != 0)
                {
                    if (textEvent->unicode == 8) // backspace
                    {
                        if (input_fields == 1 && !name.empty())
                        {
                            name.pop_back();
                        }
                        else if (input_fields == 2 && !email.empty())
                        {
                            email.pop_back();
                        }
                        else if (input_fields == 3 && !password.empty())
                        {
                            password.pop_back();
                        }
                    }
                    else if (textEvent->unicode == 13) // enter key
                    {
                        input_fields=0;
                    }
                    else if (textEvent->unicode >= 32 && textEvent->unicode < 128)
                    {
                        char character = static_cast<char>(textEvent->unicode);

                        if(input_fields == 1)
                        {
                            name += character;
                        }
                        else if (input_fields == 2)
                        {
                            email += character;
                        }
                        else if (input_fields == 3)
                        {
                            password += character;
                        }
                    }

                    nametext.setString(name);
                    emailtext.setString(email);
                    passwordtext.setString(password);
                }
            }

            if (const auto *mouseEvent =
                    event->getIf<Event::MouseButtonPressed>())
            {
                if (mouseEvent->button == Mouse::Button::Left)
                {
                    Vector2f mousePos =
                        window.mapPixelToCoords(mouseEvent->position);

                    if (namebox.getGlobalBounds().contains(mousePos))
                    {
                        input_fields = 1;
                        drop = false;
                    }

                    
                    else if (emailbox.getGlobalBounds().contains(mousePos))
                    {
                        input_fields = 2;
                        drop = false;
                    }
                    else if (rolebox.getGlobalBounds().contains(mousePos))
                    {
                        input_fields = 0;
                        drop = !drop;
                    }

    
                    else if (drop && adminoption.getGlobalBounds().contains(mousePos))
                    {
                        role = "Admin";
                        roletext.setString(role);
                        drop = false;
                    }

                    else if (drop && salesmanoption.getGlobalBounds().contains(mousePos))
                    {
                        role = "Salesman";
                        roletext.setString(role);
                        drop = false;
                    }
                    else if (passwordbox.getGlobalBounds().contains(mousePos))
                    {
                        input_fields = 3;
                        drop = false;
                    }
                    else
                    {
                        input_fields = 0;
                        drop = false;
                    }
                }
            }
        }
        window.clear(Color::Cyan); // add colors
        window.draw(namelabel);    // draw lables
        window.draw(emailtext);
        window.draw(roletext);
        window.draw(passwordtext);

        window.draw(emaillabel);
        window.draw(rolelabel);
        window.draw(passwordlabel);

        window.draw(namebox); // draw boxes
        window.draw(emailbox);
        window.draw(rolebox);
        window.draw(passwordbox);

        window.draw(nametext);
        window.draw(emailtext);
        window.draw(roletext);
        window.draw(passwordtext);

        if (drop)
        {
            window.draw(adminoption);
            window.draw(salesmanoption);

            window.draw(adminText);
            window.draw(salesmanText);
        }

        window.display();
    }

    return 0;
}