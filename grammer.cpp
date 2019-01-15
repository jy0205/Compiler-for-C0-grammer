#include <iostream>
#include <stdio.h>
#include <string.h>
#include "wordkind.h"
#include "global.h"
#include "errorInfo.h"

/*一些全局变量*/
char tempToken[100],valueStr[100];
char the_func_name[100];
int tempreadloc,templineNum,tempsymID;
char tempnowChar;
int sign,paraNum;
int kind,type,value,capicity,address = 0;    //表示符号表的一些信息
char varResult[100];
table Mytable;  //符号表
bool ifReturn;

/*函数声明如下*/
void saveInfo();
void recoverInfo();
void constDec();
void constDef();
void varDec();
void varDef();
void valueFuncDef();
void paraList();
void parameter();
void decHead();
void voidFuncDef();
void mainDef();
void compoundState();
void stateList();
void statement();
void ifState();
void condition(char label[]);
void loopState();
void voidFuncCall(int numOfPara);
void valueFuncCall(int numOfPara);
void valueParaList(int* valueParaNum,int func_index);
void assignState();
void readState();
void writeState();
void switchState();
void switchList(char result[],char label[]);
void caseState(char result[],char label[]);
void defaultState(char result[],char label[]);
void returnState();
void express(int* expType,int* expValue);
void term(int* termType,int* termValue);
void factor(int* factorType,int* factorValue);
bool isrelOp();
bool ifnext();
int calValue(char num[]);
bool ifnextstate();

void enterToTable(char name[],int kind,int type,int value,int capicity,int offset){
    if(Mytable.topIndex >= MAXNUM - 1){
        error(TABLE_OVERFLOW_ERROR,lineNum);
        return;
    }
    if(kind == 3){
        for(int i = 0;i < Mytable.numOfFunc;i++){
            int index = Mytable.funcIndex[i];
            if(strcmp(Mytable.element[index].name,name) == 0){
                error(SAME_FUNCTION_NAME_ERROR,lineNum);
                return;
            }
        }
        Mytable.funcIndex[Mytable.numOfFunc] = Mytable.topIndex + 1;
        Mytable.numOfFunc++;
    }
    else{
        if(Mytable.numOfFunc == 0){    //全局变量或常量
            for(int i = 0;i <= Mytable.topIndex;i++){
                if(strcmp(Mytable.element[i].name,name) == 0){
                    error(SAME_IDENTIFIER_NAME_ERROR,lineNum);
                    return;
                }
            }
        }
        else{         //在分程序中查找是否出现同名
            for(int i = Mytable.funcIndex[Mytable.numOfFunc - 1];i <= Mytable.topIndex;i++){
                if(strcmp(Mytable.element[i].name,name) == 0){
                    error(SAME_IDENTIFIER_NAME_ERROR,lineNum);
                    return;
                }
            }
        }
    }
    Mytable.topIndex++;
    strcpy(Mytable.element[Mytable.topIndex].name,name);
    Mytable.element[Mytable.topIndex].kind = kind;
    Mytable.element[Mytable.topIndex].type = type;
    Mytable.element[Mytable.topIndex].value = value;
    Mytable.element[Mytable.topIndex].capicity = capicity;
    Mytable.element[Mytable.topIndex].offset = offset;
    if(Mytable.numOfFunc == 0)
        Mytable.element[Mytable.topIndex].global = true;
    else
        Mytable.element[Mytable.topIndex].global = false;
}

int searchTable(char name[],bool ifFunction){    //查找符号表
    int index;
    if(ifFunction){
        for(int i = 0;i < Mytable.numOfFunc;i++){
            index = Mytable.funcIndex[i];
            if(strcmp(Mytable.element[index].name,name) == 0)
                return index;       //返回在符号表中的索引

        }
        return -1;       //返回-1说明没有找到
    }
    else{
        //先在同层查找
        for(int i = Mytable.funcIndex[Mytable.numOfFunc - 1];i <= Mytable.topIndex;i++){
            if(strcmp(Mytable.element[i].name,name) == 0)
                return i;
        }
        //没找到的话，在全局变量中查找
        for(int i = 0;i < Mytable.funcIndex[0];i++){
            if(strcmp(Mytable.element[i].name,name) == 0)
                return i;
        }
        return -1;
    }
}

void saveInfo(){
    strcpy(tempToken,token);
    tempreadloc = readloc;
    templineNum = lineNum;
    tempsymID = symID;
    tempnowChar = nowChar;
}

void recoverInfo(){
    strcpy(token,tempToken);
    readloc = tempreadloc;
    lineNum = templineNum;
    symID = tempsymID;
    nowChar = tempnowChar;
}

int calValue(char num[]){
    int tempNum = 0;
    for(int i = 0;i < strlen(num);i++){
        tempNum = tempNum * 10 + (num[i] - '0');
        if(tempNum < 0)
            return -1;        //错误超出int范围
    }
    return tempNum;
}

void program(){
    //cout << "This is a program!" << endl;
    Mytable.topIndex = -1;  //初始化符号表栈顶指针
    Mytable.numOfFunc = 0;
    if(symID == CONSTSYM){
        constDec();
    }
    while((symID == INTSYM || symID == CHARSYM) && !endflag){
        saveInfo();
        getsym();
        if(symID != IDSYM){
            error(ID_ERROR,lineNum);//error
            while(symID != INTSYM && symID != CHARSYM && symID!=VOIDSYM && !endflag)
                getsym();
            continue;
        }
        getsym();  //预读标识符后面的单词
        if(symID == LPARENSYM){  //如果直接读到'('，说明无变量声明
            recoverInfo();
            break;   //处理函数定义
        }
        else if(symID == COMMASYM || symID == LFPARENSYM || symID == SEMISYM){
            recoverInfo();
            varDec();
            break;
        }
        else{
            error(ID_DECLARE_ERROR,lineNum);//error
            while(symID != INTSYM && symID != CHARSYM && symID!=VOIDSYM && !endflag)
                getsym();
            continue;//error
        }
    }
    while((symID == INTSYM || symID == CHARSYM || symID == VOIDSYM) && !endflag){
        if(symID == INTSYM || symID == CHARSYM){
            valueFuncDef();
        }
        else{
            saveInfo();
            getsym();
            if(symID == MAINSYM){
                recoverInfo();
                break;
            }
            else{
                recoverInfo();
                voidFuncDef();
            }
        }
    }
    if(endflag){
        error(NO_MAIN_FUNCTION,lineNum);//error
    }
    else
        mainDef();
}

