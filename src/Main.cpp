#include "App.h"
#include <stdexcept>
#include <fmt/core.h>
#include <fmt/color.h>

int main(int argc, char** argv) {
  App app;
  try {
    app.run();
  } catch (const std::runtime_error& error) {
    fmt::print(fmt::fg(fmt::color::red),
      "Exception:\n{}", error.what());
    return 1;
  }
  return 0;
}