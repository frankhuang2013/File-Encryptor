using namespace std;
#include <iostream>
#include <fstream>
#include <iomanip>
#include <conio.h>
#include "BinaryData.h"

int main()
{
    //variables
    char usrOption = 0;
    string password = "";
    string fileIdentifier = "";
    string fileName = "";
    string fileVersion = "";
    string path = "";

    //intro:
    cout << "...................................." << endl;
    cout << ".    Welcome to FILE ENCRYPTOR     ." << endl;
    cout << ".               MENU               ." << endl;
    cout << ".        1. Encrypt a file         ." << endl;
    cout << ".        2. Decrypt a file         ." << endl;
    cout << ".        3. Quit                   ." << endl;
    cout << "...................................." << endl;
    cout << endl;
    cout << "Please enter an option:";
    usrOption = getch();
    cout << endl << endl << endl;

    if (usrOption == '1')
    {
        cout << "   ==========Encrypt A File==========   " << endl;
        cout << "Enter the password (or hit RETURN if no need): ";
        getline(cin, password);
        cout << endl;
        cout << "Enter the file identifier (or hit RETURN if no need): ";
        getline(cin, fileIdentifier);
        cout << endl;
        cout << "Enter the file name: ";
        getline(cin, fileName);
        while (fileName.length() == 0)
        {
            cout << "A file must have a name! Try again!" << endl;
            cout << "Enter the file name: ";
            getline(cin, fileName);
        }
        cout << endl;
        cout << "Enter the file version (or hit RETURN if no need): ";
        getline(cin, fileVersion);
        cout << endl;
        cout << "Enter the name (or path) of the encrypted file: ";
        getline(cin, path);
        while (path.length() == 0)
        {
            cout << "A file must have a name! Try again!" << endl;
            cout << "Enter the file name: ";
            getline(cin, path);
        }
        cout << endl;
        BinaryData data1(password,fileIdentifier,fileName,fileVersion);
        data1.write(fileName, path);
        cout << "All work has been done!" << endl;

        return 0;
    }
    else if (usrOption == '2')
    {
        cout << "   ==========Decrypt A File==========   " << endl;
        cout << "Enter the name (or path) of the file: ";
        getline(cin, path);
        while (path.length() == 0)
        {
            cout << "A file must have a name! Try again!" << endl;
            cout << "Enter the file name: ";
            getline(cin, path);
        }
        BinaryData data1;
        data1.read(path);
    }
    else if (usrOption == '3')
    {
        return 0;
    }
    else
    {
        cout << "Bad Command!" << endl;
        return -1;
    }


/*
    string a = "";
    string b = "";
    string c = "encrypted.png";
    string d = "";
    string e = "";
    BinaryData data1(a,b,c,d);

    cout << "Password: " << data1.getPw() << "  " << data1.hasPassword() << endl;
    cout << "fi: " << data1.getFi() << "  " << data1.hasFileIdentifier() << endl;
    cout << "fn: " << data1.getFn() << "  " << data1.hasFileName() << endl;
    cout << "fv: " << data1.getFv() << "  " << data1.hasFileVersion() << endl;

    cout << endl;
    ///data1.write("source.png", "result.png");
    data1.read("result.png");

    return 0;*/
}
