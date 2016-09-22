//
//  lexer.h
//  DJLexer
//
//  Created by Glen Fields on 9/20/16.
//  Copyright © 2016 gfields. All rights reserved.
//

#ifndef lexer_h
#define lexer_h
#define BUFFERSIZE 2048
#define KEYWORDSNUM 13

// global variables that contain extraneous information
char * text;
unsigned int lineno = 0;

// enum type  and struct declarations
typedef enum {CLASS, ID, EXTENDS, MAIN, NATTYPE, OBJECT,
    NATLITERAL, PRINTNAT, READNAT, IF, ELSE, FOR,
    ASSIGN, NUL, NEW, THIS, DOT, SEMICOLON,
    LBRACE, RBRACE, LPAREN, RPAREN, ENDOFFILE, ERROR,
    PLUS, MINUS, TIMES, GREATER, EQUALITY, OR,
    NOT} Token;

typedef enum {start, comment, wspace, digit, identifier} State;

struct TokenAttributes {
    Token tokenType;
    char * tokenString;
} RESERVEDWORDS[KEYWORDSNUM]
=   {{CLASS,"class"},{EXTENDS, "extends"},{MAIN, "main"},{NATTYPE, "nat"},
    {OBJECT, "Object"},{PRINTNAT, "printNat"},{READNAT, "readNat"},
    {IF, "if"},{ELSE, "else"},{FOR, "for"},{NUL, "null"},
    {NEW, "new"}, {THIS, "this"}};
// end enum type definitions

// function declarations
Token CheckForKey(const char * s);
int IsDigit(const char c){ return c >= '0' && c <= '9'; }
int IsAlpha(const char c){ return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'); }
int IsWSpace(const char c){ return c == ' ' || c == '\n' || c == '\r' || c == '\t'; }
int IsAlphaNum(const char c) { return (c >= '0' && c <= '9') || (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'); }
int StrCmp(const char * lh, const char * rh);
char NextChar(FILE *,char *, unsigned int *);
void RollBack(unsigned int *);
Token GetToken(FILE *);
//end function declarations



// function that performs the tokenizing
Token GetToken(FILE * fp){
    static unsigned int lexmeBegin     = 0;
    static unsigned int forward        = 0;
    static char buffer[2*BUFFERSIZE];
    char c;
    char * string                      = NULL;
    State state                        = start;
    
    lexmeBegin = forward;
    c = NextChar(fp, buffer, &forward);
    while(1){
        switch (state) {
            case start:
                switch (c) {
                    case '+':
                        return PLUS;
                    case '-':
                        return MINUS;
                    case '*':
                        return TIMES;
                    case '>':
                        return GREATER;
                    case '!':
                        return NOT;
                    case '.':
                        return DOT;
                    case '{':
                        return LBRACE;
                    case '}':
                        return RBRACE;
                    case '(':
                        return LPAREN;
                    case ')':
                        return RPAREN;
                    case ';':
                        return SEMICOLON;
                    case '=':
                        return ((c = NextChar(fp, buffer, &forward) == '=') ? EQUALITY : ASSIGN);
                    case '|':
                        return ((c = NextChar(fp, buffer, &forward) == '|') ? OR : ERROR);
                    case '/':
                        c = NextChar(fp, buffer, &forward);
                        if (c == '/') {state = comment;} else {return ERROR;}
                        break;
                    case '\0':
                        return ENDOFFILE;
                    default:
                        if (IsDigit(c)) {state = digit; RollBack(&forward); lexmeBegin = forward;}
                        else if (IsAlpha(c)){state = identifier; RollBack(&forward); lexmeBegin = forward;}
                        else if (IsWSpace(c)){state = wspace;}
                        else return ERROR;
                        break;
                }
                break;
            case comment:
                while ((c = NextChar(fp, buffer, &forward)) != '\n') { /* do nothing */}
                state = start;
                break;
            case wspace:
                while (IsWSpace(c)) {
                    if(c == '\n') ++lineno;
                    c = NextChar(fp, buffer, &forward);
                }
                state = start;
                break;
            case digit:
                while (IsDigit(c)) { c = NextChar(fp, buffer, &forward); }
                RollBack(&forward);
                string = (char *)calloc(0,(forward - lexmeBegin)*sizeof(char));
                for (int x = 0, y = (forward - lexmeBegin); x < y; x++, lexmeBegin++){
                    string[x] = buffer[lexmeBegin % (2* BUFFERSIZE)];
                }
                text = string;
                free(string);
                string = NULL;
                return NATLITERAL;
            case identifier:
                while (IsAlphaNum(c)){ c = NextChar(fp, buffer, &forward); }
                RollBack(&forward);
                string = (char *)calloc(0,(forward - lexmeBegin)*sizeof(char));
                for (int x = 0, y = (forward - lexmeBegin); x < y; x++, lexmeBegin++) {
                    string[x] = buffer[lexmeBegin % (2* BUFFERSIZE)];
                }
                text = string;
                free(string);
                string = NULL;
                return CheckForKey(text);
            default:
                break;
        }
    }
}

//Helper functions
//NextChar reads in the next char and refills buffer if needed.
char NextChar(FILE * fp, char * buffer,unsigned int * forward){
    size_t result;
    if ((*forward % BUFFERSIZE) == 0) {
        result = fread(buffer+(*forward), sizeof(char), BUFFERSIZE, fp);
        if (result < BUFFERSIZE) {
            *(buffer+(*forward+result+1)) = '\0';
        }
    }
    
    char c = buffer[*forward];
    *forward = (*forward+1) % (2*BUFFERSIZE);
    return c;
}
//RollBack rolls the forward pointer back one space.
void RollBack(unsigned int * forward){
    *forward = (*forward - 1) % (2*BUFFERSIZE);
}
// own string compare funcion
int StrCmp(const char * lh, const char * rh){
    while ((*lh != '\0') && (*lh == *rh)) ++lh, ++rh;
    return *(const unsigned char*)lh-*(const unsigned char*)rh;
}
// checks if an id is a keyword
Token CheckForKey(const char * s){
    for( int i = 0; i < KEYWORDSNUM; ++i){
        if (!(StrCmp(s, RESERVEDWORDS[i].tokenString))) {
            return RESERVEDWORDS[i].tokenType;
        }
    }
    return ID;
}
#endif /* lexer_h */
