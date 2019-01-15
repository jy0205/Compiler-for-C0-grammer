#include <stdio.h>
#include <string>
#include <string.h>
#include "global.h"
#include "errorInfo.h"

char nowChar = ' ';         //保存当前的字符
char line[1000];            //保存读入的一行
char token[100];           //保存单词
char symbol[100];

/*保留字表*/
string KeyWord[] = {"int","char","void","main","const","if","while",
                    "switch","case","default","scanf","printf","return"};
string KeySymbol[] = {"INTSYM","CHARSYM","VOIDSYM","MAINSYM","CONSTSYM",
                     "IFSYM","WHILESYM","SWITCHSYM","CASESYM","DEFAULTSYM",
                     "SCANFSYM","PRINTFSYM","RETURNSYM"};

/*变量定义
    其中wordkind表示单词种类：
    1表示标识符      2表示整数
    3表示字符        4表示字符串
    5表示单分隔符    6表示双分隔符
*/
const int keyNum = 13;
int symID;
int wordkind = 0;
int LengthOfLine = 0,readloc = 1,counter = 0,lineNum = 0;
bool endflag = false;

/*函数声明*/
bool isAlpha(char ch);
bool isNum(char ch);
bool isCalOp(char ch);
bool checkChar();
bool checkStr();
bool checkNum();
void classify();
void pregetsym();

void getch(){
    if(readloc > LengthOfLine){  //读完一行
        if(fgets(line,1000,fin) != NULL){
            readloc = 0;
            lineNum++;
            LengthOfLine = strlen(line) - 1;
            if(strlen(line) == 0){
                nowChar = '\n';
            }
            else{
                nowChar = line[readloc];
                readloc++;
            }
        }
        else{
            endflag = true;   //表示读到了文件的结尾
            nowChar = '#';
        }
    }
    else{
        nowChar = line[readloc];
        readloc++;
    }
}

void getsym(){
    pregetsym();
    if(wordkind == 2 && !checkNum())
        error(ZERO_BEFORE_NUM,lineNum);
	else if(wordkind == 3 && !checkChar())
		error(WRONG_CHAR_ERROR,lineNum);
	else if(wordkind == 4 && !checkStr())
		error(WRONG_STRING_ERROR,lineNum);
    else if(wordkind == 7){
        error(ILLEGAL_CHAR,lineNum);
        do{
            pregetsym();
        }while(wordkind == 7);
    }
}

/*读一个单词*/
void pregetsym(){
    counter = 0;
    while((nowChar == ' ' || nowChar == '\t' || nowChar == '\n') && !endflag)
        getch();
    if(endflag)
        return;
    else{
        if(isAlpha(nowChar)){  //说明是标识符
            wordkind = 1;
            token[counter++] = nowChar;
            getch();
            while(isAlpha(nowChar) || isNum(nowChar)){
                token[counter++] = nowChar;
                getch();
            }
        }
        else if(isNum(nowChar)){
            wordkind = 2;
            token[counter++] = nowChar;
            getch();
            while(isNum(nowChar)){
                token[counter++] = nowChar;
                getch();
            }
        }
        else if(nowChar == '+' || nowChar == '-' || nowChar == '*' || nowChar == '/' || nowChar == ':' || nowChar == ';' || nowChar == ','){
            wordkind = 5;
            token[counter++] = nowChar;
            getch();
        }
        else if(nowChar == '(' || nowChar == ')' || nowChar == '{' || nowChar == '}' || nowChar == '[' || nowChar == ']'){
            wordkind = 5;
            token[counter++] = nowChar;
            getch();
        }
        else if(nowChar == '\''){  //字符
            wordkind = 3;
            do{
                token[counter++] = nowChar;
                getch();
            }while(nowChar != '\'' && !endflag);
            if(!endflag){
            	token[counter++] = nowChar;
            	getch();
            }
        	else
        		wordkind = 7;
        }
        else if(nowChar == '\"'){ //字符串
            wordkind = 4;
            do{
                token[counter++] = nowChar;
                if(nowChar == '\\')
                	token[counter++] = '\\';
                getch();
            }while(nowChar != '\"' && !endflag);
            if(!endflag){
            	token[counter++] = nowChar;
            	getch();
            }
        	else
        		wordkind = 7;
        }
        else if(nowChar == '>' || nowChar == '<' || nowChar == '='){
            wordkind = 5;
            token[counter++] = nowChar;
            getch();
            if(nowChar == '='){
                wordkind = 6;
                token[counter++] = nowChar;
                getch();
            }
        }
        else if(nowChar == '!'){
            wordkind = 6;
            token[counter++] = nowChar;
            getch();
            if(nowChar == '='){
                token[counter++] = nowChar;
                getch();
            }
            else
                wordkind = 7;
        }
        else{
        	wordkind = 7; //非法字符
        	token[counter++] = nowChar;
        	getch();
        }
        token[counter] = '\0';
    }
    classify();
}


