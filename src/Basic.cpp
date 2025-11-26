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
  ParsedLine parsed_line;
  std::string line;
  Statement* stmt;
  TokenStream tokenstream;
  int line_number;
  while (std::getline(std::cin, line))
  {
    // std::cerr << "line=" << line << std::endl;
    if (line.empty())
    {
      continue;
    }
    try
    {
      // TODO: The main function.
      tokenstream = lexer.tokenize(line);
      // std::cerr << "成功拆解语句" << std::endl;
      if (tokenstream.peek()->type == TokenType::CLEAR)
      {
        program.clear();
      }
      else if (tokenstream.peek()->type == TokenType::QUIT)
      {
        // program.programEnd();
        // std::cerr << '0' << std::endl;
        break;
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
        parsed_line = parser.parseLine(tokenstream, line);
        // std::cerr << "非解释器语句" << std::endl;
        stmt = parsed_line.getStatement();
        // std::cerr << "成功提取Statement*" << std::endl;
        line_number = parsed_line.getLine().value_or(0);
        // std::cerr << "成功提取行号" << std::endl;
        if (line_number == 0)
        {
          // std::cerr << "为立即执行命令" << std::endl;
          try
          {
            program.execute(stmt);
            delete stmt;
          }
          catch (const BasicError& e)
          {
            delete stmt;
            throw e;
          }
        }
        else
        {
          if (stmt == nullptr)
          {
            // std::cerr << "empty stmt" << std::endl;
            program.removeStmt(line_number);
            continue;
          }
          // std::cerr << "为行命令" << std::endl;
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