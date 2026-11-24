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
        cout << "Unable to open users.txt" << endl;
        return false;
    }

    string line;
    string storedName;
    string storedEmail;
    string storedRole;
    string storedPassword;

    while (getline(file, line))
    {
        stringstream ss(line);
        getline(ss, storedName, ',');
        getline(ss, storedEmail, ',');
        getline(ss, storedRole, ',');
        getline(ss, storedPassword, ',');

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
    RenderWindow window(VideoMode({800, 800}), "Cafe Management System - Sign In");
    window.setFramerateLimit(60);

    Color lightGreyColor(211, 211, 211);

    Font font("Arial.ttf");

    Text cafeTitle(font, "BREW & BITE CAFE", 42);
    cafeTitle.setPosition({172.f, 40.f});
    cafeTitle.setFillColor(Color::Black);
    cafeTitle.setStyle(Text::Bold);

    Text welcomeSubtitle(font, "welcome foodie you are entring your comfort zone!", 21);
    welcomeSubtitle.setPosition({148.f, 102.f});
    welcomeSubtitle.setFillColor(Color::Black);
    welcomeSubtitle.setStyle(Text::Italic);

    RectangleShape emailbox({500.f, 48.f});
    emailbox.setPosition({150.f, 220.f});
    emailbox.setFillColor(Color::White);
    emailbox.setOutlineThickness(2.f);
    emailbox.setOutlineColor(Color::Black);

    RectangleShape passwordbox({500.f, 48.f});
    passwordbox.setPosition({150.f, 360.f});
    passwordbox.setFillColor(Color::White);
    passwordbox.setOutlineThickness(2.f);
    passwordbox.setOutlineColor(Color::Black);

    Text emailtext(font, "", 20);
    emailtext.setPosition({162.f, 230.f});
    emailtext.setFillColor(Color::Black);

    Text passwordtext(font, "", 20);
    passwordtext.setPosition({162.f, 370.f});
    passwordtext.setFillColor(Color::Black);

    Text emaillable(font, "E-MAIL:", 20);
    emaillable.setPosition({150.f, 180.f});
    emaillable.setFillColor(Color::Black);

    Text passwordlable(font, "Password:", 20);
    passwordlable.setPosition({150.f, 320.f});
    passwordlable.setFillColor(Color::Black);

    Text signinButton(font, "SIGN IN", 28);
    signinButton.setPosition({345.f, 465.f});
    signinButton.setFillColor(Color::Black);

    Text statusMessage(font, "", 19);
    statusMessage.setPosition({185.f, 520.f});
    statusMessage.setFillColor(Color::Red);

    Text registerText(font, "Don't have an account? Register", 20);
    registerText.setPosition({255.f, 690.f});
    registerText.setFillColor(Color::Blue);
    registerText.setStyle(Text::Underlined);

    // Blinking cursor setup
    RectangleShape cursor({2.f, 24.f});
    cursor.setFillColor(Color::Black);
    Clock blinkClock;
    float blinkInterval = 0.5f;
    bool showCursor = true;

    string email = "";
    string password = "";
    int input_fields = 0; // 1 = email, 2 = password

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
                        statusMessage.setString("");
                        showCursor = true;
                        blinkClock.restart();
                    }
                    else if (passwordbox.getGlobalBounds().contains(mousePos))
                    {
                        input_fields = 2;
                        statusMessage.setString("");
                        showCursor = true;
                        blinkClock.restart();
                    }
                    else if (registerText.getGlobalBounds().contains(mousePos))
                    {
                        window.close();
                        return 1;
                    }
                    else if (signinButton.getGlobalBounds().contains(mousePos))
                    {
                        if (checkLogin(email, password))
                        {
                            cout << "Login Successful!" << endl;
                            window.close();

                            return 2;
                        }
                        else
                        {
                            cout << "You are unauthorized, please register first!" << endl;
                            statusMessage.setString("You are unauthorized, please register first!");
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
                    statusMessage.setString("");

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

                    // Keep cursor visible while typing
                    showCursor = true;
                    blinkClock.restart();
                }
            }
        }

        // Blink and move cursor in the active text field
        if (input_fields != 0)
        {
            if (blinkClock.getElapsedTime().asSeconds() >= blinkInterval)
            {
                showCursor = !showCursor;
                blinkClock.restart();
            }

            if (input_fields == 1)
            {
                FloatRect bounds = emailtext.getGlobalBounds();
                float x = email.empty() ? emailtext.getPosition().x : (bounds.position.x + bounds.size.x);
                cursor.setPosition({x, emailtext.getPosition().y + 2.f});
            }
            else if (input_fields == 2)
            {
                FloatRect bounds = passwordtext.getGlobalBounds();
                float x = password.empty() ? passwordtext.getPosition().x : (bounds.position.x + bounds.size.x);
                cursor.setPosition({x, passwordtext.getPosition().y + 2.f});
            }
        }

        window.clear(lightGreyColor);
        window.draw(cafeTitle);
        window.draw(welcomeSubtitle);

        window.draw(emailbox);
        window.draw(emaillable);
        window.draw(emailtext);

        window.draw(passwordbox);
        window.draw(passwordlable);
        window.draw(passwordtext);

        if (input_fields != 0 && showCursor)
        {
            window.draw(cursor);
        }

        window.draw(signinButton);
        window.draw(statusMessage);
        window.draw(registerText);
        window.display();
    }

    return 0;
}