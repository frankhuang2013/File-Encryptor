/*
==================BinaryData.h==================

Author: Frank Huang
Developed since: 08/05/2016
Finished: Unfinished
Version: Alpha 5.0
Introduction: Providing a method to store file
              or data in a special way securely.

==================BinaryData.h==================

===================Update Log===================

------------------------------------------------
Alpha 0.1:
1. Consider the main idea of this project.
2. Establish the project and name the project.
3. finish the main frame of the class.
4. Set up the intro of this file and the update
   log.
------------------------------------------------
Alpha 1.0:
1. Fixed some syntax errors.
2. Set up new getters and new variables.
3. wrote the constructor function.
------------------------------------------------
Alpha 2.0:
1. UseD ctime functions and rand() to generate
   key.
2. Tried to write function "write" to test
   creating binary file and record some
   characters to the file.
3. Fixed some bugs, made sure main function can
   use the header file successfully without
   memory leak.
------------------------------------------------
Alpha 2.1:
1. Continued doing function "write", write all
   other info to the binary file.
------------------------------------------------
Alpha 3.0:
1. Fixed bugs in function "write".
2. Created a new function "blockize".
3. Continued writing function "write" and
   "read".
4. Created a new structure "DataBlock".
------------------------------------------------
Alpha 3.1:
1. Finished the problem of "last data block"
   that has less than 3 elements.
2. Continues writing function: "write".
------------------------------------------------
Alpha 3.2:
1. Continued writing function: "read".
------------------------------------------------
Alpha 4.0:
1. Basically finished writing functions "read"
   and "write".
2. The program successfully read and write a
   file(txt or mp3 or bmp) the first time, but
   with a small problem: the last several bytes
   could not been read correctly.
3. This project should be soon goes to Beta
   procedure as expected.
------------------------------------------------
Alpha 5.0:
1. The biggest problem "remainder1 bug" finally
   had been resolved. The reason caused this bug
   is that the variable remainder1 was redefined
   in member function read besides class
   declaration. The compiler was confused about
   which variable to use.
2. The UI (main function) was roughly done, but
   needs to be improved.
3. Fixed some bugs in function "read".
------------------------------------------------

===================Update Log===================
*/




#ifndef BINARYDATA_H
#define BINARYDATA_H
#include <cstring>
#include <fstream>
#include <ctime>
#include <cstdlib>


struct DataBlock
{
    char block3 = 0;
    char block2 = 0;
    char block1 = 0;
    char block0 = 0;
};

//==============CLASS DEFINITION==============
class BinaryData
{
private:
    //constants:
    static const int KEY_LEN = 129; // key length: 1024 bits encryption

    //length:
    unsigned int pwLen = 0; //password length
    unsigned int fiLen = 0; //file identifier length
    unsigned int fnLen = 0; //file name length
    unsigned int fvLen = 0; //file version length
    unsigned long long dLen = 0; //data length
    unsigned long long bLen = 0; //block length
    unsigned int remainder = 0;

    //info:
    char key[KEY_LEN]; //key: 128 bytes
    char* pw; //password
    char* fi; //file identifier
    char* fn; //file name
    char* fv; //file version
    DataBlock* d; //data

    //other:
    ///string sourceFile = ""; //save file name to be read
    ///string destinationFile = ""; //save file name to be written
    ///char bufferByte = 0;
    unsigned int seed = 0;

    //switchers:
    bool hasPw = false;
    bool hasFi = false;
    bool hasFn = false;
    bool hasFv = false;
    bool hasFl = false;

    //private functions:
    int keyEncpt();
    bool blockize(string);

public:
    //constructors:
    BinaryData(); //null object
    BinaryData(string,string,string,string);//tend-to-write constructor (pw,fi,fn,fv)

    //getters
    char* getPw() {return pw;}
    char* getFi() {return fi;}
    char* getFn() {return fn;}
    char* getFv() {return fv;}
    bool hasPassword() {return hasPw;}
    bool hasFileIdentifier() {return hasFi;}
    bool hasFileName() {return hasFn;}
    bool hasFileVersion() {return hasFv;}

    //featured functions
    bool write(string,string); //write from file name to file name
    bool read(string);
};
//==============CLASS DEFINITION END==============