void classify(){
	int flag = 0;
    switch(wordkind){
    case 1:
        for(int i = 0;i < keyNum;i++){
            if(strcmp(KeyWord[i].c_str(),token)==0){
                flag = 1;
                strcpy(symbol,KeySymbol[i].c_str());
                symID = i+1;
                break;
            }
        }
        if(flag == 0){
            strcpy(symbol,"IDSYM");
            symID = 14;
        }
        break;
    case 2:
        strcpy(symbol,"NUMSYM");
        symID = 15;
        break;
    case 3:
        strcpy(symbol,"CONSTCHARSYM");
        symID = 16;
        break;
    case 4:
        strcpy(symbol,"STRINGSYM");
        symID = 17;
        break;
    case 5:
        switch(token[0]){
        case '+':
            strcpy(symbol,"ADDSYM");
            symID = 18;
            break;
        case '-':
            strcpy(symbol,"SUBSYM");
            symID = 19;
            break;
        case '*':
            strcpy(symbol,"MULTSYM");
            symID = 20;
            break;
        case '/':
            strcpy(symbol,"DIVSYM");
            symID = 21;
            break;
        case '<':
            strcpy(symbol,"LESSYM");
            symID = 22;
            break;
        case '>':
            strcpy(symbol,"GTRSYM");
            symID = 23;
            break;
        case '=':
            strcpy(symbol,"BECOMESYM");
            symID = 24;
            break;
        case ':':
            strcpy(symbol,"COLONSYM");
            symID = 25;
            break;
        case ';':
            strcpy(symbol,"SEMISYM");
            symID = 26;
            break;
        case ',':
            strcpy(symbol,"COMMASYM");
            symID = 27;
            break;
        case '(':
            strcpy(symbol,"LPARENSYM");
            symID = 28;
            break;
        case ')':
            strcpy(symbol,"RPARENSYM");
            symID = 29;
            break;
        case '[':
            strcpy(symbol,"LFPARENSYM");
            symID = 30;
            break;
        case ']':
            strcpy(symbol,"RFPARENSYM");
            symID = 31;
            break;
        case '{':
            strcpy(symbol,"LHPARENSYM");
            symID = 32;
            break;
        case '}':
            strcpy(symbol,"RHPARENSYM");
            symID = 33;
            break;
        }
        break;
    case 6:
        switch(token[0]){
        case '<':
            strcpy(symbol,"LEQSYM");
            symID = 34;
            break;
        case '>':
            strcpy(symbol,"GEQSYM");
            symID = 35;
            break;
        case '=':
            strcpy(symbol,"EQLSYM");
            symID = 36;
            break;
        case '!':
            strcpy(symbol,"NEQSYM");
            symID = 37;
            break;
        }
        break;
    }
}

bool isAlpha(char ch){
    if(ch == '_')
        return true;
    else if(ch <= 'Z' && ch >= 'A')
        return true;
    else if(ch <= 'z' && ch >= 'a')
        return true;
    else
        return false;
}

bool isNum(char ch){
    if(ch >= '0' && ch <= '9')
        return true;
    else
        return false;
}

bool isCalOp(char ch){
	if(ch == '+' || ch == '-' || ch == '*' || ch == '/')
		return true;
	else
		return false;
}

bool checkChar(){
	if(strlen(token) != 3)
		return false;
	if(!isAlpha(token[1]) && !isNum(token[1]) && !isCalOp(token[1]))
		return false;
	return true;
}

bool checkStr(){
	for(int i = 1;i < strlen(token)-1;i++){
		if(token[i]==32 || token[i]==33 || (token[i] <= 126 && token[i] >= 35))
			continue;
		else
			return false;
	}
	return true;
}

bool checkNum(){
    if(strlen(token) > 1 && token[0] == '0'){
        return false;
    }
    return true;
}
