#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
using namespace std;
using namespace sf;

bool isAlreadyRegistered(const string &name, const string &email, const string &role, const string &password)
{
    ifstream file("users.txt");

    if (!file.is_open())
    {
        return false;
    }

    string line;
    while (getline(file, line))
    {
        stringstream ss(line);
        string storedName, storedEmail, storedRole, storedPassword;

        getline(ss, storedName, ',');
        getline(ss, storedEmail, ',');
        getline(ss, storedRole, ',');
        getline(ss, storedPassword, ',');

        if (storedName == name && storedEmail == email && storedRole == role && storedPassword == password)
        {
            file.close();
            return true;
        }
    }

    file.close();
    return false;
}

void saveRegistration(string name, string email, string role, string password)
{
    ofstream file("users.txt", ios::app);

    if (file.is_open())
    {
        file << name << "," << email << "," << role << "," << password << endl;

        file.close();

        cout << "Registration saved successfully!" << endl;
    }
    else
    {
        cout << "Unable to open file!" << endl;
    }
}

int openRegistrationScreen()
{
    RenderWindow window(VideoMode({800, 800}), "Cafe Management System - Registration");
    window.setFramerateLimit(60);

    Color lightGreyColor(211, 211, 211);

    Font font("Arial.ttf");

    Text cafeTitle(font, "BREW & BITE CAFE", 38);
    cafeTitle.setPosition({195.f, 25.f});
    cafeTitle.setFillColor(Color::Black);
    cafeTitle.setStyle(Text::Bold);

    Text crewSubtitle(font, "foodie are you ready to join our cafe crew by registering here...", 19);
    crewSubtitle.setPosition({110.f, 74.f});
    crewSubtitle.setFillColor(Color::Black);
    crewSubtitle.setStyle(Text::Italic);

    RectangleShape namebox({580.f, 46.f});
    RectangleShape emailbox({580.f, 46.f});
    RectangleShape rolebox({580.f, 46.f});
    RectangleShape passwordbox({580.f, 46.f});
    RectangleShape registerButton({240.f, 50.f});

    namebox.setFillColor(Color::White);
    emailbox.setFillColor(Color::White);
    rolebox.setFillColor(Color::White);
    passwordbox.setFillColor(Color::White);
    registerButton.setFillColor(Color::White);

    namebox.setPosition({110.f, 148.f});
    emailbox.setPosition({110.f, 245.f});
    rolebox.setPosition({110.f, 342.f});
    passwordbox.setPosition({110.f, 515.f});
    registerButton.setPosition({280.f, 595.f});

    namebox.setOutlineThickness(2.f);
    emailbox.setOutlineThickness(2.f);
    rolebox.setOutlineThickness(2.f);
    passwordbox.setOutlineThickness(2.f);
    registerButton.setOutlineThickness(2.f);

    namebox.setOutlineColor(Color::Black);
    emailbox.setOutlineColor(Color::Black);
    rolebox.setOutlineColor(Color::Black);
    passwordbox.setOutlineColor(Color::Black);
    registerButton.setOutlineColor(Color::Black);

    Text nametext(font, "", 20);
    nametext.setFillColor(Color::Black);
    nametext.setPosition({124.f, 158.f});

    Text emailtext(font, "", 20);
    emailtext.setFillColor(Color::Black);
    emailtext.setPosition({124.f, 255.f});

    Text roletext(font, "Select Role", 20);
    roletext.setFillColor(Color::Black);
    roletext.setPosition({124.f, 352.f});

    RectangleShape adminoption({580.f, 40.f});
    RectangleShape salesmanoption({580.f, 40.f});

    adminoption.setFillColor(Color::White);
    salesmanoption.setFillColor(Color::White);

    adminoption.setPosition({110.f, 388.f});
    salesmanoption.setPosition({110.f, 428.f});

    adminoption.setOutlineThickness(1.f);
    salesmanoption.setOutlineThickness(1.f);

    adminoption.setOutlineColor(Color::Black);
    salesmanoption.setOutlineColor(Color::Black);

    Text passwordtext(font, "", 20);
    passwordtext.setFillColor(Color::Black);
    passwordtext.setPosition({124.f, 525.f});

    Text namelabel(font, "Name:", 20);
    namelabel.setFillColor(Color::Black);
    namelabel.setPosition({110.f, 115.f});

    Text emaillabel(font, "E-MAIL:", 20);
    emaillabel.setFillColor(Color::Black);
    emaillabel.setPosition({110.f, 212.f});

    Text rolelabel(font, "Role:", 20);
    rolelabel.setFillColor(Color::Black);
    rolelabel.setPosition({110.f, 309.f});

    Text passwordlabel(font, "Password:", 20);
    passwordlabel.setFillColor(Color::Black);
    passwordlabel.setPosition({110.f, 482.f});

    Text adminText(font, "Admin", 20);
    adminText.setPosition({124.f, 396.f});
    adminText.setFillColor(Color::Black);

    Text salesmanText(font, "Salesman", 20);
    salesmanText.setPosition({124.f, 436.f});
    salesmanText.setFillColor(Color::Black);

    Text statusMessage(font, "", 18);
    statusMessage.setFillColor(Color::Red);
    statusMessage.setPosition({270.f, 650.f});

    Text backToLogin(font, "Already have an account? Sign In", 20);
    backToLogin.setFillColor(Color::Blue);
    backToLogin.setStyle(Text::Underlined);
    backToLogin.setPosition({245.f, 682.f});

    Text registerLabel(font, "REGISTER", 24);
    registerLabel.setFillColor(Color::Black);
    registerLabel.setPosition({345.f, 605.f});

    // Blinking cursor setup
    RectangleShape cursor({2.f, 24.f});
    cursor.setFillColor(Color::Black);
    Clock blinkClock;
    float blinkInterval = 0.5f;
    bool showCursor = true;

    string name = "";
    string email = "";
    string password = "";
    string role = "";

    int input_fields = 0; // 1 = name, 2 = email, 3 = password
    bool drop = false;

    while (window.isOpen())
    {
        while (optional event = window.pollEvent())
        {
            if (event->is<Event::Closed>())
            {
                window.close();
                return 0;
            }

            if (const auto *textEvent = event->getIf<Event::TextEntered>())
            {
                if (!drop && input_fields != 0)
                {
                    if (textEvent->unicode == 8)
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
                    else if (textEvent->unicode == 13)
                    {
                        input_fields = 0;
                    }
                    else if (textEvent->unicode >= 32 && textEvent->unicode < 128)
                    {
                        char character = static_cast<char>(textEvent->unicode);

                        if (input_fields == 1)
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

                    // Keep cursor visible while typing
                    showCursor = true;
                    blinkClock.restart();
                }
            }

            if (const auto *mouseEvent = event->getIf<Event::MouseButtonPressed>())
            {
                if (mouseEvent->button == Mouse::Button::Left)
                {
                    Vector2f mousePos = window.mapPixelToCoords(mouseEvent->position);

                    if (namebox.getGlobalBounds().contains(mousePos))
                    {
                        input_fields = 1;
                        drop = false;
                        showCursor = true;
                        blinkClock.restart();
                    }
                    else if (emailbox.getGlobalBounds().contains(mousePos))
                    {
                        input_fields = 2;
                        drop = false;
                        showCursor = true;
                        blinkClock.restart();
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
                        showCursor = true;
                        blinkClock.restart();
                    }
                    else if (backToLogin.getGlobalBounds().contains(mousePos))
                    {
                        window.close();
                        return 1;
                    }
                    else if (registerButton.getGlobalBounds().contains(mousePos))
                    {
                        if (!name.empty() && !email.empty() &&
                            !role.empty() && !password.empty())
                        {
                            if (isAlreadyRegistered(name, email, role, password))
                            {
                                cout << "You are already registered!" << endl;
                                statusMessage.setString("You are already registered!");
                            }
                            else
                            {
                                saveRegistration(name, email, role, password);

                                window.close();
                                return 1;
                            }
                        }
                        else
                        {
                            cout << "Please fill all fields!" << endl;
                            statusMessage.setString("Please fill all fields!");
                        }
                    }
                    else
                    {
                        input_fields = 0;
                        drop = false;
                    }
                }
            }
        }

        // Blink and move cursor in the active text field
        if (input_fields != 0 && !drop)
        {
            if (blinkClock.getElapsedTime().asSeconds() >= blinkInterval)
            {
                showCursor = !showCursor;
                blinkClock.restart();
            }

            if (input_fields == 1)
            {
                FloatRect bounds = nametext.getGlobalBounds();
                float x = name.empty() ? nametext.getPosition().x : (bounds.position.x + bounds.size.x);
                cursor.setPosition({x, nametext.getPosition().y + 2.f});
            }
            else if (input_fields == 2)
            {
                FloatRect bounds = emailtext.getGlobalBounds();
                float x = email.empty() ? emailtext.getPosition().x : (bounds.position.x + bounds.size.x);
                cursor.setPosition({x, emailtext.getPosition().y + 2.f});
            }
            else if (input_fields == 3)
            {
                FloatRect bounds = passwordtext.getGlobalBounds();
                float x = password.empty() ? passwordtext.getPosition().x : (bounds.position.x + bounds.size.x);
                cursor.setPosition({x, passwordtext.getPosition().y + 2.f});
            }
        }

        window.clear(lightGreyColor);

        window.draw(cafeTitle);
        window.draw(crewSubtitle);

        // Draw boxes and labels first
        window.draw(namelabel);
        window.draw(emaillabel);
        window.draw(rolelabel);
        window.draw(passwordlabel);

        window.draw(namebox);
        window.draw(emailbox);
        window.draw(rolebox);
        window.draw(passwordbox);
        window.draw(registerButton);

        // Draw text inside boxes
        window.draw(nametext);
        window.draw(emailtext);
        window.draw(roletext);
        window.draw(passwordtext);

        // Draw blinking cursor in active field
        if (input_fields != 0 && !drop && showCursor)
        {
            window.draw(cursor);
        }

        // Draw dropdown options on top when open
        if (drop)
        {
            window.draw(adminoption);
            window.draw(salesmanoption);
            window.draw(adminText);
            window.draw(salesmanText);
        }

        window.draw(registerLabel);
        window.draw(statusMessage);
        window.draw(backToLogin);
        window.display();
    }

    return 0;
}