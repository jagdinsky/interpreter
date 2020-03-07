#include "support.hpp"

int main() {
    std::string line;
    std::vector<Lexeme *> infix;
    std::vector<Lexeme *> postfix;
    int res;
    while (std::getline(std::cin, line)) {
        infix = parse_lexeme(line);
        print_vector(infix);
        postfix = build_postfix(infix);
        print_vector(postfix);
        res = eval(postfix);
        std::cout << res << std::endl;
        delete_vector(infix);
    }
    return 0;
}
