#include <iostream>
#include <SFML/Graphics.hpp>
#include <string>
#include <fstream>
#include <sstream>
using namespace std;
using namespace sf;

bool checkLogin(string enteredEmail, string enteredPassword)
{
    ifstream file("users.txt");

    if (!file.is_open())
    {
        cout << "Unable to open users.txt";
        return false;
    }

    string line;
    string storedName;
        string storedEmail;
    string storedPassword;
    string storedRole;
      cout << "inside funtion " << endl;
    while (getline(file, line))
    {
        stringstream ss(line);
        getline(ss, storedName, ',');
        cout << storedRole;
        getline(ss, storedEmail, ','); cout << storedEmail;
        getline(ss, storedRole, ',');
        cout << storedRole;
        getline(ss, storedPassword, ','); cout << storedPassword;
        

        if (enteredEmail == storedEmail && enteredPassword == storedPassword)
        {
            file.close();
            return true;
        }
    }

    file.close();
    return false;
}

int openSignInScreen()
{
    RenderWindow window(VideoMode({600, 665}), "Cafe Management System - Sign In");
    window.setFramerateLimit(60);

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

    Font font("Arial.ttf");

    Text emailtext(font, "", 20);
    emailtext.setPosition({112.f, 118.f});
    emailtext.setFillColor(Color::Black);

    Text passwordtext(font, "", 20);
    passwordtext.setPosition({112.f, 228.f});
    passwordtext.setFillColor(Color::Black);

    Text emaillable(font, "E-MAIL:", 20);
    emaillable.setPosition({100.f, 65.f});
    emaillable.setFillColor(Color::Black);

    Text passwordlable(font, "Password:", 20);
    passwordlable.setPosition({100.f, 165.f});
    passwordlable.setFillColor(Color::Black);

    Text registerText(font, "Don't have an account? Register", 20);
    registerText.setPosition({110.f, 560.f});
    registerText.setFillColor(Color::Blue);
    registerText.setStyle(Text::Underlined);

    Text signinButton(font, "SIGN IN", 28);
    signinButton.setPosition({230.f, 330.f});
    signinButton.setFillColor(Color::Black);

    string email = "";
    string password = "";
    int input_fields = 0;

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

                    if (emailbox.getGlobalBounds().contains(mousePos))
                    {
                        input_fields = 1;
                    }
                    else if (passwordbox.getGlobalBounds().contains(mousePos))
                    {
                        input_fields = 2;
                    }
                    else if (registerText.getGlobalBounds().contains(mousePos))
                    {
                        window.close();
                        return 1;
                    }
                    else if (signinButton.getGlobalBounds().contains(mousePos))
                    {
                        cout << email <<  password;
                        if (checkLogin(email, password))
                        {

                            cout << "Login Successful!" << endl;
                            window.close();

                            return 2;
                        }
                        else
                        {
                            cout << "Invalid Email or Password!" << endl;
                        }
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
                    else if (textEvent->unicode == 13)
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

        window.draw(signinButton);
        window.draw(registerText);
        window.display();
    }

    return 0;
}
