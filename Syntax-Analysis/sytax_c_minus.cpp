#include "syntax_c_minus.h"


bool initLexList(LexList& _lex_list) {
    for (int i = _lex_list.size()-1; i >= 0; i--) {
        if (_lex_list[i].token == LexicalName::ERROR) {
            return false;
        }
        _lex_list.erase(_lex_list.begin() + i);
    }

    return true;
}