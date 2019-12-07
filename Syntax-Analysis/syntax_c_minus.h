/*
    @Date: 2019-12-4
    @Include:
        lex_c_minus.h > lex_preset.h
    @Desc:
        C- 语言语法分析基础头文件
*/

#ifndef __SYNTAX_C_MINUS_H__
#define __SYNTAX_C_MINUS_H__

#define SEE(x) std::cout<<(#x) << ": " << x << "\n";

#include "lex_c_minus.h"
#include <iostream>

typedef std::vector<LexType> LexList;   // 词法分析器的输入

// 判断词法分析是否有错误 以及删除其中的评论 return false 表示有错
bool initLexList(LexList& _lex_list);


/*
    @Desc:
        产生式的接收单元
    @Attr:
        type: 标明接收单元类型
            0: ID
            1: NUM
            2: RESERVED_WORD
            3: SPECIAL_SYMBOL
            4: NonTerminal
            5: 空
        attr: 对应的字符串值
            type 0,1 没有意义
            type 2,3 对应的值
            type 4 对应的非终结符名字
*/
struct Token {
    int type;
    std::string val;
    bool match(LexType &_lex);
};

extern const std::string epsilon;

// 定义产生式 e.g. A->A|B
typedef std::vector<Token> TokenList;
typedef std::vector<TokenList> Production;
// 产生式和其对应名字的映射
extern std::map<std::string, Production> Productions;


// 定义语法分析接口
class Syntax_Analysis {
public:
    Syntax_Analysis(LexList _lex_list):
    __lex_list(_lex_list),__begin("program") {
        __length = __lex_list.size();
    }
    virtual void analysis() = 0;
protected:
    LexList __lex_list; // 词法分析的结果列表
    int __length;       // 词法的长度
    std::string __begin;// 产生式开始符号
};


// 递归下降分析
class Recursivs_Descent : public Syntax_Analysis {
public:
    Recursivs_Descent(LexList _lex_list);
    void analysis();
private:
    //bool __match(std::string _name, int _index);
    std::pair<bool, Production>  __match(std::string _name, TokenList _follow, int _index = 0);
};

void widthPrint(std::string _str, int _width, char _c);
void printSyntaxTree(Production& _produc, int _cur, int _layer);


#endif // !__SYNTAX_C_MINUS_H__