void constDec(){
    //cout << "This is a const declare!" << endl;
    while(symID == CONSTSYM && !endflag){
        getsym();
        constDef();
        if(symID != SEMISYM){
            error(SEMICOLON_ERROR,lineNum);  //报错
            while(!ifnext() && !endflag)
                getsym();
        }
        else{
            getsym();
        }
    }
}

bool ifnext(){
    if(symID == CONSTSYM || symID == INTSYM || symID == CHARSYM || symID == VOIDSYM)
        return true;
    else
        return false;
}

void constDef(){
    char IdentifierName[100];
    kind = 0;
    if(symID == INTSYM || symID == CHARSYM){
        type = symID - 1;
        do{
            getsym();
            if(symID == IDSYM){
                strcpy(IdentifierName,token);
                getsym();
                if(symID == BECOMESYM){
                    getsym();
                    if(symID == ADDSYM || symID == SUBSYM){
                        sign = symID;
                        getsym();
                        if(symID == NUMSYM){
                            strcpy(valueStr,token);
                            if(calValue(token) < 0){
                                error(OVERFLOW_INT,lineNum);  //超过int范围
                                while(symID != COMMASYM && symID != SEMISYM && !endflag)
                                    getsym();
                            }
                            else{
                                if(type == 1){
                                    error(DISMATCH_TYPE,lineNum);
                                    while(symID != COMMASYM && symID != SEMISYM && !endflag)
                                        getsym();
                                }
                                else{
                                    //cout << "This is a const definition!" << endl;
                                    if(sign == ADDSYM)
                                        value = calValue(token);
                                    else
                                        value = (-1) * calValue(token);
                                    enterToTable(IdentifierName,kind,type,value,-1,address);
                                    sprintf(valueStr,"%d",value);
                                    GenMidCode(1,"const","int",IdentifierName,valueStr);
                                    getsym();
                                }
                            }
                        }
                        else{
                            error(NUM_NOT_IN_OP,lineNum);  //+-号后不是数字
                            while(symID != COMMASYM && symID != SEMISYM && !endflag)
                                getsym();
                        }
                    }
                    else if(symID == NUMSYM){
                        strcpy(valueStr,token);
                        if(type == 0){
                            if(calValue(token) < 0){
                                error(OVERFLOW_INT,lineNum);  //超过int范围
                                while(symID != COMMASYM && symID != SEMISYM && !endflag)
                                    getsym();
                            }
                            else{
                                //cout << "This is a const definition!" << endl;
                                value = calValue(token);
                                enterToTable(IdentifierName,kind,type,value,-1,address);
                                GenMidCode(1,"const","int",IdentifierName,valueStr);
                                getsym();
                            }
                        }
                        else{
                            error(DISMATCH_TYPE,lineNum);
                            while(symID != COMMASYM && symID != SEMISYM && !endflag)
                                getsym();
                        }
                    }
                    else if(symID == CONSTCHARSYM){
                        strcpy(valueStr,token);
                        if(type == 1){
                            //cout << "This is a const definition!" << endl;
                            value = token[1];
                            enterToTable(IdentifierName,kind,type,value,-1,address);
                            GenMidCode(1,"const","char",IdentifierName,valueStr);
                            getsym();
                        }
                        else{
                            error(DISMATCH_TYPE,lineNum);
                            while(symID != COMMASYM && symID != SEMISYM && !endflag)
                                getsym();
                        }
                    }
                    else{
                        error(CONST_DEFINE_ERROR,lineNum);  //赋值号后面不是整数或者字符
                        while(symID != COMMASYM && symID != SEMISYM && !endflag)
                            getsym();
                    }
                }
                else{
                    error(ASSIGN_OP_ERROR,lineNum);  //等号错误
                    while(symID != COMMASYM && symID != SEMISYM && !endflag)
                        getsym();
                }
            }
            else{
                error(ID_ERROR,lineNum);  //类型标识符后的标识符错误
                while(symID != COMMASYM && symID != SEMISYM && !endflag)
                    getsym();
            }
        }while(symID == COMMASYM && !endflag);
    }
    else{
        error(NO_TYPE_ERROR,lineNum);//报错
        while(symID != SEMISYM && !endflag)
            getsym();
    }
}

void varDec(){
    //cout << "This is a var declare!" << endl;
    while((symID == INTSYM || symID == CHARSYM) && !endflag){
        saveInfo();
        getsym();
        getsym();
        if(symID == LPARENSYM){
            recoverInfo();
            break;
        }
        else{
            recoverInfo();
            varDef();
        }
        if(symID == SEMISYM){
            getsym();
        }
        else{
            error(SEMICOLON_ERROR,lineNum);  //报错
            while(!ifnext() && !endflag)
                getsym();
        }
    }
}

