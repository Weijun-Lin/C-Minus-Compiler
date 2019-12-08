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
            6: $ 分析末尾（LL1使用）
        attr: 对应的字符串值
            type 0,1 没有意义
            type 2,3 对应的值
            type 4 对应的非终结符名字
*/
struct Token {
    int type;
    std::string val;
    bool match(LexType &_lex);
    // 定义等级 SET 使用
    bool operator < (const Token &_other) const {
        // 因为所有0类,1类,5类的val都是一样的
        return type != _other.type ? type < _other.type : val < _other.val;
    }
    bool operator == (const Token& _other) const {
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
};


// 定义epsilon
extern const std::string epsilon;

// 定义产生式 e.g. A->A|B
typedef std::vector<Token> TokenList;
// 产生式右值
// 包含或 所以有vector
typedef std::vector<TokenList> ProductionRight;
// 定义文法 多条产生式
extern std::map<std::string, ProductionRight> Grammar;


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
    // 返回匹配数量以及产生式集合
    std::pair<int, ProductionRight>  __match(std::string _name, TokenList _follow, int _index = 0);
};


// 定义First 集 类型
typedef std::map<std::string, std::set<Token> > FirstSet;
// 定义 Second
typedef FirstSet FollowSet;
// 定义预测分析表
typedef std::map<std::string, std::map<Token, TokenList>> AnalysisTable;

class LL_1 :public Syntax_Analysis {
public:
    LL_1(LexList _lex_list);
    void analysis();
    void printFirstSet();
    void printFollowSet();
    void printAnalysisTable();
private:
    FirstSet __getFirstSet();
    FollowSet __getFollowSet();
    AnalysisTable __getAnalysisTable();
    FirstSet __first_set;
    FollowSet __follow_set;
    AnalysisTable __analysis_table;
    bool __isLL_1;
};

// 打印width宽的 _str 不足用 _c 填充
void widthPrint(std::string _str, char _c = ' ', int _width = 4);

// 通过所选产生式打印语法树
std::pair<int, int> printSyntaxTree(ProductionRight& _produc, int _cur, LexList& _lexes, int _lex_index = 0, int _layer = 0);


#endif // !__SYNTAX_C_MINUS_H__
