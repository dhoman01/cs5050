#ifndef UTILS_HPP
#define UTILS_HPP

#include <iostream>
#include <stdio.h>
#include <string>

int PROGRESS_BAR_WIDTH = 70;

#ifdef WINDOWS
#include <windows.h>
std::string get_current_path() {
  char result[MAX_PATH];
  return std::string(result, GetModuleFileName(NULL, result, MAX_PATH));
}

#else
#include <limits.h>
#include <unistd.h>
std::string get_current_path() {
  char result[PATH_MAX];
  ssize_t count = readlink("/proc/self/exe", result, PATH_MAX);
  return std::string(result, (count > 0) ? count : 0);
}

#endif

std::string get_current_dir() {
  std::string f = get_current_path();
  return f.substr(0, f.find_last_of("\\/"));
}

void print_progress(float progress) {
  std::cout << "[";
  int pos = PROGRESS_BAR_WIDTH * progress;
  for (int i = 0; i < PROGRESS_BAR_WIDTH; ++i) {
    if (i < pos)
      std::cout << "=";
    else if (i == pos)
      std::cout << ">";
    else
      std::cout << " ";
  }
  std::cout << "] " << int(progress * 100.0) << " %\r";

  std::cout.flush();
}

#endif
