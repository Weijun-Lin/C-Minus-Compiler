# C- 语言 词法分析以及语法分析 设计

> 仅分享编译原理课程设计的词法和语法分析部分

## 项目结构

````
├─Lexical-Analysis # 词法分析
│    Lexical_Analysis.cpp # 词法分析main函数，打印词法分析结果
│    lex_c_minus.cpp      # 实现各个词法单元的DFA以及声明扫描器接口
│    lex_c_minus.h        # 声明识别各个词法单元的DFA以及声明扫描器接口
│    lex_preset.cpp       # 实现DFA类，以及基础的数据结构
│    lex_preset.h         # 声明DFA类，以及基础的数据结构
│          
└─Syntax-Analysis # 语法分析
     LL1_Grammar.txt      # LL1文法
     ll_1.cpp             # LL1文法识别函数实现
     recursive_descent.cpp# 递归下降分析实现
     Syntax_Analysis.cpp  # 语法分析main函数，打印语法分析树
     syntax_c_minus.cpp   # C-语法分析文法定义
     syntax_c_minus.h     # C-语法文法和基础数据结构声明
````

## 项目环境

**VS 2019**开发环境，对于其他版本，需要注意SDK版本以及平台工具集的更新

对于SDK版本可以在项目->重定解决方案解决

工具集的更新可以在项目属性->配置属性->常规中修改即可

仓库已经包含工程配置文件，但仍需检查：

1. 调试**平台WIN32**（我是在此平台完成，相关配置也在此平台，你也可以调整为x64，但需检查下列的每一步）
2. 工程Lexical-Analysis的**配置类型需要为静态库**
3. 工程Syntax-Analysis的配置类型为应用程序
4. 工程Syntax-Analysis需要添加对Lexical-Analysis的引用
5. 并且工程Syntax-Analysis需要在项目属性->C/C+->常规,添加附加包含目录../Lexical-Analysis

一般情况下不需要检查，即可运行成功，但编译失败则需要检查上列事项

在解决方案下有两个工程（如上述），两个项目都有对应的main函数，可以分别展示其词法和语法分析结果

切换只需要将**对应工程设置为启动目录**即可，切换到Lexical-Analysis，注意配置类型需要改回应用程序，否则只会生成静态库

## Main函数介绍

#### Lexical_Analysis.cpp

读取输入文件，将结果输出到指定文件

这里并没有交互，通过

```cpp
std::string file_path = "./in.txt";
std::string out_file_path = "./out.txt";
```

设置输入输出即可

#### Syntax_Analysis.cpp

此处使用两个方法进行语法分析，递归下降以及LL1文法分析。

```cpp
#define OUT_FILE	// 是否输出到out.txt
//#define __RECURSIVE_DESCENT__
#define __LL_1__
```

通过预编译指令设置对应的方法即可