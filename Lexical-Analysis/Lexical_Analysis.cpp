// Lexical-Analysis.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。

#include <iostream>
#include <fstream>
#include <algorithm>
#include "lex_c_minus.h"

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

int main()
{
    lexInit();
    std::ifstream read_file; read_file.open(file_path);
    if (!read_file.is_open()) {
        return -1;
    }
    
    std::ofstream out; out.open(out_file_path);
    std::string src_code = readFile(file_path);
    std::vector<LexType> lexes = lexCompile(src_code);
    std::string line;
    int index = 1;
    std::map<int, std::vector<LexType>> lines_lexes;  // 添加行信息
    for (auto lex : lexes) {
        index += std::count(lex.lexeme.begin(), lex.lexeme.end(), '\n');
        if (lex.token != LexicalName::BLANK) {
            if (lines_lexes.count(index) == 0) {
                lines_lexes.insert(std::make_pair(index, std::vector<LexType>{lex}));
            }
            else {
                lines_lexes[index].push_back(lex);
            }
        }
    }
    for (auto i : lines_lexes) {
        out << i.first << ":\n";
        for (auto lex : i.second) {
            out << "\t" << lex.show() << "\n";
        }
    }
    out.close();
    return 0;
    
}
