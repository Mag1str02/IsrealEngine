#include "DummyEditor/Scripting/Compiler.h"

namespace DE {
    class LinuxCompilerImpl : public CompilerImpl {
        LOGGER_AUTHOR(Compiler)
    public:
        LinuxCompilerImpl() { AddDefine("DE_PLATFORM_LINUX"); }
        virtual bool Compile(const Path& source, const Path& destination) {
            if (!fs::exists(source) || destination.filename().empty()) {
                LOG_ERROR("File {} was not compiled because does not exists or destination empty", source);
                return false;
            }

            Path directory = destination;
            directory.remove_filename();
            FileSystem::CreateDirectory(directory);

            std::string compile_command;
            compile_command.append(GetCompiler());
            compile_command.append(" -c -std=c++20");
            compile_command.append(" -fPIC ");
            compile_command.append(AddDefines());
            compile_command.append(AddIncludeDirArguments());
            compile_command.append(AddSourceArgument(source));
            compile_command.append(AddDestinationArgument(source, destination));
            FixSlash(compile_command);

            int res = system(compile_command.c_str());
            return res == 0;
        }
        virtual bool Link(const std::vector<Path>& sources, const Path& destination, const std::string& library_name) {
            if (library_name.empty()) {
                return false;
            }
            if (!fs::is_directory(destination)) {
                return false;
            }
            for (const auto& source : sources) {
                if (!fs::exists(source)) {
                    return false;
                }
            }
            FileSystem::CreateDirectory(destination);

            std::string link_command;
            link_command.append(GetCompiler());
            link_command.append(" -shared");
            link_command.append(AddLinkArgs());
            link_command.append(AddSourcesArguments(sources));
            link_command.append(AddPathDLL(destination, library_name));
            FixSlash(link_command);

            int res = system(link_command.c_str());
            return res == 0;
        }
        virtual void AddIncludeDir(const Path& dir) { m_IncludeDirs.insert(dir); }
        virtual void DeleteIncludeDir(const Path& dir) { m_IncludeDirs.erase(dir); }
        virtual void AddLinkLibrary(const Path& library) { m_Libraries.insert(library); }
        virtual void DeleteLinkLibrary(const Path& library) { m_Libraries.erase(library); }
        virtual void AddDefine(const std::string& define) { m_Defines.insert(define); }
        virtual void DeleteDefine(const std::string& define) { m_Defines.erase(define); }

    private:
        std::string GetCompiler() {
            // TODO: Get compiler from config.
            return "clang++";
        }
        void FixSlash(std::string& command) { std::replace(command.begin(), command.end(), '\\', '/'); }

        std::string AddIncludeDirArguments() {
            std::string res;
            for (const auto& dir : m_IncludeDirs) {
                res.append(" -I ");
                res.append(dir.string());
            }
            return res;
        }
        std::string AddSourceArgument(const Path& source) { return " " + source.string(); }
        std::string AddDestinationArgument(const Path&, const Path& destination) { return " -o " + destination.string(); }

        std::string AddSourcesArguments(const std::vector<Path>& sources) {
            std::string res;
            for (const auto& source : sources) {
                res.append(" ");
                res.append(source.string());
            }
            return res;
        }
        std::string AddLinkArgs() {
            std::string res;
            for (auto lib : m_Libraries) {
                std::string name = lib.stem().string();
                lib.remove_filename();
                if (lib.empty()) {
                    lib = ".";
                }
                res.append(" -L ");
                res.append(lib.string());
                res.append(" -l ");
                res.append(name);
            }
            return res;
        }
        std::string AddDefines() {
            std::string res;
            for (const auto& def : m_Defines) {
                res.append(" -D");
                res.append(def);
            }
            return res;
        }
        std::string AddPathDLL(const Path& destination, const std::string& library_name) {
            return " -o " + destination.string() + "/" + library_name + ".so";
        }

        std::unordered_set<Path>        m_IncludeDirs;
        std::unordered_set<Path>        m_Libraries;
        std::unordered_set<std::string> m_Defines;
    };

    Scope<CompilerImpl> Compiler::CreateCompilerImpl() {
        return CreateScope<LinuxCompilerImpl>();
    }

};  // namespace DE