void varDef(){
    char IdentifierName[100];
    type = symID - 1;
    //cout << "This is a var definition!" << endl;
    do{
        getsym();
        if(symID == IDSYM){
            strcpy(IdentifierName,token);
            getsym();
            if(symID == COMMASYM || symID == SEMISYM){
                kind = 1;
                enterToTable(IdentifierName,kind,type,0,-1,address);
                if(type == 0)
                    GenMidCode(2,"var","int",IdentifierName,"");
                else
                    GenMidCode(2,"var","char",IdentifierName,"");
            }
            else if(symID == LFPARENSYM){
                kind = 4;
                getsym();
                if(symID == NUMSYM){
                    strcpy(valueStr,token);
                    if(calValue(token) <= 0){
                        error(ARRAY_INDEX_INVALID,lineNum);
                        while(symID != COMMASYM && symID != SEMISYM && !endflag)
                            getsym();
                    }
                    else{
                        capicity = calValue(token);
                        getsym();
                        if(symID == RFPARENSYM){
                            enterToTable(IdentifierName,kind,type,0,capicity,address);
                            if(type == 0)
                                GenMidCode(3,"array","int",IdentifierName,valueStr);
                            else
                                GenMidCode(3,"array","char",IdentifierName,valueStr);
                            getsym();
                        }
                        else{
                            error(RFPAREN_ERROR,lineNum);
                            while(symID != COMMASYM && symID != SEMISYM && !endflag)
                                getsym();
                        }
                    }
                }
                else{
                    error(ARRAY_INDEX_NOTNUM,lineNum);
                    while(symID != COMMASYM && symID != SEMISYM && !endflag)
                        getsym();
                }
            }
            else{
                error(ID_ERROR,lineNum);
                while(symID != COMMASYM && symID != SEMISYM && !endflag)
                    getsym();
            }
        }
        else{
            error(ID_ERROR,lineNum);
            while(symID != COMMASYM && symID != SEMISYM && !endflag)
                getsym();
        }
    }while(symID == COMMASYM && !endflag);
}

void valueFuncDef(){
    //cout << "This is a value return function definition!" << endl;
    decHead();
    if(symID != LPARENSYM){
        error(LPAREN_ERROR,lineNum);
    }
    else
        getsym();
    paraList();
    if(symID != RPARENSYM){
        error(RPAREN_ERROR,lineNum);
    }
    else
        getsym();
    if(symID != LHPARENSYM){
        error(LHPAREN_ERROR,lineNum);
    }
    else
        getsym();
    ifReturn = false;
    compoundState();
    Mytable.element[Mytable.funcIndex[Mytable.numOfFunc-1]].midEnd = varNum - 1;
    GenMidCode(7,"ret","","","");
    if(!ifReturn){
        error(NO_RETURN_ERROR,lineNum);
    }
    if(symID != RHPARENSYM){
        error(RHPAREN_ERROR,lineNum);
    }
    getsym();
}

void paraList(){
    paraNum = 0;
    do{
        if(symID == COMMASYM)
            getsym();
        if(symID == INTSYM || symID == CHARSYM){
            parameter();
            paraNum++;
        }
        else if(symID == RPARENSYM){
            paraNum = 0;    //说明参数列表为空
        }
        else{
            error(NO_TYPE_ERROR,lineNum);
            while(symID != COMMASYM && symID != RPARENSYM && !endflag)
                getsym();
        }
    }while(symID == COMMASYM);
    Mytable.element[Mytable.funcIndex[Mytable.numOfFunc - 1]].capicity = paraNum;
}

void parameter(){
    char IdentifierName[100];
    //cout << "This is a parameter!" << endl;
    kind = 2;
    type = symID - 1;
    getsym();
    if(symID == IDSYM){
        strcpy(IdentifierName,token);
        enterToTable(IdentifierName,kind,type,0,-1,address);
        if(type == 0)
            GenMidCode(4,"para","int",IdentifierName,"");
        else
            GenMidCode(4,"para","char",IdentifierName,"");
        getsym();
    }
    else{
        error(ID_ERROR,lineNum);
        while(symID != COMMASYM && symID != RPARENSYM && !endflag)
            getsym();
    }
}

void decHead(){
    char IdentifierName[100];
    //cout << "This is a declare head!" << endl;
    kind = 3;
    if(symID == INTSYM || symID == CHARSYM){
        type = symID - 1;
        getsym();
        if(symID == IDSYM){
            strcpy(IdentifierName,token);
            strcpy(the_func_name,IdentifierName);
            enterToTable(IdentifierName,kind,type,0,0,address);
            Mytable.element[Mytable.topIndex].midStart = varNum;
            if(type == 0)
                GenMidCode(5,"int",IdentifierName,"(",")");
            else
                GenMidCode(5,"char",IdentifierName,"(",")");
            getsym();
        }
        else{
            error(FUNCTION_NAME_ERROR,lineNum);
            while(symID != LPARENSYM && !endflag)
                getsym();
        }
    }

    else{
        error(NO_TYPE_ERROR,lineNum);
        while(symID != LPARENSYM && !endflag)
            getsym();
    }
}

void voidFuncDef(){
    char IdentifierName[100];
    kind = 3;
    type = symID - 1;    //void
    //cout << "This is void function definition!" << endl;
    getsym();
    if(symID != IDSYM){
        error(FUNCTION_NAME_ERROR,lineNum);
    }
    strcpy(IdentifierName,token);
    strcpy(the_func_name,IdentifierName);
    enterToTable(IdentifierName,kind,type,0,0,address);
    Mytable.element[Mytable.topIndex].midStart = varNum;
    GenMidCode(6,"void",IdentifierName,"(",")");
    getsym();
    if(symID != LPARENSYM){
        error(LPAREN_ERROR,lineNum);
    }
    else
        getsym();
    paraList();
    if(symID != RPARENSYM){
        error(RPAREN_ERROR,lineNum);
    }
    else
        getsym();
    if(symID != LHPARENSYM){
        error(LHPAREN_ERROR,lineNum);
    }
    else
        getsym();
    ifReturn = false;
    compoundState();
    Mytable.element[Mytable.funcIndex[Mytable.numOfFunc-1]].midEnd = varNum - 1;
    GenMidCode(7,"ret","","","");
    if(symID != RHPARENSYM){
        error(RHPAREN_ERROR,lineNum);
    }
    getsym();
}

