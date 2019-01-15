//生成基本块
#include <stdio.h>
#include <iostream>
#include <string.h>
#include "global.h"


func_blocks block_list[100];
blockInfo info[1000];   //储存一个函数所有基本块的信息
blockInfo B_exit;
DAG_Struct dag_list[1000];   //储存DAG图
Point_List point_list[1000];   //节点表
TEMP_SAVE temp_save[1000];
MidCode NewCodeSet[MAXMIDCODE];    //新的中间代码
TempCode tempCodeSet[MAXMIDCODE];
NEW_TEMP_INFO tempvar_info[5000];
int varStack[1000];
int top;

int count_code;
int point_num = 0;  //DAG对应的节点编号
int list_size = 0;   //节点表大小
int tempCodeNum = 0;
int new_var_num;

int findStartIndex(char name[]);
int findEndIndex(char name[]);
int findLabelIndex(char name[]);
int findBlockIndex(int funcindex,int codeindex);
void cal_def_use(int funcIndex);
bool if_in_use(int blockIndex,int var);
bool if_in_def(int blockIndex,int var);
void mergeSet(int x,int y);
void calInSet(int x);
bool compare(int x,int* a);
bool judge_num(char name[]);
int get_value(char name[]);
int find_list(char name[]);
int find_Dag(char name[],int left,int right);
void genDAG(int funcIndex,int startIndex,int endIndex);
void genNewCode();
void copy_code(int x,int y);
void copy_new_code(int x,int y);
void exportFromDag(int funcIndex,int endIndex);
bool if_all_in();
bool not_have_father(int index);
bool father_all_in(int index);
bool satisfy_3(int index);
void GenTempCode(int codekind,char result[],char src1[],char op[],char src2[]);
int find_point_var(int funcIndex,int pointIndex);
bool find_if_next_use(int index,char name[]);
int find_list_index(int index);
bool satisfy_1(int index,char name[]);
void create_opti_table();
bool satisfy_2(int index,char name[]);

void test(){

    genNewCode();
    for(int i = 0;i < count_code;i++)
        printf("%s %s %s %s \n", NewCodeSet[i].result,NewCodeSet[i].src1,NewCodeSet[i].op,NewCodeSet[i].src2);

    /*
    liveAnalyza(2);
    int startIndex,endIndex;
    for(int i = 0;i < block_list[2].blockNum;i++){
        printf("Block %d\n",i);
        startIndex = block_list[2].blocks[i].startIndex;
        endIndex = block_list[2].blocks[i].endIndex;

        for(int j = startIndex;j <= endIndex;j++)
            printf("%s %s %s %s \n", MidCodeSet[j].result,MidCodeSet[j].src1,MidCodeSet[j].op,MidCodeSet[j].src2);
        printf("use: \n");
        for(int j = 0; j < info[i].use_num;j++){
            printf("%s ",NewMipsTable.element[info[i].use[j]].name);
        }
        printf("\n");
        printf("def: \n");
        for(int j = 0; j < info[i].def_num;j++){
            printf("%s ",NewMipsTable.element[info[i].def[j]].name);
        }
        printf("\n");
        printf("in: \n");
        for(int j = 0; j < info[i].in_num;j++){
            printf("%s ",NewMipsTable.element[info[i].in[j]].name);
        }
        printf("\n");
        printf("out: \n");
        for(int j = 0; j < info[i].out_num;j++){
            printf("%s ",NewMipsTable.element[info[i].out[j]].name);
        }
        printf("\n");

        printf("before: \n");
        for(int j = 0; j <  block_list[1].blocks[i].before_num;j++){
            printf("%d ",block_list[1].blocks[i].before[j]);
        }
        printf("\n");
        printf("after: \n");
        for(int j = 0; j <  block_list[1].blocks[i].after_num;j++){
            printf("%d ",block_list[1].blocks[i].after[j]);
        }
        printf("\n");
        printf("\n");
    }
    */
}


