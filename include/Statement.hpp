#pragma once

#include <memory>
#include <string>

#include "Expression.hpp"

class Program;
class VarState;

class Statement
{
 public:
  explicit Statement(std::string source);
  virtual ~Statement() = default;

  virtual void execute(VarState& state, Program& program) const = 0;

  const std::string& text() const noexcept;

 private:
  std::string source_;
};

// TODO: Other statement types derived from Statement, e.g., GOTOStatement,
// LetStatement, etc.
class LETStatement : public Statement
{
 private:
  std::string name_;
  Expression* expr_;  // 右表达式

 public:
  LETStatement(std::string name, Expression* expr, std::string originLine);
  void execute(VarState& state, Program& program) const;
};

class PRINTStatement : public Statement
{
 private:
  Expression* expr_;

 public:
  PRINTStatement(Expression* expr, std::string originLine);
  void execute(VarState& state, Program& program) const;
};

class INPUTStatement : public Statement
{
 private:
  std::string name_;

 public:
  INPUTStatement(std::string name, std::string originLine);
  void execute(VarState& state, Program& program) const;
};

class GOTOStatement : public Statement
{
 private:
  int targetline_;

 public:
  GOTOStatement(int targetline, std::string originLine);
  void execute(VarState& state, Program& program) const;
};

class IFStatement : public Statement
{
 private:
  Expression* leftExpr_;
  Expression* rightExpr_;
  char op_;
  int targetline_;

 public:
  IFStatement(Expression* leftExpr, Expression* rightExptr, char op, int targetline,
              std::string originLine);
  void execute(VarState& state, Program& program) const;
};

class REMStatement : public Statement
{
 public:
  REMStatement(std::string originLine);
  void execute(VarState& state, Program& program) const;
};

class ENDStatement : public Statement
{
 public:
  ENDStatement(std::string originLine);
  void execute(VarState& state, Program& program) const;
};