void mainDef(){
    char IdentifierName[100];
    //cout << "This is main function !" << endl;
    kind = 3;
    type = symID - 1;
    getsym();
    if(symID != MAINSYM){
        error(MAIN_FUNC_NAME_ERROR,lineNum);
    }
    strcpy(IdentifierName,token);
    strcpy(the_func_name,IdentifierName);
    enterToTable(IdentifierName,kind,type,0,0,address);
    Mytable.element[Mytable.topIndex].midStart = varNum;
    GenMidCode(6,"void",IdentifierName,"(",")");
    getsym();
    if(symID != LPARENSYM){
        error(LPAREN_ERROR,lineNum);
    }
    else
        getsym();
    if(symID != RPARENSYM){
        error(RPAREN_ERROR,lineNum);
    }
    else
        getsym();
    if(symID != LHPARENSYM){
        error(LHPAREN_ERROR,lineNum);
    }
    else
        getsym();
    compoundState();
    if(MidCodeSet[codeCount - 1].codeKind == 7)
        MidCodeSet[codeCount - 1].effect = false;
    Mytable.element[Mytable.funcIndex[Mytable.numOfFunc-1]].midEnd = varNum - 1;
    GenMidCode(8,"end","","","");
    if(symID != RHPARENSYM){
        error(RHPAREN_ERROR,lineNum);
    }
    getsym();
}

void compoundState(){
    //cout << "This is a compound statement!" << endl;
    if(symID == CONSTSYM){
        constDec();
    }
    if(symID == INTSYM || symID == CHARSYM){
        varDec();
    }
    stateList();
}

void stateList(){
    //cout << "This is a statement List!" << endl;
    while(symID == IFSYM || symID == WHILESYM || symID == LHPARENSYM || symID == IDSYM
          || symID == SCANFSYM || symID == PRINTFSYM || symID == SWITCHSYM ||
          symID == RETURNSYM || symID == SEMISYM){
        statement();
        if(endflag)
            break;
    }
}

bool ifnextstate(){
    if(symID == IFSYM || symID == WHILESYM || symID == LHPARENSYM)
        return true;
    else if(symID == IDSYM || symID == SCANFSYM || symID == PRINTFSYM)
        return true;
    else if(symID == SEMISYM || symID == SWITCHSYM || symID == RETURNSYM)
        return true;
    else if(symID == RHPARENSYM)
        return true;
    else
        return false;
}

void statement(){
    char IdentifierName[100];
    if(symID == IFSYM){
        ifState();
    }
    else if(symID == WHILESYM){
        loopState();
    }
    else if(symID == LHPARENSYM){
        getsym();
        stateList();
        if(symID == RHPARENSYM){
            getsym();
        }
        else{
            error(RHPAREN_ERROR,lineNum);
            while(!ifnextstate() && !endflag)
                getsym();
        }
    }
    else if(symID == IDSYM){
        strcpy(IdentifierName,token);
        saveInfo();
        getsym();
        if(symID == LPARENSYM){
            int findTableResult = searchTable(IdentifierName,true);
            recoverInfo();
            if(findTableResult == -1){
                error(NO_SUCH_FUNCTION,lineNum);
                while(!ifnextstate() && !endflag)
                    getsym();
                return;
            }
            else{
                int functype = Mytable.element[findTableResult].type + 1;
                int funcParaNum = Mytable.element[findTableResult].capicity;
                if(functype == VOIDSYM)
                    voidFuncCall(funcParaNum);
                else
                    valueFuncCall(funcParaNum);
            }
            if(symID == SEMISYM){
                getsym();
            }
            else{
                error(SEMICOLON_ERROR,lineNum);
                while(!ifnextstate() && !endflag)
                    getsym();
            }
        }
        else if(symID == LFPARENSYM || symID == BECOMESYM){
            recoverInfo();
            assignState();
            if(symID == SEMISYM){
                getsym();
            }
            else{
                error(SEMICOLON_ERROR,lineNum);
                while(!ifnextstate() && !endflag)
                    getsym();
            }
        }
        else{
            error(STATEMENT_ERROR,lineNum);
            while(!ifnextstate() && !endflag)
                getsym();
        }
    }
    else if(symID == SCANFSYM){
        readState();
        if(symID == SEMISYM){
            getsym();
        }
        else{
            error(SEMICOLON_ERROR,lineNum);
            while(!ifnextstate() && !endflag)
                getsym();
        }
    }
    else if(symID == PRINTFSYM){
        writeState();
        if(symID == SEMISYM){
            getsym();
        }
        else{
            error(SEMICOLON_ERROR,lineNum);
            while(!ifnextstate() && !endflag)
                getsym();
        }
    }
    else if(symID == SEMISYM){
        getsym();
        //cout << "This is an empty statement!" << endl;
    }
    else if(symID == SWITCHSYM){
        switchState();
    }
    else if(symID == RETURNSYM){
        returnState();
        if(symID == SEMISYM){
            getsym();
        }
        else{
            error(SEMICOLON_ERROR,lineNum);
            while(!ifnextstate() && !endflag)
                getsym();
        }
    }
    else{
        error(STATEMENT_ERROR,lineNum);
        while(!ifnextstate() && !endflag)
            getsym();
    }
}

void ifState(){
    char label[50];
    //cout << "This is a if statement!" << endl;
    genLabel(label);
    getsym();
    if(symID != LPARENSYM){
        error(LPAREN_ERROR,lineNum);
    }
    else
        getsym();
    condition(label);
    if(symID != RPARENSYM){
        error(RPAREN_ERROR,lineNum);
    }
    else
        getsym();
    statement();
    GenMidCode(10,label,":","","");
}