void genBlocks(){
    char func_name[100];
    char label_name[100];
    int startIndex,endIndex;
    for(int i = 0;i < NewMipsTable.numOfFunc;i++){
        /*入口语句划分*/
        strcpy(func_name,NewMipsTable.element[NewMipsTable.funcIndex[i]].name);
        startIndex = findStartIndex(func_name);
        endIndex = findEndIndex(func_name);
        MidCodeSet[startIndex].enter = true;
        for(int j = startIndex;j <= endIndex;j++){
            if(MidCodeSet[j].codeKind == 9 || (MidCodeSet[j].codeKind >= 11 && MidCodeSet[j].codeKind <= 17)){
                if(MidCodeSet[j].codeKind == 17)
                    strcpy(label_name,MidCodeSet[j].src1);
                else
                    strcpy(label_name,MidCodeSet[j].result);
                for(int k = startIndex;k <= endIndex;k++){
                    if(MidCodeSet[k].codeKind == 10){
                        if(strcmp(MidCodeSet[k].result,label_name) == 0)
                            MidCodeSet[k].enter = true;
                    }
                }
                if(j + 1 <= endIndex)
                    MidCodeSet[j+1].enter = true;
            }
            else if(MidCodeSet[j].codeKind == 7 || MidCodeSet[j].codeKind == 25){
                if(j + 1 <= endIndex)
                    MidCodeSet[j+1].enter = true;
            }
        }
        /*生成基本块*/
        int blockCount = 0;
        block_list[i].funcIndex = i;
        block_list[i].blocks[blockCount].startIndex = startIndex;
        for(int j = startIndex + 1;j <= endIndex;j++){
            if(MidCodeSet[j].enter){
                block_list[i].blocks[blockCount].endIndex = j - 1;
                blockCount++;
                block_list[i].blocks[blockCount].startIndex = j;
            }
        }
        block_list[i].blocks[blockCount].endIndex = endIndex;
        block_list[i].blockNum = blockCount + 1;
        /*画出基本块流图*/
        int last,labelIndex,blockIndex;
        for(int j = 0;j <= blockCount;j++){
            block_list[i].blocks[blockCount].before_num = 0;
            block_list[i].blocks[blockCount].after_num = 0;
        }
        block_list[i].blocks[0].before[0] = -1;
        block_list[i].blocks[0].before_num++;
        for(int j = 0;j <= blockCount;j++){
            last = block_list[i].blocks[j].endIndex;
            if(MidCodeSet[last].codeKind == 7 || MidCodeSet[last].codeKind == 25){
                block_list[i].blocks[j].after[block_list[i].blocks[j].after_num] = -2;
                block_list[i].blocks[j].after_num++;
            }
            else if(MidCodeSet[last].codeKind == 17){
                labelIndex = findLabelIndex(MidCodeSet[last].src1);
                blockIndex = findBlockIndex(i,labelIndex);
                block_list[i].blocks[j].after[block_list[i].blocks[j].after_num] = blockIndex;
                block_list[i].blocks[j].after_num++;
                block_list[i].blocks[blockIndex].before[block_list[i].blocks[blockIndex].before_num] = j;
                block_list[i].blocks[blockIndex].before_num++;
            }
            else if(MidCodeSet[last].codeKind == 9 || (MidCodeSet[last].codeKind >= 11 && MidCodeSet[last].codeKind <= 16)){
                labelIndex = findLabelIndex(MidCodeSet[last].result);
                blockIndex = findBlockIndex(i,labelIndex);
                block_list[i].blocks[j].after[block_list[i].blocks[j].after_num] = blockIndex;
                block_list[i].blocks[j].after_num++;
                block_list[i].blocks[blockIndex].before[block_list[i].blocks[blockIndex].before_num] = j;
                block_list[i].blocks[blockIndex].before_num++;
                if(j + 1 > blockCount){
                    block_list[i].blocks[j].after[block_list[i].blocks[j].after_num] = -2;
                    block_list[i].blocks[j].after_num++;
                }
                else{
                    block_list[i].blocks[j].after[block_list[i].blocks[j].after_num] = j+1;
                    block_list[i].blocks[j].after_num++;
                    block_list[i].blocks[j+1].before[block_list[i].blocks[j+1].before_num] = j;
                    block_list[i].blocks[j+1].before_num++;
                }
            }
            else{
                if(j + 1 > blockCount){
                    block_list[i].blocks[j].after[block_list[i].blocks[j].after_num] = -2;
                    block_list[i].blocks[j].after_num++;
                }
                else{
                    block_list[i].blocks[j].after[block_list[i].blocks[j].after_num] = j+1;
                    block_list[i].blocks[j].after_num++;
                    block_list[i].blocks[j+1].before[block_list[i].blocks[j+1].before_num] = j;
                    block_list[i].blocks[j+1].before_num++;
                }
            }
        }
    }
    /*
    for(int i = 0; i < NewMipsTable.numOfFunc;i++){
        int index;
        cout << "funcName: " << NewMipsTable.element[NewMipsTable.funcIndex[block_list[i].funcIndex]].name << endl;
        cout << "block_num: " << block_list[i].blockNum << endl;
        cout << "blocks: " << endl;
        for(int j = 0;j < block_list[i].blockNum;j++){
            for(int k = 0; k < block_list[i].blocks[j].after_num;k++)
                cout << block_list[i].blocks[j].after[k] << endl;
            printf("\n");
        }
        printf("\n");
    }
    */
}

