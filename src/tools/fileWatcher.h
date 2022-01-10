#ifndef _FILE_WATCHER_
#define _FILE_WATCHER_
#include "uv.h"
#include "common/def.h"
#include <map>
#include <string>
#include <vector>
#include <functional>


namespace Util
{
class FileWatcher;
class FileWatcherManager
{
    DECLARE_SINGLETON(FileWatcherManager)
private:
    std::map<std::string, FileWatcher*> m_watchers;
public:
    FileWatcher* CreateFileWatcher(const char* path, bool isRecursive);
    FileWatcher* GetFileWatcher(const char* path);
    void DeleteFileWatcher(const char* path);
public:
    void Update();
};

class FileWatcher
{
private:
    uv_loop_t* m_loop = nullptr;
    uv_fs_event_t* m_fsEventHandler = nullptr;
private:
    std::vector<std::function<void(const std::string&, const std::string&)>> m_createFileCallback;
    std::vector<std::function<void(const std::string&, const std::string&)>> m_deleteFileCallback;
    std::vector<std::function<void(const std::string&, const std::string&)>> m_modifyFileCallback;
    std::string m_path;
private:
    static void FileChangeCallback(uv_fs_event_t* handle, const char* fileName, int events, int status);
    void InvokeCreateCallbacks(const std::string&, const std::string&);
    void InvokeDeleteCallbacks(const std::string&, const std::string&);
    void InvokeModifyCallbacks(const std::string&, const std::string&);
public:
    ~FileWatcher();
    void Init(const char* path, bool isRecursive);
    void Update();
public:
    inline void AddCreateFileCallback(std::function<void(const std::string&, const std::string&)> func) { m_createFileCallback.emplace_back(func); }
    inline void AddDeleteFileCallback(std::function<void(const std::string&, const std::string&)> func) { m_deleteFileCallback.emplace_back(func); }
    inline void AddModifyFileCallback(std::function<void(const std::string&, const std::string&)> func) { m_modifyFileCallback.emplace_back(func); }
};


}

#endif //_FILE_WATCHER_