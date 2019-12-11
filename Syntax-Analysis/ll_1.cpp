/*
    @Desc
        LL1文法实现
    @Include:
        sytax_c_minus.h
*/

#include "syntax_c_minus.h"
#include <stack>

// 构造函数
LL_1::LL_1(LexList _lex_list) : Syntax_Analysis(_lex_list) {
    __isLL_1 = true;
    __first_set = __getFirstSet();
    __follow_set = __getFollowSet();
    __analysis_table = __getAnalysisTable();
}


void LL_1::analysis() {
    std::pair<int, ProductionRight> res = __match();
    SEE(res.first);
    SEE(__lex_list.size());
    if (res.first == __lex_list.size()) {
        std::cout << "匹配成功\n";
        printSyntaxTree(res.second, __lex_list);
    }
    else {
        std::cout << "匹配失败\n";
    }
}

void LL_1::printFirstSet() {
    printf("\nFirst Set:\n");
    for (auto item : __first_set) {
        printf("%-25s", item.first.c_str());
        for (auto i : item.second) {
            printf("%-10s", i.val.c_str());
        }
        printf("\n");
    }
}

void LL_1::printFollowSet() {
    printf("\nFollow Set:\n");
    for (auto item : __follow_set) {
        printf("%-25s", item.first.c_str());
        for (auto i : item.second) {
            printf("%-10s", i.val.c_str());
        }
        printf("\n");
    }
}

void LL_1::printAnalysisTable() {
    printf("\nAnalysis Table\n");
    for (auto item : __analysis_table) {
        printf("%-20s", item.first.c_str());
        for (auto i : item.second) {
            printf("<%s,", i.first.val.c_str());
            for (auto j : i.second) {
                printf("%s ", j.val.c_str());
            }
            printf("%s\t", ">");
        }
        printf("\n");
    }
}

FirstSet LL_1::__getFirstSet() {
    FirstSet first_set;
    // 初始化每个非终结符
    for (auto item : Grammar) {
        first_set.insert({ item.first, std::set<Token>()});
    }
    bool isupdate = true;
    while (isupdate) {
        isupdate = false;
        for (auto item : Grammar) {
            int src_size = first_set[item.first].size();
            for (auto produc : item.second) {
                for (int i = 0; i < produc.size(); i++) {
                    // 如果是一个终结符
                    if (produc[i].type != 4) {
                        first_set[item.first].insert(produc[i]);
                        //std::cout << item.first << " : " << produc[i].val << "\n";
                        break;
                    }
                    // 如果是不包含epsilon的非终结符
                    if (first_set[produc[i].val].count({ 5, epsilon }) == 0) {
                        first_set[item.first].insert(first_set[produc[i].val].begin(), first_set[produc[i].val].end());
                        break;
                    }
                    // 包含epsilon的非终结符
                    else {
                        std::set<Token> mid = first_set[produc[i].val];
                        mid.erase(mid.find({ 5, epsilon }));
                        first_set[item.first].insert(mid.begin(), mid.end());
                        if (i == produc.size() - 1) {
                            first_set[item.first].insert({ 5, epsilon });
                        }
                    }
                }   
            }
            // 存在更新
            if (src_size != first_set[item.first].size()) {
                isupdate = true;
            }
        }
    }
    return first_set;
}

FollowSet LL_1::__getFollowSet() {
    FollowSet follow_set;
    // 初始化每个非终结符
    for (auto item : Grammar) {
        if (item.first != __begin) {
            follow_set.insert({ item.first, std::set<Token>()});
        }
        else {
            // 开始符号插入epsilon
            follow_set.insert({ item.first, {{6, "$"}} });
        }
    }
    bool isupdate = true;
    while (isupdate) {
        isupdate = false;
        // 遍历每一个非终结符
        for (auto B : Grammar) {
            // 遍历每一个产生式
            int src_size = follow_set[B.first].size();
            for (auto item : Grammar) {
                for (auto produc : item.second) {
                    for (int i = 0; i < produc.size(); i++) {
                        if (produc[i].val == B.first) {
                            // 规则2
                            for (int j = i + 1; j < produc.size(); j++) {
                                // 非终结符 则 加入First - epsilon
                                if (produc[j].type == 4) {
                                    std::set<Token> mid = __first_set[produc[j].val];
                                    if (mid.count({ 5,epsilon }) == 1) {
                                        mid.erase(mid.find({ 5, epsilon }));
                                        follow_set[B.first].insert(mid.begin(), mid.end());
                                    }
                                    else {
                                        follow_set[B.first].insert(mid.begin(), mid.end());
                                        break;
                                    }
                                }
                                // 终结符直接加入
                                else if(produc[j].type != 5){
                                    follow_set[B.first].insert(produc[j]);
                                    break;
                                }
                            }
                            // 规则3
                            if (i == produc.size() - 1) {
                                std::set<Token> A = follow_set[item.first];
                                follow_set[B.first].insert(A.begin(), A.end());
                            }
                            for (int j = i + 1; j < produc.size(); j++) {
                                if (produc[j].type == 4) {
                                    std::set<Token> mid = __first_set[produc[j].val];
                                    if (mid.count({ 5,epsilon }) == 0) {
                                        break;
                                    }
                                }
                                else if (produc[j].type != 5) {
                                    break;
                                }
                                if (j == produc.size() - 1) {
                                    std::set<Token> A = follow_set[item.first];
                                    follow_set[B.first].insert(A.begin(), A.end());
                                }
                            }
                        }
                    }
                }
            }
            // 存在更新
            if (src_size != follow_set[B.first].size()) {
                isupdate = true;
            }
        }
    }
    return follow_set;
}


