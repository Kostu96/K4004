#include "app.hpp"

int main()
{
    App app;
    if (app.Construct(688, 387, 2, 2, false, true))
        app.Start();
    else
        return -1;

    return 0;
}
