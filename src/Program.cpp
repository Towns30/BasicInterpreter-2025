// TODO: Imply interfaces declared in the Program.hpp.
#pragma once

#include "Program.hpp"

#include <memory>

#include "Recorder.hpp"
#include "VarState.hpp"

Program::Program()
{
  programCounter_ = 0;
  programEnd_ = false;
  needNewLine_ = true;
}
void Program::addStmt(int line, Statement* stmt) { recorder_.add(line, stmt); }
void Program::removeStmt(int line) { recorder_.remove(line); }
void Program::run()
{
  programEnd_ = false;
  do
  {
    if (needNewLine_)
    {
      programCounter_ = recorder_.nextLine(programCounter_);
    }
    execute(recorder_.get(programCounter_));
  } while (recorder_.hasLine(programCounter_));
}
void Program::list() const { recorder_.printLines(); }
void Program::clear()
{
  recorder_.clear();
  programCounter_ = 0;
}
void Program::execute(const Statement* stmt)
{
  stmt->execute(vars_, *this);
}
int Program::getPC() const noexcept
{
  return programCounter_;
}
void Program::changePC(int line)
{
  programCounter_ = line;
}
void Program::programEnd()
{
  programEnd_ = true;
}
void Program::noNewLine()
{
  needNewLine_ = false;
}
void Program::NewLine()
{
  needNewLine_ = true;
}