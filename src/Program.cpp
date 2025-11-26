// TODO: Imply interfaces declared in the Program.hpp.
#include "Program.hpp"

#include <iostream>
#include <memory>

#include "utils/Error.hpp"
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
  programCounter_ = 0;
  programEnd_ = false;
  needNewLine_ = true;
  do
  {
    if (needNewLine_)
    {
      programCounter_ = recorder_.nextLine(programCounter_);
    }
    // execute(recorder_.get(programCounter_));
    // std::cerr << "exec program" << std::endl;
    // std::cerr << "now at " << programCounter_ << std::endl;
    const Statement* stmt = recorder_.get(programCounter_);
    if (stmt == nullptr)
    {
      // std::cerr << "NULLPTR!!!" << std::endl;
      break;
    }
    stmt->execute(vars_, *this);
  } while (recorder_.hasLine(programCounter_) && programEnd_ == false);
}
void Program::list() const { recorder_.printLines(); }
void Program::clear()
{
  recorder_.clear();
  vars_.clear();
  programCounter_ = 0;
}
void Program::execute(const Statement* stmt) { stmt->execute(vars_, *this); }
int Program::getPC() const noexcept { return programCounter_; }
void Program::changePC(int line)
{
  if (!recorder_.hasLine(line))
  {
    throw BasicError("LINE NUMBER ERROR");
  }
  programCounter_ = line;
}
void Program::programEnd() { programEnd_ = true; }
void Program::noNewLine() { needNewLine_ = false; }
void Program::NewLine() { needNewLine_ = true; }