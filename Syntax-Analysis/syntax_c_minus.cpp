/*
    @Desc
        sytax_c_minus.h 一些变量的定义
    @Include:
        sytax_c_minus.h

*/

#include "syntax_c_minus.h"
#include <cstdio>

const std::string epsilon = "epsilon";

std::map<std::string, ProductionRight> Grammar = {
    {"program",             {{ {4,"declaration-list"} }}},
    {"declaration-list",    {{ {4,"declaration"}, {4,"declaration-list`"} }}},
    {"declaration-list`",   {{ {4,"declaration"}, {4,"declaration-list`"} },
                             { {5,epsilon} }}},
    {"declaration",         {{ {4,"type-specifier"}, {0,"ID"}, {4,"declaration`"} }}},
    {"declaration`",        {{ {4,"var-declaration`"} },
                             { {3,"("}, {4,"parmas"}, {3, ")"}, {4,"compound-stmt"} }}},
    {"var-declaration",     {{ {4,"type-specifier"}, {0, "ID"}, {4,"var-declaration`"} }}},
    {"var-declaration`",    {{ {3,"["},{1,"NUM"}, {3,"]"}, {3,";"} },
                             { {3,";"} }} },
    {"fun-declaration",     {{ {4,"type-specifier"}, {0, "ID"}, {3,"("}, {4,"parmas"}, {3,")"}, {4,"compound-stmt"} }}},
    {"type-specifier",      {{ {2,"int"} },
                             { {2,"void"} }}},
    {"compound-stmt",       {{ {3,"{"}, {4,"local-declarations"}, {4,"statement-list"}, {3,"}"} }}},
    {"parmas",              {{ {2,"int"}, {0,"ID"}, {4,"parma`"}, {4,"parma-list`"} },
                             { {2,"void"}, {4,"parmas`"} }}},
    {"parmas`",             {{ {0,"ID"}, {4,"parma`"}, {4,"parma-list`"} },
                             { {5,epsilon} }}},
    {"parma-list",          {{ {4,"parma"},{4,"parma-list`"}}}},
    {"parma-list`",         {{ {3,","},{4,"parma"},{4,"parma-list`"}},
                             { {5,epsilon}}}},
    {"parma",               {{ {4,"type-specifier"},{0, "ID"},{4,"parma`"}}}},
    {"parma`",              {{ {3,"["},{3,"]"}},{{5, epsilon}}}},
    {"local-declarations",  {{ {4,"local-declarations`"}}}},
    {"local-declarations`", {{ {4,"var-declaration"}, {4,"local-declarations`"}},
                             { {5,epsilon}}}},
    {"statement-list",      {{ {4,"statement-list`"}}}},
    {"statement-list`",     {{ {4,"statement"},{4,"statement-list`"}},
                             { {5,epsilon}}}},
    {"statement",           {{ {4,"expression-stmt"}},
                             { {4,"compound-stmt"}},
                             { {4,"selection-stmt"}},
                             { {4,"iteration-stmt"}},
                             { {4,"return-stmt"}}}},
    {"expression-stmt",     {{ {4,"expression"},{3,";"}},
                             { {3,";"}}}},
    {"selection-stmt",      {{ {2,"if"}, {3,"("},{4,"expression"}, {3,")"}, {4,"statement"},{4,"other"}}}},
    {"other",               {{ {2,"else"}, {4,"statement"} },
                             { {5,epsilon} }} },
    {"iteration-stmt",      {{ {2,"while"}, {3,"("}, {4,"expression"}, {3,")"},{4,"statement"} }}},
    {"return-stmt",         {{ {2,"return"},{4,"return-stmt`"}}}},
    {"return-stmt`",        {{ {3,";"}},
                             { {4,"expression"},{3,";"}}}},
    {"expression",          {{ {3,"("},{4,"expression"},{3,")"},{4,"term`"},{4,"additive-expression`"},{4,"simple-expression`"}},
                             { {1,"NUM"},{4,"term`"}, {4,"additive-expression`"} ,{4,"simple-expression`"}},
                             { {0,"ID"},{4,"expression`"}}}},
    {"expression`",         {{ {4,"var`"},{4,"expression``"}},
                             { {3,"("},{4,"args"},{3,")"}, {4,"term`"},{4,"additive-expression`"},{4,"simple-expression`"}}}},
    {"expression``",        {{ {4,"term`"},{4,"additive-expression`"},{4,"simple-expression`"}},
                             { {3,"="},{4,"expression"}}}},
    {"var`",                {{ {3,"["},{4,"expression"},{3,"]"}},
                             { {5,epsilon}}}},
    {"simple-expression",   {{ {4,"additive-expression"},{4,"simple-expression`"}}}},
    {"simple-expression`",  {{ {4,"relop"},{4,"additive-expression"}},
                             { {5,epsilon}}}},
    {"relop",               {{ {3,"<="}},
                             { {3,"<"} },
                             { {3,">"} },
                             { {3,">="} },
                             { {3,"=="} },
                             { {3,"!="} }}},
    {"additive-expression", {{ {4,"term"},{4,"additive-expression`"}}}},
    {"additive-expression`",{{ {4,"addop"},{4,"term"}, {4,"additive-expression`"}},
                             { {5,epsilon}}}},
    {"addop",               {{ {3,"+"}},
                             { {3,"-"}}}},
    {"term",                {{ {4,"factor"},{4,"term`"}}}},
    {"term`",               {{ {4,"mulop"},{4,"factor"}, {4,"term`"}},
                             { {5,epsilon}}}},
    {"mulop",               {{ {3,"*"}},{{3,"/"}}} },
    {"factor",              {{ {3,"("},{4,"expression"},{3,")"}},
                             { {0,"ID"},{4,"factor`"}},
                             { {1,"NUM"}}}},
    {"factor`",             {{ {4,"var`"}},{{3,"("},{4,"args"},{3,")"}}}},
    {"args",                {{ {4,"arg-list"}},
                             { {5,epsilon}}}},
    {"arg-list",            {{ {4,"expression"},{4,"arg-list`"}}}},
    {"arg-list`",           {{ {3,","}, {4,"expression"},{4,"arg-list`"}},
                             { {5,epsilon}}}},
};