//==============FUNCTION DEFINITION==============
BinaryData::BinaryData()
{

}

BinaryData::BinaryData(string _pw ,string _fi, string _fn ,string _fv)
{
    //distribute Key
    seed = time(0);
    srand(seed);
    for(int i = 0; i < 888; i++)
    {
        rand();
    }
    for (int i = 0; i < KEY_LEN - 1; i++)
    {
        key[i] = rand();
        if (key[i] == '\0')
        {
            i = i - 1;
        }
    }
    key[128] = '\0';
    ///cout << "KEY_LEN = " << KEY_LEN << endl;
    ///cout << "length of key: " << strlen(key) << endl;

    //obtain the length of each element:
    pwLen = _pw.length();
    fiLen = _fi.length();
    fnLen = _fn.length();
    fvLen = _fv.length();
    ///location = _fl;

    //apply for spaces:
    pw = new char [pwLen+1];
    fi = new char [fiLen+1];
    fn = new char [fnLen+1];
    fv = new char [fvLen+1];

    //store elements:
    strcpy(pw, _pw.c_str());
    strcpy(fi, _fi.c_str());
    strcpy(fn, _fn.c_str());
    strcpy(fv, _fv.c_str());

    //turn on switchers
    hasPw = true;
    hasFi = true;
    hasFn = true;
    hasFv = true;
}

bool BinaryData::write(string sourceFile, string destinationFile)
{
    //variables:
    char* ptr = 0;
    char buffer = 0;
    ///int remainder = 0;

    //prepare data structure:
    if (!blockize(sourceFile)) //execute the function and also test it
    {
cout << "returned false" << endl;
        return false;
    }
cout << "File Length is: " << dLen << endl;
cout << "Block Length is: " << bLen << endl;

    //create the file:
    fstream fout;
    fout.open(destinationFile, ios::out|ios::binary);

    //write the key into the file:
    fout.write(key, strlen(key));

    //write password structure:
    ptr = (char*)&pwLen;;
    for (int i = 0; i < 4; i++)
    {
        buffer = ptr[i];
        buffer += keyEncpt();
        fout.write(&buffer, 1);
        cout << "ok1" << endl;
    }
    ptr = pw;
    for (int i = 0; i < pwLen; i++)
    {
        buffer = ptr[i];
        buffer += keyEncpt();
        fout.write(&buffer, 1);
        cout << "ok2" << endl;
    }

    //write file identifier structure:
    ptr = (char*)&fiLen;;
    for (int i = 0; i < 4; i++)
    {
        buffer = ptr[i];
        buffer += keyEncpt();
        fout.write(&buffer, 1);
        cout << "ok3" << endl;
    }
    ptr = fi;
    for (int i = 0; i < fiLen; i++)
    {
        buffer = ptr[i];
        buffer += keyEncpt();
        fout.write(&buffer, 1);
        cout << "ok4" << endl;
    }

    //write file name structure:
    ptr = (char*)&fnLen;;
    for (int i = 0; i < 4; i++)
    {
        buffer = ptr[i];
        buffer += keyEncpt();
        fout.write(&buffer, 1);
        cout << "ok5" << endl;
    }
    ptr = fn;
    for (int i = 0; i < fnLen; i++)
    {
        buffer = ptr[i];
        buffer += keyEncpt();
        fout.write(&buffer, 1);
        cout << "ok6" << endl;
    }

    //write file version structure:
    ptr = (char*)&fvLen;;
    for (int i = 0; i < 4; i++)
    {
        buffer = ptr[i];
        buffer += keyEncpt();
        fout.write(&buffer, 1);
        cout << "ok7" << endl;
    }
    ptr = fv;
    for (int i = 0; i < fvLen; i++)
    {
        buffer = ptr[i];
        buffer += keyEncpt();
        fout.write(&buffer, 1);
        cout << "ok8" << endl;
    }

    //write data structure:
    ptr = (char*)&dLen;
    for (int i = 0; i < 8; i++)
    {
        buffer = ptr[i];
        buffer += keyEncpt();
        fout.write(&buffer, 1);
        cout << "ok9" << endl;
    }
    for (int i = 0; i < bLen - 1; i++)
    {
        fout.write((char*)&d[i], sizeof(d[i]));
    }
    if (remainder == 0)
    {
        fout.write((char*)&d[bLen - 1], sizeof(d[bLen - 1]));
    }
    else if (remainder == 1)
    {
        fout.write(&(d[bLen - 1].block1), 1); //go to blockize() for reference
    }
    else if (remainder == 2)
    {
        fout.write(&(d[bLen - 1].block2), 1); //go to blockize() for reference
        fout.write(&(d[bLen - 1].block1), 1); //go to blockize() for reference
    }
cout << "ok10" << endl;
cout << d[bLen - 1].block0 << endl;
cout << d[bLen - 1].block1 << endl;
cout << d[bLen - 1].block2 << endl;
cout << d[bLen - 1].block3 << endl;

    //release data memory
    delete [] d;

    //close the file:
    fout.close();

    return true;
}


