#include "signin.cpp"
#include "registertion.cpp"
#include "3rd_page_project.cpp"
#include "shared_data.cpp"
#include "reports.cpp"
#include "Price&Billing.cpp"

int main()
{
    load_date();

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
                openReportsPage();
            }
            else if (dashboardResult == 2)
            {
                SalesSection();
            }

            break;
        }
    }

    return 0;
}
