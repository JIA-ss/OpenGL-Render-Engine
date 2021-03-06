#include "FileWatcherSystem.h"
#include <vector>
#include <functional>
#include <filesystem>
#include <iostream>

void FileWatcher::Init(const char* path, bool isRecursive)
{
    m_path = path;

	m_loop = (uv_loop_t*)malloc(sizeof(uv_loop_t));
	uv_loop_init(m_loop);

	m_fsEventHandler = (uv_fs_event_t*)malloc(sizeof(uv_fs_event_t));
	uv_fs_event_init(m_loop, m_fsEventHandler);

    uv_fs_event_start(m_fsEventHandler, FileChangeCallback, path, isRecursive ? UV_FS_EVENT_RECURSIVE : 0);
}

void FileWatcher::Update()
{
    if (m_loop)
    {
        uv_run(m_loop, UV_RUN_NOWAIT);
    }
}

void FileWatcher::FileChangeCallback(uv_fs_event_t* handle, const char* fileName, int events, int status)
{
    std::filesystem::path p = std::filesystem::absolute(handle->path);
    std::string rootPath = p.string();
    FileWatcher* watcher = FileWatcherSystem::Get()->GetFileWatcher(rootPath.c_str());
    if (!watcher)
    {
        return;
    }
    std::string path_str = (p / fileName).string();
    if (events & UV_RENAME)
    {
        if (std::filesystem::exists(p / fileName))
        {
            watcher->InvokeCreateCallbacks(path_str, fileName);
        }
        else
        {
            watcher->InvokeDeleteCallbacks(path_str, fileName);
        }
    }
    else if (events & UV_CHANGE)
    {
        watcher->InvokeModifyCallbacks(path_str, fileName);
    }
}

void FileWatcher::InvokeCreateCallbacks(const std::string& path, const std::string& fileName)
{
    for (auto&& func : m_createFileCallback)
    {   
        func(path, fileName);
    }
}
void FileWatcher::InvokeDeleteCallbacks(const std::string& path, const std::string& fileName)
{
    for (auto&& func : m_deleteFileCallback)
    {   
        func(path, fileName);
    }
}
void FileWatcher::InvokeModifyCallbacks(const std::string& path, const std::string& fileName)
{
    for (auto&& func : m_modifyFileCallback)
    {   
        func(path, fileName);
    }
}

FileWatcher::~FileWatcher()
{
    if(m_loop)
    {
        uv_fs_event_stop(m_fsEventHandler);
		uv_loop_close(m_loop);
        free(m_loop);
		free(m_fsEventHandler);

        m_loop = nullptr;
		m_fsEventHandler = nullptr;
    }
}


void FileWatcherSystem::Init()
{
    //CreateFileWatcher(_RESOURCE_PATH_, true);
}

void FileWatcherSystem::UnInit()
{
    for(auto&&[path, watcher] : m_watchers)
    {
        if (watcher)
        {
            delete watcher;
        }
    }
    m_watchers.clear();
}

void FileWatcherSystem::Update()
{
    for (auto&& wPair : m_watchers)
    {
        if (wPair.second)
        {
            wPair.second->Update();
        }
    }
}


FileWatcher* FileWatcherSystem::CreateFileWatcher(const char* path, bool isRecursive)
{
    auto it = m_watchers.find(path);
    if (it == m_watchers.end())
    {
        FileWatcher* watcher = new FileWatcher();
        watcher->Init(std::filesystem::absolute(path).string().c_str(), isRecursive);
        std::cout << "CreateFileWatcher: " << path << std::endl;
        m_watchers[path] = watcher;
        return watcher;
    }
    else
    {
        return it->second;
    }
}

void FileWatcherSystem::DeleteFileWatcher(const char* path)
{
    auto it = m_watchers.find(path);
    if (it != m_watchers.end())
    {
        FileWatcher* watcher = it->second;
        delete watcher;
        watcher = nullptr;
        m_watchers.erase(it);
    }
}

FileWatcher* FileWatcherSystem::GetFileWatcher(const char* path)
{
    auto it = m_watchers.find(path);
    if (it == m_watchers.end())
    {
        return nullptr;
    }
    return it->second;
}