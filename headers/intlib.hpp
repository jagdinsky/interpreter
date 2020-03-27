#ifndef INTLIB_HPP
#define INTLIB_HPP

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <stack>
#include <ctype.h>

enum PREFIXTYPE {
    IF, ELSE, ENDIF,
    WHILE, ENDWHILE,
    NONE
};

enum LEXEMETYPE {
    NUMBERTYPE,
    OPERTYPE,
    VARTYPE,
    ARRTYPE
};

enum OPERATOR {
    ASSIGN,
    LBRACKET, RBRACKET,
    DEREF,
    OR,
    AND,
    EQ, NE, GE, GT, LE, LT,
    PLUS, MINUS,
    MULTIPLY, DIV, MOD
};

class Lexeme {
    LEXEMETYPE lexemetype;
public:
    Lexeme();
    void set_type(LEXEMETYPE);
    LEXEMETYPE get_type();
    virtual void print() {};
    virtual ~Lexeme() {};
};

class Number : Lexeme {
    int value;
public:
    Number();
    Number(int);
    int get_value();
    void set_value(int);
    void print();
    ~Number();
};

class Oper : Lexeme {
    OPERATOR op;
public:
    Oper();
    Oper(OPERATOR);
    OPERATOR get_oper();
    int get_prio();
    void print();
    ~Oper();
};

class Var : Lexeme {
    std::string var_name;
public:
    Var();
    Var(std::string);
    int get_value();
    void set_value(int);
    void print();
    ~Var();
};

class Arr : Lexeme {
    std::string arr_name;
    int index;
public:
    Arr();
    Arr(std::string);
    void set_index(int);
    int get_value();
    void set_value(int);
    void print();
    ~Arr();
};

PREFIXTYPE get_prefix_type(std::string &);

void fill_label_map(std::vector<PREFIXTYPE>);

void print_vector(std::vector<Lexeme *>);

void clear_vector(std::vector<Lexeme *>);

void print_vars();

void print_arrs();

void clear_arrs();

Lexeme *get_number(std::string, unsigned &);

Lexeme *get_oper(std::string, unsigned &);

Lexeme *get_arr_var(std::string, unsigned &);

std::vector<Lexeme *> parse_lexeme(std::string);

std::vector<Lexeme *> build_postfix(std::vector<Lexeme *>);

int get_lex_val(Lexeme *);

void set_lex_val(Lexeme *, int);

Lexeme *calc(Lexeme *, Lexeme *, Lexeme *);

int eval_line(std::vector<Lexeme *>);

void exec_code(std::vector<std::vector<Lexeme *>>, std::vector<PREFIXTYPE>);

void clear_lines(std::vector<std::vector<Lexeme *>>);

#endif
