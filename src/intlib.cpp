#include <intlib.hpp>

std::string PREFIXTEXT[] = {
    "if", "else", "endif",
    "while", "endwhile"
};

std::string OPERTEXT[] = {
    ":=",
    "(", ")",
    "[",
    "OR",
    "AND",
    "==", "!=", ">=", ">", "<=", "<",
    "+", "-",
    "*", "/", "%"
};

int PRIORITY[] = {
    0,
    1, 1,
    2,
    3,
    4,
    5, 5, 5, 5, 5, 5,
    6, 6,
    7, 7, 7
};

std::unordered_map<unsigned, unsigned> label_map;

std::unordered_map<std::string, int> var_map;

std::unordered_map<std::string, int> arr_size_map;

std::unordered_map<std::string, int *> arr_map;

Lexeme::Lexeme() {
}

void Lexeme::set_type(LEXEMETYPE x) {
    lexemetype = x;
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

Oper::Oper(OPERATOR x) {
    set_type(OPERTYPE);
    op = x;
}

OPERATOR Oper::get_oper() {
    return op;
}

int Oper::get_prio() {
    return PRIORITY[op];
}

void Oper::print() {
    std::cout << OPERTEXT[op] << ' ';
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

void Var::print() {
    std::cout << var_name << "(" << var_map[var_name] << ") ";
}

Var::~Var() {
}

Arr::Arr() {
}

Arr::Arr(std::string s) {
    set_type(ARRTYPE);
    arr_name = s;
    index = -1;
}

void Arr::set_index(int x) {
    if (x < 0 || (arr_size_map.count(arr_name) && x > arr_size_map[arr_name])) {
        throw "Invalid array derefence";
    }
    if (!arr_size_map.count(arr_name)) {
        arr_size_map[arr_name] = x + 1;
        int *ptr = new int[(unsigned)(x + 1)];
        for (unsigned i = 0; i <= (unsigned)x; i++) {
            ptr[i] = 0;
        }
        arr_map[arr_name] = ptr;
    }
    index = x;
}

int Arr::get_value() {
    if (index < 0) {
        throw "Invalid array derefence.";
    }
    return (arr_map[arr_name])[index];
}

void Arr::set_value(int val) {
    (arr_map[arr_name])[index] = val;
}

void Arr::print() {
    std::cout << '_' << arr_name << ' ';
}

Arr::~Arr() {
}

PREFIXTYPE get_prefix_type(std::string & line) {
    unsigned enum_len = sizeof(PREFIXTEXT) / sizeof(std::string), unit_len;
    unsigned i = 0;
    while (line[i] == ' ' || line[i] == '\t')
        i++;
    for (unsigned j = 0; j < enum_len; j++) {
        unit_len = PREFIXTEXT[j].size();
        if (line.substr(i, unit_len) == PREFIXTEXT[j]) {
            line.erase(0, i + unit_len);
            return PREFIXTYPE(j);
        }
    }
    return PREFIXTYPE(enum_len);
}

void fill_label_map(std::vector<PREFIXTYPE> prefix_vector) {
    std::stack<unsigned> stack;
    unsigned key;
    for (unsigned r = 0; r < prefix_vector.size(); r++) {
        if (prefix_vector[r] == NONE) {
            continue;
        } else if (prefix_vector[r] == IF) {
            stack.push(r);
        } else if (prefix_vector[r] == ELSE) {
            if (stack.empty()) {
                throw "Invalid logic.";
            }
            key = stack.top();
            stack.pop();
            label_map[key] = r + 1;
            stack.push(r);
        } else if (prefix_vector[r] == ENDIF) {
            if (stack.empty()) {
                throw "Invalid logic.";
            }
            key = stack.top();
            stack.pop();
            label_map[key] = r;
        } else if (prefix_vector[r] == WHILE) {
            stack.push(r);
        } else if (prefix_vector[r] == ENDWHILE) {
            if (stack.empty()) {
                throw "Invalid logic.";
            }
            key = stack.top();
            stack.pop();
            label_map[key] = r + 1;
            label_map[r] = key;
        }
    }
}

void print_vector(std::vector<Lexeme *> v) {
    if (v.empty())
        return;
    for (auto it: v) {
        it -> print();
    }
    std::cout << std::endl;
}

void clear_vector(std::vector<Lexeme *> v) {
    if (v.empty())
        return;
    for (auto it: v) {
        delete it;
    }
}

void print_vars() {
    std::cout << std::endl;
    for (auto & unit: var_map)
        std::cout << unit.first << ": "
                            << unit.second << std::endl;
    std::cout << std::endl;
}

void print_arrs() {
    std::string key;
    for (auto & unit: arr_map) {
        key = unit.first;
        std::cout << key << ": " << std::endl;
        for (unsigned i = 0; i < (unsigned)(arr_size_map[key]); i++) {
            std::cout << (arr_map[key])[i] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

void clear_arrs() {
    for (auto & unit: arr_map) {
        delete[] arr_map[unit.first];
    }
}

Lexeme *get_number(std::string line, unsigned & i) {
    if (!isdigit(line[i]))
        return nullptr;
    int value = line[i] - '0';
    i++;
    for (; i < line.size() && isdigit(line[i]); i++) {
        value *= 10;
        value += line[i] - '0';
    }
    return (Lexeme *)(new Number(value));
}

Lexeme *get_oper(std::string line, unsigned & i) {
    int enum_len = sizeof(OPERTEXT) / sizeof(std::string), unit_len;
    for (int j = 0; j < enum_len; j++) {
        unit_len = OPERTEXT[j].size();
        if (line.substr(i, unit_len) == OPERTEXT[j]) {
            i += unit_len;
            return (Lexeme *)(new Oper(OPERATOR(j)));
        }
    }
    return nullptr;
}

Lexeme *get_arr_var(std::string line, unsigned & i) {
    if (!isalpha(line[i]))
        return nullptr;
    std::string s = "";
    s += line[i];
    i++;
    for (; i < line.size() && isalpha(line[i]); i++) {
        s += line[i];
    }
    if (i < line.size() && line[i] == '[') {
        return (Lexeme *)(new Arr(s));
    }
    return (Lexeme *)(new Var(s));
}

std::vector<Lexeme *> parse_lexeme(std::string line) {
    std::vector<Lexeme *> infix;
    Lexeme *ptr = nullptr;
    for (unsigned i = 0; i < line.size(); ) {
        while (line[i] == ' ' || line[i] == '\t' || line[i] == ']')
            i++;
        if (!ptr)
            ptr = get_number(line, i);
        if (!ptr)
            ptr = get_oper(line, i);
        if (!ptr)
            ptr = get_arr_var(line, i);
        if (ptr) {
            infix.push_back(ptr);
            ptr = nullptr;
        } else {
            throw "Invalid syntax.";
        }
        while (line[i] == ' ' || line[i] == '\t' || line[i] == ']')
            i++;
    }
    return infix;
}

std::vector<Lexeme *> build_postfix(std::vector<Lexeme *> infix) {
    if (infix.empty())
        return infix;
    std::vector<Lexeme *> postfix;
    std::stack<Lexeme *> stack;
    for (auto it: infix) {
        if (it -> get_type() == NUMBERTYPE || it -> get_type() == VARTYPE
                                            || it -> get_type() == ARRTYPE) {
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

int get_lex_val(Lexeme *unit) {
    if (unit -> get_type() == VARTYPE)
        return ((Var *)unit) -> get_value();
    if (unit -> get_type() == ARRTYPE)
        return ((Arr *)unit) -> get_value();
    return ((Number *)unit) -> get_value();
}

void set_lex_val(Lexeme *unit, int x) {
    if (unit -> get_type() == ARRTYPE) {
        ((Arr *)unit) -> set_value(x);
    } else {
        ((Var *)unit) -> set_value(x);
    }
}

Lexeme *calc(Lexeme *left, Lexeme *op, Lexeme *right) {
    int right_val = get_lex_val(right);
    if (((Oper *)op) -> get_oper() == ASSIGN) {
        if (left -> get_type() == NUMBERTYPE) {
            throw "Invalid syntax.";
        }
        set_lex_val(left, right_val);
        return (Lexeme *)(new Number(right_val));
    }
    int left_val = get_lex_val(left);
    int res_val = 0;
    if (((Oper *)op) -> get_oper() == PLUS) {
        res_val = left_val + right_val;
    } else if (((Oper *)op) -> get_oper() == MINUS) {
        res_val = left_val - right_val;
    } else if (((Oper *)op) -> get_oper() == MULTIPLY) {
        res_val = left_val * right_val;
    } else if (((Oper *)op) -> get_oper() == DIV) {
        res_val = left_val / right_val;
    } else if (((Oper *)op) -> get_oper() == MOD) {
        res_val = left_val % right_val;
    } else if (((Oper *)op) -> get_oper() == OR) {
        if (left_val || right_val)
            res_val = 1;
    } else if (((Oper *)op) -> get_oper() == AND) {
        if (left_val && right_val)
            res_val = 1;
    } else if (((Oper *)op) -> get_oper() == EQ) {
        if (left_val == right_val)
            res_val = 1;
    } else if (((Oper *)op) -> get_oper() == NE) {
        if (left_val != right_val)
            res_val = 1;
    } else if (((Oper *)op) -> get_oper() == GE) {
        if (left_val >= right_val)
            res_val = 1;
    } else if (((Oper *)op) -> get_oper() == GT) {
        if (left_val > right_val)
            res_val = 1;
    } else if (((Oper *)op) -> get_oper() == LE) {
        if (left_val <= right_val)
            res_val = 1;
    } else if (((Oper *)op) -> get_oper() == LT) {
        if (left_val < right_val)
            res_val = 1;
    }
    return (Lexeme *)(new Number(res_val));
}

int eval_line(std::vector<Lexeme *> postfix) {
    if (postfix.empty())
        return 1;
    std::stack<Lexeme *> stack;
    std::vector<Lexeme *> memory;
    Lexeme *left, *right, *res;
    for (auto it: postfix) {
        if (it -> get_type() == NUMBERTYPE || it -> get_type() == VARTYPE
                                            || it -> get_type() == ARRTYPE) {
            stack.push(it);
        } else if (it -> get_type() == OPERTYPE) {
            if (stack.empty()) {
                clear_vector(memory);
                throw "Evaluation error.";
            }
            right = stack.top();
            stack.pop();
            if (stack.empty()) {
                clear_vector(memory);
                throw "Evaluation error.";
            }
            left = stack.top();
            stack.pop();
            if (((Oper *)it) -> get_oper() == DEREF) {
                if (left -> get_type() != ARRTYPE) {
                    clear_vector(memory);
                    throw "Evaluation error.";
                }
                ((Arr *)left) -> set_index(get_lex_val(right));
                stack.push(left);
            } else {
                res = calc(left, it, right);
                stack.push(res);
                memory.push_back(res);
            }
        }
    }
    if (stack.empty()) {
        clear_vector(memory);
        throw "Evaluation error.";
    }
    int tmp = get_lex_val(stack.top());
    stack.pop();
    clear_vector(memory);
    return tmp;
}

void exec_code(std::vector<std::vector<Lexeme *>> postfix_lines,
                                    std::vector<PREFIXTYPE> prefix_vector) {
    int eval_res;
    for (unsigned r = 0; r < postfix_lines.size(); ) {
        try {
            eval_res = eval_line(postfix_lines[r]);
        } catch (const char *ex) {
            throw ex;
        }
        if (prefix_vector[r] == NONE) {
            r++;
        } else if (prefix_vector[r] == IF) {
            if (eval_res) {
                r++;
            } else {
                r = label_map[r];
            }
        } else if (prefix_vector[r] == ELSE) {
            r = label_map[r];
        } else if (prefix_vector[r] == ENDIF) {
            r++;
        } else if (prefix_vector[r] == WHILE) {
            if (eval_res) {
                r++;
            } else {
                r = label_map[r];
            }
        } else if (prefix_vector[r] == ENDWHILE) {
            r = label_map[r];
        }
    }
}

void clear_lines(std::vector<std::vector<Lexeme *>> infix_lines) {
    for (unsigned r = 0; r < infix_lines.size(); r++) {
        clear_vector(infix_lines[r]);
    }
}
