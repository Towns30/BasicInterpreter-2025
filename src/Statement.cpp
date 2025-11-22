#include "Statement.hpp"

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

PRINTStatement::PRINTStatement(std::string name, std::string originLine) : Statement(originLine)
{
  name = name_;
}
void PRINTStatement::execute(VarState& state, Program& program) const
{
  std::cout << state.getValue(name_) << std::endl;
  program.NewLine();  // 换行
}

INPUTStatement::INPUTStatement(std::string name, std::string originLine) : Statement(originLine)
{
  name_ = name;
}
void INPUTStatement::execute(VarState& state, Program& program) const
{
  int value;
  std::cout << '?' << std::endl;
  std::cin >> value;
  state.setValue(name_, value);
  program.NewLine();  // 换行
}

GOTOStatement::GOTOStatement(int targetline, std::string originLine) : Statement(originLine)
{
  targetline_ = targetline;
}
void GOTOStatement::execute(VarState& state, Program& program) const { program.changePC(targetline_); }

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
    case '<':
      flag = (left < right);
    case '=':
      flag = (left = right);
  }
  if (flag)
  {
    program.changePC(targetline_);
    program.noNewLine();  // 不换行
  }
}

REMStatement::REMStatement(std::string originLine) : Statement(originLine) {}
void REMStatement::execute(VarState& state, Program& program) const
{
  program.NewLine();
}

ENDStatement::ENDStatement(std::string originLine) : Statement(originLine) {}
void ENDStatement::execute(VarState& state, Program& program) const
{
  program.programEnd();
}