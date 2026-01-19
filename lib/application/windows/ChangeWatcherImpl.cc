// Copyright 2023-2024 DreamWorks Animation LLC
// SPDX-License-Identifier: Apache-2.0

// Windows stub implementation of ChangeWatcher
// TODO: Implement using ReadDirectoryChangesW for real file watching

#include "../ChangeWatcher.h"

#include <windows.h>
#include <set>
#include <string>

namespace moonray {

class ChangeWatcherWindows : public ChangeWatcher
{
public:
    ChangeWatcherWindows() {}
    virtual ~ChangeWatcherWindows() {}

    void watchFile(const std::string& /*filePath*/) override
    {
        // Stub: file watching not implemented on Windows
        // Files will not be automatically reloaded when modified
    }

    bool hasChanged(std::set<std::string>* /*changedFiles*/ = nullptr) override
    {
        // Stub: always return false - no file change detection
        return false;
    }

    void waitForChange() override
    {
        // Stub: just sleep briefly to avoid busy-waiting
        // In a real implementation, this would block until a file changes
        Sleep(100);
    }
};

ChangeWatcher* ChangeWatcher::CreateChangeWatcher()
{
    return new ChangeWatcherWindows();
}

} // namespace moonray
