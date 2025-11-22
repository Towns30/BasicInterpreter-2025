#pragma once

#include <memory>

#include "Recorder.hpp"
#include "VarState.hpp"

class Statement;

class Program
{
 public:
  Program();

  void addStmt(int line, Statement* stmt);
  void removeStmt(int line);

  void run();
  void list() const;
  void clear();

  void execute(Statement* stmt);

  int getPC() const noexcept;
  void changePC(int line);
  void programEnd();
  void noNewLine();
  void NewLine();

 private:
  Recorder recorder_;
  VarState vars_;
  int programCounter_;
  bool programEnd_;
  bool needNewLine;
  void resetAfterRun() noexcept;
};