void liveAnalyza(int funcIndex){      //活跃变量分析
    int blockNum = block_list[funcIndex].blockNum;
    int block_index;
    cal_def_use(funcIndex);        //计算每个基本块的def和use
    int origin_set[1000],origin_num;
    for(int i = 0;i < blockNum;i++){
        info[i].in_num = 0;
        info[i].out_num = 0;
    }
    B_exit.in_num = 0;
    B_exit.out_num = 0;
    bool ifchange = false;
    do{
        for(int i = blockNum - 1;i >= 0;i--){
            //计算block i的out
            info[i].out_num = 0;
            for(int j = 0;j < block_list[funcIndex].blocks[i].after_num;j++){
                block_index = block_list[funcIndex].blocks[i].after[j];
                mergeSet(i,block_index);
            }
            //计算block i的in
            origin_num = 0;
            for(int j = 0;j < info[i].in_num;j++){
                origin_set[origin_num++] = info[i].in[j];
            }
            info[i].in_num = 0;
            calInSet(i);
            if(info[i].in_num != origin_num)
                ifchange = true;
            else
                ifchange = compare(i,origin_set);
        }
    }while(ifchange);
}

bool compare(int x,int* a){
    bool if_in;
    for(int i = 0;i < info[x].in_num;i++){
        if_in = false;
        for(int j = 0;j < info[x].in_num;j++){
            if(info[x].in[i] == a[j])
                if_in = true;
        }
        if(!if_in)
            return true;
    }
    return false;
}

void calInSet(int x){
    bool if_in;
    int out_minus_def_num = 0;
    int out_minus_def[1000];
    //计算out-def
    for(int i = 0;i < info[x].out_num;i++){
        if_in = false;
        for(int j = 0;j < info[x].def_num;j++){
            if(info[x].def[j] == info[x].out[i])
                if_in = true;                   //说明即在out,也在def
        }
        if(!if_in){
            out_minus_def[out_minus_def_num] = info[x].out[i];
            out_minus_def_num++;
        }
    }
    //将use并入到in中
    for(int i = 0;i < info[x].use_num;i++){
        info[x].in[info[x].in_num] = info[x].use[i];
        info[x].in_num++;
    }
    //计算并集
    for(int i = 0;i < out_minus_def_num;i++){
        if_in = false;
        for(int j = 0;j < info[x].in_num;j++){
            if(info[x].in[j] == out_minus_def[i])
                if_in = true;
        }
        if(!if_in){
            info[x].in[info[x].in_num] = out_minus_def[i];
            info[x].in_num++;
        }
    }
}

void mergeSet(int x,int y){
    bool if_in = false;
    if(y < 0){   //说明是出口块
        //不改变
    }
    else{
        for(int i = 0;i < info[y].in_num;i++){
            if_in = false;
            for(int j = 0;j < info[x].out_num;j++){
                if(info[x].out[j] == info[y].in[i])
                    if_in = true;
            }
            if(!if_in){     //如果没在out集合里面
                info[x].out[info[x].out_num] = info[y].in[i];
                info[x].out_num++;
            }
        }
    }
}

