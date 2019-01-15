#include <iostream>
#include <stdio.h>
#include <string.h>
#include "global.h"

MipsTable myMipsTable;

reg tempReg[20];      //代表t0-t9寄存器
reg saveReg[10];      //代表s0-s7

int regstack[20];  //保存已经用的寄存器编号
int saveRegStack[20];
int stackPoint = -1;
int savestackPoint = -1;
int pushedParaNum;

//函数声明如下
void MipsData();
void Create_Mips_Table();
int searchMipsTable(char name[],int funcofIndex,bool ifFunction);
int searchFuncIndex(char name[]);
int getfuncSize(int index);
int getParaNum(int index);
int findTempReg(char name[]);
int findFreeReg();
int findSaveReg(char name[]);
int findFreeSaveReg();
void saveCurrent(int funcofIndex);
void recoverCurrent(int funcofIndex);
void MipsText();
void initReg();
int calVarValue(char name[]);
bool ifNum(char name[]);
int findTempType(char name[]);

void MipsData(){
    char newline[10];
    fprintf(mipsOut,".data\n");//data数据段，储存全局变量和字符串
    int global_end_index = Mytable.funcIndex[0];
    for(int i = 0;i < global_end_index;i++){
        int datakind = Mytable.element[i].kind;
        if(datakind == 0)
            fprintf(mipsOut,"%s:.word %d\n",Mytable.element[i].name,Mytable.element[i].value);
        else if(datakind == 1)
            fprintf(mipsOut,"%s:.word 0\n",Mytable.element[i].name);
        else
            fprintf(mipsOut,"%s:.space %d\n",Mytable.element[i].name,(Mytable.element[i].capicity + 5)*4);
    }
    for(int i = 0;i < codeCount;i++){
        if(MidCodeSet[i].codeKind == 24){
            if(strlen(MidCodeSet[i].src1) != 0){
                fprintf(mipsOut,"string%d:.asciiz %s\n",i,MidCodeSet[i].src1);
            }
        }
    }
    strcpy(newline,"\"\\n\"");
    fprintf(mipsOut,"string%d:.asciiz %s\n",codeCount,newline);

}

void Create_Mips_Table(){     //构建代码生成时的符号表
    int startindex,endindex;
    int midsart,midend,offset;
    char tempvarName[10];
    myMipsTable.numOfFunc = Mytable.numOfFunc;
    myMipsTable.topIndex = -1;
    for(int i = 0;i < Mytable.funcIndex[0];i++){
        myMipsTable.topIndex++;
        strcpy(myMipsTable.element[i].name,Mytable.element[i].name);
        myMipsTable.element[i].kind = Mytable.element[i].kind;
        myMipsTable.element[i].type = Mytable.element[i].type;
        myMipsTable.element[i].value = Mytable.element[i].value;
        myMipsTable.element[i].capicity = Mytable.element[i].capicity;
        myMipsTable.element[i].offset = 0;       //全局变量的偏移已经在.data段设置好
        myMipsTable.element[i].global = true;
    }
    for(int i = 0;i < Mytable.numOfFunc;i++){
        offset = 0;
        myMipsTable.funcIndex[i] = myMipsTable.topIndex + 1;
        startindex = Mytable.funcIndex[i];
        if(i == Mytable.numOfFunc - 1)
            endindex = Mytable.topIndex;
        else
            endindex = Mytable.funcIndex[i+1] - 1;
        for(int j = startindex;j <= endindex;j++){
            myMipsTable.topIndex++;
            strcpy(myMipsTable.element[myMipsTable.topIndex].name,Mytable.element[j].name);
            myMipsTable.element[myMipsTable.topIndex].kind = Mytable.element[j].kind;
            myMipsTable.element[myMipsTable.topIndex].type = Mytable.element[j].type;
            myMipsTable.element[myMipsTable.topIndex].value = Mytable.element[j].value;
            myMipsTable.element[myMipsTable.topIndex].capicity = Mytable.element[j].capicity;
            myMipsTable.element[myMipsTable.topIndex].offset = offset;
            myMipsTable.element[myMipsTable.topIndex].global = false;
            if(Mytable.element[j].kind == 4)
                offset += Mytable.element[j].capicity;
            else
                offset += 1;
        }
        midsart = Mytable.element[Mytable.funcIndex[i]].midStart;
        midend = Mytable.element[Mytable.funcIndex[i]].midEnd;
        for(int j = midsart;j <= midend;j++){
            myMipsTable.topIndex++;
            sprintf(tempvarName,"$t%d",j);
            strcpy(myMipsTable.element[myMipsTable.topIndex].name,tempvarName);
            myMipsTable.element[myMipsTable.topIndex].kind = 5;
            myMipsTable.element[myMipsTable.topIndex].type = findTempType(tempvarName);
            myMipsTable.element[myMipsTable.topIndex].value = 0;    //先把中间变量都初始化为0
            myMipsTable.element[myMipsTable.topIndex].capicity = -1;
            myMipsTable.element[myMipsTable.topIndex].offset = offset;
            myMipsTable.element[myMipsTable.topIndex].global = false;
            offset += 1;
        }
        myMipsTable.element[myMipsTable.funcIndex[i]].functSize = offset;
    }
}



int findTempType(char name[]){
    for(int i = 0;i < tempNum;i++){
        if(strcmp(name,tempTable[i].tempvarName) == 0){
            return tempTable[i].type;
        }
    }
    return -1;
}


void printTable(){
    for(int i = 0;i <= myMipsTable.topIndex;i++){
        cout << myMipsTable.element[i].name << " " << myMipsTable.element[i].kind << " "
        << myMipsTable.element[i].type << " " << myMipsTable.element[i].value << " "
        << myMipsTable.element[i].capicity << " " << myMipsTable.element[i].offset << " "
        << myMipsTable.element[i].global << endl;
    }
}

int searchMipsTable(char name[],int funcofIndex,bool ifFunction){    //funcofIndex从1开始
    int index,startIndex,endIndex;
    if(ifFunction){
        for(int i = 0;i < myMipsTable.numOfFunc;i++){
            index = myMipsTable.funcIndex[i];
            if(strcmp(myMipsTable.element[index].name,name) == 0)
                return index;       //返回在符号表中的索引

        }
        return -1;       //返回-1说明没有找到
    }
    else{
        //先在同层查找
        if(funcofIndex == myMipsTable.numOfFunc){
            startIndex = myMipsTable.funcIndex[funcofIndex - 1];
            endIndex = myMipsTable.topIndex;
        }
        else{
            startIndex = myMipsTable.funcIndex[funcofIndex - 1];
            endIndex = myMipsTable.funcIndex[funcofIndex] - 1;
        }
        for(int i = startIndex;i <= endIndex;i++){
            if(strcmp(myMipsTable.element[i].name,name) == 0)
                return i;
        }
        //没找到的话，在全局变量中查找
        for(int i = 0;i < myMipsTable.funcIndex[0];i++){
            if(strcmp(myMipsTable.element[i].name,name) == 0)
                return i;
        }
        return -1;
    }
}

int searchFuncIndex(char name[]){    //查询是第几个函数，而不是索引
    for(int i = 0;i < myMipsTable.numOfFunc;i++){
        if(strcmp(myMipsTable.element[myMipsTable.funcIndex[i]].name,name) == 0)
            return i+1;
    }
    return -1;
}

int getfuncSize(int index){    //从1开始
    return myMipsTable.element[myMipsTable.funcIndex[index - 1]].functSize;
}

int getParaNum(int index){      //从1开始
    return myMipsTable.element[myMipsTable.funcIndex[index - 1]].capicity;
}

int findTempReg(char name[]){
    for(int i = 0;i < 10;i++){
        if(tempReg[i].occupy && strcmp(tempReg[i].varName,name) == 0){
            return i;
        }
    }
    return -1;
}

