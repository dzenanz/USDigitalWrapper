#include "SEIDrv.h"
#include <Windows.h>

#include <chrono>
#include <iostream>

int main()
{
    enumerateEncodersAll();

    std::cout << "A few seconds of callbacks\n";
    Sleep(5000);
    removeCallbacks(); // otherwise the application does not shut down

    std::cout << "\nA few seconds of polling\n";
    auto startTime = std::chrono::system_clock::now();
    while (std::chrono::system_clock::now() - startTime < std::chrono::seconds(5))
    {
        printUpdatedPositions();
        Sleep(25);
    }

    return 0;
}
