#include <iostream>
#include <stdio.h>
#include <string>
#include "global.h"
#include "errorInfo.h"

FILE* fin;   //原文件的指针
FILE* mipsOut;
FILE* midOut;
FILE* NewMips;
FILE* NewmidOut;
bool compileResult = true;

int main()
{
    string filename;
    cin >> filename;
    fin = fopen(filename.c_str(),"r");
    mipsOut = fopen("MyMips.asm","w");
    midOut = fopen("MidCode.txt","w");
    NewmidOut = fopen("NewMidCode.txt","w");
    NewMips = fopen("NewMips.asm","w");
    if(fin == NULL){
        error(FILE_ERROR,0);
    }
    else{
        getch();
        getsym();
        program();
        if(compileResult){
            cout << "Compile Successfully !" << endl;
            printMidCode(midOut);       //优化前的中间代码
            genMipsCode();              //优化前的mips
            GenNewMips();               //优化后的mips
            printMidCode(NewmidOut);    //优化后的中间代码
            //test();
        }
        else
            cout << "Compile Fail,Please modify your errors!" << endl;
    }
    return 0;
}

