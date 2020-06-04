#include "lex_preset.h"
#include <iostream>

std::set<std::string> reserved_word{ "else", "if", "int", "return", "void", "while" };

std::map<LexicalName, std::string> Error_Info = {
    {LexicalName::ID,       "ERROR: ILLEGAL ID"},
    {LexicalName::NUM,      "ERROR: ILLEGAL NUM"},
    {LexicalName::COMMENT,  "ERROR: ILLEGAL COMMENT"},
};

std::map<LexicalName, std::string> LexName_Str = {
    {LexicalName::ID,               "ID"},
    {LexicalName::NUM,              "NUM"},
    {LexicalName::RESERVED_WORD,    "RESERVED_WORD"},
    {LexicalName::SPECIAL_SYMBOL,   "SPECIAL_SYMBOL"},
    {LexicalName::BLANK,            "BLANK"}, 
    {LexicalName::COMMENT,          "COMMENT"},
    {LexicalName::ERROR,            "ERROR"},
};

std::map<std::string, Special_Symbol> Str_SpeSym = {
    {"<",   Special_Symbol::LT},
    {"<=",  Special_Symbol::LE},
    {"==",  Special_Symbol::EQ},
    {"!=",  Special_Symbol::NE},
    {">=",  Special_Symbol::GE},
    {">",   Special_Symbol::GT},
};

std::map<Special_Symbol, std::string> SpeSym_str = {
    { Special_Symbol::LT, "LT" },
    { Special_Symbol::LE, "LE" },
    { Special_Symbol::EQ, "EQ" },
    { Special_Symbol::NE, "NE" },
    { Special_Symbol::GE, "GE" },
    { Special_Symbol::GT, "GT" },
};

std::string LexType::show() {
    std::string res;
    res = "< \"" + lexeme + "\" , " + LexName_Str[token];
    // 如果是特殊符号则赋予特殊意义
    if (Str_SpeSym.count(lexeme) != 0) {
        attr = SpeSym_str[Str_SpeSym[lexeme]];
    }
    if (attr.length() != 0) {
        res += ", " + attr;
    }
    res += " >";
    return res;
}

DFA::DFA(MoveTable _move_table, LexicalName _token_name, FinalStates _final_states):
    __move_table(_move_table), 
    __token_name(_token_name), 
    __final_states(_final_states) 
{}

std::pair<int, LexType> DFA::dfa(int _forward, std::string& _src_code) {
    int st_pos = _forward, state = 0, last_state = 0;    // 定义源代码开始位置，以及初始状态0
    std::pair<int, LexType> res;
    char letter;
    
    // 状态转移
    while (_forward < _src_code.length()) {
        last_state = state;
        letter = _src_code[_forward];
        state = __state_trans(state, letter);
        if (state == -1) {
            break;
        }
        _forward++;
    }

    // 如果读到末尾也需要更新状态
    if (_forward == _src_code.length()) {
        last_state = state;
    }
    
    res.first = _forward - st_pos;
    // ERROR
    if (__final_states.find(last_state) == __final_states.end()) {
        res.second.lexeme = _src_code.substr(st_pos, res.first);
        res.second.token = LexicalName::ERROR;
        // 返回相应的错误信息
        res.second.attr = Error_Info.count(__token_name) ? Error_Info[__token_name] : "ILLEGAL LEXEME"; 
    }
    // RIGHT
    else {
        res.second.lexeme = _src_code.substr(st_pos, res.first);
        res.second.token = __token_name;
    }

    return res;
}

int DFA::__state_trans(int _state, char _letter) {
    std::vector<std::pair<std::string, int>>& trans_line = __move_table[_state];

    for (auto iter = trans_line.begin(); iter != trans_line.end(); iter++) {
        if (iter->first.find(_letter) != std::string::npos || iter->first.length() == 0) {
            return iter->second;
        }
    }

    return -1;
}

DFA::~DFA(){}
