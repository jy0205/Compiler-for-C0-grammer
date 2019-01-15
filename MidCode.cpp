#include <iostream>
#include <string.h>
#include <stdio.h>
#include "global.h"
#include "errorInfo.h"

MidCode MidCodeSet[MAXMIDCODE];
TempVarTable tempTable[MAXMIDCODE];
int codeCount = 0;
int varNum = 0;
int labelNum = 0;
int tempNum = 0;

/*
1 const int/char a 10
2 var int/char name
3 array int/char name size
4 para int/char name
5 int/char functionName ()
6 void funcionName ()
7 ret
8 end
9 label src1   src1为0时跳转
10 label :
11 label src1 < src2
12 label src1 > src2
13 label src1 <= src2
14 label src1 >= src2
15 label src1 == src2
16 label src1 != src2    不满足上述条件的话跳转
17 goto label
18 call void functionName
19 call value functionName
20 push x
21 result = src1
22 result[src1] = src2
23 scanf x
24 printf x y
25 ret x
26 result = x op y  op = + - * /
27 result = src1[src2]
28 x = ret
*/

void GenMidCode(int codekind,char result[],char src1[],char op[],char src2[]){
    if(codeCount > MAXMIDCODE){
        //记得报错
        return;
    }
    MidCodeSet[codeCount].codeKind = codekind;
    MidCodeSet[codeCount].effect = true;
    MidCodeSet[codeCount].enter = false;
    strcpy(MidCodeSet[codeCount].op,op);
    strcpy(MidCodeSet[codeCount].result,result);
    strcpy(MidCodeSet[codeCount].src1,src1);
    strcpy(MidCodeSet[codeCount].src2,src2);
    codeCount++;
}

void addTempVar(char name[],int type){
    strcpy(tempTable[tempNum].tempvarName,name);
    tempTable[tempNum].type = type;
    tempNum++;
}

void genTempVar(char varName[]){
    sprintf(varName,"$t%d",varNum);
    varNum++;
}

void genLabel(char labelName[]){
    sprintf(labelName,"$label%d",labelNum);
    labelNum++;
}

void printMidCode(FILE* out){
    for(int i = 0;i < codeCount;i++){
        if(!MidCodeSet[i].effect)
            continue;
        switch(MidCodeSet[i].codeKind){
            case 1:{
                fprintf(out,"%s %s %s = %s \n", MidCodeSet[i].result,MidCodeSet[i].src1,MidCodeSet[i].op,MidCodeSet[i].src2);
                break;
            }
            case 2:{
                fprintf(out,"%s %s %s\n", MidCodeSet[i].result,MidCodeSet[i].src1,MidCodeSet[i].op);
                break;
            }
            case 3:{
                fprintf(out,"%s %s %s [%s]\n", MidCodeSet[i].result,MidCodeSet[i].src1,MidCodeSet[i].op,MidCodeSet[i].src2);
                break;
            }
            case 4:{
                fprintf(out,"%s %s %s\n", MidCodeSet[i].result,MidCodeSet[i].src1,MidCodeSet[i].op);
                break;
            }
            case 5:{
                fprintf(out,"%s %s%s%s\n", MidCodeSet[i].result,MidCodeSet[i].src1,MidCodeSet[i].op,MidCodeSet[i].src2);
                break;
            }
            case 6:{
                fprintf(out,"%s %s%s%s\n", MidCodeSet[i].result,MidCodeSet[i].src1,MidCodeSet[i].op,MidCodeSet[i].src2);
                break;
            }
            case 7:{
                fprintf(out,"%s\n", MidCodeSet[i].result);
                break;
            }
            case 8:{
                fprintf(out,"%s\n", MidCodeSet[i].result);
                break;
            }
            case 9:{
                fprintf(out,"%s != 0\n", MidCodeSet[i].src1);
                fprintf(out,"BZ %s\n", MidCodeSet[i].result);
                break;
            }
            case 10:{
                fprintf(out,"%s :\n", MidCodeSet[i].result);
                break;
            }
            case 11:
            case 12:
            case 13:
            case 14:
            case 15:
            case 16:{
                fprintf(out,"%s %s %s\n", MidCodeSet[i].src1,MidCodeSet[i].op,MidCodeSet[i].src2);
                fprintf(out,"BZ %s\n", MidCodeSet[i].result);
                break;
            }
            case 17:{
                fprintf(out,"%s %s\n", MidCodeSet[i].result,MidCodeSet[i].src1);
                break;
            }
            case 18:
            case 19:{
                fprintf(out,"%s %s\n", MidCodeSet[i].result,MidCodeSet[i].op);
                break;
            }
            case 20:{
                fprintf(out,"%s %s\n", MidCodeSet[i].result,MidCodeSet[i].src1);
                break;
            }
            case 21:{
                fprintf(out,"%s = %s\n", MidCodeSet[i].result,MidCodeSet[i].src1);
                break;
            }
            case 22:{
                fprintf(out,"%s[%s] = %s\n", MidCodeSet[i].result,MidCodeSet[i].src1,MidCodeSet[i].op);
                break;
            }
            case 23:{
                fprintf(out,"%s %s\n", MidCodeSet[i].result,MidCodeSet[i].src1);
                break;
            }
            case 24:{
                fprintf(out,"%s %s %s\n", MidCodeSet[i].result,MidCodeSet[i].src1,MidCodeSet[i].op);
                break;
            }
            case 25:{
                fprintf(out,"%s %s\n", MidCodeSet[i].result,MidCodeSet[i].src1);
                break;
            }
            case 26:{
                fprintf(out,"%s = %s %s %s\n", MidCodeSet[i].result,MidCodeSet[i].src1,MidCodeSet[i].op,MidCodeSet[i].src2);
                break;
            }
            case 27:{
                fprintf(out,"%s = %s[%s]\n", MidCodeSet[i].result,MidCodeSet[i].src1,MidCodeSet[i].op);
                break;
            }
            case 28:{
                fprintf(out,"%s %s %s\n", MidCodeSet[i].result,MidCodeSet[i].src1,MidCodeSet[i].op);
                break;
            }

        }
    }
}
