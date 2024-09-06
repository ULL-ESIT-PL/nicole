#include <iostream>

#include "../inc/lexer/lexer.h"
using namespace nicole;

int main() {
  const Lexer lexer{std::vector<Category>{
      Category{"(?<SPACE>\\s+)", true},
      Category{"(?<COMMENT>\\/\\*(.|\\n)*?\\*\\/)", true},

      Category{"(?<NUMBER>\\d+(\\.\\d+)?([eE][-]?\\d+)?)", false},
      Category{"(?<STRING>\"(?:\\\\.|[^\"\\\\])*\")", false},
      Category{"(?<TRUE>true)", false},
      Category{"(?<FALSE>false)", false},

      Category{"(?<CONST>const)", false},

      Category{"(?<IMPORT>import)", false},

      Category{"(?<PRINT>print)", false},
      Category{"(?<SYSTEM>system)", false},
      Category{"(?<TYPE>type)", false},

      Category{"(?<IF>if)", false},
      Category{"(?<ELSE>else)", false},
      Category{"(?<WHILE>while)", false},
      Category{"(?<WHILE>while)", false},
      Category{"(?<FOR>for)", false},
      Category{"(?<STOP>stop)", false},
      Category{"(?<PASS>pass)", false},
      Category{"(?<RETURN>return)", false},
      Category{"(?<FUNCTION>def)", false},

      Category{"(?<OR>or)", false},
      Category{"(?<AND>and)", false},

      Category{"(?<ID>[a-zA-Z]+[a-zA-Z0-9]*)", false},

      Category{"(?<RP>\\))", false},
      Category{"(?<LP>\\()", false},
      Category{"(?<RB>\\})", false},
      Category{"(?<LB>\\{)", false},
      Category{"(?<RC>\\])", false},
      Category{"(?<LC>\\[)", false},
      Category{"(?<SEMICOLON>;)", false},
      Category{"(?<COMMA>,)", false},
      Category{"(?<TERNARY>\\?)", false},
      Category{"(?<DOTDOT>:)", false},

      Category{"(?<SMALLEREQUAL><=)", false},
      Category{"(?<BIGGEREQUAL>>=)", false},
      Category{"(?<NOTEQUAL>!=)", false},
      Category{"(?<EQUAL>==)", false},

      Category{"(?<ASSIGNMENT>=)", false},

      Category{"(?<INCREMENT>\\+\\+|\\-\\-)", false},
      Category{"(?<OPERATOR>[+-/\\*<>!%])", false},
  }};
  const std::filesystem::path path{"../test/test1.nc"};
  const auto tokens{lexer.analyze(path, true)};

  return 0;
}