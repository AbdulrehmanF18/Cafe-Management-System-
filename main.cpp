#include "signin.cpp"
#include "registertion.cpp"
#include "3rd_page_project.cpp"
#include "main_control_box.cpp"

int main()
{
    while (true)
    {
        int signInResult = openSignInScreen();

        if (signInResult == 0)
        {
            break;
        }

        if (signInResult == 1)
        {
            int registerResult = openRegistrationScreen();

            if (registerResult == 0)
            {
                break;
            }

            if (registerResult == 1)
            {
                continue;
            }
        }

        if (signInResult == 2)
        {
            int dashboardResult = openThirdScreen();

            if (dashboardResult == 3)
            {
                openMainControlBox();
            }

            break;
        }
    }

    return 0;
}
