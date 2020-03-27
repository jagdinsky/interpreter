#include <intlib.hpp>

int main(void) {
    std::string line;
    std::vector<PREFIXTYPE> prefix_vector;
    std::vector<std::vector<Lexeme *>> infix_lines, postfix_lines;
    std::vector<Lexeme *> infix;
    for (unsigned r = 0; std::getline(std::cin, line); r++) {
        if (line == "exit")
            break;
        prefix_vector.push_back(get_prefix_type(line));
        try {
            infix = parse_lexeme(line);
        } catch (const char *ex) {
            std::cout << ex << std::endl;
            clear_lines(infix_lines);
            return 0;
        }
        infix_lines.push_back(infix);
    }
    // for (unsigned r = 0; r < infix_lines.size(); r++)
    //     print_vector(infix_lines[r]);
    try {
        fill_label_map(prefix_vector);
    } catch (const char *ex) {
        std::cout << ex << std::endl;
        clear_lines(infix_lines);
        clear_arrs();
        return 0;
    }
    for (unsigned r = 0; r < infix_lines.size(); r++)
        postfix_lines.push_back(build_postfix(infix_lines[r]));
    // for (unsigned r = 0; r < infix_lines.size(); r++)
    //     print_vector(postfix_lines[r]);
    try {
        exec_code(postfix_lines, prefix_vector);
    } catch (const char *ex) {
        std::cout << ex << std::endl;
        clear_lines(infix_lines);
        clear_arrs();
        return 0;
    }
    print_vars();
    print_arrs();
    clear_lines(infix_lines);
    clear_arrs();
    return 0;
}