// 此文法对悬空else没有做额外处理 所以需要认为选择 去除二义性 此处 other->epislon 在 else处和other->else statement 冲突 选择后者
AnalysisTable LL_1::__getAnalysisTable() {
    AnalysisTable analysis_table;
    // 初始化每个非终结符
    for (auto item : Grammar) {
        analysis_table.insert({item.first, std::map<Token, TokenList>()});
    }
    // 遍历所有的产生式
    for (auto item : Grammar) {
        for (auto produc : item.second) {
            // 获得产生式的First集合
            std::set<Token> first_set;
            for (int i = 0; i < produc.size(); i++) {
                Token token = produc[i];
                if (token.type != 4) {
                    first_set.insert(token);
                    break;
                }
                std::set<Token> mid = __first_set[token.val];
                if (mid.count({ 5,epsilon })) {
                    mid.erase(mid.find({ 5,epsilon }));
                    first_set.insert(mid.begin(), mid.end());
                    if (i == produc.size() - 1) {
                        first_set.insert({ 5,epsilon });
                    }
                }
                else {
                    first_set.insert(mid.begin(), mid.end());
                    break;
                }
            }
            for (auto first : first_set) {
                if (first.type == 5) {
                    continue;
                }
                if (analysis_table[item.first].count(first) == 1) {
                    __isLL_1 = false;
                    std::cout << "冲突 " << item.first << " " << first.val << " " << ": ";
                    for (auto temp : produc) {
                        std::cout << temp.val << " ";
                    }
                    std::cout << "\n";
                }
                else {
                    analysis_table[item.first].insert({ first, produc });
                }
            }
            if (first_set.count({ 5,epsilon }) == 1) {
                for (auto follow : __follow_set[item.first]) {
                    if (analysis_table[item.first].count(follow) == 1) {
                        __isLL_1 = false;
                        std::cout << "冲突 " << item.first << " " << follow.val << " "<< ": ";
                        for (auto temp : produc) {
                            std::cout << temp.val << " ";
                        }
                        std::cout << "\n";
                    }
                    else {
                        analysis_table[item.first].insert({ follow, produc });
                    }
                }
                if (__follow_set[item.first].count({ 6, "$" }) == 1) {
                    analysis_table[item.first].insert({ { 6,"$" }, produc });
                }
            }
        }
    }
    return analysis_table;
}

std::pair<int, ProductionRight> LL_1::__match() {
    // 末尾加入$
    TokenList lexlist;
    for (auto lex : __lex_list) {
        lexlist.push_back(Token::LexToToken(lex));
    }
    lexlist.push_back({ 6,"$" });

    std::stack<Token> stac;
    ProductionRight res;    // 存最左推导的产生式
    int ip = 0; //分析下标
    // 初始化栈 stac = begin $
    stac.push({ 6, "$" });
    stac.push({ 4, __begin });
    Token top = stac.top();
    // 只要没有到末尾就一直匹配
    while (top.type != 6) {
        // 终结符
        if (top.type != 4) {
            // 匹配到最后报错
            if (ip == lexlist.size()) {
                std::cout << "溢出\n";
                break;
            }
            if (top == lexlist[ip]) {
                std::cout << "匹配: " << lexlist[ip].val << "\n";
                ip++;
                stac.pop();
            }
            else {
                std::cout << "不匹配" << lexlist[ip].val << "\n";
                break;  // 退出
            }
        }
        // 非终结符
        else {
            // 错误项
            Token tar = lexlist[ip];
            if (__analysis_table[top.val].count(tar) == 0) {
                std::cout << top.val << " : " << tar.val << " 错误项" << "\n";
                break;
            }
            else {
                stac.pop();
                TokenList produc =  __analysis_table[top.val][tar];
                for (int i = produc.size() - 1; i >= 0; i--) {
                    if (produc[i].type != 5) {
                        stac.push(produc[i]);
                    }
                }
                std::cout << "匹配产生式: " << top.val << "->";
                for (auto temp : produc) {
                    std::cout << temp.val << " " ;
                }
                std::cout << "\n";
                produc.insert(produc.begin(), { 4, top.val });
                res.push_back(produc);
            }
        }
        top = stac.top();
    }
    return {ip, res};
}
