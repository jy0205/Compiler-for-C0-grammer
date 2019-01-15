#include <iostream>
#include "global.h"

using namespace std;

void error(int errorID,int errorLoc){
    compileResult = false;
    switch(errorID){
    case 0:
        cout << "line " << errorLoc << ": File Open Error!" << endl;
        break;
    case 1:
        cout << "line " << errorLoc << ": Identifier Error: No Identifier or Wrong Identifier!" << endl;
        break;
    case 2:
        cout << "line " << errorLoc << ": Declare Identifier Error!" << endl;
        break;
    case 3:
        cout << "line " << errorLoc << ": Semicolon Error!" << endl;
        break;
    case 4:
        cout << "line " << errorLoc << ": Num not after Add or Sub!" << endl;
        break;
    case 5:
        cout << "line " << errorLoc << ": Const definition Error!" << endl;
        break;
    case 6:
        cout << "line " << errorLoc << ": Assign Operator Error!" << endl;
        break;
    case 7:
        cout << "line " << errorLoc << ": Type after const Error!" << endl;
        break;
    case 8:
        cout << "line " << errorLoc << ": There should be a ']'!" << endl;
        break;
    case 9:
        cout << "line " << errorLoc << ": Array Index is Not a Num!" << endl;
        break;
    case 10:
        cout << "line " << errorLoc << ": There should be a '('!" << endl;
        break;
    case 11:
        cout << "line " << errorLoc << ": There should be ')'!" << endl;
        break;
    case 12:
        cout << "line " << errorLoc << ": There should be '{'!" << endl;
        break;
    case 13:
        cout << "line " << errorLoc << ": There should be '}'!" << endl;
        break;
    case 14:
        cout << "line " << errorLoc << ": Function Nmae is Illegal!" << endl;
        break;
    case 15:
        cout << "line " << errorLoc << ": Main Function Nmae is Illegal!" << endl;
        break;
    case 16:
        cout << "line " << errorLoc << ": Wrong Statement, NO Symbol '('!" << endl;
        break;
    case 17:
        cout << "line " << errorLoc << ": Wrong Condition!" << endl;
        break;
    case 18:
        cout << "line " << errorLoc << ": Wrong Assign Statement!" << endl;
        break;
    case 19:
        cout << "line " << errorLoc << ": Wrong Write Statement!" << endl;
        break;
    case 20:
        cout << "line " << errorLoc << ": The Word after case is not a const!" << endl;
        break;
    case 21:
        cout << "line " << errorLoc << ": Wrong case keyword!" << endl;
        break;
    case 22:
        cout << "line " << errorLoc << ": There should be ':'!" << endl;
        break;
    case 23:
        cout << "line " << errorLoc << ": Wrong default keyword!" << endl;
        break;
    case 24:
        cout << "line " << errorLoc << ": Illegal Word after Identifier!" << endl;
        break;
    case 25:
        cout << "line " << errorLoc << ": Wrong factor!" << endl;
        break;
    case 26:
        cout << "line " << errorLoc << ": Illegal char Error!" << endl;
        break;
    case 27:
        cout << "line " << errorLoc << ": zero before num Error!" << endl;
        break;
    case 28:
        cout << "line " << errorLoc << ": Wrong char!" << endl;
        break;
    case 29:
        cout << "line " << errorLoc << ": Wrong String!" << endl;
        break;
    case 30:
        cout << "line " << errorLoc << ": TABLE_OVERFLOW_ERROR!" << endl;
        break;
    case 31:
        cout << "line " << errorLoc << ": SAME_FUNCTION_NAME_ERROR!" << endl;
        break;
    case 32:
        cout << "line " << errorLoc << ": SAME_IDENTIFIER_NAME_ERROR!" << endl;
        break;
    case 33:
        cout << "line " << errorLoc << ": NO_SUCH_FUNCTION!" << endl;
        break;
    case 34:
        cout << "line " << errorLoc << ": NO_SUCH_IDENTIFIER!" << endl;
        break;
    case 35:
        cout << "line " << errorLoc << ": OVERFLOW_INT!" << endl;
        break;
    case 36:
        cout << "line " << errorLoc << ": TYPE ERROR: Dismatch type!" << endl;
        break;
    case 37:
        cout << "line " << errorLoc << ": ARRAY_INDEX_INVALID!" << endl;
        break;
    case 38:
        cout << "line " << errorLoc << ": NO_RETURN_ERROR!" << endl;
        break;
    case 39:
        cout << "line " << errorLoc << ": PARANUM_NOT_MATCH_ERROR!" << endl;
        break;
    case 40:
        cout << "line " << errorLoc << ": EXP_CALL_VOID_FUNCTION!" << endl;
        break;
    case 41:
        cout << "line " << errorLoc << ": ARRAY_OVERFLOW!" << endl;
        break;
    case 42:
        cout << "line " << errorLoc << ": IDENTIFY_ISNOT_ARRAY!" << endl;
        break;
    case 43:
        cout << "line " << errorLoc << ": IDENTIFY_ISNOT_FUNCTION!" << endl;
        break;
    case 44:
        cout << "line " << errorLoc << ": ASSIGN_ARRAY_OR_FUNCT_OR_CONST!" << endl;
        break;
    case 45:
        cout << "line " << errorLoc << ": ASSIGN_TYPE_NOTMATCH!" << endl;
        break;
    case 46:
        cout << "line " << errorLoc << ": The Identifier is not Array!" << endl;
        break;
    case 47:
        cout << "line " << errorLoc << ": READ_NOT_VAR!" << endl;
        break;
    case 48:
        cout << "line " << errorLoc << ": VALUE_PARA_NOT_MATCH!" << endl;
        break;
    case 49:
        cout << "line " << errorLoc << ": CHARTYPE_IN_CONDITION!" << endl;
        break;
    case 50:
        cout << "line " << errorLoc << ": RETURN_TYPE_NOT_MATCH!" << endl;
        break;
    case 51:
        cout << "line " << errorLoc << ": return error,void function return value!" << endl;
        break;
    case 52:
        cout << "line " << errorLoc << ": No Main Function Error!" << endl;
        break;
    }
}
