#include <filesystem>

#ifdef _WIN32
#include <Windows.h>
#endif

std::string relpath(std::string path)
{
#ifdef _WIN32
  char buffer[MAX_PATH];
  GetModuleFileNameA(NULL, buffer, MAX_PATH);
  return std::filesystem::path(buffer).replace_filename(path).string();
#elif __linux__
  return std::filesystem::canonical("/proc/self/exe").replace_filename(path).string();
#endif
}