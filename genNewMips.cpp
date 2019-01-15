#include <iostream>
#include <stdio.h>
#include <string.h>
#include "global.h"

NewTable NewMipsTable;

reg tReg[15];      //代表t0-t9寄存器
reg sReg[10];      //代表s0-s7
reg vReg[5];       //代表v0-v1
reg aReg[5];       //代表a0-a3
bool sReg_use[1000][10];
int astack[10];
int a_point = 0;

void CreateNewTable();
void GenData();
void GenText();
void printNewTable();
int getFuncIndex(char name[]);
void initRegister();
int cal_Value(char name[]);
bool is_Num(char name[]);
int find_t_reg(char name[]);
int find_s_reg(char name[]);
int find_a_reg(char name[]);
int find_new_s_reg(int blockIndex,char name[]);
int find_free_s_reg(int index);
int find_free_t_reg();
void saveState(int funcofIndex,int call_index);
void recoverState(int funcofIndex,int call_index);
bool find_if_next(int index,char name[]);    //判断该中间变量接下来是否还会用到
void allocReg(int funcIndex,int blockIndex);
void freeReg(int blockIndex,int blockNum);
bool find_if_use(int blockIndex,int blockNum,char name[]);
bool func_use(int funcIndex,int s_index);
int searchFucIndex(char name[]);


void CreateNewTable(){     //构建代码生成时的符号表
    int startindex,endindex;
    int midsart,midend,offset;
    char tempvarName[10];
    NewMipsTable.numOfFunc = Mytable.numOfFunc;
    NewMipsTable.topIndex = -1;
    for(int i = 0;i < Mytable.funcIndex[0];i++){
        NewMipsTable.topIndex++;
        strcpy(NewMipsTable.element[i].name,Mytable.element[i].name);
        NewMipsTable.element[i].kind = Mytable.element[i].kind;
        NewMipsTable.element[i].type = Mytable.element[i].type;
        NewMipsTable.element[i].value = Mytable.element[i].value;
        NewMipsTable.element[i].capicity = Mytable.element[i].capicity;
        NewMipsTable.element[i].offset = 0;       //全局变量的偏移已经在.data段设置好
        NewMipsTable.element[i].global = true;
        NewMipsTable.element[i].RegNum = -1;
    }
    for(int i = 0;i < Mytable.numOfFunc;i++){
        offset = 0;
        NewMipsTable.funcIndex[i] = NewMipsTable.topIndex + 1;
        startindex = Mytable.funcIndex[i];
        if(i == Mytable.numOfFunc - 1)
            endindex = Mytable.topIndex;
        else
            endindex = Mytable.funcIndex[i+1] - 1;
        for(int j = startindex;j <= endindex;j++){
            NewMipsTable.topIndex++;
            strcpy(NewMipsTable.element[NewMipsTable.topIndex].name,Mytable.element[j].name);
            NewMipsTable.element[NewMipsTable.topIndex].kind = Mytable.element[j].kind;
            NewMipsTable.element[NewMipsTable.topIndex].type = Mytable.element[j].type;
            NewMipsTable.element[NewMipsTable.topIndex].value = Mytable.element[j].value;
            NewMipsTable.element[NewMipsTable.topIndex].capicity = Mytable.element[j].capicity;
            NewMipsTable.element[NewMipsTable.topIndex].offset = offset;
            NewMipsTable.element[NewMipsTable.topIndex].global = false;
            NewMipsTable.element[NewMipsTable.topIndex].RegNum = -1;
            if(Mytable.element[j].kind == 4)
                offset += Mytable.element[j].capicity;
            else
                offset += 1;
        }
        midsart = Mytable.element[Mytable.funcIndex[i]].midStart;
        midend = Mytable.element[Mytable.funcIndex[i]].midEnd;
        for(int j = midsart;j <= midend;j++){
            NewMipsTable.topIndex++;
            sprintf(tempvarName,"$t%d",j);
            strcpy(NewMipsTable.element[NewMipsTable.topIndex].name,tempvarName);
            NewMipsTable.element[NewMipsTable.topIndex].kind = 5;
            NewMipsTable.element[NewMipsTable.topIndex].type = getTempType(tempvarName);
            NewMipsTable.element[NewMipsTable.topIndex].value = 0;    //先把中间变量都初始化为0
            NewMipsTable.element[NewMipsTable.topIndex].capicity = -1;
            NewMipsTable.element[NewMipsTable.topIndex].offset = offset;
            NewMipsTable.element[NewMipsTable.topIndex].global = false;
            NewMipsTable.element[NewMipsTable.topIndex].RegNum = -1;
            offset += 1;
        }
        NewMipsTable.element[NewMipsTable.funcIndex[i]].functSize = offset;
    }
}

int searchFucIndex(char name[]){
    int index;
    for(int i = 0;i < NewMipsTable.numOfFunc;i++){
        index = NewMipsTable.funcIndex[i];
        if(strcmp(NewMipsTable.element[index].name,name) == 0)
            return i;       //返回是第几个函数
    }
}

int searchNewTable(char name[],int funcofIndex,bool ifFunction){    //funcofIndex从0开始
    int index,startIndex,endIndex;
    if(ifFunction){
        for(int i = 0;i < NewMipsTable.numOfFunc;i++){
            index = NewMipsTable.funcIndex[i];
            if(strcmp(NewMipsTable.element[index].name,name) == 0)
                return index;       //返回在符号表中的索引
        }
        return -1;       //返回-1说明没有找到
    }
    else{
        //先在同层查找
        if(funcofIndex == NewMipsTable.numOfFunc - 1){
            startIndex = NewMipsTable.funcIndex[funcofIndex];
            endIndex = NewMipsTable.topIndex;
        }
        else{
            startIndex = NewMipsTable.funcIndex[funcofIndex];
            endIndex = NewMipsTable.funcIndex[funcofIndex + 1] - 1;
        }
        for(int i = startIndex;i <= endIndex;i++){
            if(strcmp(NewMipsTable.element[i].name,name) == 0)
                return i;
        }
        //没找到的话，在全局变量中查找
        for(int i = 0;i < NewMipsTable.funcIndex[0];i++){
            if(strcmp(NewMipsTable.element[i].name,name) == 0)
                return i;
        }
        return -1;
    }
}

int getFuncIndex(char name[]){    //查询是第几个函数，从1开始
    for(int i = 0;i < NewMipsTable.numOfFunc;i++){
        if(strcmp(NewMipsTable.element[NewMipsTable.funcIndex[i]].name,name) == 0)
            return i+1;
    }
    return -1;
}

int getTempType(char name[]){
    for(int i = 0;i < tempNum;i++){
        if(strcmp(name,tempTable[i].tempvarName) == 0){
            return tempTable[i].type;
        }
    }
    return -1;
}

void initRegister(){
    for(int i = 0;i < 10;i++){
        tReg[i].occupy = false;
        tReg[i].index = i;
        sprintf(tReg[i].regName,"t%d",i);
    }
    for(int i = 0;i < 8;i++){
        sReg[i].occupy = false;
        sReg[i].index = i;
        sprintf(sReg[i].regName,"s%d",i);
    }
    for(int i = 0;i < 2;i++){
        vReg[i].occupy = false;
        vReg[i].index = i;
        sprintf(vReg[i].regName,"v%d",i);
    }
    for(int i = 0;i < 4;i++){
        aReg[i].occupy = false;
        aReg[i].index = i;
        sprintf(aReg[i].regName,"a%d",i);
    }
}

