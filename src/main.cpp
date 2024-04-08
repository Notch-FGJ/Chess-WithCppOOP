#include <iostream>
#include <windows.h>

std::ostream &operator<<(std::ostream &os, WCHAR c)
{
    DWORD n;
    WriteConsoleW(GetStdHandle(STD_OUTPUT_HANDLE), &c, 1, &n, NULL);
    return os;
}

std::ostream &operator<<(std::ostream &os, const WCHAR *c)
{
    DWORD n;
    WriteConsoleW(GetStdHandle(STD_OUTPUT_HANDLE), c, lstrlenW(c), &n, NULL);
    return os;
}

int main()
{
    std::cout << L'\u2659' << std::endl;
    std::cout << L"\u2659\u2659\u2659" << std::endl;
    return 0;
}