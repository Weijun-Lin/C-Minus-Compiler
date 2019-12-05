/*
    @Date: 2019-12-4
    @Include:
        lex_c_minus.h > lex_preset.h
    @Desc:
        C- 语言语法分析基础头文件
*/

#ifndef __SYNTAX_C_MINUS_H__
#define __SYNTAX_C_MINUS_H__

#include "lex_c_minus.h"

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
        attr: 对应的字符串值
            type 0,1 没有意义
            type 2,3 对应的值
            type 4 对应的非终结符名字
*/
struct Token {
    int type;
    std::string val;
};

// 定义语法分析接口
class Syntax_Analysis {
public:
    virtual void analysis() = 0;
protected:
    LexList __lex_list;
};

#endif // !__SYNTAX_C_MINUS_H__
