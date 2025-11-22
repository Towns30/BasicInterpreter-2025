// TODO: Imply interfaces declared in the Recorder.hpp.
#include "Recorder.hpp"

#include <iostream>
#include <map>
#include <memory>
#include <vector>

#include "Statement.hpp"
#include "utils/Error.hpp"

Recorder::~Recorder()
{
  for (auto i = Stmts_.begin(); i != Stmts_.end(); i++)
  {
    delete i->second;
  }
}
void Recorder::add(int line, Statement* stmt) { Stmts_[line] = stmt; }
void Recorder::remove(int line)
{
  if (!Stmts_.count(line))
  {
    return;
  }
  delete Stmts_[line];  // 归还内存
  Stmts_.erase(line);   // 删键
}
const Statement* Recorder::get(int line) const noexcept
{
  if (!Stmts_.count(line))
  {
    return nullptr;
  }
  return Stmts_.at(line);
}
bool Recorder::hasLine(int line) const noexcept { return (Stmts_.count(line)); }
void Recorder::clear() noexcept
{
  for (auto i = Stmts_.begin(); i != Stmts_.end(); i++)
  {
    delete i->second;
    Stmts_.erase(i->first);
  }
}
void Recorder::printLines() const
{
  for (auto i = Stmts_.begin(); i != Stmts_.end(); i++)
  {
    std::cout << i->second->text() << std::endl;
  }
}
int Recorder::nextLine(int line) const noexcept
{
  auto i = Stmts_.find(line);
  i++;
  if(i == Stmts_.end())
  {
    return -1;
  }
  return i->first;
}