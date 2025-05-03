#include <iostream>

#ifdef _WIN32
#include <conio.h>
#else
#include <termios.h>
#include <unistd.h>

char getch()
{
    termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    char c = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return c;
}
#endif

char getKeyboardInput()
{
#ifdef _WIN32
    return _getch();
#else
    return getch();
#endif
}

int getAction()
{
    while (true)
    {
        char input = getKeyboardInput();
        switch (input)
        {
        case 'n':
            return 0; // do nothing
        case 'w':
            return 1; // Move up
        case 'a':
            return 2; // Move left
        case 's':
            return 3; // Move right
        case 'd':
            return 4; // Move down
        case 'x':
            std::cout << "Game manually exited." << std::endl;
            exit(0);
        default:
            continue; // Invalid input, continue to wait for valid input
        }
    }
    return 0;
}