void condition(char label[]){
    int expType,expValue;
    char src1[50];
    char src2[50];
    int op;
    //cout << "This is a condition!" << endl;
    express(&expType,&expValue);
    if(expType == CHARSYM || expType == CONSTCHARSYM)
        error(CHARTYPE_IN_CONDITION,lineNum);
    strcpy(src1,varResult);
    if(isrelOp()){
        op = symID;
        getsym();
        express(&expType,&expValue);
        if(expType == CHARSYM || expType == CONSTCHARSYM)
            error(CHARTYPE_IN_CONDITION,lineNum);
        strcpy(src2,varResult);
        switch(op){
        case LESSYM:
            GenMidCode(11,label,src1,"<",src2);
            break;
        case GTRSYM:
            GenMidCode(12,label,src1,">",src2);
            break;
        case LEQSYM:
            GenMidCode(13,label,src1,"<=",src2);
            break;
        case GEQSYM:
            GenMidCode(14,label,src1,">=",src2);
            break;
        case EQLSYM:
            GenMidCode(15,label,src1,"==",src2);
            break;
        case NEQSYM:
            GenMidCode(16,label,src1,"!=",src2);
            break;
        }
    }
    else if(symID == RPARENSYM){
        GenMidCode(9,label,src1,"","");
        return;
    }
    else{
        error(CONDITION_ERROR,lineNum);
        while(symID != RPARENSYM && !endflag)
            getsym();
    }
}

void loopState(){
    char labelStart[50];
    char labelEnd[50];
    genLabel(labelStart);
    GenMidCode(10,labelStart,":","","");
    //cout << "This is a loop state!" << endl;
    getsym();
    if(symID != LPARENSYM){
        error(LPAREN_ERROR,lineNum);
    }
    else
        getsym();
    genLabel(labelEnd);
    condition(labelEnd);
    if(symID != RPARENSYM){
        error(RPAREN_ERROR,lineNum);
    }
    else
        getsym();
    statement();
    GenMidCode(17,"goto",labelStart,"","");
    GenMidCode(10,labelEnd,":","","");
}

void voidFuncCall(int numOfPara){
    char IdentifierName[100];
    int valueParaNum = 0;
    int func_index;
    //cout << "This is a void function call statement!" << endl;
    strcpy(IdentifierName,token);
    func_index = searchTable(IdentifierName,true);
    getsym();
    getsym();
    valueParaList(&valueParaNum,func_index);
    if(numOfPara != valueParaNum){
        error(PARANUM_NOT_MATCH_ERROR,lineNum);
        while(symID != SEMISYM && !endflag)
            getsym();
    }
    else{
        if(symID != RPARENSYM){
            error(RPAREN_ERROR,lineNum);
            while(symID != SEMISYM && !endflag)
                getsym();
        }
        else{
            GenMidCode(18,"call","void",IdentifierName,"");
            getsym();
        }
    }
}

void valueFuncCall(int numOfPara){
    char IdentifierName[100];
    int func_index;
    int valueParaNum = 0;
    //cout << "This is a value function call statement!" << endl;
    strcpy(IdentifierName,token);
    func_index = searchTable(IdentifierName,true);
    getsym();
    getsym();
    valueParaList(&valueParaNum,func_index);
    if(numOfPara != valueParaNum){
        error(PARANUM_NOT_MATCH_ERROR,lineNum);
        while(symID != SEMISYM && !endflag)
            getsym();
    }
    else{
        if(symID != RPARENSYM){
            error(RPAREN_ERROR,lineNum);
            while(symID != SEMISYM && !endflag)
                getsym();
        }
        else{
            GenMidCode(19,"call","value",IdentifierName,"");
            getsym();
        }
    }
}

void valueParaList(int* valueParaNum,int func_index){
    char paraStack[50][100];
    int expType,expValue;
    //cout << "This is a value parameter list!" << endl;
    do{
        if(symID == COMMASYM)
            getsym();
        if(symID == RPARENSYM){
            *valueParaNum = 0;
            return;
        }
        else{
            express(&expType,&expValue);
            strcpy(paraStack[*valueParaNum],varResult);
            *valueParaNum = *valueParaNum + 1;
            if(Mytable.element[func_index + *valueParaNum].type + 1 == INTSYM){
                if(expType != INTSYM && expType != NUMSYM){
                    error(VALUE_PARA_NOT_MATCH,lineNum);
                }
            }
            else{
                if(expType != CHARSYM && expType != CONSTCHARSYM){
                    error(VALUE_PARA_NOT_MATCH,lineNum);
                }
            }
        }
    }while(symID == COMMASYM);
    for(int i = 0; i < *valueParaNum;i++)
        GenMidCode(20,"push",paraStack[i],"","");
}

