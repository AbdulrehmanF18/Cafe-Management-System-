#include "signin.cpp"
#include "registertion.cpp"

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
    }

    return 0;
}
