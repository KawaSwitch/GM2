#pragma once
#include <iostream>
using namespace std;

class Error
{
private:
    static void Exit()
    {
        cerr << "ƒvƒƒOƒ‰ƒ€‚ðI—¹‚µ‚Ü‚·" << endl;
        system("pause>nul");
        exit(1);
    }

public:
    Error() = delete;

public:

    static void ShowMessage(const char* error_name, const char* detail = NULL)
    {
        cerr << "an error has occured : " << error_name << endl;
        if (detail != NULL) cerr << "error detail : " << detail << endl << endl;
    }

    static void ShowAndExit(const char* error_name, const char* detail = NULL)
    {
        ShowMessage(error_name, detail);
        Exit();
    }
};