int cal_Value(char name[]){
    int value = 0,signs = 1;
    if(is_Num(name)){
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

bool is_Num(char name[]){
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

int find_a_reg(char name[]){
    for(int i = 0;i < 4;i++){
        if(aReg[i].occupy && strcmp(aReg[i].varName,name) == 0){
            return i;
        }
    }
    return -1;
}

int find_free_a_reg(){
    for(int i = 0;i < 4;i++){
        if(!aReg[i].occupy)
            return i;
    }
    return -1;
}

int find_s_reg(char name[]){
    for(int i = 0;i < 8;i++){
        if(sReg[i].occupy && strcmp(sReg[i].varName,name) == 0){
            return i;
        }
    }
    return -1;
}

int find_free_s_reg(int index){
    for(int i = 0;i < 8;i++){
        if(!sReg[i].occupy){
            sReg_use[index][i] = true;
            return i;
        }
    }
    return -1;
}

int find_new_s_reg(int blockIndex,char name[]){
    char varName[100];
    bool if_in = false;
    for(int j = 0;j < info[blockIndex].out_num;j++){
        if(strcmp(NewMipsTable.element[info[blockIndex].out[j]].name,name) == 0)
            if_in = true;
    }
    if(if_in){
        for(int i = 0;i < 8;i++){
            strcpy(varName,sReg[i].varName);
            if_in = false;
            for(int j = 0;j < info[blockIndex].out_num;j++){
                if(strcmp(varName,NewMipsTable.element[info[blockIndex].out[j]].name) == 0)
                    if_in = true;
            }
            if(!if_in)
                return i;
        }
    }
    return -1;
}

int find_t_reg(char name[]){
    for(int i = 0;i < 10;i++){
        if(tReg[i].occupy && strcmp(tReg[i].varName,name) == 0){
            return i;
        }
    }
    return -1;
}

int find_free_t_reg(){
    for(int i = 0;i < 10;i++){
        if(!tReg[i].occupy)
            return i;
    }
    return -1;
}

bool find_if_next(int index,char name[]){
    for(int i = index + 1;i < codeCount;i++){
        if(strcmp(MidCodeSet[i].result,name) == 0)
            return true;
        else if(strcmp(MidCodeSet[i].src1,name) == 0)
            return true;
        else if(strcmp(MidCodeSet[i].op,name) == 0)
            return true;
        else if(strcmp(MidCodeSet[i].src2,name) == 0)
            return true;
        else
            continue;
    }
    return false;
}


bool func_use(int funcIndex,int s_index){    //如果调用的函数，没有使用寄存器，那么就不保存
    int startIndex,endIndex;
    bool if_have_call = false;
    startIndex = block_list[funcIndex].blocks[0].startIndex;
    endIndex = block_list[funcIndex].blocks[block_list[funcIndex].blockNum - 1].endIndex;
    for(int i = startIndex;i <= endIndex;i++){
        if(MidCodeSet[i].codeKind == 18 || MidCodeSet[i].codeKind == 19){
            if_have_call = true;
        }
    }
    if(!if_have_call){     //没有函数调用再进行优化
        return sReg_use[funcIndex][s_index];
    }
    return true;
}


void saveState(int funcofIndex,int call_index){
    int index,offset;
    for(int i = 0;i < 8;i++){
        if(sReg[i].occupy){
            if(func_use(call_index,i)){
                index = searchNewTable(sReg[i].varName,funcofIndex,false);
                offset = (NewMipsTable.element[index].offset + 1) * 4;
                fprintf(NewMips,"sw $s%d,-%d($fp)\n",i,offset);    //写入内存
            }
        }
    }
    for(int i = 0;i < 10;i++){     //保守处理,对于t寄存器都先保存
        if(tReg[i].occupy){
            index = searchNewTable(tReg[i].varName,funcofIndex,false);
            offset = (NewMipsTable.element[index].offset + 1) * 4;
            fprintf(NewMips,"sw $t%d,-%d($fp)\n",i,offset);    //写入内存
        }
    }
}

void recoverState(int funcofIndex,int call_index){     //适用于返回值语句
    int index,offset;
    fprintf(NewMips,"move $sp,$fp\n");     //恢复调用前的sp的值
    fprintf(NewMips,"lw $fp,-4($fp)\n");       //恢复fp的值
    for(int i = 0;i < 8;i++){
        if(sReg[i].occupy){
            if(func_use(call_index,i)){
                index = searchNewTable(sReg[i].varName,funcofIndex,false);
                offset = (NewMipsTable.element[index].offset + 1) * 4;
                fprintf(NewMips,"lw $s%d,-%d($fp)\n",i,offset);    //写入内存
            }
        }
    }
    for(int i = 0;i < 10;i++){
        if(tReg[i].occupy){
            index = searchNewTable(tReg[i].varName,funcofIndex,false);
            offset = (NewMipsTable.element[index].offset + 1) * 4;
            fprintf(NewMips,"lw $t%d,-%d($fp)\n",i,offset);    //写入内存
        }
    }
    for(int i = 0;i < 4;i++){
        if(aReg[i].occupy){
            index = searchNewTable(aReg[i].varName,funcofIndex,false);
            offset = (NewMipsTable.element[index].offset + 1) * 4;
            fprintf(NewMips,"lw $a%d,-%d($fp)\n",i,offset);    //写入内存
        }
    }
}

void allocReg(int funcIndex,int blockIndex){
    char varName[100];
    int regIndex,index,offset;
    for(int i = 0;i < info[blockIndex].in_num;i++){
        index = info[blockIndex].in[i];
        strcpy(varName,NewMipsTable.element[index].name);
        offset = (NewMipsTable.element[index].offset + 1) * 4;
        if(NewMipsTable.element[index].kind == 1){   //全局变量
            regIndex = find_s_reg(varName);
            if(regIndex != -1){
                continue;
            }
            else{
                regIndex = find_free_s_reg(funcIndex);  //找到一个空闲的寄存器为其分配
                if(regIndex != -1){
                    sReg[regIndex].occupy = true;
                    strcpy(sReg[regIndex].varName,varName);
                    fprintf(NewMips,"lw $s%d,-%d($fp)\n",regIndex,offset);
                }
            }
        }
        else if(NewMipsTable.element[index].kind == 5){
            regIndex = find_t_reg(varName);
            if(regIndex != -1){
                continue;
            }
            else{
                regIndex = find_free_t_reg();  //找到一个空闲的寄存器为其分配
                if(regIndex != -1){
                    tReg[regIndex].occupy = true;
                    strcpy(tReg[regIndex].varName,varName);
                    fprintf(NewMips,"lw $t%d,-%d($fp)\n",regIndex,offset);
                }
            }
        }
    }
}

bool find_if_use(int blockIndex,int blockNum,char name[]){
    for(int i = 0;i < info[blockIndex].out_num;i++){
        if(strcmp(NewMipsTable.element[info[blockIndex].out[i]].name,name) == 0)
            return true;
    }
    for(int i = blockIndex + 1;i < blockNum;i++){  //查看后续基本块是否还用到
        for(int j = 0;j < info[i].out_num;j++){
            if(strcmp(NewMipsTable.element[info[i].out[j]].name,name) == 0)
                return true;
        }
        for(int j = 0;j < info[i].in_num;j++){
            if(strcmp(NewMipsTable.element[info[i].in[j]].name,name) == 0)
                return true;
        }
    }
    return false;
}

void freeReg(int blockIndex,int blockNum){
    bool if_in;
    for(int i = 0;i < 10;i++){     //释放没用的t寄存器
        if(tReg[i].occupy){
            if(find_if_use(blockIndex,blockNum,tReg[i].varName) == false)
                tReg[i].occupy = false;
        }
    }
    for(int i = 0;i < 8;i++){
        if(sReg[i].occupy){
            if(find_if_use(blockIndex,blockNum,sReg[i].varName) == false)
                sReg[i].occupy = false;
        }
    }
    for(int i = 0;i < 4;i++){
        if(aReg[i].occupy){
            if(find_if_use(blockIndex,blockNum,aReg[i].varName) == false)
                aReg[i].occupy = false;
        }
    }
}

void GenData(){
    char newline[10];
    fprintf(NewMips,".data\n");//data数据段，储存全局变量和字符串
    int global_end_index = Mytable.funcIndex[0];
    for(int i = 0;i < global_end_index;i++){
        int datakind = Mytable.element[i].kind;
        if(datakind == 0)
            fprintf(NewMips,"%s:.word %d\n",Mytable.element[i].name,Mytable.element[i].value);
        else if(datakind == 1)
            fprintf(NewMips,"%s:.word 0\n",Mytable.element[i].name);
        else
            fprintf(NewMips,"%s:.space %d\n",Mytable.element[i].name,(Mytable.element[i].capicity + 5)*4);   //数组元素
    }
    for(int i = 0;i < codeCount;i++){
        if(MidCodeSet[i].codeKind == 24){
            if(strlen(MidCodeSet[i].src1) != 0){
                fprintf(NewMips,"string%d:.asciiz %s\n",i,MidCodeSet[i].src1);
            }
        }
    }
    strcpy(newline,"\"\\n\"");
    fprintf(NewMips,"string%d:.asciiz %s\n",codeCount,newline);
}

void GenText(){
    int startIndex,endIndex,offset,func_size;
    int pushParaNum,index,varValue,varValue_1,src1Index,src2Index;
    int tempOffset,tempIndex;
    int offset_1,index_1,offset_2,index_2;
    int cal_result;
    bool if_const_1,if_const_2,save_to_mem;
    char src1RegName[100],src2RegName[100],resultRegName[100];
    char src1[100],src2[100],op[100],result[100];
    for(int i = 0;i < NewMipsTable.numOfFunc;i++){
        liveAnalyza(i);    //进行该函数的活跃变量分析
        pushParaNum = 0;
        fprintf(NewMips,"%s:\n",NewMipsTable.element[NewMipsTable.funcIndex[i]].name);    //label:
        fprintf(NewMips,"sw $fp,-4($sp)\n");  //把上一个该函数的fp给存下来
        fprintf(NewMips,"move $fp,$sp\n");  //
        fprintf(NewMips,"sw $ra,0($fp)\n");  //将返回地址存储
        func_size = NewMipsTable.element[NewMipsTable.funcIndex[i]].functSize;
        offset = (func_size + 2) * 4;
        fprintf(NewMips,"subiu $sp,$sp,%d\n",offset);  //此时的fp为下一个函数的运行栈栈顶
        int theParaNum = NewMipsTable.element[NewMipsTable.funcIndex[i]].capicity;
        initRegister();
        for(int t = 0;t < 8;t++){
            sReg_use[i][t] = false;
        }
        for(int t = 0;t < theParaNum;t++){
            if(t < 4){
                strcpy(aReg[t].varName,NewMipsTable.element[NewMipsTable.funcIndex[i] + t + 1].name);
                aReg[t].occupy = true;
            }
        }
        for(int j = 0;j < block_list[i].blockNum;j++){
            startIndex = block_list[i].blocks[j].startIndex;
            endIndex = block_list[i].blocks[j].endIndex;
            allocReg(i,j);
            for(int k = startIndex;k <= endIndex;k++){
                if(!MidCodeSet[k].effect)
                    continue;
                switch(MidCodeSet[k].codeKind){
                    case 1:
                    case 2:
                    case 3:
                    case 4:
                    case 5:
                    case 6:
                        break;
                    case 7:{
                        fprintf(NewMips,"lw $ra,0($fp)\n");
                        fprintf(NewMips,"jr $ra\n");
                        break;
                    }
                    case 8:{
                        break;
                    }
                    case 9:
                    case 20:
                    case 21:
                    case 24:
                    case 25:
                    case 27:{
                        if_const_1 = false;
                        if(MidCodeSet[k].codeKind == 24 || MidCodeSet[k].codeKind == 27)
                            strcpy(src1,MidCodeSet[k].op);
                        else
                            strcpy(src1,MidCodeSet[k].src1);
                        index = searchNewTable(src1,i,false);
                        if(index == -1){   //符号表没有，说明是常量,使用$v1寄存器保存常量
                            if_const_1 = true;
                            varValue = cal_Value(src1);
                            if(MidCodeSet[k].codeKind == 9){
                                fprintf(NewMips,"li $v1,%d\n",varValue);
                                strcpy(src1RegName,"v1");
                            }
                        }
                        else{
                            offset = (NewMipsTable.element[index].offset + 1) * 4;
                            if(NewMipsTable.element[index].kind == 0){
                                if_const_1 = true;
                                varValue = NewMipsTable.element[index].value;
                                if(MidCodeSet[k].codeKind == 9){
                                    fprintf(NewMips,"li $v1,%d\n",varValue);
                                    strcpy(src1RegName,"v1");
                                }
                            }
                            else if(NewMipsTable.element[index].kind == 2){   //src1是参数
                                src1Index = find_a_reg(src1);
                                if(src1Index != -1){         //说明找到了,是$a0-$a3
                                    sprintf(src1RegName,"a%d",src1Index);
                                }
                                else{       //说明在内存中,参数的个数一定大于4
                                    fprintf(NewMips,"lw $v1,-%d($fp)\n",offset);
                                    strcpy(src1RegName,"v1");
                                }
                            }
                            else if(NewMipsTable.element[index].kind == 1){//src1为局部变量或全局变量
                                if(NewMipsTable.element[index].global){     //全局变量
                                    fprintf(NewMips,"lw $v1,%s\n",src1);
                                    strcpy(src1RegName,"v1");
                                }
                                else{      //局部变量
                                    src1Index = find_s_reg(src1);
                                    if(src1Index != -1){         //说明找到了
                                        sprintf(src1RegName,"s%d",src1Index);
                                    }
                                    else{       //说明在内存中
                                        src1Index = find_free_s_reg(i);
                                        if(src1Index != -1){     //找一个空闲的
                                            strcpy(sReg[src1Index].varName,src1);
                                            sReg[src1Index].occupy = true;
                                            fprintf(NewMips,"lw $s%d,-%d($fp)\n",src1Index,offset);
                                            sprintf(src1RegName,"s%d",src1Index);
                                        }
                                        else{
                                            src1Index = find_new_s_reg(j,src1);
                                            if(src1Index != -1){
                                                tempIndex = searchNewTable(sReg[src1Index].varName,i,false);
                                                tempOffset = (NewMipsTable.element[tempIndex].offset + 1) * 4;
                                                strcpy(sReg[src1Index].varName,src1);
                                                fprintf(NewMips,"sw $s%d,-%d($fp)\n",src1Index,tempOffset);
                                                fprintf(NewMips,"lw $s%d,-%d($fp)\n",src1Index,offset);
                                                sprintf(src1RegName,"s%d",src1Index);
                                            }
                                            else{     //说明保存的这些变量接下来都有用
                                                fprintf(NewMips,"lw $v1,-%d($fp)\n",offset);
                                                strcpy(src1RegName,"v1");
                                            }
                                        }
                                    }
                                }
                            }
                            else{      //是中间变量
                                src1Index = find_t_reg(src1);
                                if(src1Index != -1){         //说明找到了
                                    sprintf(src1RegName,"t%d",src1Index);
                                }
                                else{
                                    src1Index = find_free_t_reg();
                                    if(src1Index != -1){     //找一个空闲的
                                        strcpy(tReg[src1Index].varName,src1);
                                        tReg[src1Index].occupy = true;
                                        fprintf(NewMips,"lw $t%d,-%d($fp)\n",src1Index,offset);
                                        sprintf(src1RegName,"t%d",src1Index);
                                    }
                                    else{
                                        //没找到空闲的话,用$t0
                                        tempIndex = searchNewTable(tReg[0].varName,i,false);
                                        tempOffset = (NewMipsTable.element[tempIndex].offset + 1) * 4;
                                        strcpy(tReg[0].varName,src1);
                                        fprintf(NewMips,"sw $t0,-%d($fp)\n",tempOffset);
                                        fprintf(NewMips,"lw $t0,-%d($fp)\n",offset);
                                        sprintf(src1RegName,"t%d",0);
                                        src1Index = 0;
                                    }
                                }
                                if(!find_if_next(k,src1)){
                                    tReg[src1Index].occupy = false;      //接下来用不到了，直接释放
                                }
                            }
                        }
                        if(MidCodeSet[k].codeKind == 9)
                            fprintf(NewMips,"beq $%s,0,%s\n",src1RegName,MidCodeSet[k].result);
                        else if(MidCodeSet[k].codeKind == 20){
                            for(int t = 0;t < 4;t++){
                                if(aReg[t].occupy){
                                    tempIndex = searchNewTable(aReg[t].varName,i,false);
                                    tempOffset = (NewMipsTable.element[tempIndex].offset + 1) * 4;
                                    fprintf(NewMips,"sw $a%d,-%d($fp)\n",t,tempOffset);
                                    aReg[t].occupy = false;
                                    astack[a_point++] = t;
                                }
                            }
                            pushParaNum++;
                            if(pushParaNum > 4){ //参数个数大于4，直接存内存
                                if(if_const_1){
                                    fprintf(NewMips,"li $v0,%d\n",varValue);
                                    fprintf(NewMips,"sw $v0,-%d($sp)\n",(pushParaNum + 1) * 4);
                                }
                                else
                                    fprintf(NewMips,"sw $%s,-%d($sp)\n",src1RegName,(pushParaNum + 1) * 4);
                            }
                            else{     //放到a寄存器中
                                if(if_const_1){

                                    fprintf(NewMips,"li $a%d,%d\n",pushParaNum - 1,varValue);
                                }
                                else{
                                    fprintf(NewMips,"move $a%d,$%s\n",pushParaNum - 1,src1RegName);
                                }
                            }
                        }
                        else if(MidCodeSet[k].codeKind == 21 || MidCodeSet[k].codeKind == 27){
                            save_to_mem = false;
                            strcpy(result,MidCodeSet[k].result);
                            index_2 = searchNewTable(result,i,false);
                            offset_2 = (NewMipsTable.element[index_2].offset + 1) * 4;
                            if(NewMipsTable.element[index_2].kind == 1){
                                if(NewMipsTable.element[index_2].global){     //全局变量
                                    save_to_mem = true;
                                }
                                else{
                                    src1Index = find_s_reg(result);
                                    if(src1Index != -1){         //说明找到了
                                        sprintf(resultRegName,"s%d",src1Index);
                                    }
                                    else{       //说明在内存中
                                        src1Index = find_free_s_reg(i);
                                        if(src1Index != -1){     //找一个空闲的
                                            strcpy(sReg[src1Index].varName,result);
                                            sReg[src1Index].occupy = true;
                                            sprintf(resultRegName,"s%d",src1Index);
                                            //cout << result << " " << resultRegName << endl;
                                        }
                                        else{
                                            src1Index = find_new_s_reg(j,result);
                                            if(src1Index != -1){
                                                tempIndex = searchNewTable(sReg[src1Index].varName,i,false);
                                                tempOffset = (NewMipsTable.element[tempIndex].offset + 1) * 4;
                                                strcpy(sReg[src1Index].varName,result);
                                                fprintf(NewMips,"sw $s%d,-%d($fp)\n",src1Index,tempOffset);
                                                sprintf(resultRegName,"s%d",src1Index);
                                            }
                                            else{     //直接存到内存中
                                                save_to_mem = true;
                                            }
                                        }
                                    }
                                }
                            }
                            else if(NewMipsTable.element[index_2].kind == 5){
                                src1Index = find_t_reg(result);
                                if(src1Index != -1){         //说明找到了
                                    sprintf(resultRegName,"t%d",src1Index);
                                }
                                else{
                                    src1Index = find_free_t_reg();
                                    if(src1Index != -1){     //找一个空闲的
                                        strcpy(tReg[src1Index].varName,result);
                                        tReg[src1Index].occupy = true;
                                        sprintf(resultRegName,"t%d",src1Index);
                                    }
                                    else{
                                        save_to_mem = true;
                                    }
                                }
                            }
                            else{    //只可能是参数
                                src1Index = find_a_reg(result);
                                if(src1Index != -1){         //说明找到了
                                    sprintf(resultRegName,"a%d",src1Index);
                                }
                                else{       //说明在内存中,参数的个数一定大于4
                                    save_to_mem = true;
                                }
                            }
                            if(save_to_mem){
                                if(MidCodeSet[k].codeKind == 21){
                                    if(if_const_1){
                                        fprintf(NewMips,"li $v0,%d\n",varValue);
                                        if(NewMipsTable.element[index_2].global)
                                            fprintf(NewMips,"sw $v0,%s\n",result);
                                        else
                                            fprintf(NewMips,"sw $v0,-%d($fp)\n",offset_2);
                                    }
                                    else{
                                        if(NewMipsTable.element[index_2].global)
                                            fprintf(NewMips,"sw $%s,%s\n",src1RegName,result);
                                        else
                                            fprintf(NewMips,"sw $%s,-%d($fp)\n",src1RegName,offset_2);
                                    }
                                }
                                else{
                                    strcpy(src2,MidCodeSet[k].src1);
                                    index_1 = searchNewTable(src2,i,false);
                                    offset_1 = (NewMipsTable.element[index_1].offset + 1) * 4;
                                    if(NewMipsTable.element[index_1].global){    //全局数组
                                        if(if_const_1){  //索引是常量
                                            fprintf(NewMips,"li $v0,%d\n",varValue);
                                            fprintf(NewMips,"sll $v0,$v0,2\n");
                                            fprintf(NewMips,"lw $v1,%s($v0)\n",src2);
                                            if(NewMipsTable.element[index_2].global)
                                                fprintf(NewMips,"sw $v1,%s\n",result);
                                            else
                                                fprintf(NewMips,"sw $v1,-%d($fp)\n",offset_2);
                                        }
                                        else{
                                            fprintf(NewMips,"sll $v0,$%s,2\n",src1RegName);
                                            fprintf(NewMips,"lw $v1,%s($v0)\n",src2);
                                            if(NewMipsTable.element[index_2].global)
                                                fprintf(NewMips,"sw $v1,%s\n",result);
                                            else
                                                fprintf(NewMips,"sw $v1,-%d($fp)\n",offset_2);
                                        }
                                    }
                                    else{
                                        if(if_const_1){    //索引是常量
                                            fprintf(NewMips,"li $v0,%d\n",varValue);
                                            fprintf(NewMips,"sll $v0,$v0,2\n");
                                            fprintf(NewMips,"addiu $v0,$v0,%d\n",offset_1);
                                            fprintf(NewMips,"subu $fp,$fp,$v0\n");
                                            fprintf(NewMips,"lw $v1,0($fp)\n");
                                            fprintf(NewMips,"addu $fp,$fp,$v0\n");
                                            if(NewMipsTable.element[index_2].global)
                                                fprintf(NewMips,"sw $v1,%s\n",result);
                                            else
                                                fprintf(NewMips,"sw $v1,-%d($fp)\n",offset_2);
                                        }
                                        else{
                                            fprintf(NewMips,"sll $v0,$%s,2\n",src1RegName);
                                            fprintf(NewMips,"addiu $v0,$v0,%d\n",offset_1);
                                            fprintf(NewMips,"subu $fp,$fp,$v0\n");
                                            fprintf(NewMips,"lw $v1,0($fp)\n");
                                            fprintf(NewMips,"addu $fp,$fp,$v0\n");
                                            if(NewMipsTable.element[index_2].global)
                                                fprintf(NewMips,"sw $v1,%s\n",result);
                                            else
                                                fprintf(NewMips,"sw $v1,-%d($fp)\n",offset_2);
                                        }
                                    }
                                }
                            }
                            else{
                                if(MidCodeSet[k].codeKind == 21){
                                    //cout << result << " " << resultRegName << endl;
                                    if(if_const_1)
                                        fprintf(NewMips,"li $%s,%d\n",resultRegName,varValue);
                                    else
                                        fprintf(NewMips,"move $%s,$%s\n",resultRegName,src1RegName);
                                }
                                else{
                                    //存到寄存器中
                                    strcpy(src2,MidCodeSet[k].src1);
                                    index_1 = searchNewTable(src2,i,false);
                                    offset_1 = (NewMipsTable.element[index_1].offset + 1) * 4;
                                    if(NewMipsTable.element[index_1].global){    //全局数组
                                        if(if_const_1){  //索引是常量
                                            fprintf(NewMips,"li $v0,%d\n",varValue);
                                            fprintf(NewMips,"sll $v0,$v0,2\n");
                                            fprintf(NewMips,"lw $%s,%s($v0)\n",resultRegName,src2);
                                        }
                                        else{
                                            fprintf(NewMips,"sll $v0,$%s,2\n",src1RegName);
                                            fprintf(NewMips,"lw $%s,%s($v0)\n",resultRegName,src2);
                                        }
                                    }
                                    else{
                                        if(if_const_1){    //索引是常量
                                            fprintf(NewMips,"li $v0,%d\n",varValue);
                                            fprintf(NewMips,"sll $v0,$v0,2\n");
                                            fprintf(NewMips,"addiu $v0,$v0,%d\n",offset_1);
                                            fprintf(NewMips,"subu $fp,$fp,$v0\n");
                                            fprintf(NewMips,"lw $%s,0($fp)\n",resultRegName);
                                            fprintf(NewMips,"addu $fp,$fp,$v0\n");
                                        }
                                        else{
                                            fprintf(NewMips,"sll $v0,$%s,2\n",src1RegName);
                                            fprintf(NewMips,"addiu $v0,$v0,%d\n",offset_1);
                                            fprintf(NewMips,"subu $fp,$fp,$v0\n");
                                            fprintf(NewMips,"lw $%s,0($fp)\n",resultRegName);
                                            fprintf(NewMips,"addu $fp,$fp,$v0\n");
                                        }
                                    }
                                }
                            }
                        }
                        else if(MidCodeSet[k].codeKind == 24){
                            strcpy(src2,MidCodeSet[k].src1);
                            if(aReg[0].occupy){
                                fprintf(NewMips,"move $ra,$a0\n");
                            }
                            if(strlen(src2) != 0){
                                fprintf(NewMips,"la $a0,string%d\n",k);
                                fprintf(NewMips,"li $v0,4\n");
                                fprintf(NewMips,"syscall\n");
                            }
                            if(strlen(src1) != 0){
                                if(if_const_1){
                                    fprintf(NewMips,"li $a0,%d\n",varValue);
                                    if(index == -1){
                                        if(is_Num(src1)){
                                            fprintf(NewMips,"li $v0,1\n");
                                            fprintf(NewMips,"syscall\n");
                                        }
                                        else{
                                            fprintf(NewMips,"li $v0,11\n");
                                            fprintf(NewMips,"syscall\n");
                                        }
                                    }
                                    else{
                                        if(NewMipsTable.element[index].type == 1){
                                            fprintf(NewMips,"li $v0,11\n");
                                            fprintf(NewMips,"syscall\n");
                                        }
                                        else{
                                            fprintf(NewMips,"li $v0,1\n");
                                            fprintf(NewMips,"syscall\n");
                                        }
                                    }
                                }
                                else{
                                    if(strcmp("a0",src1RegName) == 0 && aReg[0].occupy){
                                        fprintf(NewMips,"move $a0,$ra\n");
                                    }
                                    else
                                        fprintf(NewMips,"move $a0,$%s\n",src1RegName);
                                    if(NewMipsTable.element[index].type == 1){
                                        fprintf(NewMips,"li $v0,11\n");
                                        fprintf(NewMips,"syscall\n");
                                    }
                                    else{
                                        fprintf(NewMips,"li $v0,1\n");
                                        fprintf(NewMips,"syscall\n");
                                    }
                                }
                            }
                            fprintf(NewMips,"la $a0,string%d\n",codeCount);
                            fprintf(NewMips,"li $v0,4\n");
                            fprintf(NewMips,"syscall\n");
                            if(aReg[0].occupy){
                                fprintf(NewMips,"move $a0,$ra\n");
                            }
                        }
                        else if(MidCodeSet[k].codeKind == 25){
                            if(if_const_1)
                                fprintf(NewMips,"li $v0,%d\n",varValue);
                            else
                                fprintf(NewMips,"move $v0,$%s\n",src1RegName);
                            fprintf(NewMips,"lw $ra,0($fp)\n");
                            fprintf(NewMips,"jr $ra\n");
                        }
                        break;
                    }
                    case 10:{
                        fprintf(NewMips,"%s:\n",MidCodeSet[k].result);
                        break;
                    }
                    case 11:
                    case 12:
                    case 13:
                    case 14:
                    case 15:
                    case 16:
                    case 22:
                    case 26:{   //两个寄存器的情况
                        if((MidCodeSet[k].codeKind >= 11 && MidCodeSet[k].codeKind <= 16) || MidCodeSet[k].codeKind == 26){
                            strcpy(src1,MidCodeSet[k].src1);
                            strcpy(src2,MidCodeSet[k].src2);
                        }
                        else if(MidCodeSet[k].codeKind == 22){
                            strcpy(src1,MidCodeSet[k].src1);
                            strcpy(src2,MidCodeSet[k].op);
                        }
                        index = searchNewTable(src1,i,false);
                        index_1 = searchNewTable(src2,i,false);
                        if_const_1 = false;
                        if_const_2 = false;
                        if(index == -1){   //符号表没有，说明是常量,使用$v0寄存器保存常量
                            if_const_1 = true;
                            varValue = cal_Value(src1);
                            if(MidCodeSet[k].codeKind >= 11 && MidCodeSet[k].codeKind <= 16){
                                fprintf(NewMips,"li $v0,%d\n",varValue);
                                strcpy(src1RegName,"v0");
                            }
                        }
                        else{
                            offset = (NewMipsTable.element[index].offset + 1) * 4;
                            if(NewMipsTable.element[index].kind == 0){
                                if_const_1 = true;
                                varValue = NewMipsTable.element[index].value;
                                if(MidCodeSet[k].codeKind >= 11 && MidCodeSet[k].codeKind <= 16){
                                    fprintf(NewMips,"li $v0,%d\n",varValue);
                                    strcpy(src1RegName,"v0");
                                }
                            }
                            else if(NewMipsTable.element[index].kind == 2){   //src1是参数
                                src1Index = find_a_reg(src1);
                                if(src1Index != -1){         //说明找到了
                                    sprintf(src1RegName,"a%d",src1Index);
                                }
                                else{       //说明在内存中,参数的个数一定大于4
                                    fprintf(NewMips,"lw $v0,-%d($fp)\n",offset);
                                    strcpy(src1RegName,"v0");
                                }
                                //cout << src1 << " " << aReg[0].varName << endl;
                            }
                            else if(NewMipsTable.element[index].kind == 1){//src1为局部变量或全局变量
                                if(NewMipsTable.element[index].global){     //全局变量
                                    fprintf(NewMips,"lw $v0,%s\n",src1);
                                    strcpy(src1RegName,"v0");
                                }
                                else{      //局部变量
                                    src1Index = find_s_reg(src1);
                                    if(src1Index != -1){         //说明找到了
                                        sprintf(src1RegName,"s%d",src1Index);
                                    }
                                    else{       //说明在内存中
                                        src1Index = find_free_s_reg(i);
                                        if(src1Index != -1){     //找一个空闲的
                                            strcpy(sReg[src1Index].varName,src1);
                                            sReg[src1Index].occupy = true;
                                            fprintf(NewMips,"lw $s%d,-%d($fp)\n",src1Index,offset);
                                            sprintf(src1RegName,"s%d",src1Index);
                                        }
                                        else{
                                            src1Index = find_new_s_reg(j,src1);
                                            if(src1Index != -1){
                                                tempIndex = searchNewTable(sReg[src1Index].varName,i,false);
                                                tempOffset = (NewMipsTable.element[tempIndex].offset + 1) * 4;
                                                strcpy(sReg[src1Index].varName,src1);
                                                fprintf(NewMips,"sw $s%d,-%d($fp)\n",src1Index,tempOffset);
                                                fprintf(NewMips,"lw $s%d,-%d($fp)\n",src1Index,offset);
                                                sprintf(src1RegName,"s%d",src1Index);
                                            }
                                            else{     //说明保存的这些变量接下来都有用
                                                fprintf(NewMips,"lw $v0,-%d($fp)\n",offset);
                                                strcpy(src1RegName,"v0");
                                            }
                                        }
                                    }
                                }
                            }
                            else{      //是中间变量
                                src1Index = find_t_reg(src1);
                                if(src1Index != -1){         //说明找到了
                                    sprintf(src1RegName,"t%d",src1Index);
                                }
                                else{
                                    src1Index = find_free_t_reg();
                                    if(src1Index != -1){     //找一个空闲的
                                        strcpy(tReg[src1Index].varName,src1);
                                        tReg[src1Index].occupy = true;
                                        fprintf(NewMips,"lw $t%d,-%d($fp)\n",src1Index,offset);
                                        sprintf(src1RegName,"t%d",src1Index);
                                    }
                                    else{
                                        //没找到空闲的话,用$t0
                                        tempIndex = searchNewTable(tReg[0].varName,i,false);
                                        tempOffset = (NewMipsTable.element[tempIndex].offset + 1) * 4;
                                        strcpy(tReg[0].varName,src1);
                                        fprintf(NewMips,"sw $t0,-%d($fp)\n",tempOffset);
                                        fprintf(NewMips,"lw $t0,-%d($fp)\n",offset);
                                        sprintf(src1RegName,"t%d",0);
                                        src1Index = 0;
                                    }
                                }
                            }
                        }
                        if(index_1 == -1){
                            if_const_2 = true;
                            varValue_1 = cal_Value(src2);
                            if((MidCodeSet[k].codeKind >= 11 && MidCodeSet[k].codeKind <= 16) || MidCodeSet[k].codeKind == 22){
                                fprintf(NewMips,"li $v1,%d\n",varValue_1);
                                strcpy(src2RegName,"v1");
                            }
                        }
                        else{
                            offset_1 = (NewMipsTable.element[index_1].offset + 1) * 4;
                            if(NewMipsTable.element[index_1].kind == 0){
                                if_const_2 = true;
                                varValue_1 = NewMipsTable.element[index_1].value;
                                if((MidCodeSet[k].codeKind >= 11 && MidCodeSet[k].codeKind <= 16) || MidCodeSet[k].codeKind == 22){
                                    fprintf(NewMips,"li $v1,%d\n",varValue_1);
                                    strcpy(src2RegName,"v1");
                                }
                            }
                            else if(NewMipsTable.element[index_1].kind == 2){   //src1是参数
                                src2Index = find_a_reg(src2);
                                if(src2Index != -1){         //说明找到了
                                    sprintf(src2RegName,"a%d",src2Index);
                                }
                                else{       //说明在内存中,参数的个数一定大于4
                                    fprintf(NewMips,"lw $v1,-%d($fp)\n",offset_1);
                                    strcpy(src2RegName,"v1");
                                }
                            }
                            else if(NewMipsTable.element[index_1].kind == 1){//src1为局部变量或全局变量
                                if(NewMipsTable.element[index_1].global){     //全局变量
                                    fprintf(NewMips,"lw $v1,%s\n",src2);
                                    strcpy(src2RegName,"v1");
                                }
                                else{      //局部变量
                                    src2Index = find_s_reg(src2);
                                    if(src2Index != -1){         //说明找到了
                                        sprintf(src2RegName,"s%d",src2Index);
                                    }
                                    else{       //说明在内存中
                                        src2Index = find_free_s_reg(i);
                                        if(src2Index != -1){     //找一个空闲的
                                            strcpy(sReg[src2Index].varName,src2);
                                            sReg[src2Index].occupy = true;
                                            fprintf(NewMips,"lw $s%d,-%d($fp)\n",src2Index,offset_1);
                                            sprintf(src2RegName,"s%d",src2Index);
                                        }
                                        else{
                                            src2Index = find_new_s_reg(j,src2);
                                            if(src2Index != -1){
                                                tempIndex = searchNewTable(sReg[src2Index].varName,i,false);
                                                tempOffset = (NewMipsTable.element[tempIndex].offset + 1) * 4;
                                                strcpy(sReg[src2Index].varName,src2);
                                                fprintf(NewMips,"sw $s%d,-%d($fp)\n",src2Index,tempOffset);
                                                fprintf(NewMips,"lw $s%d,-%d($fp)\n",src2Index,offset_1);
                                                sprintf(src2RegName,"s%d",src2Index);
                                            }
                                            else{     //说明保存的这些变量接下来都有用
                                                fprintf(NewMips,"lw $v1,-%d($fp)\n",offset_1);
                                                strcpy(src2RegName,"v1");
                                            }
                                        }
                                    }
                                }
                            }
                            else{      //是中间变量
                                src2Index = find_t_reg(src2);
                                if(src2Index != -1){         //说明找到了
                                    sprintf(src2RegName,"t%d",src2Index);
                                }
                                else{
                                    src2Index = find_free_t_reg();
                                    if(src2Index != -1){     //找一个空闲的
                                        strcpy(tReg[src2Index].varName,src2);
                                        tReg[src2Index].occupy = true;
                                        fprintf(NewMips,"lw $t%d,-%d($fp)\n",src2Index,offset_1);
                                        sprintf(src2RegName,"t%d",src2Index);
                                    }
                                    else{
                                        //没找到空闲的话,用$t1
                                        tempIndex = searchNewTable(tReg[1].varName,i,false);
                                        tempOffset = (NewMipsTable.element[tempIndex].offset + 1) * 4;
                                        strcpy(tReg[1].varName,src2);
                                        fprintf(NewMips,"sw $t1,-%d($fp)\n",tempOffset);
                                        fprintf(NewMips,"lw $t1,-%d($fp)\n",offset_1);
                                        sprintf(src2RegName,"t%d",1);
                                        src2Index = 1;
                                    }
                                }
                            }
                        }
                        if(NewMipsTable.element[index].kind == 5){
                            if(!find_if_next(k,src1)){
                                tReg[src1Index].occupy = false;      //接下来用不到了，直接释放
                            }
                        }
                        if(NewMipsTable.element[index_1].kind == 5){
                            if(!find_if_next(k,src2)){
                                tReg[src2Index].occupy = false;      //接下来用不到了，直接释放
                            }
                        }
                        if(MidCodeSet[k].codeKind >= 11 && MidCodeSet[k].codeKind <= 16){  //11-16的指令
                            strcpy(op,MidCodeSet[k].op);
                            if(strcmp(op,"<") == 0)
                                fprintf(NewMips,"bge $%s,$%s,%s\n",src1RegName,src2RegName,MidCodeSet[k].result);
                            if(strcmp(op,">") == 0)
                                fprintf(NewMips,"ble $%s,$%s,%s\n",src1RegName,src2RegName,MidCodeSet[k].result);
                            if(strcmp(op,"<=") == 0)
                                fprintf(NewMips,"bgt $%s,$%s,%s\n",src1RegName,src2RegName,MidCodeSet[k].result);
                            if(strcmp(op,">=") == 0)
                                fprintf(NewMips,"blt $%s,$%s,%s\n",src1RegName,src2RegName,MidCodeSet[k].result);
                            if(strcmp(op,"==") == 0)
                                fprintf(NewMips,"bne $%s,$%s,%s\n",src1RegName,src2RegName,MidCodeSet[k].result);
                            if(strcmp(op,"!=") == 0)
                                fprintf(NewMips,"beq $%s,$%s,%s\n",src1RegName,src2RegName,MidCodeSet[k].result);
                        }
                        else if(MidCodeSet[k].codeKind == 22){
                            strcpy(result,MidCodeSet[k].result);
                            index_2 = searchNewTable(result,i,false);
                            offset_2 = (NewMipsTable.element[index_2].offset + 1) * 4;
                            if(NewMipsTable.element[index_2].global){     //全局数组
                                if(if_const_1){    //索引是常数
                                    fprintf(NewMips,"li $v0,%d\n",varValue);
                                    fprintf(NewMips,"sll $v0,$v0,2\n");
                                    fprintf(NewMips,"sw $%s,%s($v0)\n",src2RegName,result);
                                }
                                else{
                                    fprintf(NewMips,"sll $v0,$%s,2\n",src1RegName);
                                    fprintf(NewMips,"sw $%s,%s($v0)\n",src2RegName,result);
                                }
                            }
                            else{
                                if(if_const_1){    //索引是常数
                                    fprintf(NewMips,"li $v0,%d\n",varValue);
                                    fprintf(NewMips,"sll $v0,$v0,2\n");
                                    fprintf(NewMips,"addiu $v0,$v0,%d\n",offset_2);
                                    fprintf(NewMips,"subu $fp,$fp,$v0\n");
                                    fprintf(NewMips,"sw $%s,0($fp)\n",src2RegName);
                                    fprintf(NewMips,"addu $fp,$fp,$v0\n");
                                }
                                else{
                                    fprintf(NewMips,"sll $v0,$%s,2\n",src1RegName);
                                    fprintf(NewMips,"addiu $v0,$v0,%d\n",offset_2);
                                    fprintf(NewMips,"subu $fp,$fp,$v0\n");
                                    fprintf(NewMips,"sw $%s,0($fp)\n",src2RegName);
                                    fprintf(NewMips,"addu $fp,$fp,$v0\n");
                                }
                            }
                        }
                        else if(MidCodeSet[k].codeKind == 26){
                            save_to_mem = false;
                            strcpy(result,MidCodeSet[k].result);
                            index_2 = searchNewTable(result,i,false);
                            offset_2 = (NewMipsTable.element[index_2].offset + 1) * 4;
                            if(NewMipsTable.element[index_2].kind == 1){
                                if(NewMipsTable.element[index_2].global){     //全局变量
                                    save_to_mem = true;
                                }
                                else{
                                    src1Index = find_s_reg(result);
                                    if(src1Index != -1){         //说明找到了
                                        sprintf(resultRegName,"s%d",src1Index);
                                    }
                                    else{       //说明在内存中
                                        src1Index = find_free_s_reg(i);
                                        if(src1Index != -1){     //找一个空闲的
                                            strcpy(sReg[src1Index].varName,result);
                                            sReg[src1Index].occupy = true;
                                            sprintf(resultRegName,"s%d",src1Index);
                                        }
                                        else{
                                            src1Index = find_new_s_reg(j,result);
                                            if(src1Index != -1){
                                                tempIndex = searchNewTable(sReg[src1Index].varName,i,false);
                                                tempOffset = (NewMipsTable.element[tempIndex].offset + 1) * 4;
                                                strcpy(sReg[src1Index].varName,result);
                                                fprintf(NewMips,"sw $s%d,-%d($fp)\n",src1Index,tempOffset);
                                                sprintf(resultRegName,"s%d",src1Index);
                                            }
                                            else{     //直接存到内存中
                                                save_to_mem = true;
                                            }
                                        }
                                    }
                                }
                            }
                            else if(NewMipsTable.element[index_2].kind == 5){
                                src1Index = find_t_reg(result);
                                if(src1Index != -1){         //说明找到了
                                    sprintf(resultRegName,"t%d",src1Index);
                                }
                                else{
                                    src1Index = find_free_t_reg();
                                    if(src1Index != -1){     //找一个空闲的
                                        strcpy(tReg[src1Index].varName,result);
                                        tReg[src1Index].occupy = true;
                                        sprintf(resultRegName,"t%d",src1Index);
                                    }
                                    else{
                                        save_to_mem = true;
                                    }
                                }
                            }
                            else{    //只可能是参数
                                src1Index = find_a_reg(result);
                                if(src1Index != -1){         //说明找到了
                                    sprintf(resultRegName,"a%d",src1Index);
                                }
                                else{       //说明在内存中,参数的个数一定大于4
                                    save_to_mem = true;
                                }
                            }
                            strcpy(op,MidCodeSet[k].op);
                            if(if_const_1 && if_const_2){
                                if(op[0] == '+') cal_result = varValue + varValue_1;
                                if(op[0] == '-') cal_result = varValue - varValue_1;
                                if(op[0] == '*') cal_result = varValue * varValue_1;
                                if(op[0] == '/') cal_result = varValue / varValue_1;
                                if(save_to_mem){
                                    fprintf(NewMips,"li $v1,%d\n",cal_result);
                                    if(NewMipsTable.element[index_2].global)
                                        fprintf(NewMips,"sw $v1,%s\n",result);
                                    else
                                        fprintf(NewMips,"sw $v1,-%d($fp)\n",offset_2);
                                }
                                else{
                                    fprintf(NewMips,"li $%s,%d\n",resultRegName,cal_result);
                                }
                            }
                            else if(!if_const_1 && if_const_2){
                                switch(op[0]){
                                    case '+':{
                                        if(save_to_mem){
                                            fprintf(NewMips,"addiu $v1,$%s,%d\n",src1RegName,varValue_1);
                                            if(NewMipsTable.element[index_2].global)
                                                fprintf(NewMips,"sw $v1,%s\n",result);
                                            else
                                                fprintf(NewMips,"sw $v1,-%d($fp)\n",offset_2);
                                        }
                                        else
                                            fprintf(NewMips,"addiu $%s,$%s,%d\n",resultRegName,src1RegName,varValue_1);
                                        break;
                                    }
                                    case '-':{
                                        if(save_to_mem){
                                            fprintf(NewMips,"subiu $v1,$%s,%d\n",src1RegName,varValue_1);
                                            if(NewMipsTable.element[index_2].global)
                                                fprintf(NewMips,"sw $v1,%s\n",result);
                                            else
                                                fprintf(NewMips,"sw $v1,-%d($fp)\n",offset_2);
                                        }
                                        else
                                            fprintf(NewMips,"subiu $%s,$%s,%d\n",resultRegName,src1RegName,varValue_1);
                                        break;
                                    }
                                    case '*':{
                                        fprintf(NewMips,"li $v1,%d\n",varValue_1);
                                        fprintf(NewMips,"mult $%s,$v1\n",src1RegName);
                                        if(save_to_mem){
                                            fprintf(NewMips,"mflo $v1\n");
                                            if(NewMipsTable.element[index_2].global)
                                                fprintf(NewMips,"sw $v1,%s\n",result);
                                            else
                                                fprintf(NewMips,"sw $v1,-%d($fp)\n",offset_2);
                                        }
                                        else
                                            fprintf(NewMips,"mflo $%s\n",resultRegName);
                                        break;
                                    }
                                    case '/':{
                                        fprintf(NewMips,"li $v1,%d\n",varValue_1);
                                        fprintf(NewMips,"div $%s,$v1\n",src1RegName);
                                        if(save_to_mem){
                                            fprintf(NewMips,"mflo $v1\n");
                                            if(NewMipsTable.element[index_2].global)
                                                fprintf(NewMips,"sw $v1,%s\n",result);
                                            else
                                                fprintf(NewMips,"sw $v1,-%d($fp)\n",offset_2);
                                        }
                                        else
                                            fprintf(NewMips,"mflo $%s\n",resultRegName);
                                        break;
                                    }
                                }
                            }
                            else if(if_const_1 && !if_const_2){
                                fprintf(NewMips,"li $v0,%d\n",varValue);
                                switch(op[0]){
                                    case '+':{
                                        if(save_to_mem){
                                            fprintf(NewMips,"addu $v0,$v0,$%s\n",src2RegName);
                                            if(NewMipsTable.element[index_2].global)
                                                fprintf(NewMips,"sw $v0,%s\n",result);
                                            else
                                                fprintf(NewMips,"sw $v0,-%d($fp)\n",offset_2);
                                        }
                                        else
                                            fprintf(NewMips,"addu $%s,$v0,$%s\n",resultRegName,src2RegName);
                                        break;
                                    }
                                    case '-':{
                                        if(save_to_mem){
                                            fprintf(NewMips,"subu $v0,$v0,$%s\n",src2RegName);
                                            if(NewMipsTable.element[index_2].global)
                                                fprintf(NewMips,"sw $v0,%s\n",result);
                                            else
                                                fprintf(NewMips,"sw $v0,-%d($fp)\n",offset_2);
                                        }
                                        else
                                            fprintf(NewMips,"subu $%s,$v0,$%s\n",resultRegName,src2RegName);
                                        break;
                                    }
                                    case '*':{
                                        fprintf(NewMips,"mult $v0,$%s\n",src2RegName);
                                        if(save_to_mem){
                                            fprintf(NewMips,"mflo $v1\n");
                                            if(NewMipsTable.element[index_2].global)
                                                fprintf(NewMips,"sw $v1,%s\n",result);
                                            else
                                                fprintf(NewMips,"sw $v1,-%d($fp)\n",offset_2);
                                        }
                                        else
                                            fprintf(NewMips,"mflo $%s\n",resultRegName);
                                        break;
                                    }
                                    case '/':{
                                        fprintf(NewMips,"div $v0,$%s\n",src2RegName);
                                        if(save_to_mem){
                                            fprintf(NewMips,"mflo $v1\n");
                                            if(NewMipsTable.element[index_2].global)
                                                fprintf(NewMips,"sw $v1,%s\n",result);
                                            else
                                                fprintf(NewMips,"sw $v1,-%d($fp)\n",offset_2);
                                        }
                                        else
                                            fprintf(NewMips,"mflo $%s\n",resultRegName);
                                        break;
                                    }
                                }
                            }
                            else{
                                switch(op[0]){
                                    case '+':{
                                        if(save_to_mem){
                                            fprintf(NewMips,"addu $v1,$%s,$%s\n",src1RegName,src2RegName);
                                            if(NewMipsTable.element[index_2].global)
                                                fprintf(NewMips,"sw $v1,%s\n",result);
                                            else
                                                fprintf(NewMips,"sw $v1,-%d($fp)\n",offset_2);
                                        }
                                        else
                                            fprintf(NewMips,"addu $%s,$%s,$%s\n",resultRegName,src1RegName,src2RegName);
                                        break;
                                    }
                                    case '-':{
                                        if(save_to_mem){
                                            fprintf(NewMips,"subu $v1,$%s,$%s\n",src1RegName,src2RegName);
                                            if(NewMipsTable.element[index_2].global)
                                                fprintf(NewMips,"sw $v1,%s\n",result);
                                            else
                                                fprintf(NewMips,"sw $v1,-%d($fp)\n",offset_2);
                                        }
                                        else
                                            fprintf(NewMips,"subu $%s,$%s,$%s\n",resultRegName,src1RegName,src2RegName);
                                        break;
                                    }
                                    case '*':{
                                        fprintf(NewMips,"mult $%s,$%s\n",src1RegName,src2RegName);
                                        if(save_to_mem){
                                            fprintf(NewMips,"mflo $v1\n");
                                            if(NewMipsTable.element[index_2].global)
                                                fprintf(NewMips,"sw $v1,%s\n",result);
                                            else
                                                fprintf(NewMips,"sw $v1,-%d($fp)\n",offset_2);
                                        }
                                        else
                                            fprintf(NewMips,"mflo $%s\n",resultRegName);
                                        break;
                                    }
                                    case '/':{
                                        fprintf(NewMips,"div $%s,$%s\n",src1RegName,src2RegName);
                                        if(save_to_mem){
                                            fprintf(NewMips,"mflo $v1\n");
                                            if(NewMipsTable.element[index_2].global)
                                                fprintf(NewMips,"sw $v1,%s\n",result);
                                            else
                                                fprintf(NewMips,"sw $v1,-%d($fp)\n",offset_2);
                                        }
                                        else
                                            fprintf(NewMips,"mflo $%s\n",resultRegName);
                                        break;
                                    }
                                }
                            }
                        }
                        break;
                    }
                    case 17:{
                        fprintf(NewMips,"j %s\n",MidCodeSet[k].src1);
                        break;
                    }
                    case 18:
                    case 19:{
                        int call_index = searchFucIndex(MidCodeSet[k].op);
                        for(int t = 0;t < a_point;t++){
                            aReg[astack[t]].occupy = true;
                        }
                        a_point = 0;
                        saveState(i,call_index);     //保存寄存器现场
                        fprintf(NewMips,"jal %s\n",MidCodeSet[k].op);
                        recoverState(i,call_index);
                        pushParaNum = 0;
                        break;
                    }
                    case 23:
                    case 28:{
                        if(MidCodeSet[k].codeKind == 23)
                            strcpy(src1,MidCodeSet[k].src1);
                        else
                            strcpy(src1,MidCodeSet[k].result);
                        index = searchNewTable(src1,i,false);
                        offset = (NewMipsTable.element[index].offset + 1) * 4;
                        if(MidCodeSet[k].codeKind == 23){
                            if(NewMipsTable.element[index].type == 1){
                                fprintf(NewMips,"li $v0,12\n");
                                fprintf(NewMips,"syscall\n");
                            }
                            else{
                                fprintf(NewMips,"li $v0,5\n");
                                fprintf(NewMips,"syscall\n");
                            }
                        }
                        if(NewMipsTable.element[index].kind == 1){
                            if(NewMipsTable.element[index].global){     //全局变量
                                fprintf(NewMips,"sw $v0,%s\n",src1);
                            }
                            else{
                                src1Index = find_s_reg(src1);
                                if(src1Index != -1){         //说明找到了
                                    fprintf(NewMips,"move $s%d,$v0\n",src1Index);
                                }
                                else{       //说明在内存中
                                    src1Index = find_free_s_reg(i);
                                    if(src1Index != -1){     //找一个空闲的
                                        strcpy(sReg[src1Index].varName,src1);
                                        sReg[src1Index].occupy = true;
                                        fprintf(NewMips,"move $s%d,$v0\n",src1Index);
                                    }
                                    else{
                                        src1Index = find_new_s_reg(j,src1);
                                        if(src1Index != -1){
                                            tempIndex = searchNewTable(sReg[src1Index].varName,i,false);
                                            tempOffset = (NewMipsTable.element[tempIndex].offset + 1) * 4;
                                            strcpy(sReg[src1Index].varName,src1);
                                            fprintf(NewMips,"sw $s%d,-%d($fp)\n",src1Index,tempOffset);
                                            fprintf(NewMips,"move $s%d,$v0\n",src1Index);
                                        }
                                        else{     //直接存到内存中
                                            fprintf(NewMips,"sw $v0,-%d($fp)\n",offset);
                                        }
                                    }
                                }
                            }
                        }
                        else if(NewMipsTable.element[index].kind == 5){
                            src1Index = find_t_reg(src1);
                            if(src1Index != -1){         //说明找到了
                                fprintf(NewMips,"move $t%d,$v0\n",src1Index);
                            }
                            else{
                                src1Index = find_free_t_reg();
                                if(src1Index != -1){     //找一个空闲的
                                    strcpy(tReg[src1Index].varName,src1);
                                    tReg[src1Index].occupy = true;
                                    fprintf(NewMips,"move $t%d,$v0\n",src1Index);
                                }
                                else{
                                    fprintf(NewMips,"sw $v0,-%d($fp)\n",offset);
                                }
                            }
                        }
                        else{    //只可能是参数
                            src1Index = find_a_reg(src1);
                            if(src1Index != -1){         //说明找到了
                                fprintf(NewMips,"move $a%d,$v0\n",src1Index);
                            }
                            else{       //说明在内存中,参数的个数一定大于4
                                fprintf(NewMips,"sw $v0,-%d($fp)\n",offset);
                            }
                        }
                        break;
                    }
                }
            }
            freeReg(j,block_list[i].blockNum);
        }
    }
}

void printNewTable(){
    for(int i = 0;i <= NewMipsTable.topIndex;i++){
        cout << NewMipsTable.element[i].name << " " << NewMipsTable.element[i].kind << " "
        << NewMipsTable.element[i].type << " " << NewMipsTable.element[i].value << " "
        << NewMipsTable.element[i].capicity << " " << NewMipsTable.element[i].offset << " "
        << NewMipsTable.element[i].global << endl;
    }
}

void GenNewMips(){
    CreateNewTable();
    genBlocks();     //生成基本块
    DAG_Optimize();
    constMerge();     //常数合并
    GenData();
    fprintf(NewMips,"\n");
    fprintf(NewMips,".text\n");
    fprintf(NewMips,"j main\n");
    initRegister();   //初始化寄存器
    GenText();
}
