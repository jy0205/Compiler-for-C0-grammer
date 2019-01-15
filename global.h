#ifndef GLOBAL_H_INCLUDED
#define GLOBAL_H_INCLUDED

using namespace std;
//一些全局的宏定义
#define MAXNUM 10000
#define FUNCMAXNUM 100
#define MAXMIDCODE 10000

void getsym();
void getch();
void program();
void error(int errorID,int errorLoc);
void GenMidCode(int codekind,char result[],char src1[],char op[],char src2[]);
void genTempVar(char varName[]);
void genLabel(char labelName[]);
void printMidCode(FILE* out);
void genMipsCode();
void GenNewMips();
void addTempVar(char name[],int type);
int searchNewTable(char name[],int funcofIndex,bool ifFunction);
void test();
void liveAnalyza(int funcIndex);
void genBlocks();
void process();
void constMerge();
void DAG_Optimize();
int getTempType(char name[]);

extern FILE* fin;
extern FILE* mipsOut;
extern FILE* NewMips;
extern FILE* midOut;
extern FILE* NewmidOut;
extern char token[100];      //保存单词
extern char symbol[100];
extern int symID;             //表示类型
extern int readloc;
extern char nowChar;
extern int lineNum;
extern int codeCount;
extern int varNum;
extern int labelNum;
extern bool endflag;
extern bool compileResult;
extern bool sReg_use[1000][10];


/*符号表的数据结构*/
typedef struct{
	char name[100];   //标识符的名称
	int kind;  // 0代表常量 1代表变量 2代表参数 3代表函数 4代表数组 5代表中间变量
	int type;  //0代表int 1代表char 2代表void
	int value;  //表示标识符的值(仅存放常量,数字的值或ascii码)
	int capicity;  //表示参数的个数或者数组的大小，对于非函数或数组其值为-1
	int offset;  //相对地址
	int midStart;    //中间变量的起始索引
	int midEnd;   //中间变量的终止索引
	bool global;
}tableItem;
typedef struct{
	tableItem element[MAXNUM];   //用于存放表项
	int topIndex;    //栈顶指针
	int numOfFunc;    //分程序的个数
	int funcIndex[FUNCMAXNUM]; //分程序索引
}table;
extern table Mytable;


/*生成目标代码时的符号表*/
typedef struct{
	char name[100];   //标识符的名称
	int kind;  // 0代表常量 1代表变量 2代表参数 3代表函数 4代表数组 5代表中间变量
	int type;  //0代表int 1代表char 2代表void
	int value;  //表示标识符的值(仅存放常量,数字的值或ascii码)
	int capicity;  //表示参数的个数或者数组的大小，对于非函数或数组其值为-1
	int offset;  //基于fp的偏移
	int functSize;
	bool global;     //代表是否是全局变量
}MipsTableItem;
typedef struct{
	MipsTableItem element[MAXNUM];   //用于存放表项
	int topIndex;    //栈顶指针
	int numOfFunc;    //分程序的个数
	int funcIndex[FUNCMAXNUM]; //分程序索引
}MipsTable;
extern MipsTable myMipsTable;


/*优化时的符号表*/
typedef struct{
	char name[100];   //标识符的名称
	int kind;  // 0代表常量 1代表变量 2代表参数 3代表函数 4代表数组 5代表中间变量
	int type;  //0代表int 1代表char 2代表void
	int value;  //表示标识符的值(仅存放常量,数字的值或ascii码)
	int capicity;  //表示参数的个数或者数组的大小，对于非函数或数组其值为-1
	int offset;  //基于fp的偏移
	int functSize;
	int RegNum;      //分配的寄存器编号
	bool global;     //代表是否是全局变量
}NewTableItem;
typedef struct{
	NewTableItem element[MAXNUM];   //用于存放表项
	int topIndex;    //栈顶指针
	int numOfFunc;    //分程序的个数
	int funcIndex[FUNCMAXNUM]; //分程序索引
}NewTable;
extern NewTable NewMipsTable;

/*中间代码的数据结构*/
typedef struct{
    char result[100];
    char src1[100];
    char op[100];
    char src2[100];
    int codeKind;    //中间代码类型
    bool effect;
    bool enter;
}MidCode;

extern MidCode MidCodeSet[MAXMIDCODE];
extern MidCode NewCodeSet[MAXMIDCODE];

/*中间代码的数据结构*/
typedef struct{
    char result[100];
    char src1[100];
    char op[100];
    char src2[100];
    int codeKind;    //中间代码类型
    bool effect;
}TempCode;
extern TempCode tempCodeSet[MAXMIDCODE];

typedef struct{
    char tempvarName[100];
    int type;
}TempVarTable;

extern TempVarTable tempTable[MAXMIDCODE];
extern int tempNum;

/*寄存器和变量的对应关系*/
typedef struct{
    int index;
	char regName[100];
	char varName[100];
	bool occupy;
}reg;

typedef struct{
    int block_index;
    int startIndex;
    int endIndex;
    int before[100];      //前序块的块号,-1代表入口
    int after[100];       //后继块的块号,-2代表出口
    int before_num;
    int after_num;
}Block;
typedef struct{
    int funcIndex;
    Block blocks[1000];
    int blockNum;
}func_blocks;
extern func_blocks block_list[100];

typedef struct{
    int in[1000];       //储存在符号表中索引
    int out[1000];
    int in_num;
    int out_num;
    int def[1000];
    int use[1000];
    int def_num;
    int use_num;
}blockInfo;
extern blockInfo info[1000];
extern blockInfo B_exit;


typedef struct{
    int index;
    bool leaf;    //是否是叶子结点
    int left;
    int right;
    bool if_in;
    char indentify[100];   //节点标识
}DAG_Struct;
extern DAG_Struct dag_list[1000];

typedef struct{
    char name[100];
    int index;
}Point_List;
extern Point_List point_list[1000];

typedef struct{
    int point_index;
    int list_loc;
}TEMP_SAVE;
extern TEMP_SAVE temp_save[1000];


typedef struct{
    char name[100];
    int funcIndex;
    int type;
}NEW_TEMP_INFO;
extern NEW_TEMP_INFO tempvar_info[5000];
#endif // GLOBAL_H_INCLUDED

