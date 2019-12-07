/*
    @Desc
        递归下降语法分析实现
    @Include:
        sytax_c_minus.h

*/

#include "syntax_c_minus.h"

// 构造函数
Recursivs_Descent::Recursivs_Descent(LexList _lex_list):
   Syntax_Analysis(_lex_list) {}


void Recursivs_Descent::analysis() {
    std::pair<bool, Production> res = __match(__begin, {});
    if (res.first) {
        std::cout << "匹配成功" << std::endl;
        printSyntaxTree(res.second, res.second.size() - 1, 0);
    }
    else {
        std::cout << "Error!!!!!!!!\n";
    }
}

// 没有贪心匹配的
//std::pair<bool, Production> Recursivs_Descent::__match(std::string _name, TokenList _follow, int _index)
//{
//    std::cout << _name << ":\n" << "follow:\n";
//
//    for (auto i : _follow) {
//        std::cout << i.val << " ";
//    }
//    std::cout << "\n";
//    std::pair<bool, Production> res;
//    int cur = _index;
//    Production produc = Productions[_name]; // 获取对应的产生式集
//    std::cout << "进入产生式: " << _name << ":";
//    for (auto i : produc) {
//        for (auto j : i) {
//            std::cout << j.val << " ";
//        }
//        std::cout << ";";
//    }
//    std::cout << "\n\n";
//    // 选择一个产生式
//    for (auto one_produc : produc) {
//        //std::cout << "expand proc:";
//        TokenList src_one_produc = one_produc;
//        one_produc.insert(one_produc.end(), _follow.begin(), _follow.end());
//        std::cout << "待匹配: ";
//        for (auto i : one_produc) {
//            std::cout << i.val << " ";
//        }
//        std::cout << "\n\n";
//        bool flag = true;
//        // 遍历每个符号
//        for (auto token_iter = one_produc.begin(); token_iter != one_produc.end(); token_iter++) {
//            // 如果是匹配一个终结符
//            if (token_iter->type != 4) {
//                if (token_iter->type == 5) {
//                    std::cout << _name << ": empty" << "\n";
//                    //break;
//                }
//                // 匹配成功
//                else if (cur < __length && token_iter->match(__lex_list[cur])) {
//                    token_iter->val = __lex_list[cur].lexeme;
//                    std::cout << "匹配:" << token_iter->val << "\n";
//                    cur++;
//                }
//                // 匹配失败
//                else {
//                    flag = false;
//                    break;
//                }
//            }
//            // 匹配非终结符
//            else {
//                //std::cout << "non-terminal: " << token_iter->val << "\n";
//                TokenList follow(token_iter + 1, one_produc.end());
//                //SEE(follow.size());
//                std::pair<bool, Production> match_res = __match(token_iter->val, follow, cur);
//                if (match_res.first) {
//                    res.second.insert(res.second.end(), match_res.second.begin(), match_res.second.end());
//                    //SEE(flag);
//                }
//                else {
//                    flag = false;
//                }
//                break;
//            }            
//        }
//        if (flag) {
//            std::cout << "匹配产生式: " << _name << " : ";
//            for (auto i : src_one_produc) {
//                std::cout << i.val << " ";
//            }
//            std::cout << "\n";
//            res.first = true;
//            src_one_produc.insert(src_one_produc.begin(), { 4, _name });
//            res.second.push_back(src_one_produc);
//            return res;
//        }
//        else {
//            cur = _index; // 尝试下一个产生式
//        }
//    }
//    res.first = false;
//    return res;
//}

std::pair<bool, Production> Recursivs_Descent::__match(std::string _name, TokenList _follow, int _index)
{
    // 打印产生式 左符号和follow follow 为待匹配集合
    std::cout << _name << ":\n" << "follow:\n";

    for (auto i : _follow) {
        std::cout << i.val << " ";
    }
    std::cout << "\n";
    std::pair<bool, Production> res;
    int cur = _index;
    Production produc = Productions[_name]; // 获取对应的产生式集
    // 打印产生式
    std::cout << "进入产生式: " << _name << ":";
    for (auto i : produc) {
        for (auto j : i) {
            std::cout << j.val << " ";
        }
        std::cout << ";";
    }
    std::cout << "\n\n";
    std::vector< TokenList> potential_res;  // 潜在的结果集合 多个产生式匹配取最长的
    // 选择一个产生式
    for (auto one_produc : produc) {
        //std::cout << "expand proc:";
        TokenList src_one_produc = one_produc;
        // 将自己的产生式和待匹配产生式合并
        one_produc.insert(one_produc.end(), _follow.begin(), _follow.end());
        std::cout << "待匹配: ";
        for (auto i : one_produc) {
            std::cout << i.val << " ";
        }
        std::cout << "\n\n";
        bool flag = true;
        // 遍历每个符号
        for (auto token_iter = one_produc.begin(); token_iter != one_produc.end(); token_iter++) {
            // 如果是匹配一个终结符
            if (token_iter->type != 4) {
                if (token_iter->type == 5) {
                    std::cout << _name << ": empty" << "\n";
                    //break;
                }
                // 匹配成功
                else if (cur < __length && token_iter->match(__lex_list[cur])) {
                    token_iter->val = __lex_list[cur].lexeme;
                    std::cout << "匹配:" << token_iter->val << "\n";
                    cur++;
                }
                // 匹配失败
                else {
                    flag = false;
                    break;
                }
            }
            // 匹配非终结符
            else {
                //std::cout << "non-terminal: " << token_iter->val << "\n";
                TokenList follow(token_iter + 1, one_produc.end());
                //SEE(follow.size());
                std::pair<bool, Production> match_res = __match(token_iter->val, follow, cur);
                if (match_res.first) {
                    res.second.insert(res.second.end(), match_res.second.begin(), match_res.second.end());
                    //SEE(flag);
                }
                else {
                    flag = false;
                }
                break;
            }
        }
        cur = _index;   // 尝试下一个
        if (flag) {
            std::cout << "匹配产生式: " << _name << " : ";
            for (auto i : src_one_produc) {
                std::cout << i.val << " ";
            }
            std::cout << "\n";
            src_one_produc.insert(src_one_produc.begin(), { 4, _name });
            //res.second.push_back(src_one_produc);
            potential_res.push_back(src_one_produc);
            //return res;
        }
    }
    // 没有匹配
    if (potential_res.empty()) {
        res.first = false;
    }
    // 取最长产生式
    else {
        res.first = true;
        int tar = 0;
        for (int i = 0; i < potential_res.size(); i++) {
            if (potential_res[i].size() > potential_res[tar].size()) {
                tar = i;
            }
        }
        res.second.push_back(potential_res[tar]);
    }
    return res;
}