void assignState(){
    int expType,expValue;
    char src1[50];
    char src2[50];
    char result[50];
    int findTableResult;
    int resultType;
    //cout << "This is a assign statement!" << endl;
    strcpy(result,token);
    findTableResult = searchTable(result,false);
    if(findTableResult == -1){          //判断被赋值的变量是否存在
        error(NO_SUCH_IDENTIFIER,lineNum);
        while(symID != SEMISYM && !endflag)
            getsym();
        return;        //直接结束
    }
    resultType = Mytable.element[findTableResult].kind;
    if(resultType == 0 || resultType == 3){      //给常量、函数赋值
        error(ASSIGN_ARRAY_OR_FUNCT,lineNum);
        while(symID != SEMISYM && !endflag)
            getsym();
        return;
    }
    getsym();
    if(symID == BECOMESYM){
        resultType = Mytable.element[findTableResult].kind;
        if(resultType == 4){
            error(ASSIGN_ARRAY_OR_FUNCT,lineNum);
            while(symID != SEMISYM && !endflag)
                getsym();
            return;
        }
        getsym();
        express(&expType,&expValue);
        resultType = Mytable.element[findTableResult].type + 1;
        if(resultType == INTSYM && expType != INTSYM && expType != NUMSYM){
            error(ASSIGN_TYPE_NOTMATCH,lineNum);
            while(symID != SEMISYM && !endflag)
                getsym();
            return;
        }
        else if(resultType == CHARSYM && expType != CHARSYM && expType != CONSTCHARSYM){
            error(ASSIGN_TYPE_NOTMATCH,lineNum);
            while(symID != SEMISYM && !endflag)
                getsym();
            return;
        }
        else{
            strcpy(src1,varResult);
            GenMidCode(21,result,src1,"","");
        }
    }
    else if(symID == LFPARENSYM){
        resultType = Mytable.element[findTableResult].kind;
        if(resultType != 4){
            error(VAR_HAVE_LFPARENSYM,lineNum);
            while(symID != SEMISYM && !endflag)
                getsym();
            return;
        }
        getsym();
        express(&expType,&expValue);
        strcpy(src1,varResult);
        if(expType == CONSTCHARSYM || expType == NUMSYM){
            int arraySize = Mytable.element[findTableResult].capicity;
            if(expValue < 0 || expValue >= arraySize){
                error(ARRAY_OVERFLOW,lineNum);
                while(symID != SEMISYM && !endflag)
                    getsym();
                return;
            }
        }
        if(symID != RFPARENSYM){
            error(RFPAREN_ERROR,lineNum);
            while(symID != SEMISYM && !endflag)
                getsym();
            return;
        }
        else{
            getsym();
            if(symID == BECOMESYM){
                getsym();
                express(&expType,&expValue);
                resultType = Mytable.element[findTableResult].type + 1;
                if(resultType == INTSYM && expType != INTSYM && expType != NUMSYM){
                    error(ASSIGN_TYPE_NOTMATCH,lineNum);
                    while(symID != SEMISYM && !endflag)
                        getsym();
                    return;
                }
                else if(resultType == CHARSYM && expType != CHARSYM && expType != CONSTCHARSYM){
                    error(ASSIGN_TYPE_NOTMATCH,lineNum);
                    while(symID != SEMISYM && !endflag)
                        getsym();
                    return;
                }
                else{
                    strcpy(src2,varResult);
                    GenMidCode(22,result,src1,src2,"");
                }
            }
            else{
                error(ASSIGN_OP_ERROR,lineNum);
                while(symID != SEMISYM && !endflag)
                    getsym();
            }
        }
    }
    else{
        error(ASSIGNSTATE_ERROR,lineNum);
        while(symID != SEMISYM && !endflag)
            getsym();
    }
}

void readState(){
    char IdentifierName[100];
    int findTableResult;
    //cout << "This is a read statement!" << endl;
    getsym();
    if(symID == LPARENSYM){
        do{
            getsym();
            if(symID == IDSYM){
                strcpy(IdentifierName,token);
                findTableResult = searchTable(IdentifierName,false);
                if(findTableResult == -1){
                    error(NO_SUCH_IDENTIFIER,lineNum);
                    while(symID != SEMISYM && !endflag)
                        getsym();
                    return;
                }
                if(Mytable.element[findTableResult].kind != 1 && Mytable.element[findTableResult].kind != 2){
                    error(READ_NOT_VAR,lineNum);
                    while(symID != SEMISYM && !endflag)
                        getsym();
                    return;
                }  //不是变量报错
                GenMidCode(23,"scanf",IdentifierName,"","");
                getsym();
            }
            else{
                error(ID_ERROR,lineNum);
                while(symID != SEMISYM && !endflag)
                    getsym();
                return;
            }
        }while(symID == COMMASYM);
        if(symID == RPARENSYM){
            getsym();
        }
        else{
            error(RPAREN_ERROR,lineNum);
            while(symID != SEMISYM && !endflag)
                getsym();
        }
    }
    else{
        error(LPARENSYM,lineNum);
        while(symID != SEMISYM && !endflag)
            getsym();
    }
}

void writeState(){
    int expType,expValue;
    char src1[100];
    char src2[100];
    //cout << "This is a write statement!" << endl;
    getsym();
    if(symID == LPARENSYM){
        getsym();
        if(symID == STRINGSYM){
            strcpy(src1,token);
            getsym();
            if(symID == COMMASYM){
                getsym();
                express(&expType,&expValue);
                strcpy(src2,varResult);
                if(symID == RPARENSYM){
                    GenMidCode(24,"printf",src1,src2,"");
                    getsym();
                }
                else{
                    error(RPAREN_ERROR,lineNum);
                    while(symID != SEMISYM && !endflag)
                        getsym();
                }
            }
            else if(symID == RPARENSYM){
                GenMidCode(24,"printf",src1,"","");
                getsym();
            }
            else{
                error(WRITESTATE_ERROR,lineNum);
                while(symID != SEMISYM && !endflag)
                    getsym();
            }
        }
        else{
            express(&expType,&expValue);
            strcpy(src2,varResult);
            if(symID == RPARENSYM){
                GenMidCode(24,"printf","",src2,"");
                getsym();
            }
            else{
                error(RPAREN_ERROR,lineNum);
                while(symID != SEMISYM && !endflag)
                    getsym();
            }
        }
    }
    else{
        error(LPAREN_ERROR,lineNum);
        while(symID != SEMISYM && !endflag)
            getsym();
    }
}

