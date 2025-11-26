#include "Statement.hpp"

#include <algorithm>
#include <iostream>
#include <limits>
#include <sstream>
#include <utility>

#include "Program.hpp"
#include "VarState.hpp"
#include "utils/Error.hpp"

Statement::Statement(std::string source) : source_(std::move(source)) {}

const std::string& Statement::text() const noexcept { return source_; }

// TODO: Imply interfaces declared in the Statement.hpp.
LETStatement::LETStatement(std::string name, Expression* expr, std::string originLine)
    : Statement(originLine)
{
  name_ = name;
  expr_ = expr;
}
void LETStatement::execute(VarState& state, Program& program) const
{
  state.setValue(name_, expr_->evaluate(state));
  program.NewLine();  // 换行
}
LETStatement::~LETStatement() { delete expr_; }
PRINTStatement::PRINTStatement(Expression* expr, std::string originLine) : Statement(originLine)
{
  expr_ = expr;
}
void PRINTStatement::execute(VarState& state, Program& program) const
{
  int result = expr_->evaluate(state);
  std::cout << result << std::endl;
  program.NewLine();  // 换行
}
PRINTStatement::~PRINTStatement()
{
  // std::cerr << "destructing print" << std::endl;
  delete expr_;
}
INPUTStatement::INPUTStatement(std::string name, std::string originLine) : Statement(originLine)
{
  name_ = name;
}
void INPUTStatement::execute(VarState& state, Program& program) const
{
  // std::cerr << "entered exec" << std::endl;
  int value;
  while (true)
  {
    std::cout << " ? ";
    std::string line_got;
    std::getline(std::cin, line_got);

    auto IsInt = [](const std::string& a) -> bool
    {
      if (a[0] != '-' && (a[0] > '9' || a[0] < '0'))
      {
        return false;
      }
      if (a == "-")
      {
        return false;
      }
      for (auto i = 1; i + 1 <= a.size(); i++)
      {
        if(a[i] > '9' || a[i] < '0')
        {
          return false;
        }
      }
      return true;
    };
    if (IsInt(line_got))
    {
      value = stoi(line_got);
      break;
    }
    else
    {
      std::cout << "INVALID NUMBER" << std::endl;
    }
  }

  state.setValue(name_, value);
  program.NewLine();  // 换行
}

GOTOStatement::GOTOStatement(int targetline, std::string originLine) : Statement(originLine)
{
  targetline_ = targetline;
}
void GOTOStatement::execute(VarState& state, Program& program) const
{
  program.changePC(targetline_);
  program.noNewLine();  // 不换行
}

IFStatement::IFStatement(Expression* leftExpr, Expression* rightExpr, char op, int targetline,
                         std::string originLine)
    : Statement(originLine)
{
  leftExpr_ = leftExpr;
  rightExpr_ = rightExpr;
  op_ = op;
  targetline_ = targetline;
}
void IFStatement::execute(VarState& state, Program& program) const
{
  int left = leftExpr_->evaluate(state);
  int right = rightExpr_->evaluate(state);
  bool flag;
  switch (op_)
  {
    case '>':
      flag = (left > right);
      break;
    case '<':
      flag = (left < right);
      break;
    case '=':
      flag = (left == right);
      break;
  }
  if (flag)
  {
    program.changePC(targetline_);
    program.noNewLine();  // 不换行
  }
  else
  {
    program.NewLine();
  }
}
IFStatement::~IFStatement()
{
  delete leftExpr_;
  delete rightExpr_;
}
REMStatement::REMStatement(std::string originLine) : Statement(originLine) {}
void REMStatement::execute(VarState& state, Program& program) const { program.NewLine(); }

ENDStatement::ENDStatement(std::string originLine) : Statement(originLine) {}
void ENDStatement::execute(VarState& state, Program& program) const { program.programEnd(); }