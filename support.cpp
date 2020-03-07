#include "support.hpp"

std::string OPERTEXT[] = {
    "(", ")",
    "+", "-",
    "*", "/", "%"
};

int PRIORITY[] = {
    0, 0,
    1, 1,
    2, 2, 2
};

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

void Number::print() {
    std::cout << value << " ";
}

int Number::get_value() {
    return value;
}

Number::~Number() {
}

Oper::Oper() {
}

Oper::Oper(int x) {
    set_type(OPERTYPE);
    op = OPERATOR(x);
}

void Oper::print() {
    std::cout << OPERTEXT[op] << " ";
}

OPERATOR Oper::get_oper() {
    return op;
}

int Oper::get_prio() {
    return PRIORITY[op];
}

Oper::~Oper() {
}

void print_vector(std::vector<Lexeme *> v) {
    for (auto it: v)
        it -> print();
    std::cout << std::endl;
}

void delete_vector(std::vector<Lexeme *> v) {
    for (auto it: v)
        delete it;
}

Lexeme *get_number(std::string line, unsigned & i) {
    if (!isdigit(line[i]))
        return nullptr;
    int value = line[i] - '0';
    while (isdigit(line[i + 1])) {
        value *= 10;
        value += line[i + 1] - '0';
        i++;
    }
    return new Number(value);
}

Lexeme *get_oper(std::string line, unsigned & i) {
    bool flag;
    int enum_len = sizeof(OPERTEXT) / sizeof(std::string), unit_len;
    for (int j = 0, k, shift; j < enum_len; j++) {
        flag = true;
        unit_len = OPERTEXT[j].size();
        for (k = 0, shift = i; k < unit_len && line[i] != '\n'
                                        && line[i] != '\0'; k++, shift++) {
            if (OPERTEXT[j][k] != line[shift]) {
                flag = false;
                break;
            }
        }
        if (flag && k == unit_len) {
            i += unit_len - 1;
            return new Oper(j);
        }
    }
    return nullptr;
}

std::vector<Lexeme *> parse_lexeme(std::string line) {
    std::vector<Lexeme *> infix;
    Lexeme *ptr;
    for (unsigned i = 0; line[i] != '\n' && line[i] != '\0'; i++) {
        while (line[i] == ' ' || line[i] == '\t')
            i++;
        ptr = get_number(line, i);
        if (ptr) {
            infix.push_back(ptr);
            continue;
        }
        ptr = get_oper(line, i);
        if (ptr) {
            infix.push_back(ptr);
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
        if (it -> get_type() == NUMBERTYPE) {
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

int eval(std::vector<Lexeme *> postfix) {
    std::stack<int> stack;
    int tmp1, tmp2;
    for (auto it: postfix) {
        if (it -> get_type() == NUMBERTYPE) {
            stack.push(((Number *)it) -> get_value());
        } else if (it -> get_type() == OPERTYPE /*&& stack numb*/) {
            if (stack.empty()) {
                std::cout << "EVAL ERROR" << std::endl; //ERROR
                return 0;
            }
            tmp2 = stack.top();
            stack.pop();
            if (stack.empty()) {
                std::cout << "EVAL ERROR" << std::endl; //ERROR
                return 0;
            }
            tmp1 = stack.top();
            stack.pop();
            if (((Oper *)it) -> get_oper() == PLUS) {
                tmp1 += tmp2;
            } else if (((Oper *)it) -> get_oper() == MINUS) {
                tmp1 -= tmp2;
            } else if (((Oper *)it) -> get_oper() == MULTIPLY) {
                tmp1 *= tmp2;
            } else if (((Oper *)it) -> get_oper() == DIV) {
                tmp1 /= tmp2;
            } else if (((Oper *)it) -> get_oper() == MOD) {
                tmp1 %= tmp2;
            }
            stack.push(tmp1);
        }
    }
    return stack.top();
}