// 测试是否完全回溯的测试文法
// A->BC
// B->ab|a
// C->bc
//std::map<std::string, Production> Productions = {
//    {"A", { {{4,"B"},{4,"C"}} } },
//    {"B", {{{3, "a"},{3,"b"}}, {{3,"a"}}} },
//    {"C", {{{3,"b"},{3,"c"}}}},
//};

// 同上 测试多层
//std::map<std::string, Production> Productions = {
//    {"A", {{{3,"k"}, {4,"B"},{4,"C"}}}},
//    {"B", {{{4, "E"}}, {{4,"D"}}} },
//    {"C", {{{3,"b"},{3,"c"}}}},
//    {"D", {{{3,"a"}}}},
//    {"E", {{{3,"a"},{3,"b"}}}},
//};

// 匹配括号的测试文法
//std::map<std::string, Production> Productions = {
//    {"A", { {{3,"("},{4,"A"},{3,")"}}, {{5, "epsilon"}}}},
//};

/*
// 测试 Follow First 编译原理 例4.30
std::map<std::string, ProductionRight> Grammar = {
    {"E", { {{4,"T"},{4,"E`"}}}},
    {"E`", { {{3,"+"},{4,"T"},{4,"E`"}}, {{5, "epsilon"}}}},
    {"T", { {{4,"F"},{4,"T`"}}}},
    {"T`", { {{3,"*"},{4,"F"},{4,"T`"}}, {{5, "epsilon"}}}},
    {"F", { {{3,"("},{4,"E"},{3,")"}}, {{3, "id"}}}},
};
*/