bool BinaryData::read(string fileName)
{
    //variables:
    char* ptr = 0;
    char buffer = 0;
    char remainder1 = 0;
    char remainder2 = 0;

    //open file to read:
    fstream fin;
    fin.open(fileName, ios::in|ios::binary);

    //read the key
    for (int i = 0; i < 128; i++)
    {
        fin.read(&buffer, 1);
        key[i] = buffer;
        ///cout << key[i];
    }

    //read password structure:
    ptr = new char [4];
    fin.read(ptr, 4);
    for (int i = 0; i < 4; i++) //decrypt
    {
        ptr[i] -= keyEncpt();
    }
    pwLen = *((unsigned int*)ptr); //convert pointer array to int
cout << pwLen << endl;
    delete [] ptr;
    ///ptr = new char [pwLen];
    pw = new char [pwLen];
    fin.read(pw, pwLen);
    for (int i = 0; i < pwLen; i++) //decrypt
    {
        pw[i] -= keyEncpt();
    }
cout << pw << endl;

    //read file identifier file:
    ptr = new char [4];
    fin.read(ptr, 4);
    for (int i = 0; i < 4; i++) //decrypt
    {
        ptr[i] -= keyEncpt();
    }
    fiLen = *((unsigned int*)ptr);
    cout << fiLen << endl;
    delete [] ptr;
    fi = new char [fiLen];
    fin.read(fi, fiLen);
    for (int i = 0; i < fiLen; i++) //decrypt
    {
        fi[i] -= keyEncpt();
    }
    cout << fi << endl;

    //read file name:
    ptr = new char [4];
    fin.read(ptr, 4);
    for (int i = 0; i < 4; i++) //decrypt
    {
        ptr[i] -= keyEncpt();
    }
    fnLen = *((unsigned int*)ptr);
    cout << fnLen << endl;
    delete [] ptr;
    fn = new char [fnLen];
    fin.read(fn, fnLen);
    for (int i = 0; i < fnLen; i++) //decrypt
    {
        fn[i] -= keyEncpt();
    }
    cout << fn << endl;


    //read file version:
    ptr = new char [4];
    fin.read(ptr, 4);
    for (int i = 0; i < 4; i++) //decrypt
    {
        ptr[i] -= keyEncpt();
    }
    fvLen = *((unsigned int*)ptr);
    cout << fvLen << endl;
    delete [] ptr;
    fv = new char [fvLen];
    fin.read(fv, fvLen);
    for (int i = 0; i < fvLen; i++) //decrypt
    {
        fv[i] -= keyEncpt();
    }
    cout << fv << endl;


    //prepare to read data blocks:
    ptr = new char [8];
    fin.read(ptr, 8);
    for (int i = 0; i < 8; i++) //decrypt
    {
        ptr[i] -= keyEncpt();
    }
    dLen = *((unsigned long long*)ptr);
cout << dLen << endl;
    delete [] ptr;
    remainder = dLen % 3;
    bLen = dLen / 3;
    d = new DataBlock [bLen];
cout << bLen << endl;
cout << "remainder = " << remainder << endl;

    //read data blocks:
    for (int i = 0; i < bLen; i++)
    {
        fin.read((char*)&d[i], sizeof(d[i]));
///cout << d[i].block1 << d[i].block2 << d[i].block3 << endl;
    }

    for (int i = 0; i < bLen; i++) //decrypt
    {
        d[i].block1 -= keyEncpt();
        d[i].block2 -= keyEncpt();
        d[i].block3 -= keyEncpt();
    }

    if (remainder == 1)
    {
        fin.read(&remainder1, 1);
cout << "process if remainder=1" << endl;
cout << "remainder1 = " << (short)remainder1 << endl;
        remainder1 -= keyEncpt(); //decrypt
    }
    else if (remainder == 2)
    {
        fin.read(&remainder2, 1);
        fin.read(&remainder1, 1);
cout << "process if remainder=2" << endl;
cout << "remainder1 = " << remainder1 << endl;
cout << "remainder2 = " << remainder2 << endl;
        remainder1 -= keyEncpt(); //decrypt
        remainder2 -= keyEncpt(); //decrypt
    }

    //close the input file:
    fin.close();


    //open output file:
    fstream fout;
    fout.open(fn, ios::out|ios::binary);

    //write data blocks to the output file:
    for (int i = 0; i < bLen; i++)
    {
        fout.write(&(d[i].block1), 1);
        fout.write(&(d[i].block2), 1);
        fout.write(&(d[i].block3), 1);
    }
    if (remainder == 1)
    {
        fout.write(&remainder1, 1);
    }
    else if (remainder == 2)
    {
        fout.write(&remainder1, 1);
        fout.write(&remainder2, 1);
    }

    //close the output file:
    fout.close();


    return true;



}


