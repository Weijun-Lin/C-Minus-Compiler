#include "lex_c_minus.h"
#include <iostream>

DFA Id;
DFA Comment;
DFA Num;
DFA Special_sign;
DFA Blank;
std::string Letter;
std::string Digit = "0123456789";
std::string Separator = "+-*/<>=()[]{};,\n\t\r ";

void lexInit() {
    MoveTable table = {};
    FinalStates final_set = {};
    // 初始化ID, letter
    for (char i = 'a'; i != 'z'; i++) {
        Letter += i;
        Letter += i + 'A' - 'a';
    }
    final_set = { 2 };
    table = {
        { {Letter, 1} },
        { {Letter, 1}, {Separator, 2} },
        {}
    };
    Id = { table, LexicalName::ID, final_set };
    // 初始化NUM
    final_set = { 2 };
    table = {
        { {Digit, 1} },
        { {Digit, 1}, {Separator, 2} },
        {}
    };
    Num = { table, LexicalName::NUM, final_set };
    // 初始化空格
    final_set = { 1 };
    table = {
        { {"\n\t\r ", 1} },
        { {"\n\t\r ", 1} },
    };
    Blank = { table, LexicalName::BLANK, final_set };
    // 初始化注释
    final_set = { 5 };
    table = {
        { {"/", 1} },
        { {"*", 3},{"", 2} },
        {},
        { {"*", 4},{"", 3} },
        { {"/", 5}, {"*", 4}, {"", 3} },
        {},
    };

    Comment = { table, LexicalName::COMMENT, final_set };
    // 初始化特殊符号
    final_set = { 1, 2, 3, 5 };
    table = {
        { {"+-*()[]{};,", 1}, {"<>=", 2}, {"!", 4}, {"/", 5 } },
        {},
        { {"=", 3} },
        {},
        { {"=", 3} },
        { {"*", 6} },
        {},
    };
    Special_sign = { table, LexicalName::SPECIAL_SYMBOL, final_set };
}

int dealError(int _forward, std::string& _src_code) {
    int st = _forward;
    while (_forward < _src_code.length()) {
        if (Separator.find(_src_code[_forward]) != std::string::npos) {
            break;
        }
        _forward++;
    }

    return _forward - st;
}

std::vector<LexType> lexCompile(std::string _src_code) {
    std::vector<LexType> res;
    int forward = 0, length = _src_code.length();
    std::vector<DFA> dfas = {Id, Comment, Num, Special_sign, Blank};
    //std::vector<DFA> dfas = {Comment, Blank};
    int dfas_size = dfas.size();
    std::vector<std::pair<int, LexType>> biases(dfas_size);
  
    while (forward < length) {
        // 遍历所有的DFA 找出匹配最长的那个
        std::pair<int, LexType> temp;
        for (int i = 0; i < dfas_size; i++) {
            temp = dfas[i].dfa(forward, _src_code);
            biases[i] = temp;
        }
        // 返回最好的搭配
        int best_i = std::max_element(biases.begin(),
            biases.end(),
            [](std::pair<int, LexType> a, std::pair<int, LexType> b)->bool {
                int a_ = a.second.token != LexicalName::ERROR;
                int b_ = b.second.token != LexicalName::ERROR;
                // 分为两部分 大的是不是ERROR的
                if (a_ != b_) {
                    return a_ < b_;
                }
                // 一样的情况下按匹配字符数排列
                else {
                    return a.first < b.first;
                }
            }) - biases.begin();

        forward += biases[best_i].first;
        LexType _T = biases[best_i].second;

        if (biases[best_i].second.token == LexicalName::ERROR) {
            int delt = dealError(forward, _src_code);
            _T.lexeme += _src_code.substr(forward, delt);
            // 没有一个匹配的DFA 。。 其他错误
            if (biases[best_i].first == 0) {
                _T.attr = "ERROR: ILLEGAL LEXEME";
            }
            forward += delt;           
        }
        else {
            // 这两个需要回退一格
            if (_T.token == LexicalName::ID || _T.token == LexicalName::NUM) {
                _T.lexeme.erase(_T.lexeme.end()-1);
                forward--;
            }
            // 判断是不是保留字
            if (_T.token == LexicalName::ID) {
                if (reserved_word.count(_T.lexeme)) {
                    _T.token = LexicalName::RESERVED_WORD;
                }
            }
        }

        res.push_back(_T);
    }        
    return res;
}