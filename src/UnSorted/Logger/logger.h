#pragma once

#include <filesystem>
#include <fstream>
#include <map>
#include <set>
#include <string>

namespace fs = std::filesystem;

class Logger {
private:
    std::set<std::string> _openned_files;
    std::map<std::string, std::ofstream> _ofstreams;

    Logger();
    ~Logger();

    void IPrintTime(std::ostream& out);
    void IPrintValue(std::ostream& out, const int& value);

    void IOpen(const fs::path& path_to_file, const std::string& log_name);
    void IClose(const std::string& log_name);
    void IWrite(const std::string& log_name, const std::string& author, const std::string& massage, const std::string& message_type);
    void IError(const std::string& log_name, const std::string& author, const std::string& massage);
    void IInfo(const std::string& log_name, const std::string& author, const std::string& massage);
    void IStage(const std::string& log_name, const std::string& author, const std::string& massage);
    void IWarning(const std::string& log_name, const std::string& author, const std::string& massage);
    void IFatal(const std::string& log_name, const std::string& author, const std::string& massage);

public:
    static Logger& Get();

    static void Open(const fs::path& path_to_file, const std::string& log_name);
    static void Close(const std::string& log_name);
    static void Write(const std::string& log_name, const std::string& author, const std::string& massage, const std::string& message_type);
    static void Error(const std::string& log_name, const std::string& author, const std::string& massage);
    static void Info(const std::string& log_name, const std::string& author, const std::string& massage);
    static void Stage(const std::string& log_name, const std::string& author, const std::string& massage);
    static void Warning(const std::string& log_name, const std::string& author, const std::string& massage);
    static void Fatal(const std::string& log_name, const std::string& author, const std::string& massage);
};