int findFreeReg(){
    for(int i = 0;i < 10;i++){
        if(!tempReg[i].occupy){
            return i;
        }
    }
    return -1;
}

int findSaveReg(char name[]){
    for(int i = 0;i < 8;i++){
        if(saveReg[i].occupy && strcmp(saveReg[i].varName,name) == 0){
            return i;
        }
    }
    return -1;
}

int findFreeSaveReg(){
    for(int i = 0;i < 8;i++){
        if(!saveReg[i].occupy)
            return i;
    }
    return -1;
}


void saveCurrent(int funcofIndex){
    int index,offset;
    for(int i = 0;i < 10;i++){   //保存现在还依然有效的寄存器
        if(tempReg[i].occupy){
            index = searchMipsTable(tempReg[i].varName,funcofIndex,false);
            if(index != -1){
                stackPoint++;
                regstack[stackPoint] = i;
                offset = (myMipsTable.element[index].offset + 1) * 4;
                fprintf(mipsOut,"sw $t%d,-%d($fp)\n",i,offset);    //保存寄存器的值
            }
        }
    }
    for(int i = 0;i < 8;i++){
        if(saveReg[i].occupy){
            savestackPoint++;
            saveRegStack[savestackPoint] = i;
            fprintf(mipsOut,"sw $s%d,%s\n",i,saveReg[i].varName);    //保存寄存器的值
        }
    }
}

void recoverCurrent(int funcofIndex){     //适用于返回值语句
    int index,offset;
    fprintf(mipsOut,"move $sp,$fp\n");     //恢复调用前的sp的值
    fprintf(mipsOut,"lw $fp,-4($fp)\n");       //恢复fp的值
    while(stackPoint != -1){
        int i = regstack[stackPoint];
        index = searchMipsTable(tempReg[i].varName,funcofIndex,false);
        offset = (myMipsTable.element[index].offset + 1) * 4;
        fprintf(mipsOut,"lw $t%d,-%d($fp)\n",i,offset);    //恢复寄存器的值
        stackPoint--;
    }
    while(savestackPoint != -1){
        int i = saveRegStack[savestackPoint];
        fprintf(mipsOut,"lw $s%d,%s\n",i,saveReg[i].varName);    //保存寄存器的值
        savestackPoint--;
    }
}

int findAregIndex(int index,int regKind){
    if(regKind == 0){
        for(int i = 0;i < 8;i++){
            if(i != index)
                return i;
        }
    }
    else{
        for(int i = 0;i < 10;i++){
            if(i != index)
                return i;
        }
    }
}

bool ifNum(char name[]){
    char ch;
    for(int i = 0;i < strlen(name);i++){
        ch = name[i];
        if((ch <= '9' && ch >= '0') || ch == '-')
            continue;
        else
            return false;
    }
    return true;
}

int calVarValue(char name[]){
    int value = 0,signs = 1;
    if(ifNum(name)){
        if(name[0] == '-')
            signs = -1;
        for(int i = 0;i < strlen(name);i++){
            if(name[i] == '-')
                continue;
            else
                value = value * 10 + (name[i] - '0');
        }
        value = value * signs;
    }
    else{
        value = name[1];
    }
    return value;
}

