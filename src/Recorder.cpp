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
    // std::cerr << "delete " << i->first << std::endl;
    delete i->second;
  }
}
void Recorder::add(int line, Statement* stmt)
{
  if (Stmts_.count(line))
  {
    delete Stmts_.find(line)->second;
    Stmts_.erase(line);
  }
  Stmts_[line] = stmt;
}
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
    // std::cerr << "fuck recorder" << std::endl;
    return nullptr;
  }
  // std::cerr << "recorder get" << std::endl;
  // std::cerr << Stmts_.at(line) << std::endl;
  // std::cerr << Stmts_.at(line)->text() << std::endl;
  return Stmts_.at(line);
}
bool Recorder::hasLine(int line) const noexcept { return (Stmts_.count(line)); }
void Recorder::clear() noexcept
{
  for (auto i = Stmts_.begin(); i != Stmts_.end();)
  {
    auto tmp = next(i);
    delete i->second;
    Stmts_.erase(i->first);
    i = tmp;
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
  // auto i = Stmts_.find(line);
  // i++;
  // if(i == Stmts_.end())
  // {
  //   return -1;
  // }
  // return i->first;
  auto it = Stmts_.upper_bound(line);
  if (it == Stmts_.end())
  {
    return -1;
  }
  return it->first;
}
int Recorder::getFirstLine()
{
  return Stmts_.begin()->first;
}