void switchState(){
    int expType,expValue;
    //cout << "This is a switch statement!" << endl;
    char result[50];
    char label[50];
    genLabel(label);
    getsym();
    if(symID == LPARENSYM){
        getsym();
        express(&expType,&expValue);
        strcpy(result,varResult);
        if(symID == RPARENSYM){
            getsym();
            if(symID == LHPARENSYM){
                getsym();
                switchList(result,label);
                defaultState(result,label);
                if(symID == RHPARENSYM){
                    GenMidCode(10,label,":","","");
                    getsym();
                }
                else{
                    error(RHPAREN_ERROR,lineNum);
                    while(!ifnextstate() && !endflag)
                        getsym();
                }
            }
            else{
                error(LHPAREN_ERROR,lineNum);
                while(!ifnextstate() && !endflag)
                    getsym();
            }
        }
        else{
            error(RPAREN_ERROR,lineNum);
            while(!ifnextstate() && !endflag)
                getsym();
        }
    }
    else{
        error(LPAREN_ERROR,lineNum);
        while(!ifnextstate() && !endflag)
            getsym();
    }
}

void switchList(char result[],char label[]){
    //cout << "This is situation list!" << endl;
    do{
        caseState(result,label);
    }while(symID == CASESYM);
}

void caseState(char result[],char label[]){
    int caseValue;
    char nextcaseLabel[50];
    char src1[50];
    genLabel(nextcaseLabel);      //生成下一个case的label
    //cout << "This is a case statement!" << endl;
    if(symID == CASESYM){
        getsym();
        if(symID == CONSTCHARSYM){
            strcpy(src1,token);
        }
        else if(symID == ADDSYM || symID == SUBSYM){
            sign = (symID == ADDSYM)?1:-1;
            getsym();
            if(symID == NUMSYM){
                caseValue = calValue(token) * sign;
                sprintf(src1,"%d",caseValue);
            }
            else{
                error(NUM_NOT_IN_OP,lineNum);
                while(!ifnextstate() && !endflag)
                    getsym();
            }
        }
        else if(symID == NUMSYM){
           strcpy(src1,token);
        }
        else{
            error(CASE_KIND_ERROR,lineNum);  //case后面不是常量
            while(!ifnextstate() && !endflag)
                getsym();
        }
        getsym();
        if(symID == COLONSYM){
            GenMidCode(15,nextcaseLabel,result,"==",src1);
            getsym();
            statement();
            GenMidCode(17,"goto",label,"","");
            GenMidCode(10,nextcaseLabel,":","","");
        }
        else{
            error(COLON_ERROR,lineNum);
            while(!ifnextstate() && !endflag)
                getsym();
        }
    }
    else{
        error(CASE_ERROR,lineNum);
        while(!ifnextstate() && !endflag)
            getsym();
    }
}

void defaultState(char result[],char label[]){
    //cout << "This is a default statement!" << endl;
    if(symID == DEFAULTSYM){
        getsym();
        if(symID == COLONSYM){
            getsym();
            statement();
        }
        else{
            error(COLON_ERROR,lineNum);
            while(!ifnextstate() && !endflag)
                getsym();
        }
    }
    else if(symID == RHPARENSYM){
        //为空的情况
    }
    else{
        error(DEFAULT_ERROR,lineNum);
        getsym();
    }
}

void returnState(){
    int expType,expValue;
    int func_index = searchTable(the_func_name,true);
    ifReturn = true;
    //cout << "This is a return statement!" << endl;
    getsym();
    if(symID == LPARENSYM){
        getsym();
        express(&expType,&expValue);
        if(Mytable.element[func_index].type + 1 == INTSYM){
            if(expType != INTSYM && expType != NUMSYM){
                error(RETURN_TYPE_NOT_MATCH,lineNum);
            }
        }
        else if(Mytable.element[func_index].type + 1 == CHARSYM){
            if(expType != CHARSYM && expType != CONSTCHARSYM){
                error(RETURN_TYPE_NOT_MATCH,lineNum);
            }
        }
        else{
            error(RETURN_ERROE,lineNum);
        }
        if(symID == RPARENSYM){
            GenMidCode(25,"ret",varResult,"","");
            getsym();
        }
        else{
            error(RPAREN_ERROR,lineNum);
            while(symID != SEMISYM && !endflag)
                getsym();
        }
    }
    else{
        GenMidCode(7,"ret","","","");
        //为空的情况
    }
}

void express(int* expType,int* expValue){
    int termType,termValue;
    //cout << "This is a expression!" << endl;
    char src1[50];
    char src2[50];
    char result[50];
    char op[50];
    int temp_sign = 0;
    if(symID == ADDSYM || symID == SUBSYM){
        temp_sign = symID;
        getsym();
    }
    term(&termType,&termValue);
    *expType = termType;*expValue = termValue;
    if(temp_sign != 0)
        *expType = INTSYM;
    if(temp_sign == SUBSYM){
        genTempVar(result);
        addTempVar(result,0);
        GenMidCode(26,result,"0","-",varResult);
        strcpy(varResult,result);
        *expValue = 0 - *expValue;
    }
    strcpy(result,varResult);
    while(symID == ADDSYM || symID == SUBSYM){
        *expType = INTSYM;*expValue = -1;
        strcpy(src1,result);
        if(symID == ADDSYM)
            strcpy(op,"+");
        else
            strcpy(op,"-");
        getsym();
        term(&termType,&termValue);
        strcpy(src2,varResult);
        genTempVar(result);
        addTempVar(result,0);
        GenMidCode(26,result,src1,op,src2);
    }
    strcpy(varResult,result);
}

void term(int* termType,int* termValue){
    int factorType,factorValue;
    //cout << "This is term!" << endl;
    char src1[50];
    char src2[50];
    char result[50];
    char op[50];
    factor(&factorType,&factorValue);
    strcpy(result,varResult);
    *termType = factorType;*termValue = factorValue;
    while(symID == MULTSYM || symID == DIVSYM){
        *termType = INTSYM;*termValue = -1;
        strcpy(src1,result);
        if(symID == MULTSYM)
            strcpy(op,"*");
        else
            strcpy(op,"/");
        getsym();
        factor(&factorType,&factorValue);
        strcpy(src2,varResult);
        genTempVar(result);
        addTempVar(result,0);
        GenMidCode(26,result,src1,op,src2);
    }
    strcpy(varResult,result);
}

