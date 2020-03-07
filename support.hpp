#ifndef SUPPORT_HPP
#define SUPPORT_HPP

#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <ctype.h>
#include <unordered_map>

enum LEXEMETYPE {
    NUMBERTYPE,
    OPERTYPE,
    VARTYPE
};

enum OPERATOR {
    ASSIGN,
    LBRACKET, RBRACKET,
    OR,
    AND,
    EQ, NE, GE, GT, LE, LT,
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
    int get_value();
    void set_value(int);
    void print();
    ~Number();
private:
    int value;
};

struct Oper : Lexeme {
    Oper();
    Oper(int);
    OPERATOR get_oper();
    int get_prio();
    void print();
    ~Oper();
private:
    OPERATOR op;
};

struct Var : Lexeme {
    Var();
    Var(std::string);
    int get_value();
    void set_value(int);
    std::string get_name();
    void print();
    ~Var();
private:
    std::string var_name;
};

void print_vector(std::vector<Lexeme *>);

void clear_vector(std::vector<Lexeme *>);

void print_vars();

Lexeme *get_number(std::string, std::string::iterator &);

Lexeme *get_oper(std::string, std::string::iterator &);

Lexeme *get_var(std::string, std::string::iterator &);

std::vector<Lexeme *> parse_lexeme(std::string);

std::vector<Lexeme *> build_postfix(std::vector<Lexeme *>);

Lexeme *calc(Lexeme *, Lexeme *, Lexeme *);

void eval(std::vector<Lexeme *>);

#endif
