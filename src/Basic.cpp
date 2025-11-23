#include <iostream>
#include <memory>
#include <string>

#include "Lexer.hpp"
#include "Parser.hpp"
#include "Program.hpp"
#include "Token.hpp"
#include "utils/Error.hpp"

int main()
{
  Lexer lexer;
  Parser parser;
  Program program;
  TokenStream tokenstream;
  std::string line;
  Statement* stmt;
  int line_number;
  while (std::getline(std::cin, line))
  {
    if (line.empty())
    {
      continue;
    }
    try
    {
      // TODO: The main function.
      tokenstream = lexer.tokenize(line);
      if (tokenstream.peek()->type == TokenType::CLEAR)
      {
        program.clear();
      }
      else if (tokenstream.peek()->type == TokenType::QUIT)
      {
        program.programEnd();
      }
      else if (tokenstream.peek()->type == TokenType::LIST)
      {
        program.list();
      }
      else if (tokenstream.peek()->type == TokenType::RUN)
      {
        program.run();
      }
      else if (tokenstream.peek()->type == TokenType::HELP)
      {
        continue;
      }
      else
      {
        stmt = parser.parseLine(tokenstream, line).getStatement();
        line_number = parser.parseLine(tokenstream, line).getLine().value_or(0);
        if (line_number == 0)
        {
          program.execute(stmt);
        }
        else
        {
          program.addStmt(line_number, stmt);
        }
      }
    }
    catch (const BasicError& e)
    {
      std::cout << e.message() << "\n";
    }
  }
  return 0;
}