void cal_def_use(int funcIndex){   //funcIndex是从0开始
    int blockNum = block_list[funcIndex].blockNum;
    int startIndex,endIndex,index;
    char varName[100];
    for(int i = 0;i < blockNum;i++){
        info[i].def_num = 0;
        info[i].use_num = 0;
    }
    B_exit.def_num = 0;
    B_exit.use_num = 0;
    for(int i = 0;i < blockNum;i++){
        startIndex = block_list[funcIndex].blocks[i].startIndex;
        endIndex = block_list[funcIndex].blocks[i].endIndex;
        for(int j = startIndex;j <= endIndex;j++){
            switch(MidCodeSet[j].codeKind){
                case 9:
                case 20:
                case 23:
                case 24:
                case 25:{
                    if(MidCodeSet[j].codeKind == 24)
                        strcpy(varName,MidCodeSet[j].op);
                    else
                        strcpy(varName,MidCodeSet[j].src1);
                    index = searchNewTable(varName,funcIndex,false);
                    if(index != -1){       //如果为-1，说明是常数。
                        if(!if_in_def(i,index) && !if_in_use(i,index)){
                            info[i].use[info[i].use_num] = index;
                            info[i].use_num++;
                        }
                    }
                    break;
                }
                case 11:
                case 12:
                case 13:
                case 14:
                case 15:
                case 16:{
                    strcpy(varName,MidCodeSet[j].src1);
                    index = searchNewTable(varName,funcIndex,false);
                    if(index != -1){       //如果为-1，说明是常数。
                        if(!if_in_def(i,index) && !if_in_use(i,index)){
                            info[i].use[info[i].use_num] = index;
                            info[i].use_num++;
                        }
                    }
                    strcpy(varName,MidCodeSet[j].src2);
                    index = searchNewTable(varName,funcIndex,false);
                    if(index != -1){       //如果为-1，说明是常数。
                        if(!if_in_def(i,index) && !if_in_use(i,index)){
                            info[i].use[info[i].use_num] = index;
                            info[i].use_num++;
                        }
                    }
                    break;
                }
                case 21:{
                    strcpy(varName,MidCodeSet[j].src1);
                    index = searchNewTable(varName,funcIndex,false);
                    if(index != -1){       //如果为-1，说明是常数。
                        if(!if_in_def(i,index) && !if_in_use(i,index)){
                            info[i].use[info[i].use_num] = index;
                            info[i].use_num++;
                        }
                    }
                    strcpy(varName,MidCodeSet[j].result);
                    index = searchNewTable(varName,funcIndex,false);
                    if(index != -1){       //如果为-1，说明是常数。
                        if(!if_in_def(i,index) && !if_in_use(i,index)){
                            info[i].def[info[i].def_num] = index;
                            info[i].def_num++;
                        }
                    }
                    break;
                }
                case 22:{
                    strcpy(varName,MidCodeSet[j].src1);
                    index = searchNewTable(varName,funcIndex,false);
                    if(index != -1){       //如果为-1，说明是常数。
                        if(!if_in_def(i,index) && !if_in_use(i,index)){
                            info[i].use[info[i].use_num] = index;
                            info[i].use_num++;
                        }
                    }
                    strcpy(varName,MidCodeSet[j].op);
                    index = searchNewTable(varName,funcIndex,false);
                    if(index != -1){       //如果为-1，说明是常数。
                        if(!if_in_def(i,index) && !if_in_use(i,index)){
                            info[i].use[info[i].use_num] = index;
                            info[i].use_num++;
                        }
                    }
                    break;
                }
                case 26:{
                    strcpy(varName,MidCodeSet[j].src1);
                    index = searchNewTable(varName,funcIndex,false);
                    if(index != -1){       //如果为-1，说明是常数。
                        if(!if_in_def(i,index) && !if_in_use(i,index)){
                            info[i].use[info[i].use_num] = index;
                            info[i].use_num++;
                        }
                    }
                    strcpy(varName,MidCodeSet[j].src2);
                    index = searchNewTable(varName,funcIndex,false);
                    if(index != -1){       //如果为-1，说明是常数。
                        if(!if_in_def(i,index) && !if_in_use(i,index)){
                            info[i].use[info[i].use_num] = index;
                            info[i].use_num++;
                        }
                    }
                    strcpy(varName,MidCodeSet[j].result);
                    index = searchNewTable(varName,funcIndex,false);
                    if(index != -1){       //如果为-1，说明是常数。
                        if(!if_in_def(i,index) && !if_in_use(i,index)){
                            info[i].def[info[i].def_num] = index;
                            info[i].def_num++;
                        }
                    }
                    break;
                }
                case 27:{
                    strcpy(varName,MidCodeSet[j].op);
                    index = searchNewTable(varName,funcIndex,false);
                    if(index != -1){       //如果为-1，说明是常数。
                        if(!if_in_def(i,index) && !if_in_use(i,index)){
                            info[i].use[info[i].use_num] = index;
                            info[i].use_num++;
                        }
                    }
                    strcpy(varName,MidCodeSet[j].result);
                    index = searchNewTable(varName,funcIndex,false);
                    if(index != -1){       //如果为-1，说明是常数。
                        if(!if_in_def(i,index) && !if_in_use(i,index)){
                            info[i].def[info[i].def_num] = index;
                            info[i].def_num++;
                        }
                    }
                    break;
                }
                case 28:{
                    strcpy(varName,MidCodeSet[j].result);
                    index = searchNewTable(varName,funcIndex,false);
                    if(index != -1){       //如果为-1，说明是常数。
                        if(!if_in_def(i,index) && !if_in_use(i,index)){
                            info[i].def[info[i].def_num] = index;
                            info[i].def_num++;
                        }
                    }
                    break;
                }
            }
        }
    }
}