void MipsText(){
    int startIndex,offset,offset_1,offset_2,index,index_1,index_2;
    int funcIndex,src1Value,src2Value;
    char src1[100],src2[100],op[100],result[100];
    int tempRegIndex,tempRegIndex_1,saveRegIndex,saveRegIndex_1;
    int oldtempRegindex,tempoffset;
    int src1Index,src2Index,resultIndex;
    int src1RegKind,src2RegKind,resultRegKind;   //0代表s寄存器,1代表t寄存器
    int newRegIndex;
    for(int i = 0;i < codeCount;i++){
        startIndex = i;
        if(MidCodeSet[i].codeKind == 5 || MidCodeSet[i].codeKind == 6)
            break;              //因为全局变量的那些声明已经在.data段，所以需要跳过
    }
    for(int i = startIndex;i < codeCount;i++){
        if(!MidCodeSet[i].effect)
            continue;
        switch(MidCodeSet[i].codeKind){
            case 1:{
                strcpy(src1,MidCodeSet[i].op);
                strcpy(src2,MidCodeSet[i].src2);
                index = searchMipsTable(src1,funcIndex,false);
                offset = (myMipsTable.element[index].offset + 1) * 4;
                src1Value = calVarValue(src2);
                fprintf(mipsOut,"li $t0,%d\n",src1Value);
                fprintf(mipsOut,"sw $t0,-%d($fp)\n",offset);
                break;
            }
            case 2:
            case 3:
            case 4:
                break;
            case 5:
            case 6:{
                funcIndex = searchFuncIndex(MidCodeSet[i].src1);    //获取现在处理的函数是第几个函数
                pushedParaNum = 0;
                fprintf(mipsOut,"%s:\n",MidCodeSet[i].src1);    //label:
                fprintf(mipsOut,"sw $fp,-4($sp)\n");  //把上一个该函数的fp给存下来
                fprintf(mipsOut,"move $fp,$sp\n");  //
                fprintf(mipsOut,"sw $ra,0($fp)\n");  //将返回值存储
                offset = (getfuncSize(funcIndex) + 2) * 4;
                fprintf(mipsOut,"subiu $sp,$sp,%d\n",offset);  //此时的fp为下一个函数的运行栈栈顶
                int theParaNum = getParaNum(funcIndex);
                for(int j = 0;j < theParaNum;j++){
                    if(j < 4){
                        fprintf(mipsOut,"sw $a%d,-%d($fp)\n",j,(j+2) * 4); //把参数储存下来
                    }
                }
                for(int j = 0;j < 10;j++){
                    tempReg[j].occupy = false;       //清空所有寄存器
                }
                for(int j = 0;j < 8;j++){
                    saveReg[j].occupy = false;
                }
                break;
            }
            case 7:{
                fprintf(mipsOut,"lw $ra,0($fp)\n");
                fprintf(mipsOut,"jr $ra\n");
                break;
            }
            case 8:{
                break;
            }
            case 21:
            case 25:
            case 9:{
                strcpy(src1,MidCodeSet[i].src1);
                index = searchMipsTable(src1,funcIndex,false);
                if(index == -1){   //符号表没有，说明是常量
                    src1Value = calVarValue(src1);
                    tempRegIndex = findFreeReg();
                    if(tempRegIndex != -1){
                        tempReg[tempRegIndex].occupy = true;
                        strcpy(tempReg[tempRegIndex].varName,src1);
                        fprintf(mipsOut,"li $t%d,%d\n",tempRegIndex,src1Value);
                        src1Index = tempRegIndex;
                        src1RegKind = 1;
                    }
                    else{
                        oldtempRegindex = searchMipsTable(tempReg[0].varName,funcIndex,false);
                        if(oldtempRegindex != -1){
                            tempoffset = (myMipsTable.element[oldtempRegindex].offset + 1) * 4;
                            fprintf(mipsOut,"sw $t0,-%d($fp)\n",tempoffset);
                        }
                        strcpy(tempReg[0].varName,src1);
                        fprintf(mipsOut,"li $t0,%d\n",src1Value);
                        src1Index = 0;
                        src1RegKind = 1;
                    }
                }
                else{
                    offset = (myMipsTable.element[index].offset + 1) * 4;
                    if(myMipsTable.element[index].global){   //如果是全局变量
                        if(findSaveReg(src1) != -1){  //已经保存到了寄存器中
                            saveRegIndex = findSaveReg(src1);
                            src1Index = saveRegIndex;
                            src1RegKind = 0;
                        }
                        else{   //还没有保存到寄存器中
                            saveRegIndex = findFreeSaveReg();
                            if(saveRegIndex != -1){
                                fprintf(mipsOut,"lw $s%d,%s\n",saveRegIndex,src1);
                                saveReg[saveRegIndex].occupy = true;
                                strcpy(saveReg[saveRegIndex].varName,src1);
                                src1Index = saveRegIndex;
                                src1RegKind = 0;
                            }
                            else{
                                fprintf(mipsOut,"sw $s0,%s\n",saveReg[0].varName);  //将值写回内存
                                fprintf(mipsOut,"lw $s0,%s\n",src1);      //将新值装入寄存器
                                strcpy(saveReg[0].varName,src1);
                                src1Index = 0;
                                src1RegKind = 0;
                            }
                        }
                    }
                    else{
                        if(findTempReg(src1) != -1){
                            tempRegIndex = findTempReg(src1);
                            src1Index = tempRegIndex;
                            src1RegKind = 1;
                        }
                        else{
                            tempRegIndex = findFreeReg();
                            if(tempRegIndex != -1){
                                tempReg[tempRegIndex].occupy = true;
                                strcpy(tempReg[tempRegIndex].varName,src1);
                                fprintf(mipsOut,"lw $t%d,-%d($fp)\n",tempRegIndex,offset);
                                src1Index = tempRegIndex;
                                src1RegKind = 1;
                            }
                            else{
                                oldtempRegindex = searchMipsTable(tempReg[0].varName,funcIndex,false);
                                if(oldtempRegindex != -1){
                                    tempoffset = (myMipsTable.element[oldtempRegindex].offset + 1) * 4;
                                    fprintf(mipsOut,"sw $t0,-%d($fp)\n",tempoffset);
                                }
                                fprintf(mipsOut,"lw $t0,-%d($fp)\n",offset);
                                strcpy(tempReg[0].varName,src1);
                                src1Index = 0;
                                src1RegKind = 1;
                            }
                        }
                    }
                }
                if(MidCodeSet[i].codeKind == 9){
                    if(src1RegKind == 0)
                        fprintf(mipsOut,"beq $s%d,0,%s\n",src1Index,MidCodeSet[i].result);
                    else
                        fprintf(mipsOut,"beq $t%d,0,%s\n",src1Index,MidCodeSet[i].result);
                }
                else if(MidCodeSet[i].codeKind == 25){
                    if(src1RegKind == 0)
                        fprintf(mipsOut,"move $v0,$s%d\n",src1Index);
                    else
                        fprintf(mipsOut,"move $v0,$t%d\n",src1Index);
                    fprintf(mipsOut,"lw $ra,0($fp)\n");
                    fprintf(mipsOut,"jr $ra\n");
                }
                else if(MidCodeSet[i].codeKind == 21){
                    index_2 = searchMipsTable(MidCodeSet[i].result,funcIndex,false);
                    offset_2 = (myMipsTable.element[index_2].offset + 1) * 4;
                    //分为全局变量和局部变量两种方式去修改值
                    if(myMipsTable.element[index_2].global){
                        if(src1RegKind == 0){
                            fprintf(mipsOut,"sw $s%d,%s\n",src1Index,MidCodeSet[i].result);
                            //更新寄存器的值
                            for(int j = 0;j < 8;j++){
                                if(saveReg[j].occupy && strcmp(saveReg[j].varName,MidCodeSet[i].result) == 0)
                                    fprintf(mipsOut,"move $s%d,$s%d\n",j,src1Index);
                            }
                        }
                        else{
                            fprintf(mipsOut,"sw $t%d,%s\n",src1Index,MidCodeSet[i].result);
                            for(int j = 0;j < 8;j++){
                                if(saveReg[j].occupy && strcmp(saveReg[j].varName,MidCodeSet[i].result) == 0)
                                    fprintf(mipsOut,"move $s%d,$t%d\n",j,src1Index);
                            }
                        }
                    }
                    else{
                        if(src1RegKind == 0){
                            fprintf(mipsOut,"sw $s%d,-%d($fp)\n",src1Index,offset_2);
                            //更新寄存器的值
                            for(int j = 0;j < 10;j++){
                                if(tempReg[j].occupy && strcmp(tempReg[j].varName,MidCodeSet[i].result) == 0)
                                    fprintf(mipsOut,"move $t%d,$s%d\n",j,src1Index);
                            }
                        }
                        else{
                            fprintf(mipsOut,"sw $t%d,-%d($fp)\n",src1Index,offset_2);
                            for(int j = 0;j < 10;j++){
                                if(tempReg[j].occupy && strcmp(tempReg[j].varName,MidCodeSet[i].result) == 0)
                                    fprintf(mipsOut,"move $t%d,$t%d\n",j,src1Index);
                            }
                        }
                    }
                }
                break;
            }
            case 10:{
                for(int j = 0;j < 10;j++){   //保存现在还依然有效的寄存器
                    if(tempReg[j].occupy){
                        tempRegIndex = searchMipsTable(tempReg[j].varName,funcIndex,false);
                        if(tempRegIndex != -1){
                            offset = (myMipsTable.element[tempRegIndex].offset + 1) * 4;
                            fprintf(mipsOut,"sw $t%d,-%d($fp)\n",j,offset);    //保存寄存器的值
                        }
                    }
                    tempReg[j].occupy = false;
                }
                for(int j = 0; j < 8;j++){
                    if(saveReg[j].occupy){
                       fprintf(mipsOut,"sw $s%d,%s\n",j,saveReg[j].varName);
                    }
                    saveReg[j].occupy = false;
                }
                fprintf(mipsOut,"%s:\n",MidCodeSet[i].result);
                break;
            }
            case 22:
            case 26:
            case 27:
            case 11:
            case 12:
            case 13:
            case 14:
            case 15:
            case 16:{
                if(MidCodeSet[i].codeKind == 27){
                    strcpy(src1,MidCodeSet[i].result);
                    strcpy(src2,MidCodeSet[i].op);
                }
                else if(MidCodeSet[i].codeKind == 22){
                    strcpy(src1,MidCodeSet[i].src1);
                    strcpy(src2,MidCodeSet[i].op);
                }
                else{
                    strcpy(src1,MidCodeSet[i].src1);
                    strcpy(src2,MidCodeSet[i].src2);
                    strcpy(op,MidCodeSet[i].op);
                }
                index = searchMipsTable(src1,funcIndex,false);
                index_1 = searchMipsTable(src2,funcIndex,false);
                if(index == -1){
                    src1Value = calVarValue(src1);
                    tempRegIndex = findFreeReg();
                    if(tempRegIndex != -1){
                        tempReg[tempRegIndex].occupy = true;
                        strcpy(tempReg[tempRegIndex].varName,src1);
                        fprintf(mipsOut,"li $t%d,%d\n",tempRegIndex,src1Value);
                        src1Index = tempRegIndex;
                        src1RegKind = 1;
                    }
                    else{
                        oldtempRegindex = searchMipsTable(tempReg[0].varName,funcIndex,false);
                        if(oldtempRegindex != -1){
                            tempoffset = (myMipsTable.element[oldtempRegindex].offset + 1) * 4;
                            fprintf(mipsOut,"sw $t0,-%d($fp)\n",tempoffset);
                        }
                        strcpy(tempReg[0].varName,src1);
                        fprintf(mipsOut,"li $t0,%d\n",src1Value);
                        src1Index = 0;
                        src1RegKind = 1;
                    }
                }
                else{
                    offset = (myMipsTable.element[index].offset + 1) * 4;
                    if(myMipsTable.element[index].global){   //src1也是全局变量
                        if(findSaveReg(src1) != -1){   //src1在寄存器中
                            saveRegIndex = findSaveReg(src1);
                            src1Index = saveRegIndex;
                            src1RegKind = 0;
                        }
                        else{   //src1在内存中
                            saveRegIndex = findFreeSaveReg();
                            if(saveRegIndex != -1){
                                saveReg[saveRegIndex].occupy = true;
                                strcpy(saveReg[saveRegIndex].varName,src1);
                                fprintf(mipsOut,"lw $s%d,%s\n",saveRegIndex,src1);
                                src1Index = saveRegIndex;
                                src1RegKind = 0;
                            }
                            else{
                                fprintf(mipsOut,"sw $s0,%s\n",saveReg[0].varName);  //将值写回内存
                                fprintf(mipsOut,"lw $s0,%s\n",src1);      //将新值装入寄存器
                                strcpy(saveReg[0].varName,src1);
                                src1Index = 0;
                                src1RegKind = 0;
                            }
                        }
                    }
                    else{
                        if(findTempReg(src1) != -1){
                            tempRegIndex = findTempReg(src1);
                            src1Index = tempRegIndex;
                            src1RegKind = 1;
                        }
                        else{
                            tempRegIndex = findFreeReg();
                            if(tempRegIndex != -1){
                                tempReg[tempRegIndex].occupy = true;
                                strcpy(tempReg[tempRegIndex].varName,src1);
                                fprintf(mipsOut,"lw $t%d,-%d($fp)\n",tempRegIndex,offset);
                                src1Index = tempRegIndex;
                                src1RegKind = 1;
                            }
                            else{
                                oldtempRegindex = searchMipsTable(tempReg[0].varName,funcIndex,false);
                                if(oldtempRegindex != -1){
                                    tempoffset = (myMipsTable.element[oldtempRegindex].offset + 1) * 4;
                                    fprintf(mipsOut,"sw $t0,-%d($fp)\n",tempoffset);
                                }
                                fprintf(mipsOut,"lw $t0,-%d($fp)\n",offset);
                                strcpy(tempReg[0].varName,src1);
                                src1Index = 0;
                                src1RegKind = 1;
                            }
                        }
                    }
                }
                if(index_1 == -1){
                    src2Value = calVarValue(src2);
                    tempRegIndex = findFreeReg();
                    if(tempRegIndex != -1){
                        tempReg[tempRegIndex].occupy = true;
                        strcpy(tempReg[tempRegIndex].varName,src2);
                        fprintf(mipsOut,"li $t%d,%d\n",tempRegIndex,src2Value);
                        src2Index = tempRegIndex;
                        src2RegKind = 1;
                    }
                    else{
                        newRegIndex = findAregIndex(src1Index,1);
                        oldtempRegindex = searchMipsTable(tempReg[newRegIndex].varName,funcIndex,false);
                        if(oldtempRegindex != -1){
                            tempoffset = (myMipsTable.element[oldtempRegindex].offset + 1) * 4;
                            fprintf(mipsOut,"sw $t%d,-%d($fp)\n",newRegIndex,tempoffset);
                        }
                        strcpy(tempReg[newRegIndex].varName,src2);
                        fprintf(mipsOut,"li $t%d,%d\n",newRegIndex,src2Value);
                        src2Index = newRegIndex;
                        src2RegKind = 1;
                    }
                }
                else{
                    offset_1 = (myMipsTable.element[index_1].offset + 1) * 4;
                    if(myMipsTable.element[index_1].global){   //src2也是全局变量
                        if(findSaveReg(src2) != -1){   //src2在寄存器中
                            saveRegIndex_1 = findSaveReg(src2);
                            src2Index = saveRegIndex_1;
                            src2RegKind = 0;
                        }
                        else{   //src2在内存中
                            saveRegIndex_1 = findFreeSaveReg();
                            if(saveRegIndex_1 != -1){
                                saveReg[saveRegIndex_1].occupy = true;
                                strcpy(saveReg[saveRegIndex_1].varName,src2);
                                fprintf(mipsOut,"lw $s%d,%s\n",saveRegIndex_1,src2);
                                src2Index = saveRegIndex_1;
                                src2RegKind = 0;
                            }
                            else{
                                newRegIndex = findAregIndex(src1Index,0);
                                fprintf(mipsOut,"sw $s%d,%s\n",newRegIndex,saveReg[newRegIndex].varName);  //将值写回内存
                                fprintf(mipsOut,"lw $s%d,%s\n",newRegIndex,src2);      //将新值装入寄存器
                                strcpy(saveReg[newRegIndex].varName,src2);
                                src2Index = newRegIndex;
                                src2RegKind = 0;
                            }
                        }
                    }
                    else{
                        if(findTempReg(src2) != -1){
                            tempRegIndex_1 = findTempReg(src2);
                            src2Index = tempRegIndex_1;
                            src2RegKind = 1;
                        }
                        else{
                            tempRegIndex_1 = findFreeReg();
                            if(tempRegIndex_1 != -1){
                                tempReg[tempRegIndex_1].occupy = true;
                                strcpy(tempReg[tempRegIndex_1].varName,src2);
                                fprintf(mipsOut,"lw $t%d,-%d($fp)\n",tempRegIndex_1,offset_1);
                                src2Index = tempRegIndex_1;
                                src2RegKind = 1;
                            }
                            else{
                                newRegIndex = findAregIndex(src1Index,1);
                                oldtempRegindex = searchMipsTable(tempReg[newRegIndex].varName,funcIndex,false);
                                if(oldtempRegindex != -1){
                                    tempoffset = (myMipsTable.element[oldtempRegindex].offset + 1) * 4;
                                    fprintf(mipsOut,"sw $t%d,-%d($fp)\n",newRegIndex,tempoffset);
                                }
                                fprintf(mipsOut,"lw $t%d,-%d($fp)\n",newRegIndex,offset_1);
                                strcpy(tempReg[newRegIndex].varName,src2);
                                src2Index = newRegIndex;
                                src2RegKind = 1;
                            }
                        }
                    }
                }

                if(MidCodeSet[i].codeKind == 26){
                    strcpy(result,MidCodeSet[i].result);
                    index_2 = searchMipsTable(result,funcIndex,false);
                    offset_2 = (myMipsTable.element[index_2].offset + 1) * 4;
                    if(myMipsTable.element[index_2].global){   //result是全局变量
                        if(findSaveReg(result) != -1){   //result在寄存器中
                            saveRegIndex = findSaveReg(result);
                            resultIndex = saveRegIndex;
                            resultRegKind = 0;
                        }
                        else{   //result在内存中
                            saveRegIndex = findFreeSaveReg();
                            if(saveRegIndex != -1){
                                saveReg[saveRegIndex].occupy = true;
                                strcpy(saveReg[saveRegIndex].varName,result);
                                resultIndex = saveRegIndex;
                                resultRegKind = 0;
                            }
                            else{
                                fprintf(mipsOut,"sw $s2,%s\n",saveReg[2].varName);  //将值写回内存
                                strcpy(saveReg[2].varName,result);
                                resultIndex = 2;
                                resultRegKind = 0;
                            }
                        }
                    }
                    else{
                        if(findTempReg(result) != -1){
                            tempRegIndex = findTempReg(result);
                            resultIndex = tempRegIndex;
                            resultRegKind = 1;
                        }
                        else{
                            tempRegIndex = findFreeReg();
                            if(tempRegIndex != -1){
                                tempReg[tempRegIndex].occupy = true;
                                strcpy(tempReg[tempRegIndex].varName,result);
                                resultIndex = tempRegIndex;
                                resultRegKind = 1;
                            }
                            else{
                                oldtempRegindex = searchMipsTable(tempReg[2].varName,funcIndex,false);
                                if(oldtempRegindex != -1){
                                    tempoffset = (myMipsTable.element[oldtempRegindex].offset + 1) * 4;
                                    fprintf(mipsOut,"sw $t2,-%d($fp)\n",tempoffset);
                                }
                                strcpy(tempReg[2].varName,result);
                                resultIndex = 2;
                                resultRegKind = 1;
                            }
                        }
                    }
                    switch(op[0]){
                        case '+':{
                            if(resultRegKind == 0){
                                if(src1RegKind == 0 && src2RegKind == 0)
                                    fprintf(mipsOut,"addu $s%d,$s%d,$s%d\n",resultIndex,src1Index,src2Index);
                                else if(src1RegKind == 0 && src2RegKind == 1)
                                    fprintf(mipsOut,"addu $s%d,$s%d,$t%d\n",resultIndex,src1Index,src2Index);
                                else if(src1RegKind == 1 && src2RegKind == 0)
                                    fprintf(mipsOut,"addu $s%d,$t%d,$s%d\n",resultIndex,src1Index,src2Index);
                                else
                                    fprintf(mipsOut,"addu $s%d,$t%d,$t%d\n",resultIndex,src1Index,src2Index);
                                fprintf(mipsOut,"sw $s%d,%s\n",resultIndex,result);
                            }
                            else{
                                if(src1RegKind == 0 && src2RegKind == 0)
                                    fprintf(mipsOut,"addu $t%d,$s%d,$s%d\n",resultIndex,src1Index,src2Index);
                                else if(src1RegKind == 0 && src2RegKind == 1)
                                    fprintf(mipsOut,"addu $t%d,$s%d,$t%d\n",resultIndex,src1Index,src2Index);
                                else if(src1RegKind == 1 && src2RegKind == 0)
                                    fprintf(mipsOut,"addu $t%d,$t%d,$s%d\n",resultIndex,src1Index,src2Index);
                                else
                                    fprintf(mipsOut,"addu $t%d,$t%d,$t%d\n",resultIndex,src1Index,src2Index);
                                fprintf(mipsOut,"sw $t%d,-%d($fp)\n",resultIndex,offset_2);
                            }
                            break;
                        }
                        case '-':{
                            if(resultRegKind == 0){
                                if(src1RegKind == 0 && src2RegKind == 0)
                                    fprintf(mipsOut,"subu $s%d,$s%d,$s%d\n",resultIndex,src1Index,src2Index);
                                else if(src1RegKind == 0 && src2RegKind == 1)
                                    fprintf(mipsOut,"subu $s%d,$s%d,$t%d\n",resultIndex,src1Index,src2Index);
                                else if(src1RegKind == 1 && src2RegKind == 0)
                                    fprintf(mipsOut,"subu $s%d,$t%d,$s%d\n",resultIndex,src1Index,src2Index);
                                else
                                    fprintf(mipsOut,"subu $s%d,$t%d,$t%d\n",resultIndex,src1Index,src2Index);
                                fprintf(mipsOut,"sw $s%d,%s\n",resultIndex,result);
                            }
                            else{
                                if(src1RegKind == 0 && src2RegKind == 0)
                                    fprintf(mipsOut,"subu $t%d,$s%d,$s%d\n",resultIndex,src1Index,src2Index);
                                else if(src1RegKind == 0 && src2RegKind == 1)
                                    fprintf(mipsOut,"subu $t%d,$s%d,$t%d\n",resultIndex,src1Index,src2Index);
                                else if(src1RegKind == 1 && src2RegKind == 0)
                                    fprintf(mipsOut,"subu $t%d,$t%d,$s%d\n",resultIndex,src1Index,src2Index);
                                else
                                    fprintf(mipsOut,"subu $t%d,$t%d,$t%d\n",resultIndex,src1Index,src2Index);
                                fprintf(mipsOut,"sw $t%d,-%d($fp)\n",resultIndex,offset_2);
                            }
                            break;
                        }
                        case '*':{
                            if(src1RegKind == 0 && src2RegKind == 0)
                                fprintf(mipsOut,"mult $s%d,$s%d\n",src1Index,src2Index);
                            else if(src1RegKind == 0 && src2RegKind == 1)
                                fprintf(mipsOut,"mult $s%d,$t%d\n",src1Index,src2Index);
                            else if(src1RegKind == 1 && src2RegKind == 0)
                                fprintf(mipsOut,"mult $t%d,$s%d\n",src1Index,src2Index);
                            else
                                fprintf(mipsOut,"mult $t%d,$t%d\n",src1Index,src2Index);
                            if(resultRegKind == 0){
                                fprintf(mipsOut,"mflo $s%d\n",resultIndex);
                                fprintf(mipsOut,"sw $s%d,%s\n",resultIndex,result);
                            }
                            else{
                                fprintf(mipsOut,"mflo $t%d\n",resultIndex);
                                fprintf(mipsOut,"sw $t%d,-%d($fp)\n",resultIndex,offset_2);
                            }
                            break;
                        }
                        case '/':{
                            if(src1RegKind == 0 && src2RegKind == 0)
                                fprintf(mipsOut,"div $s%d,$s%d\n",src1Index,src2Index);
                            else if(src1RegKind == 0 && src2RegKind == 1)
                                fprintf(mipsOut,"div $s%d,$t%d\n",src1Index,src2Index);
                            else if(src1RegKind == 1 && src2RegKind == 0)
                                fprintf(mipsOut,"div $t%d,$s%d\n",src1Index,src2Index);
                            else
                                fprintf(mipsOut,"div $t%d,$t%d\n",src1Index,src2Index);
                            if(resultRegKind == 0){
                                fprintf(mipsOut,"mflo $s%d\n",resultIndex);
                                fprintf(mipsOut,"sw $s%d,%s\n",resultIndex,result);
                            }
                            else{
                                fprintf(mipsOut,"mflo $t%d\n",resultIndex);
                                fprintf(mipsOut,"sw $t%d,-%d($fp)\n",resultIndex,offset_2);
                            }
                            break;
                        }
                    }
                }
                else if(MidCodeSet[i].codeKind == 22){
                    strcpy(result,MidCodeSet[i].result);
                    index_2 = searchMipsTable(result,funcIndex,false);
                    offset_2 = (myMipsTable.element[index_2].offset + 1) * 4;
                    //数组元素，不用修改寄存器，只需要写回内存
                    if(myMipsTable.element[index_2].global){
                        if(src1RegKind == 0 && src2RegKind == 0){
                            fprintf(mipsOut,"sll $v1,$s%d,2\n",src1Index);
                            fprintf(mipsOut,"sw $s%d,%s($v1)\n",src2Index,result);
                        }
                        else if(src1RegKind == 0 && src2RegKind == 1){
                            fprintf(mipsOut,"sll $v1,$s%d,2\n",src1Index);
                            fprintf(mipsOut,"sw $t%d,%s($v1)\n",src2Index,result);
                        }
                        else if(src1RegKind == 1 && src2RegKind == 0){
                            fprintf(mipsOut,"sll $v1,$t%d,2\n",src1Index);
                            fprintf(mipsOut,"sw $s%d,%s($v1)\n",src2Index,result);
                        }
                        else{
                            fprintf(mipsOut,"sll $v1,$t%d,2\n",src1Index);
                            fprintf(mipsOut,"sw $t%d,%s($v1)\n",src2Index,result);
                        }
                    }
                    else{
                        if(src1RegKind == 0){
                            fprintf(mipsOut,"sll $v1,$s%d,2\n",src1Index);
                            fprintf(mipsOut,"addiu $v1,$v1,%d\n",offset_2);
                            if(src2RegKind == 0){
                                fprintf(mipsOut,"subu $fp,$fp,$v1\n");
                                fprintf(mipsOut,"sw $s%d,0($fp)\n",src2Index);
                                fprintf(mipsOut,"addu $fp,$fp,$v1\n");
                            }
                            else{
                                fprintf(mipsOut,"subu $fp,$fp,$v1\n");
                                fprintf(mipsOut,"sw $t%d,0($fp)\n",src2Index);
                                fprintf(mipsOut,"addu $fp,$fp,$v1\n");
                            }
                        }
                        else{
                            fprintf(mipsOut,"sll $v1,$t%d,2\n",src1Index);
                            fprintf(mipsOut,"addiu $v1,$v1,%d\n",offset_2);
                            if(src2RegKind == 0){
                                fprintf(mipsOut,"subu $fp,$fp,$v1\n");
                                fprintf(mipsOut,"sw $s%d,0($fp)\n",src2Index);
                                fprintf(mipsOut,"addu $fp,$fp,$v1\n");
                            }
                            else{
                                fprintf(mipsOut,"subu $fp,$fp,$v1\n");
                                fprintf(mipsOut,"sw $t%d,0($fp)\n",src2Index);
                                fprintf(mipsOut,"addu $fp,$fp,$v1\n");
                            }
                        }
                    }
                }
                else if(MidCodeSet[i].codeKind == 27){
                    strcpy(result,MidCodeSet[i].src1);
                    index_2 = searchMipsTable(result,funcIndex,false);
                    offset_2 = (myMipsTable.element[index_2].offset + 1) * 4;
                    if(myMipsTable.element[index_2].global){
                        if(src1RegKind == 0 && src2RegKind == 0){
                            fprintf(mipsOut,"sll $v1,$s%d,2\n",src2Index);
                            fprintf(mipsOut,"lw $s%d,%s($v1)\n",src1Index,result);
                            fprintf(mipsOut,"sw $s%d,%s\n",src1Index,src1);
                        }
                        else if(src1RegKind == 0 && src2RegKind == 1){
                            fprintf(mipsOut,"sll $v1,$t%d,2\n",src2Index);
                            fprintf(mipsOut,"lw $s%d,%s($v1)\n",src1Index,result);
                            fprintf(mipsOut,"sw $s%d,%s\n",src1Index,src1);
                        }
                        else if(src1RegKind == 1 && src2RegKind == 0){
                            fprintf(mipsOut,"sll $v1,$s%d,2\n",src2Index);
                            fprintf(mipsOut,"lw $t%d,%s($v1)\n",src1Index,result);
                            fprintf(mipsOut,"sw $t%d,-%d($fp)\n",src1Index,offset);
                        }
                        else{
                            fprintf(mipsOut,"sll $v1,$t%d,2\n",src2Index);
                            fprintf(mipsOut,"lw $t%d,%s($v1)\n",src1Index,result);
                            fprintf(mipsOut,"sw $t%d,-%d($fp)\n",src1Index,offset);
                        }
                    }
                    else{
                        if(src2RegKind == 0){
                            fprintf(mipsOut,"sll $v1,$s%d,2\n",src2Index);
                            fprintf(mipsOut,"addiu $v1,$v1,%d\n",offset_2);
                            if(src1RegKind == 0){
                                fprintf(mipsOut,"subu $fp,$fp,$v1\n");
                                fprintf(mipsOut,"lw $s%d,0($fp)\n",src1Index);
                                fprintf(mipsOut,"addu $fp,$fp,$v1\n");
                                fprintf(mipsOut,"sw $s%d,%s\n",src1Index,src1);
                            }
                            else{
                                fprintf(mipsOut,"subu $fp,$fp,$v1\n");
                                fprintf(mipsOut,"lw $t%d,0($fp)\n",src1Index);
                                fprintf(mipsOut,"addu $fp,$fp,$v1\n");
                                fprintf(mipsOut,"sw $t%d,-%d($fp)\n",src1Index,offset);
                            }
                        }
                        else{
                            fprintf(mipsOut,"sll $v1,$t%d,2\n",src2Index);
                            fprintf(mipsOut,"addiu $v1,$v1,%d\n",offset_2);
                            if(src1RegKind == 0){
                                fprintf(mipsOut,"subu $fp,$fp,$v1\n");
                                fprintf(mipsOut,"lw $s%d,0($fp)\n",src1Index);
                                fprintf(mipsOut,"addu $fp,$fp,$v1\n");
                                fprintf(mipsOut,"sw $s%d,%s\n",src1Index,src1);
                            }
                            else{
                                fprintf(mipsOut,"subu $fp,$fp,$v1\n");
                                fprintf(mipsOut,"lw $t%d,0($fp)\n",src1Index);
                                fprintf(mipsOut,"addu $fp,$fp,$v1\n");
                                fprintf(mipsOut,"sw $t%d,-%d($fp)\n",src1Index,offset);
                            }
                        }
                    }
                }
                else{
                    if(src1RegKind == 0 && src2RegKind == 0){
                        if(strcmp(op,"<") == 0)
                            fprintf(mipsOut,"bge $s%d,$s%d,%s\n",src1Index,src2Index,MidCodeSet[i].result);
                        if(strcmp(op,">") == 0)
                            fprintf(mipsOut,"ble $s%d,$s%d,%s\n",src1Index,src2Index,MidCodeSet[i].result);
                        if(strcmp(op,"<=") == 0)
                            fprintf(mipsOut,"bgt $s%d,$s%d,%s\n",src1Index,src2Index,MidCodeSet[i].result);
                        if(strcmp(op,">=") == 0)
                            fprintf(mipsOut,"blt $s%d,$s%d,%s\n",src1Index,src2Index,MidCodeSet[i].result);
                        if(strcmp(op,"==") == 0)
                            fprintf(mipsOut,"bne $s%d,$s%d,%s\n",src1Index,src2Index,MidCodeSet[i].result);
                        if(strcmp(op,"!=") == 0)
                            fprintf(mipsOut,"beq $s%d,$s%d,%s\n",src1Index,src2Index,MidCodeSet[i].result);
                    }
                    else if(src1RegKind == 0 && src2RegKind == 1){
                        if(strcmp(op,"<") == 0)
                            fprintf(mipsOut,"bge $s%d,$t%d,%s\n",src1Index,src2Index,MidCodeSet[i].result);
                        if(strcmp(op,">") == 0)
                            fprintf(mipsOut,"ble $s%d,$t%d,%s\n",src1Index,src2Index,MidCodeSet[i].result);
                        if(strcmp(op,"<=") == 0)
                            fprintf(mipsOut,"bgt $s%d,$t%d,%s\n",src1Index,src2Index,MidCodeSet[i].result);
                        if(strcmp(op,">=") == 0)
                            fprintf(mipsOut,"blt $s%d,$t%d,%s\n",src1Index,src2Index,MidCodeSet[i].result);
                        if(strcmp(op,"==") == 0)
                            fprintf(mipsOut,"bne $s%d,$t%d,%s\n",src1Index,src2Index,MidCodeSet[i].result);
                        if(strcmp(op,"!=") == 0)
                            fprintf(mipsOut,"beq $s%d,$t%d,%s\n",src1Index,src2Index,MidCodeSet[i].result);
                    }
                    else if(src1RegKind == 1 && src2RegKind == 0){
                        if(strcmp(op,"<") == 0)
                            fprintf(mipsOut,"bge $t%d,$s%d,%s\n",src1Index,src2Index,MidCodeSet[i].result);
                        if(strcmp(op,">") == 0)
                            fprintf(mipsOut,"ble $t%d,$s%d,%s\n",src1Index,src2Index,MidCodeSet[i].result);
                        if(strcmp(op,"<=") == 0)
                            fprintf(mipsOut,"bgt $t%d,$s%d,%s\n",src1Index,src2Index,MidCodeSet[i].result);
                        if(strcmp(op,">=") == 0)
                            fprintf(mipsOut,"blt $t%d,$s%d,%s\n",src1Index,src2Index,MidCodeSet[i].result);
                        if(strcmp(op,"==") == 0)
                            fprintf(mipsOut,"bne $t%d,$s%d,%s\n",src1Index,src2Index,MidCodeSet[i].result);
                        if(strcmp(op,"!=") == 0)
                            fprintf(mipsOut,"beq $t%d,$s%d,%s\n",src1Index,src2Index,MidCodeSet[i].result);
                    }
                    else{
                        if(strcmp(op,"<") == 0)
                            fprintf(mipsOut,"bge $t%d,$t%d,%s\n",src1Index,src2Index,MidCodeSet[i].result);
                        if(strcmp(op,">") == 0)
                            fprintf(mipsOut,"ble $t%d,$t%d,%s\n",src1Index,src2Index,MidCodeSet[i].result);
                        if(strcmp(op,"<=") == 0)
                            fprintf(mipsOut,"bgt $t%d,$t%d,%s\n",src1Index,src2Index,MidCodeSet[i].result);
                        if(strcmp(op,">=") == 0)
                            fprintf(mipsOut,"blt $t%d,$t%d,%s\n",src1Index,src2Index,MidCodeSet[i].result);
                        if(strcmp(op,"==") == 0)
                            fprintf(mipsOut,"bne $t%d,$t%d,%s\n",src1Index,src2Index,MidCodeSet[i].result);
                        if(strcmp(op,"!=") == 0)
                            fprintf(mipsOut,"beq $t%d,$t%d,%s\n",src1Index,src2Index,MidCodeSet[i].result);
                    }
                }
                break;
            }
            case 17:{
                fprintf(mipsOut,"j %s\n",MidCodeSet[i].src1);
                break;
            }
            case 18:
            case 19:{
                saveCurrent(funcIndex);//保存寄存器现场
                fprintf(mipsOut,"jal %s\n",MidCodeSet[i].op);
                recoverCurrent(funcIndex);
                pushedParaNum = 0;
                break;
            }
            case 20:{
                pushedParaNum++;
                strcpy(src1,MidCodeSet[i].src1);
                index = searchMipsTable(MidCodeSet[i].src1,funcIndex,false);
                if(index == -1){
                    src1Value = calVarValue(src1);
                    if(pushedParaNum > 4){
                        fprintf(mipsOut,"sw $a0,0($sp)\n");
                        fprintf(mipsOut,"li $a0,%d\n",src1Value);
                        fprintf(mipsOut,"sw $a0,-%d($sp)\n",(pushedParaNum + 1) * 4);
                        fprintf(mipsOut,"lw $a0,0($sp)\n");
                    }
                    else
                        fprintf(mipsOut,"li $a%d,%d\n",pushedParaNum - 1,src1Value);
                }
                else{
                    offset = (myMipsTable.element[index].offset + 1) * 4;
                    if(myMipsTable.element[index].global){      //如果是全局变量
                        if(findSaveReg(src1) != -1){  //已经保存到了寄存器中
                            saveRegIndex = findSaveReg(src1);
                            if(pushedParaNum > 4)
                                fprintf(mipsOut,"sw $s%d,-%d($sp)\n",saveRegIndex,(pushedParaNum + 1) * 4);
                            else
                                fprintf(mipsOut,"move $a%d,$s%d\n",pushedParaNum - 1,saveRegIndex);
                        }
                        else{     //还没有保存到寄存器中
                            saveRegIndex = findFreeSaveReg();
                            if(saveRegIndex != -1){
                                fprintf(mipsOut,"lw $s%d,%s\n",saveRegIndex,src1);
                                if(pushedParaNum > 4)
                                    fprintf(mipsOut,"sw $s%d,-%d($sp)\n",saveRegIndex,(pushedParaNum + 1) * 4);
                                else
                                    fprintf(mipsOut,"move $a%d,$s%d\n",pushedParaNum - 1,saveRegIndex);
                                saveReg[saveRegIndex].occupy = true;
                                strcpy(saveReg[saveRegIndex].varName,src1);
                            }
                            else{
                                fprintf(mipsOut,"sw $s0,%s\n",saveReg[0].varName);  //将值写回内存
                                fprintf(mipsOut,"lw $s0,%s\n",src1);      //将新值装入寄存器
                                strcpy(saveReg[0].varName,src1);
                                if(pushedParaNum > 4)
                                    fprintf(mipsOut,"sw $s0,-%d($sp)\n",(pushedParaNum + 1) * 4);
                                else
                                    fprintf(mipsOut,"move $a%d,$s0\n",pushedParaNum - 1);
                            }
                        }
                    }
                    else{              //局部变量和中间变量
                        if(findTempReg(src1) != -1){ //该变量已经存入寄存器
                            if(pushedParaNum > 4)
                                fprintf(mipsOut,"sw $t%d,-%d($sp)\n",findTempReg(src1),(pushedParaNum + 1) * 4);
                            else
                                fprintf(mipsOut,"move $a%d,$t%d\n",pushedParaNum - 1,findTempReg(src1));
                        }
                        else{
                            tempRegIndex = findFreeReg();
                            if(tempRegIndex != -1){   //说明找到空闲的寄存器
                                tempReg[tempRegIndex].occupy = true;
                                strcpy(tempReg[tempRegIndex].varName,src1);
                                fprintf(mipsOut,"lw $t%d,-%d($fp)\n",tempRegIndex,offset);
                                if(pushedParaNum > 4)
                                    fprintf(mipsOut,"sw $t%d,-%d($sp)\n",tempRegIndex,(pushedParaNum + 1) * 4);
                                else
                                    fprintf(mipsOut,"move $a%d,$t%d\n",pushedParaNum - 1,tempRegIndex);
                            }
                            else{
                                oldtempRegindex = searchMipsTable(tempReg[0].varName,funcIndex,false);
                                if(oldtempRegindex != -1){
                                    tempoffset = (myMipsTable.element[oldtempRegindex].offset + 1) * 4;
                                    fprintf(mipsOut,"sw $t0,-%d($fp)\n",tempoffset);
                                }
                                fprintf(mipsOut,"lw $t0,-%d($fp)\n",offset);
                                strcpy(tempReg[0].varName,src1);
                                if(pushedParaNum > 4)
                                    fprintf(mipsOut,"sw $t0,-%d($sp)\n",(pushedParaNum + 1) * 4);
                                else
                                    fprintf(mipsOut,"move $a%d,$t0\n",pushedParaNum - 1);
                            }
                        }
                    }
                }
                break;
            }
            case 23:{
                strcpy(src1,MidCodeSet[i].src1);
                index = searchMipsTable(src1,funcIndex,false);
                offset = (myMipsTable.element[index].offset + 1) * 4;
                if(myMipsTable.element[index].global){   //如果是全局变量
                    if(findSaveReg(src1) != -1){  //已经保存到了寄存器中
                        saveRegIndex = findSaveReg(src1);
                        src1Index = saveRegIndex;
                        src1RegKind = 0;
                    }
                    else{   //还没有保存到寄存器中
                        saveRegIndex = findFreeSaveReg();
                        if(saveRegIndex != -1){
                            saveReg[saveRegIndex].occupy = true;
                            strcpy(saveReg[saveRegIndex].varName,src1);
                            src1Index = saveRegIndex;
                            src1RegKind = 0;
                        }
                        else{
                            fprintf(mipsOut,"sw $s0,%s\n",saveReg[0].varName);  //将值写回内存
                            strcpy(saveReg[0].varName,src1);
                            src1Index = 0;
                            src1RegKind = 0;
                        }
                    }
                }
                else{
                    if(findTempReg(src1) != -1){
                        tempRegIndex = findTempReg(src1);
                        src1Index = tempRegIndex;
                        src1RegKind = 1;
                    }
                    else{
                        tempRegIndex = findFreeReg();
                        if(tempRegIndex != -1){
                            tempReg[tempRegIndex].occupy = true;
                            strcpy(tempReg[tempRegIndex].varName,src1);
                            src1Index = tempRegIndex;
                            src1RegKind = 1;
                        }
                        else{
                            oldtempRegindex = searchMipsTable(tempReg[0].varName,funcIndex,false);
                            if(oldtempRegindex != -1){
                                tempoffset = (myMipsTable.element[oldtempRegindex].offset + 1) * 4;
                                fprintf(mipsOut,"sw $t0,-%d($fp)\n",tempoffset);
                            }
                            strcpy(tempReg[0].varName,src1);
                            src1Index = 0;
                            src1RegKind = 1;
                        }
                    }
                }
                if(myMipsTable.element[index].type == 1){
                    fprintf(mipsOut,"li $v0,12\n");
                    fprintf(mipsOut,"syscall\n");
                }
                else{
                    fprintf(mipsOut,"li $v0,5\n");
                    fprintf(mipsOut,"syscall\n");
                }
                if(src1RegKind == 0){
                    fprintf(mipsOut,"move $s%d,$v0\n",src1Index);
                    fprintf(mipsOut,"sw $s%d,%s\n",src1Index,src1);
                }
                else{
                    fprintf(mipsOut,"move $t%d,$v0\n",src1Index);
                    fprintf(mipsOut,"sw $t%d,-%d($fp)\n",src1Index,offset);
                }
                break;
            }
            case 24:{
                strcpy(src1,MidCodeSet[i].op);
                strcpy(src2,MidCodeSet[i].src1);
                if(strlen(src2) != 0){
                    fprintf(mipsOut,"la $a0,string%d\n",i);
                    fprintf(mipsOut,"li $v0,4\n");
                    fprintf(mipsOut,"syscall\n");
                }
                if(strlen(src1) != 0){
                    index = searchMipsTable(src1,funcIndex,false);
                    if(index == -1){
                        src1Value = calVarValue(src1);
                        fprintf(mipsOut,"li $a0,%d\n",src1Value);
                        if(ifNum(src1)){
                            fprintf(mipsOut,"li $v0,1\n");
                            fprintf(mipsOut,"syscall\n");
                        }
                        else{
                            fprintf(mipsOut,"li $v0,11\n");
                            fprintf(mipsOut,"syscall\n");
                        }
                    }
                    else{
                        offset = (myMipsTable.element[index].offset + 1) * 4;
                        if(myMipsTable.element[index].global){   //如果是全局变量
                            if(findSaveReg(src1) != -1){  //已经保存到了寄存器中
                                saveRegIndex = findSaveReg(src1);
                                src1Index = saveRegIndex;
                                src1RegKind = 0;
                            }
                            else{   //还没有保存到寄存器中
                                saveRegIndex = findFreeSaveReg();
                                if(saveRegIndex != -1){
                                    fprintf(mipsOut,"lw $s%d,%s\n",saveRegIndex,src1);
                                    saveReg[saveRegIndex].occupy = true;
                                    strcpy(saveReg[saveRegIndex].varName,src1);
                                    src1Index = saveRegIndex;
                                    src1RegKind = 0;
                                }
                                else{
                                    fprintf(mipsOut,"sw $s0,%s\n",saveReg[0].varName);  //将值写回内存
                                    fprintf(mipsOut,"lw $s0,%s\n",src1);      //将新值装入寄存器
                                    strcpy(saveReg[0].varName,src1);
                                    src1Index = 0;
                                    src1RegKind = 0;
                                }
                            }
                        }
                        else{
                            if(findTempReg(src1) != -1){
                                tempRegIndex = findTempReg(src1);
                                src1Index = tempRegIndex;
                                src1RegKind = 1;
                            }
                            else{
                                tempRegIndex = findFreeReg();
                                if(tempRegIndex != -1){
                                    tempReg[tempRegIndex].occupy = true;
                                    strcpy(tempReg[tempRegIndex].varName,src1);
                                    fprintf(mipsOut,"lw $t%d,-%d($fp)\n",tempRegIndex,offset);
                                    src1Index = tempRegIndex;
                                    src1RegKind = 1;
                                }
                                else{
                                    oldtempRegindex = searchMipsTable(tempReg[0].varName,funcIndex,false);
                                    if(oldtempRegindex != -1){
                                        tempoffset = (myMipsTable.element[oldtempRegindex].offset + 1) * 4;
                                        fprintf(mipsOut,"sw $t0,-%d($fp)\n",tempoffset);
                                    }
                                    fprintf(mipsOut,"lw $t0,-%d($fp)\n",offset);
                                    strcpy(tempReg[0].varName,src1);
                                    src1Index = 0;
                                    src1RegKind = 1;
                                }

                            }
                        }
                        if(src1RegKind == 0){
                            fprintf(mipsOut,"move $a0,$s%d\n",src1Index);
                            if(myMipsTable.element[index].type == 1){
                                fprintf(mipsOut,"li $v0,11\n");
                                fprintf(mipsOut,"syscall\n");
                            }
                            else{
                                fprintf(mipsOut,"li $v0,1\n");
                                fprintf(mipsOut,"syscall\n");
                            }
                        }
                        else{
                            fprintf(mipsOut,"move $a0,$t%d\n",src1Index);
                            if(myMipsTable.element[index].type == 1){
                                fprintf(mipsOut,"li $v0,11\n");
                                fprintf(mipsOut,"syscall\n");
                            }
                            else{
                                fprintf(mipsOut,"li $v0,1\n");
                                fprintf(mipsOut,"syscall\n");
                            }
                        }
                    }
                }
                fprintf(mipsOut,"la $a0,string%d\n",codeCount);
                fprintf(mipsOut,"li $v0,4\n");
                fprintf(mipsOut,"syscall\n");
                break;
            }
            case 28:{
                strcpy(src1,MidCodeSet[i].result);
                index = searchMipsTable(src1,funcIndex,false);
                offset = (myMipsTable.element[index].offset + 1) * 4;
                //分为全局变量和局部变量两种方式去修改值
                if(myMipsTable.element[index].global){
                    fprintf(mipsOut,"sw $v0,%s\n",src1);
                    //更新寄存器的值
                    for(int j = 0;j < 8;j++){
                        if(saveReg[j].occupy && strcmp(saveReg[j].varName,src1) == 0)
                            fprintf(mipsOut,"move $s%d,$v0\n",j);
                    }
                }
                else{
                    fprintf(mipsOut,"sw $v0,-%d($fp)\n",offset);
                    //更新寄存器的值
                    for(int j = 0;j < 10;j++){
                        if(tempReg[j].occupy && strcmp(tempReg[j].varName,src1) == 0)
                            fprintf(mipsOut,"move $t%d,$v0\n",j);
                    }
                }
                break;
            }
            default:
                fprintf(mipsOut,"ERROR!\n");
        }
    }
}

void initReg(){
    for(int i = 0;i < 10;i++){
        tempReg[i].occupy = false;
        tempReg[i].index = i;
        sprintf(tempReg[i].regName,"t%d",i);
    }
    for(int i = 0;i < 8;i++){
        saveReg[i].occupy = false;
        saveReg[i].index = i;
        sprintf(saveReg[i].regName,"s%d",i);
    }
}

void genMipsCode(){
    Create_Mips_Table();
    //printTable();
    MipsData();
    fprintf(mipsOut,"\n");
    fprintf(mipsOut,".text\n");
    fprintf(mipsOut,"j main\n");
    initReg();   //初始化寄存器
    MipsText();
}
