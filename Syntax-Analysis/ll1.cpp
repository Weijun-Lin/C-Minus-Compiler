/*
    @Desc
        LL1文法实现
    @Include:
        sytax_c_minus.h
*/

#include "syntax_c_minus.h"

// 构造函数
LL_1::LL_1(LexList _lex_list) : Syntax_Analysis(_lex_list) {
    __isLL_1 = true;
    __first_set = __getFirstSet();
    __follow_set = __getFollowSet();
    __analysis_table = __getAnalysisTable();
}


void LL_1::analysis() {
    //std::pair<bool, Production> res = __match(__begin, {});
    //if (res.first) {
    //    std::cout << "匹配成功" << std::endl;
    //    printSyntaxTree(res.second, res.second.size() - 1, __lex_list);
    //}
    //else {
    //    std::cout << "Error!!!!!!!!\n";
    //}
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
        printf("%-25s", item.first.c_str());
        for (auto i : item.second) {
            printf("<%s,", i.first.val.c_str());
            for (auto j : i.second) {
                printf("%s", j.val.c_str());
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
                            if (i != produc.size() - 1) {
                                // 非终结符 则 加入First - epsilon
                                if (produc[i + 1].type == 4) {
                                    std::set<Token> mid = __first_set[produc[i + 1].val];
                                    if (mid.count({ 5,epsilon }) != 0) {
                                        mid.erase(mid.find({ 5, epsilon }));
                                    }
                                    follow_set[B.first].insert(mid.begin(), mid.end());
                                }
                                // 终结符直接加入
                                else if(produc[i + 1].type != 5){
                                    follow_set[B.first].insert(produc[i+1]);
                                }
                            }
                            // 规则3
                            if (i == produc.size() - 1) {
                                std::set<Token> A = follow_set[item.first];
                                follow_set[B.first].insert(A.begin(), A.end());
                            }
                            if (i == produc.size() - 2 && produc[i + 1].type == 4) {
                                if (__first_set[produc[i + 1].val].count({ 5,epsilon }) == 1) {
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
                }
                analysis_table[item.first].insert({ first, produc });
            }
            if (first_set.count({ 5,epsilon }) == 1) {
                for (auto follow : __follow_set[item.first]) {
                    if (analysis_table[item.first].count(follow) == 1) {
                        __isLL_1 = false;
                    }
                    analysis_table[item.first].insert({ follow, produc });
                }
                if (__follow_set[item.first].count({ 6, "$" }) == 1) {
                    analysis_table[item.first].insert({ { 6,"$" }, produc });
                }
            }
        }
    }
    return analysis_table;
}