bool judge_num(char name[]){
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

int get_value(char name[]){
    int value = 0,signs = 1;
    if(judge_num(name)){
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

void constMerge(){
    int startIndex,endIndex;
    int the_value,src1Value,src2Value;
    char tempName[100],index;
    for(int i = 0;i < NewMipsTable.numOfFunc;i++){
        for(int j = 0;j < block_list[i].blockNum;j++){
            startIndex = block_list[i].blocks[j].startIndex;
            endIndex = block_list[i].blocks[j].endIndex;
            for(int k = startIndex;k <= endIndex;k++){
                if(!MidCodeSet[k].effect)
                    continue;
                if(MidCodeSet[k].codeKind == 26){
                    index = searchNewTable(MidCodeSet[k].result,i,false);
                    if(judge_num(MidCodeSet[k].src1) && judge_num(MidCodeSet[k].src2) && NewMipsTable.element[index].kind == 5){  //都是常数
                        src1Value = get_value(MidCodeSet[k].src1);
                        src2Value = get_value(MidCodeSet[k].src2);
                        switch(MidCodeSet[k].op[0]){
                            case '+':{
                                the_value = src1Value + src2Value;
                                break;
                            }
                            case '-':{
                                the_value = src1Value - src2Value;
                                break;
                            }
                            case '*':{
                                the_value = src1Value * src2Value;
                                break;
                            }
                            case '/':{
                                the_value = src1Value / src2Value;
                                break;
                            }
                        }
                        strcpy(tempName,MidCodeSet[k].result);
                        MidCodeSet[k].effect = false;
                        for(int t = startIndex;t <= endIndex;t++){
                            if(!MidCodeSet[t].effect)
                                continue;
                            if(strcmp(MidCodeSet[t].result,tempName) == 0)
                                sprintf(MidCodeSet[t].result,"%d",the_value);
                            if(strcmp(MidCodeSet[t].src1,tempName) == 0)
                                sprintf(MidCodeSet[t].src1,"%d",the_value);
                            if(strcmp(MidCodeSet[t].op,tempName) == 0)
                                sprintf(MidCodeSet[t].op,"%d",the_value);
                            if(strcmp(MidCodeSet[t].src2,tempName) == 0)
                                sprintf(MidCodeSet[t].src2,"%d",the_value);
                        }
                    }
                }
            }
        }
    }
}

void copy_code(int x,int y){
    strcpy(NewCodeSet[x].result,MidCodeSet[y].result);
    strcpy(NewCodeSet[x].src1,MidCodeSet[y].src1);
    strcpy(NewCodeSet[x].src2,MidCodeSet[y].src2);
    strcpy(NewCodeSet[x].op,MidCodeSet[y].op);
    NewCodeSet[x].codeKind = MidCodeSet[y].codeKind;
    NewCodeSet[x].effect = true;
}

void copy_new_code(int x,int y){
    strcpy(NewCodeSet[x].result,tempCodeSet[y].result);
    strcpy(NewCodeSet[x].src1,tempCodeSet[y].src1);
    strcpy(NewCodeSet[x].src2,tempCodeSet[y].src2);
    strcpy(NewCodeSet[x].op,tempCodeSet[y].op);
    NewCodeSet[x].codeKind = tempCodeSet[y].codeKind;
    NewCodeSet[x].effect = true;
}

void DAG_Optimize(){
    genNewCode();
    codeCount = 0;
    for(int i = 0;i < count_code;i++){
        strcpy(MidCodeSet[codeCount].result,NewCodeSet[i].result);
        strcpy(MidCodeSet[codeCount].src1,NewCodeSet[i].src1);
        strcpy(MidCodeSet[codeCount].src2,NewCodeSet[i].src2);
        strcpy(MidCodeSet[codeCount].op,NewCodeSet[i].op);
        MidCodeSet[codeCount].codeKind = NewCodeSet[i].codeKind;
        MidCodeSet[codeCount].effect = true;
        MidCodeSet[codeCount].enter = false;
        codeCount++;
    }
    create_opti_table();
    genBlocks();
}

void create_opti_table(){
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
        for(int j = 0;j < new_var_num;j++){
            if(tempvar_info[j].funcIndex == i){
                NewMipsTable.topIndex++;
                strcpy(NewMipsTable.element[NewMipsTable.topIndex].name,tempvar_info[j].name);
                NewMipsTable.element[NewMipsTable.topIndex].kind = 5;
                NewMipsTable.element[NewMipsTable.topIndex].type = tempvar_info[j].type;
                NewMipsTable.element[NewMipsTable.topIndex].value = 0;    //先把中间变量都初始化为0
                NewMipsTable.element[NewMipsTable.topIndex].capicity = -1;
                NewMipsTable.element[NewMipsTable.topIndex].offset = offset;
                NewMipsTable.element[NewMipsTable.topIndex].global = false;
                NewMipsTable.element[NewMipsTable.topIndex].RegNum = -1;
                offset += 1;
            }
        }
        NewMipsTable.element[NewMipsTable.funcIndex[i]].functSize = offset;
    }
}

void genNewCode(){   //从DAG图导出
    int startIndex,endIndex;
    int start,End,k;
    bool start_count;
    count_code = 0;
    new_var_num = 0;
    for(int i = 0;i < block_list[0].blocks[0].startIndex;i++){
        copy_code(count_code,i);
        count_code++;
    }
    for(int i = 0;i < NewMipsTable.numOfFunc;i++){
        for(int j = 0;j < block_list[i].blockNum;j++){
            startIndex = block_list[i].blocks[j].startIndex;
            endIndex = block_list[i].blocks[j].endIndex;
            k = startIndex;
            start_count = false;
            while(k <= endIndex){
                if(MidCodeSet[k].effect){
                    if(MidCodeSet[k].codeKind == 26 || MidCodeSet[k].codeKind == 21){
                        if(!start_count){
                            start = k;
                            start_count = true;
                        }
                    }
                    else{
                        if(start_count){
                            End = k - 1;
                            start_count = false;
                            genDAG(i,start,End);
                            for(int t = 0;t < tempCodeNum;t++){
                                if(tempCodeSet[t].effect){
                                    copy_new_code(count_code,t);
                                    count_code++;
                                }
                            }
                        }
                        copy_code(count_code,k);
                        count_code++;
                    }
                }
                k++;
            }
            if(start_count){
                End = k - 1;
                start_count = false;
                genDAG(i,start,End);
                for(int t = 0;t < tempCodeNum;t++){
                    if(tempCodeSet[t].effect){
                        copy_new_code(count_code,t);
                        count_code++;
                    }
                }
            }
        }
    }
}

void GenTempCode(int codekind,char result[],char src1[],char op[],char src2[]){
    if(tempCodeNum > MAXMIDCODE){
        return;
    }
    tempCodeSet[tempCodeNum].codeKind = codekind;
    tempCodeSet[tempCodeNum].effect = true;
    strcpy(tempCodeSet[tempCodeNum].op,op);
    strcpy(tempCodeSet[tempCodeNum].result,result);
    strcpy(tempCodeSet[tempCodeNum].src1,src1);
    strcpy(tempCodeSet[tempCodeNum].src2,src2);
    tempCodeNum++;
}

void genDAG(int funcIndex,int startIndex,int endIndex){
    char src1[100],src2[100],op[100],result[100];
    char tempVar[100];
    point_num = 0;
    list_size = 0;
    tempCodeNum = 0;
    int src1_point_index,src2_point_index,op_point_index,index;
    for(int i = startIndex;i <= endIndex;i++){
        strcpy(src1,MidCodeSet[i].src1);strcpy(op,MidCodeSet[i].op);
        strcpy(result,MidCodeSet[i].result);strcpy(src2,MidCodeSet[i].src2);
        index = searchNewTable(src1,funcIndex,false);
        if(find_list(src1) != -1){
            src1_point_index = point_list[find_list(src1)].index;
        }
        else{
            dag_list[point_num].index = point_num + 1;
            dag_list[point_num].leaf = true;
            dag_list[point_num].if_in = false;
            dag_list[point_num].left = -1;
            dag_list[point_num].right = -1;
            if(index != -1){
                sprintf(tempVar,"#%s",src1);
                strcpy(dag_list[point_num].indentify,tempVar);
                strcpy(point_list[list_size].name,src1);
            }
            else{
                strcpy(dag_list[point_num].indentify,src1);
                strcpy(point_list[list_size].name,src1);
            }
            point_num++;
            point_list[list_size].index = point_num;
            list_size++;
            src1_point_index = point_num;
        }
        if(strlen(src2) != 0){
            index = searchNewTable(src2,funcIndex,false);
            if(find_list(src2) != -1){
                src2_point_index = point_list[find_list(src2)].index;
            }
            else{
                dag_list[point_num].index = point_num + 1;
                dag_list[point_num].leaf = true;
                dag_list[point_num].if_in = false;
                dag_list[point_num].left = -1;
                dag_list[point_num].right = -1;
                if(index != -1){
                    sprintf(tempVar,"#%s",src2);
                    strcpy(dag_list[point_num].indentify,tempVar);
                    strcpy(point_list[list_size].name,src2);
                }
                else{
                    strcpy(dag_list[point_num].indentify,src2);
                    strcpy(point_list[list_size].name,src2);
                }
                point_num++;
                point_list[list_size].index = point_num;
                list_size++;
                src2_point_index = point_num;
            }
        }
        if(strlen(op) != 0){
            if(find_Dag(op,src1_point_index,src2_point_index) != -1){
                op_point_index = find_Dag(op,src1_point_index,src2_point_index);
            }
            else{
                dag_list[point_num].index = point_num + 1;
                dag_list[point_num].leaf = false;
                dag_list[point_num].if_in = false;
                dag_list[point_num].left = src1_point_index;
                dag_list[point_num].right = src2_point_index;
                strcpy(dag_list[point_num].indentify,op);
                point_num++;
                op_point_index = point_num;
            }
        }
        if(find_list(result) != -1){
            if(strlen(op) == 0)
                point_list[find_list(result)].index = src1_point_index;
            else
                point_list[find_list(result)].index = op_point_index;
        }
        else{
            if(strlen(op) == 0)
                point_list[list_size].index = src1_point_index;
            else
                point_list[list_size].index = op_point_index;
            strcpy(point_list[list_size].name,result);
            list_size++;
        }
    }
    exportFromDag(funcIndex,endIndex);
}

void exportFromDag(int funcIndex,int endIndex){    //从DAG图导出中间代码
    top = -1;
    int index,var_index;
    bool if_in;
    while(!if_all_in()){   //还有节点没有加入到队列中
        for(int i = 0;i < point_num;i++){
            if(satisfy_3(dag_list[i].index)){   //如果满足条件3
                varStack[++top] = dag_list[i].index;
                dag_list[i].if_in = true;
                index = dag_list[i].left - 1;
                while(satisfy_3(dag_list[index].index)){
                    varStack[++top] = dag_list[index].index;
                    dag_list[index].if_in = true;
                    index = dag_list[index].left - 1;
                }
            }
        }
    }
    int resultIndex,originIndex;
    char originName[100],src1[100],src2[100];
    for(int i = 0;i < point_num;i++){
        if(dag_list[i].leaf){
            if(strchr(dag_list[i].indentify,'#') != NULL){
                if(satisfy_1(dag_list[i].index,dag_list[i].indentify)){
                    strcpy(dag_list[i].indentify,dag_list[i].indentify + 1);  //将a0改成a
                }
                else{
                    strcpy(originName,dag_list[i].indentify + 1);
                    GenTempCode(21,dag_list[i].indentify,originName,"","");
                    strcpy(tempvar_info[new_var_num].name,dag_list[i].indentify);
                    originIndex = searchNewTable(originName,funcIndex,false);
                    tempvar_info[new_var_num].type = NewMipsTable.element[originIndex].type;
                    tempvar_info[new_var_num].funcIndex = funcIndex;
                    new_var_num++;
                }
            }
        }
    }
    while(top != -1){
        index = varStack[top--];
        resultIndex = find_point_var(funcIndex,index);   //在节点表中的位置
        temp_save[index].list_loc = resultIndex;
        temp_save[index].point_index = index;
        if(dag_list[dag_list[index - 1].left - 1].leaf)
            strcpy(src1,dag_list[dag_list[index - 1].left - 1].indentify);
        else
            strcpy(src1,point_list[temp_save[dag_list[index - 1].left].list_loc].name);
        if(dag_list[dag_list[index - 1].right - 1].leaf)
            strcpy(src2,dag_list[dag_list[index - 1].right - 1].indentify);
        else
            strcpy(src2,point_list[temp_save[dag_list[index - 1].right].list_loc].name);
        GenTempCode(26,point_list[resultIndex].name,src1,dag_list[index - 1].indentify,src2);
        for(int j = 0;j < list_size;j++){
            if(point_list[j].index == index && j != resultIndex){
                var_index = searchNewTable(point_list[j].name,funcIndex,false);
                if(NewMipsTable.element[var_index].kind != 5){   //不是局部变量
                    GenTempCode(21,point_list[j].name,point_list[resultIndex].name,"","");
                }
                else{
                    if(find_if_next_use(endIndex,point_list[j].name)){
                        GenTempCode(21,point_list[j].name,point_list[resultIndex].name,"","");
                    }
                }
            }
        }
    }
    for(int i = 0;i < list_size;i++){
        if(dag_list[point_list[i].index - 1].leaf){
            if(strcmp(point_list[i].name,dag_list[point_list[i].index - 1].indentify) != 0){
                var_index = searchNewTable(point_list[i].name,funcIndex,false);
                if(NewMipsTable.element[var_index].kind != 5){   //不是局部变量
                    GenTempCode(21,point_list[i].name,dag_list[point_list[i].index - 1].indentify,"","");
                }
                else{
                    if(find_if_next_use(endIndex,point_list[i].name)){
                        GenTempCode(21,point_list[i].name,dag_list[point_list[i].index - 1].indentify,"","");
                    }
                }
            }
        }
    }
    for(int i = 0;i < tempCodeNum;i++){
        if(tempCodeSet[i].codeKind == 21){
            if(strchr(tempCodeSet[i].result,'#') != NULL){
                if(satisfy_2(i,tempCodeSet[i].result)){
                    tempCodeSet[i].effect = false;
                    for(int j = i + 1;j < tempCodeNum;j++){
                        if(strcmp(tempCodeSet[j].op,tempCodeSet[i].result) == 0)
                            strcpy(tempCodeSet[j].op,tempCodeSet[i].result + 1);
                        if(strcmp(tempCodeSet[j].src1,tempCodeSet[i].result) == 0)
                            strcpy(tempCodeSet[j].src1,tempCodeSet[i].result + 1);
                        if(strcmp(tempCodeSet[j].src2,tempCodeSet[i].result) == 0)
                            strcpy(tempCodeSet[j].src2,tempCodeSet[i].result + 1);
                    }
                }
            }
        }
    }
}

bool satisfy_2(int index,char name[]){
    char originName[100];
    int startIndex;
    strcpy(originName,name + 1);
    for(int i = index + 1;i < tempCodeNum;i++){
        if(strcmp(tempCodeSet[i].result,originName) == 0)
            startIndex = i;
    }
    for(int i = startIndex + 1;i < tempCodeNum;i++){
        if(strcmp(tempCodeSet[i].op,name) == 0)
            return false;
        if(strcmp(tempCodeSet[i].src1,name) == 0)
            return false;
        if(strcmp(tempCodeSet[i].src2,name) == 0)
            return false;
    }
    return true;
}

int find_list_index(int index){
    for(int i = 0;i < list_size;i++){
        if(strcmp(point_list[i].name,dag_list[index - 1].indentify) == 0)
            return i;
    }
    cout << "WRONG !!!"  << endl;
    return -1;
}

bool find_if_next_use(int index,char name[]){
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

bool satisfy_1(int index,char name[]){
    char origin_name[100];
    strcpy(origin_name,name + 1);
    for(int i = 0;i < list_size;i++){
        if(point_list[i].index == index){
            if(strcmp(point_list[i].name,origin_name) == 0){
                return true;
            }
        }
    }
    return false;
}

int find_point_var(int funcIndex,int pointIndex){
    int result;
    char varIndex;
    for(int i = 0;i < list_size;i++){
        if(point_list[i].index == pointIndex){
            result = i;
            varIndex = searchNewTable(point_list[i].name,funcIndex,false);
            if(varIndex != -1 && NewMipsTable.element[varIndex].kind != 5)
                return i;
        }
    }
    return result;
}

bool if_all_in(){
    for(int i = 0;i < point_num;i++){
        if(!dag_list[i].leaf && !dag_list[i].if_in)
            return false;
    }
    return true;
}

bool not_have_father(int index){
    for(int i = 0;i < point_num;i++){
        if(dag_list[i].left == index)
            return false;
        if(dag_list[i].right == index)
            return false;
    }
    return true;
}

bool father_all_in(int index){
    for(int i = 0;i < point_num;i++){
        if(dag_list[i].left == index){
            if(!dag_list[i].if_in)
                return false;
        }
        if(dag_list[i].right == index){
            if(!dag_list[i].if_in)
                return false;
        }
    }
    return true;
}

bool satisfy_3(int index){
    if(!dag_list[index - 1].leaf){        //表明是中间节点
        if(!dag_list[index - 1].if_in){
            if(not_have_father(index))
                return true;
            if(father_all_in(index))
                return true;
        }
    }
    return false;
}

int find_list(char name[]){
    for(int i = 0;i < list_size;i++){
        if(strcmp(point_list[i].name,name) == 0)
            return i;
    }
    return -1;
}

int find_Dag(char name[],int left,int right){
    for(int i = 0;i < point_num;i++){
        if(strcmp(dag_list[i].indentify,name) == 0){
            if(dag_list[i].left == left && dag_list[i].right == right)
                return dag_list[i].index;
        }
    }
    return -1;
}

bool if_in_use(int blockIndex,int var){
    for(int i = 0; i < info[blockIndex].use_num;i++){
        if(info[blockIndex].use[i] == var)
            return true;
    }
    return false;
}

bool if_in_def(int blockIndex,int var){
    for(int i = 0; i < info[blockIndex].def_num;i++){
        if(info[blockIndex].def[i] == var)
            return true;
    }
    return false;
}

int findStartIndex(char name[]){
    for(int i = 0;i < codeCount;i++){
        if(MidCodeSet[i].codeKind == 5 || MidCodeSet[i].codeKind == 6){
            if(strcmp(MidCodeSet[i].src1,name) == 0)
                return i;
        }
    }
    return -1;
}

int findEndIndex(char name[]){
    if(strcmp(name,"main") == 0)
        return codeCount - 1;
    for(int i = 0;i < codeCount;i++){
        if(MidCodeSet[i].codeKind == 5 || MidCodeSet[i].codeKind == 6){
            if(strcmp(MidCodeSet[i].src1,name) == 0){
                for(int j = i + 1;j < codeCount;j++){
                    if(MidCodeSet[j].codeKind == 5 || MidCodeSet[j].codeKind == 6)
                        return j - 1;
                }
            }
        }
    }
    return -1;
}

int findLabelIndex(char name[]){
    for(int i = 0;i < codeCount;i++){
        if(MidCodeSet[i].codeKind == 10){
            if(strcmp(MidCodeSet[i].result,name) == 0)
                return i;
        }
    }
    return -1;
}

int findBlockIndex(int funcindex,int codeindex){
    for(int i = 0;i < block_list[funcindex].blockNum;i++){
        if(codeindex >= block_list[funcindex].blocks[i].startIndex && codeindex <= block_list[funcindex].blocks[i].endIndex)
            return i;
    }
    return -1;
}
