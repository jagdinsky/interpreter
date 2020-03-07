#ifndef SUPPORT_HPP
#define SUPPORT_HPP

#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <ctype.h>

enum LEXEMETYPE {
    NOTYPE,
    NUMBERTYPE,
    OPERTYPE
};

enum OPERATOR {
    LBRACKET, RBRACKET,
    PLUS, MINUS,
    MULTIPLY, DIV, MOD
};

struct Lexeme {
    Lexeme();
    void set_type(LEXEMETYPE);
    LEXEMETYPE get_type();
    virtual void print() {};
    virtual ~Lexeme() {};
private:
    LEXEMETYPE lexemetype;
};

struct Number : Lexeme {
    Number();
    Number(int);
    void print();
    int get_value();
    ~Number();
private:
    int value;
};

struct Oper : Lexeme {
    Oper();
    Oper(int);
    void print();
    OPERATOR get_oper();
    int get_prio();
    ~Oper();
private:
    OPERATOR op;
};

void print_vector(std::vector<Lexeme *>);

void delete_vector(std::vector<Lexeme *>);

Lexeme *get_number(std::string, std::string::iterator &);

Lexeme *get_oper(std::string, std::string::iterator &);

std::vector<Lexeme *> parse_lexeme(std::string);

std::vector<Lexeme *> build_postfix(std::vector<Lexeme *>);

int eval(std::vector<Lexeme *>);

#endif
