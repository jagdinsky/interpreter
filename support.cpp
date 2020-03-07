#include "support.hpp"

std::string OPERTEXT[] = {
    ":=",
    "(", ")",
    "OR",
    "AND",
    "==", "!=", ">=", ">", "<=", "<",
    "+", "-",
    "*", "/", "%"
};

int PRIORITY[] = {
    -1,
    0, 0,
    1,
    2,
    3, 3, 3, 3, 3, 3,
    4, 4,
    5, 5, 5
};

std::unordered_map<std::string, int> var_map;

Lexeme::Lexeme() {
}

void Lexeme::set_type(LEXEMETYPE lex) {
    lexemetype = lex;
}

LEXEMETYPE Lexeme::get_type() {
    return lexemetype;
}

Number::Number() {
}

Number::Number(int x) {
    set_type(NUMBERTYPE);
    value = x;
}

int Number::get_value() {
    return value;
}

void Number::set_value(int x) {
    value = x;
}

void Number::print() {
    std::cout << value << " ";
}

Number::~Number() {
}

Oper::Oper() {
}

Oper::Oper(int x) {
    set_type(OPERTYPE);
    op = OPERATOR(x);
}

OPERATOR Oper::get_oper() {
    return op;
}

int Oper::get_prio() {
    return PRIORITY[op];
}

void Oper::print() {
    std::cout << OPERTEXT[op] << " ";
}

Oper::~Oper() {
}

Var::Var() {
}

Var::Var(std::string s) {
    set_type(VARTYPE);
    var_name = s;
}

int Var::get_value() {
    return var_map[var_name];
}

void Var::set_value(int x) {
    var_map[var_name] = x;
}

std::string Var::get_name() {
    return var_name;
}

void Var::print() {
    std::cout << var_map[var_name] << ' ';
}

Var::~Var() {
}

void print_vector(std::vector<Lexeme *> v) {
    for (auto it: v)
        it -> print();
    std::cout << std::endl;
}

void clear_vector(std::vector<Lexeme *> v) {
    for (auto it: v)
        delete it;
}

void print_vars() {
    std::cout << std::endl;
    for (auto & unit: var_map)
        std::cout << unit.first << ": "
                            << unit.second << std::endl;
    std::cout << std::endl;
}

Lexeme *get_number(std::string line, unsigned & i) {
    if (!isdigit(line[i]))
        return nullptr;
    int value = line[i] - '0';
    while (i + 1 < line.size() && isdigit(line[i + 1])) {
        value *= 10;
        value += line[i + 1] - '0';
        i++;
    }
    return new Number(value);
}

Lexeme *get_oper(std::string line, unsigned & i) {
    int enum_len = sizeof(OPERTEXT) / sizeof(std::string), unit_len;
    for (int j = 0; j < enum_len; j++) {
        unit_len = OPERTEXT[j].size();
        if (line.substr(i, unit_len) == OPERTEXT[j]) {
            i += unit_len - 1;
            return new Oper(j);
        }
    }
    return nullptr;
}

Lexeme *get_var(std::string line, unsigned & i) {
    if (!isalpha(line[i]))
        return nullptr;
    std::string s = "";
    for (s += line[i]; i + 1 < line.size() && isalpha(line[i + 1]); i++) {
        s += line[i + 1];
    }
    return new Var(s);
}

std::vector<Lexeme *> parse_lexeme(std::string line) {
    std::vector<Lexeme *> infix;
    Lexeme *ptr;
    for (unsigned i = 0; i < line.size(); i++) {
        while (line[i] == ' ' || line[i] == '\t')
            i++;
        if (!ptr)
            ptr = get_number(line, i);
        if (!ptr)
            ptr = get_oper(line, i);
        if (!ptr)
            ptr = get_var(line, i);
        if (ptr) {
            infix.push_back(ptr);
            ptr = nullptr;
            continue;
        }
        // ERROR
    }
    return infix;
}

std::vector<Lexeme *> build_postfix(std::vector<Lexeme *> infix) {
    std::vector<Lexeme *> postfix;
    std::stack<Lexeme *> stack;
    for (auto it: infix) {
        if (it -> get_type() == NUMBERTYPE || it -> get_type() == VARTYPE) {
            postfix.push_back(it);
        } else if (it -> get_type() == OPERTYPE) {
            if (((Oper *)it) -> get_oper() == RBRACKET) {
                while (!stack.empty() && ((Oper *)stack.top()) -> get_oper()
                != LBRACKET) {
                    postfix.push_back(stack.top());
                    stack.pop();
                }
                if (!stack.empty()) {
                    stack.pop();
                }
            } else {
                while (!stack.empty()
                && ((Oper *)stack.top()) -> get_oper() != LBRACKET
                && ((Oper *)it) -> get_oper() != LBRACKET
                && ((Oper *)it) -> get_prio()
                <= ((Oper *)stack.top()) -> get_prio()) {
                    postfix.push_back(stack.top());
                    stack.pop();
                }
                stack.push(it);
            }
        }
    }
    while (!stack.empty()) {
        postfix.push_back(stack.top());
        stack.pop();
    }
    return postfix;
}

Lexeme *calc(Lexeme *left, Lexeme *op, Lexeme *right) {
    int left_val, right_val, res_val;
    if (left -> get_type() == NUMBERTYPE)
        left_val = ((Number *) left) -> get_value();
    if (left -> get_type() == VARTYPE)
        left_val = ((Var *) left) -> get_value();
    if (right -> get_type() == NUMBERTYPE)
        right_val = ((Number *) right) -> get_value();
    if (right -> get_type() == NUMBERTYPE)
        right_val = ((Number *) right) -> get_value();
    if (((Oper *) op) -> get_oper() == ASSIGN) {
        res_val = right_val;
        ((Var *) left) -> set_value(res_val);
    } else if (((Oper *) op) -> get_oper() == PLUS) {
        res_val = left_val + right_val;
    } else if (((Oper *) op) -> get_oper() == MINUS) {
        res_val = left_val - right_val;
    } else if (((Oper *) op) -> get_oper() == MULTIPLY) {
        res_val = left_val * right_val;
    } else if (((Oper *) op) -> get_oper() == DIV) {
        res_val = left_val / right_val;
    } else if (((Oper *) op) -> get_oper() == MOD) {
        res_val = left_val % right_val;
    }
    Number *res = new Number(res_val);
    return (Lexeme *) res;
}

void eval(std::vector<Lexeme *> postfix) {
    std::stack<Lexeme *> stack;
    std::vector<Lexeme *> memory;
    Lexeme *left, *right, *res;
    for (auto it: postfix) {
        if (it -> get_type() == NUMBERTYPE
                            || it -> get_type() == VARTYPE) {
            stack.push(it);
        } else if (it -> get_type() == OPERTYPE) {
            if (stack.empty()) {
                std::cout << "EVAL ERROR" << std::endl; //ERROR
                return;
            }
            right = stack.top();
            stack.pop();
            if (stack.empty()) {
                std::cout << "EVAL ERROR" << std::endl; //ERROR
                return;
            }
            left = stack.top();
            stack.pop();
            res = calc(left, it, right);
            stack.push(res);
            memory.push_back(res);
        }
    }
    clear_vector(memory);
}
