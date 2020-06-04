/*
    @Date: 2019-10-31
    @Include:
        preset.h
    @Desc:
        C- 语言各种类型的DFA定义
*/

#ifndef __LEX_C_MINUS_H__
#define __LEX_C_MINUS_H__

#include <algorithm>
#include "lex_preset.h"

extern DFA Id;
extern DFA Comment;
extern DFA Num;
extern DFA Special_sign;
extern DFA Blank;
extern std::string Letter;
extern std::string Digit;
extern std::string Separator;

/*
    @desc:
        DFA Id;
        DFA Comment;
        DFA Num;
        DFA Special_sign;
        DFA Blank;
        std::string Letter;
        std::string Digit;
        std::string Separator;
        对上面词法单元的初始化
*/
void lexInit();
/* 
    @desc:
        编译词法
    @Param:
        std::string _src_code: 输入源代码
    @Return:
        std::vector<LexType> 词法分析列表
*/
std::vector<LexType> lexCompile(std::string _src_code);

#endif // !__LEX_C_MINUS_H__