void factor(int *factorType,int *factorValue){
    int expType,expValue;
    char factorId[100];
    char result[100];
    int findTableResult;
    //cout << "This is factor!" << endl;
    if(symID == IDSYM){
        strcpy(factorId,token);  //
        saveInfo();
        getsym();
        if(symID == LFPARENSYM){
            getsym();
            express(&expType,&expValue);
            if(symID == RFPARENSYM){
                findTableResult = searchTable(factorId,false);
                if(findTableResult == -1){
                    error(NO_SUCH_IDENTIFIER,lineNum);
                    while(symID != SEMISYM && !endflag)
                        getsym();
                }
                else{
                    if(expType == CONSTCHARSYM || expType == NUMSYM){
                        int arraySize = Mytable.element[findTableResult].capicity;
                        if(expValue < 0 || expValue >= arraySize){
                            error(ARRAY_OVERFLOW,lineNum);
                        }
                    }
                    if(Mytable.element[findTableResult].kind != 4){
                        error(IDENTIFY_ISNOT_ARRAY,lineNum);
                        while(symID != SEMISYM && !endflag)
                            getsym();
                    }
                    strcpy(result,varResult);
                    genTempVar(varResult);
                    addTempVar(varResult,Mytable.element[findTableResult].type);
                    GenMidCode(27,varResult,factorId,result,"");
                    *factorType = Mytable.element[findTableResult].type + 1;*factorValue = -1;
                    getsym();
                }
            }
            else{
                error(RFPAREN_ERROR,lineNum);
                while(symID != SEMISYM && !endflag)
                    getsym();
            }
        }
        else if(symID == LPARENSYM){
            recoverInfo();
            findTableResult = searchTable(factorId,true);
            if(findTableResult == -1){
                error(NO_SUCH_FUNCTION,lineNum);
                while(symID != SEMISYM && !endflag)
                    getsym();
            }
            else{
                if(Mytable.element[findTableResult].kind != 3){
                    error(IDENTIFY_ISNOT_FUNCTION,lineNum);
                    while(symID != SEMISYM && !endflag)
                        getsym();
                    return;
                }
                int functype = Mytable.element[findTableResult].type + 1;
                int funcParaNum = Mytable.element[findTableResult].capicity;
                if(functype == VOIDSYM){
                    error(EXP_CALL_VOID_FUNCTION,lineNum);
                    while(symID != SEMISYM && !endflag)
                        getsym();
                }
                else{
                    valueFuncCall(funcParaNum);
                    genTempVar(varResult);
                    addTempVar(varResult,functype - 1);
                    GenMidCode(28,varResult,"=","ret","");
                    *factorType = functype;*factorValue = -1;
                }
            }
        }
        else{
            findTableResult = searchTable(factorId,false);
            if(findTableResult == -1){
                error(NO_SUCH_IDENTIFIER,lineNum);
                while(symID != SEMISYM && !endflag)
                    getsym();
            }
            else{
                //新加的，为了fuck从左到右
                if(Mytable.element[findTableResult].global){
                    genTempVar(varResult);
                    addTempVar(varResult,Mytable.element[findTableResult].type);
                    GenMidCode(21,varResult,factorId,"","");
                }
                else
                    strcpy(varResult,factorId);
                if(Mytable.element[findTableResult].kind == 0){
                    if(Mytable.element[findTableResult].type + 1 == INTSYM){
                        *factorType = NUMSYM;
                        *factorValue = Mytable.element[findTableResult].value;
                    }
                    else{
                        *factorType = CONSTCHARSYM;
                        *factorValue = Mytable.element[findTableResult].value;
                    }
                }
                else if(Mytable.element[findTableResult].kind == 1 || Mytable.element[findTableResult].kind == 2){
                    if(Mytable.element[findTableResult].type + 1 == INTSYM){
                        *factorType = INTSYM;
                        *factorValue = -1;
                    }
                    else{
                        *factorType = CHARSYM;
                        *factorValue = -1;
                    }
                }
                else{
                    error(ASSIGN_ARRAY_OR_FUNCT,lineNum);
                    while(symID != SEMISYM && !endflag)
                        getsym();
                    return;
                }
            }
        }
    }
    else if(symID == LPARENSYM){
        getsym();
        express(&expType,&expValue);
        if(symID == RPARENSYM){
            getsym();
            *factorType = INTSYM;*factorValue = expValue;
        }
        else{
            error(RPAREN_ERROR,lineNum);
            while(symID != SEMISYM && !endflag)
                getsym();
        }
    }
    else if(symID == CONSTCHARSYM){
        strcpy(varResult,token);
        *factorType = CONSTCHARSYM;*factorValue = token[1];
        getsym();
    }
    else if(symID == ADDSYM || symID == SUBSYM){
        sign = (symID == ADDSYM)?1:-1;
        getsym();
        if(symID == NUMSYM){
            *factorType = NUMSYM;*factorValue = sign * calValue(token);
            sprintf(varResult,"%d",*factorValue);
            getsym();
        }
        else{
            error(NUM_NOT_IN_OP,lineNum);
        }
    }
    else if(symID == NUMSYM){
        *factorType = NUMSYM;*factorValue = calValue(token);
        sprintf(varResult,"%d",*factorValue);
        getsym();
    }
    else{
        error(FACTOR_ERROR,lineNum);
        while(symID != SEMISYM && !endflag)
            getsym();
    }
}

bool isrelOp(){
    if(symID == LESSYM || symID == GTRSYM || symID == LEQSYM || symID == GEQSYM || symID == EQLSYM || symID == NEQSYM)
        return true;
    else
        return false;
}

