#include <iostream>
#include <fstream>
#include <algorithm>
#include <cstdlib>
#include "syntax_c_minus.h"

#define OUT_FILE

// 测试自己的文法 全部为 SPECIAL_SYMBOL 单独匹配
//LexList lexes = {
//    {"k", LexicalName::SPECIAL_SYMBOL},
//    {"a", LexicalName::SPECIAL_SYMBOL},
//    {"b", LexicalName::SPECIAL_SYMBOL},
//    {"c", LexicalName::SPECIAL_SYMBOL},
//};

std::string file_path = "./in.txt";
std::string out_file_path = "./out.txt";

// 读取文本文件
std::string readFile(std::string _file_path) {
    std::string res;
    std::ifstream read_file; read_file.open(file_path);

    if (!read_file.is_open()) {
        std::cout << "READ FILE ERROR\n";
        return res;
    }

    char c;
    while (read_file.eof() == 0) {
        read_file.get(c);
        if (read_file.fail()) {
            break;
        }
        res += c;
    }

    read_file.close();
    res += '\n';    // 添加末尾分隔符
    return res;
}


int main() {
#ifdef OUT_FILE
    freopen("./out.txt", "w", stdout);
#endif // OUT_FILE

    lexInit();
    //std::ofstream out; out.open(out_file_path);
    std::string src_code = readFile(file_path);

    LexList lexes = lexCompile(src_code);
    if (initLexList(lexes)) {
        Recursivs_Descent re(lexes);
        re.analysis();
    }
    else {
        std::cout << "error!!!" << std::endl;
    }

    return 0;
}