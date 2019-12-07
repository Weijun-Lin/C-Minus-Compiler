/*
    @Date: 2019-10-30
    @Desc: 词法分析器预配置
*/


#ifndef __PRESET_H__
#define __PRESET_H__

#include <set>
#include <string>
#include <map>
#include <vector>

// 词法单元类型（名字）
enum class LexicalName { RESERVED_WORD, ID, SPECIAL_SYMBOL, NUM, COMMENT, BLANK, ERROR };
// 每一个匹配错误的信息描述
extern std::map<LexicalName, std::string> Error_Info;
// 某些特殊符号的意义
enum class Special_Symbol { LT, LE, EQ, NE, GT, GE };
// <= == 这类字符的str到Enum的映射
extern std::map<std::string, Special_Symbol> Str_SpeSym;
// SpecialSymbol 到 str的映射
extern std::map<Special_Symbol, std::string> SpeSym_str;
// LexicalName 到 String 的 映射
extern std::map<LexicalName, std::string> LexName_Str;

// 词法类型
struct LexType {
    std::string lexeme;    // 词素
    LexicalName token;    // 词法单元名字
    std::string attr;    // 属性值
    std::string show();
};

/*
    状态转移表：
    结构如下：（为简便 string代替原来的 char， 一组字母可以到同一个状态）
    e.g.
    states<vector's index>  map<string, state>
    0                        ("+-*...", 1),('/', 2)

    p.s.
    string.length() == 0 表明接受任意字符
*/
typedef std::vector<std::vector<std::pair<std::string, int>>> MoveTable;
typedef std::set<int> FinalStates;

// 计算一个确定状态转换
class DFA {
public:
    // 初始化DFA默认使用空构造
    DFA(MoveTable _move_table = {}, 
        LexicalName _token_name = LexicalName::COMMENT, 
        FinalStates _final_states = {}
    );
    /*
        @Param:
            int:forward 当前指针下标
            string:srccode 源代码
        @Return:
            <bool,LexType> 是否合法，合法返回相应信息
    */
    std::pair<int, LexType> dfa(int _forward, std::string& _srccode);    // DFA执行
    ~DFA ();
    
private:
    LexicalName __token_name;
    MoveTable __move_table;
    FinalStates __final_states;

    // 获取下一个状态 -1 表示非法
    int __state_trans(int _state, char _letter);
};

// 保留字集合
extern std::set<std::string> reserved_word;

#endif // !__PRESET_H__