/*
// 测试 Follow First 编译原理 例4.33
std::map<std::string, ProductionRight> Grammar = {
    {"S", { {{3,"i"},{4,"E"},{3,"t"},{4,"S"},{4,"S`"}},{{3,"a"}}}},
    {"S`", { {{3,"e"},{4,"S"}}, {{5, "epsilon"}}}},
    {"E", { {{3,"b"}}}},
};
*/


bool Token::match(LexType& _lex) {
    if (_lex.token == LexicalName::ID) {
        if (type == 0) return true;
    }
    else if (_lex.token == LexicalName::NUM) {
        if (type == 1) return true;
    }
    else if (_lex.token == LexicalName::RESERVED_WORD) {
        if (type == 2 && _lex.lexeme == val) return true;
    }
    else {
        return val == _lex.lexeme;
    }
    return false;
}

Token Token::LexToToken(LexType _tar) {
    Token token;
    token.val = _tar.lexeme;
    if (_tar.token == LexicalName::ID) {
        token.type = 0;
        token.val = "ID";
    }
    else if (_tar.token == LexicalName::NUM) {
        token.type = 1;
        token.val = "NUM";
    }
    else if (_tar.token == LexicalName::RESERVED_WORD) {
        token.type = 2;
    }
    else if (_tar.token == LexicalName::SPECIAL_SYMBOL) {
        token.type = 3;
    }
    return token;
}

bool Token::operator<(const Token& _other) const {
    // 因为所有0类,1类,5类的val都是一样的
    return type != _other.type ? type < _other.type : val < _other.val;
}

bool Token::operator==(const Token& _other) const {
    if (type == 0 || type == 1 || type == 5 || type == 6) {
        return type == _other.type;
    }
    else {
        if (type != _other.type) {
            return false;
        }
        else {
            return val == _other.val;
        }
    }
}

bool Token::operator==(const LexType& _other) const {
    return *this == LexToToken(_other);
}


bool initLexList(LexList& _lex_list) {
    for (auto item : _lex_list) {
        std::cout << item.show() << std::endl;
    }
    for (int i = _lex_list.size() - 1; i >= 0; i--) {
        if (_lex_list[i].token == LexicalName::ERROR) {
            return false;
        }
        if (_lex_list[i].token == LexicalName::COMMENT || _lex_list[i].token == LexicalName::BLANK) {
            _lex_list.erase(_lex_list.begin() + i);
        }
    }        
    return true;
}

void widthPrint(std::string _str, char _c, int _width) {
    int len = _str.size();
    std::cout << _str;
    for (int i = len; i < _width; i++) {
        std::cout << _c;
    }
}

std::pair<int,int> printSyntaxTree(ProductionRight& _produc, LexList &_lexes, int _lex_index, int _cur, int _layer) {
    // 打印树
    int width = 20;
    std::pair<int, int> nums = { _lex_index, _cur }; // 匹配到词法单元的数量
    widthPrint(_produc[_cur][0].val, '-', width);
    bool flag = true;
    for (int i = 1; i < _produc[_cur].size(); i++) {
        if(!flag){
            for (int j = 0; j < _layer*2; j++) {
                // 名字占位
                if (j % 2 == 0) {
                    widthPrint("|", ' ', width);
                }
                else {
                    widthPrint("");
                }
            }
            widthPrint("|", '-', width);
            widthPrint("", '-');
        }
        else {
            widthPrint("", '-');
        }
        if (_produc[_cur][i].type == 5) {
            widthPrint(_produc[_cur][i].val);
            std::cout << "\n";
        }
        else if (_produc[_cur][i].type != 4) {
            if (_produc[_cur][i].type == 0 || _produc[_cur][i].type == 1) {
                std::cout << _produc[_cur][i].val << " ";
            }
            std::cout << nums.first << " " << _lexes[nums.first].lexeme << " ";
            std::cout << "\n";
            nums.first += 1;
        }
        else {
            nums.second += 1;
            nums = printSyntaxTree(_produc,  _lexes, nums.first, nums.second, _layer + 1);
        }
        flag = false;
    }
    return nums;
}
