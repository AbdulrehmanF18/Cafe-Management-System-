#include "signin.cpp"
#include "registertion.cpp"
#include "3rd_page_project.cpp"
#include "shared_data.cpp"
#include "reports.cpp"
#include "Price&Billing/Price&Billing/Price&Billing.cpp"
#include "Price&Billing/Price&Billing/GUI_for_Salesman.cpp"

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
            // keep showing the dashboard until the user closes it
            while (true)
            {
                int dashboardResult = openThirdScreen();

                if (dashboardResult == 3)
                {
                    if (openReportsPage() == 0) // 0 = window closed, 1 = Back
                        break;
                }
                else if (dashboardResult == 2)
                {
                    SalesSection(); // returns here when the sales window closes
                }
                else
                {
                    break; // dashboard closed
                }
            }

            break;
        }
    }

    return 0;
}
