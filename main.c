//
//  main.c
//  DJLexer
//
//  Created by Glen Fields on 9/9/16.
//  Copyright © 2016 gfields. All rights reserved.
// “I pledge my Honor that I have not cheated, and will not cheat, on this assignment.” -Glen Fields

#include <stdio.h>
#include <stdlib.h>
#include "lexer.h"

int main(int argc, const char * argv[]) {
    Token token;
    if(argc != 2){
        perror("There is no source file.");
        exit(-1);
    }
    
    FILE * source = fopen(argv[1], "r");
    if (source == NULL) { perror("The file could not be opened."); exit(-1); }
    while ((token = GetToken(source)) != ENDOFFILE && token != ERROR) {
        switch (token) {
            case CLASS:
                printf("CLASS ");
                break;
            case ID:
                printf("ID(%s) ", text);
                break;
            case EXTENDS:
                printf("EXTENDS ");
                break;
            case MAIN:
                printf("MAIN ");
                break;
            case NATTYPE:
                printf("NATTYPE ");
                break;
            case OBJECT:
                printf("OBJECT ");
                break;
            case NATLITERAL:
                printf("NATLITERAL(%s) ", text);
                break;
            case PRINTNAT:
                printf("PRINTNAT ");
                break;
            case READNAT:
                printf("READNAT ");
                break;
            case IF:
                printf("IF ");
                break;
            case ELSE:
                printf("ELSE ");
                break;
            case FOR:
                printf("FOR ");
                break;
            case ASSIGN:
                printf("ASSIGN ");
                break;
            case NUL:
                printf("NULL ");
                break;
            case NEW:
                printf("NEW ");
                break;
            case THIS:
                printf("THIS ");
                break;
            case DOT:
                printf("DOT ");
                break;
            case SEMICOLON:
                printf("SEMICOLON ");
                break;
            case LBRACE:
                printf("LBRACE ");
                break;
            case RBRACE:
                printf("RBRACE ");
                break;
            case LPAREN:
                printf("LPAREN ");
                break;
            case RPAREN:
                printf("RPAREN ");
                break;
            case PLUS:
                printf("PLUS ");
                break;
            case MINUS:
                printf("MINUS ");
                break;
            case TIMES:
                printf("TIMES ");
                break;
            case GREATER:
                printf("GREATER ");
                break;
            case EQUALITY:
                printf("EQUALITY ");
                break;
            case OR:
                printf("OR ");
                break;
            case NOT:
                printf("NOT ");
                break;
            default:
                break;
        }
        text = NULL;
    }
    if (token == ENDOFFILE) {
        printf("ENDOFFILE\n");
    }
    else if(token == ERROR){printf("ERROR\n");}
    free(text);
    fclose(source);
    return 0;
}