int BinaryData::keyEncpt()
{
    int result = 888;


    return result;
}

bool BinaryData::blockize(string fileName)
{
    //prepare random byte:
    seed = time(0);
    srand(seed);
    for(int i = 0; i < 333; i++) //the value of i can be changed, the greater the better, but slower
    {
        rand();
    }

    //open file
    fstream fin;
    fin.open(fileName, ios::in|ios::binary);
    if(!fin) //fail to open the file
    {
        return false;
    }

    //get length of file:
    fin.seekg (0, fin.end); //put cursor to the end of the file
    dLen = fin.tellg(); //count file's size
    fin.seekg (0, fin.beg); //recover the cursor to the beginning of the file
    if (dLen == 0) //file can be open but empty
    {
        return false;
    }

    //get length of blocks:
    remainder = dLen % 3;
    if (remainder == 0)
    {
        bLen = dLen / 3;
    }
    else
    {
        bLen = dLen / 3 + 1;
    }

    //put bytes into blocks:
    d = new DataBlock [bLen];
    for (int i = 0; i < bLen - 1; i++)
    {
        do
        {
            d[i].block0 = rand();
        } while (d[i].block0 == '\0');
        fin.read(&(d[i].block1), 1);
        fin.read(&(d[i].block2), 1);
        fin.read(&(d[i].block3), 1);
    }
    for (int i = 0; i < bLen - 1; i++) //encrypt the data
    {
        d[i].block1 += keyEncpt();
        d[i].block2 += keyEncpt();
        d[i].block3 += keyEncpt();
    }


    //for the last block:
    do
    {
        d[bLen-1].block0 = rand();
    } while (d[bLen-1].block0 == '\0');


    if (remainder == 0)
    {
        fin.read(&(d[bLen-1].block1), 1);
        fin.read(&(d[bLen-1].block2), 1);
        fin.read(&(d[bLen-1].block3), 1);

        d[bLen-1].block1 += keyEncpt(); //encrypt the data
        d[bLen-1].block2 += keyEncpt(); //encrypt the data
        d[bLen-1].block3 += keyEncpt(); //encrypt the data
    }
    else if (remainder == 1)
    {
        fin.read(&(d[bLen-1].block1), 1);
        d[bLen-1].block2 = '\0';
        d[bLen-1].block3 = '\0';

        d[bLen-1].block1 += keyEncpt(); //encrypt the data
    }
    else if (remainder == 2)
    {
        fin.read(&(d[bLen-1].block1), 1);
        fin.read(&(d[bLen-1].block2), 1);
        d[bLen-1].block3 = '\0';

        d[bLen-1].block1 += keyEncpt(); //encrypt the data
        d[bLen-1].block2 += keyEncpt(); //encrypt the data
    }


    return true;
}
//==============FUNCTION DEFINITION END==============

#endif //BINARYDATA_H
