// Copyright 2024 DreamWorks Animation LLC
// SPDX-License-Identifier: Apache-2.0

// Windows stub implementation of AffinityManager
// NUMA and CPU affinity APIs are Linux-specific, so we provide no-op stubs on Windows

#ifdef _WIN32

#include "AffinityManager.h"

namespace moonray {
namespace mcrt_common {

//-----------------------------------------------------------------------------
// MemoryNode stubs
//-----------------------------------------------------------------------------

MemoryNode::MemoryNode(const unsigned activeThreadCount)
    : mActiveThreadCount(activeThreadCount)
{
    mArenaBlockPool = scene_rdl2::util::Ref<scene_rdl2::alloc::ArenaBlockPool>(
        new scene_rdl2::alloc::ArenaBlockPool());
}

std::string MemoryNode::show() const
{
    return "MemoryNode (Windows stub - no NUMA support)";
}

std::string MemoryNode::showNumaNodeId(const unsigned numaNodeId)
{
    return "NUMA N/A";
}

//-----------------------------------------------------------------------------
// MemoryAffinityManager stubs
//-----------------------------------------------------------------------------

MemoryAffinityManager::MemoryAffinityManager(const std::string& memAffinityDef)
    : mMemAffinityDef(memAffinityDef)
    , mMemAffinityEnable(false)
{
}

bool MemoryAffinityManager::init(const std::shared_ptr<const CpuAffinityManager>& cpuAff)
{
    // Create a single global memory node for all threads
    unsigned threadCount = cpuAff ? cpuAff->getNumThreads() : 1;
    mMemGlobal = std::make_shared<MemoryNode>(threadCount);
    mMemAffinityEnable = false;
    return true;
}

unsigned MemoryAffinityManager::getActiveNumaNodeCount() const
{
    return 1; // Single "node" on Windows
}

void MemoryAffinityManager::setupLogInfo(std::vector<std::string>& titleTbl,
                                          std::vector<std::string>& msgTbl,
                                          const std::shared_ptr<const CpuAffinityManager>& cpuAff) const
{
    titleTbl.push_back("Memory Affinity");
    msgTbl.push_back("Disabled (Windows)");
}

std::string MemoryAffinityManager::show() const
{
    return "MemoryAffinityManager (Windows stub - NUMA affinity not supported)";
}

std::string MemoryAffinityManager::showNumaUtil() const { return ""; }
std::string MemoryAffinityManager::showMemGlobal() const { return ""; }
std::string MemoryAffinityManager::showMemNodeTbl() const { return ""; }
std::string MemoryAffinityManager::showMcrtThreadIdToMemNodeIdTbl() const { return ""; }

//-----------------------------------------------------------------------------
// CpuAffinityManager stubs
//-----------------------------------------------------------------------------

CpuAffinityManager::CpuAffinityManager(const unsigned desiredNumThreads,
                                       const std::string& cpuAffinityDef,
                                       const std::string& socketAffinityDef)
    : mNumThreads(desiredNumThreads)
    , mCpuAffinityDef(cpuAffinityDef)
    , mSocketAffinityDef(socketAffinityDef)
    , mEnableRenderPrepCpuAffinity(false)
    , mEnableMcrtCpuAffinity(false)
    , mEnableMcrtCpuAffinityAll(true)
{
    // Populate CPU ID table with sequential IDs
    for (unsigned i = 0; i < mNumThreads; ++i) {
        mCpuIdTbl.push_back(i);
    }
    mRenderPrepMessage = "CPU affinity disabled (Windows)";
    mMcrtMessage = "CPU affinity disabled (Windows)";
}

bool CpuAffinityManager::doRenderPrepCpuAffinity(std::string& msg)
{
    msg = "CPU affinity not supported on Windows";
    return true;
}

void CpuAffinityManager::setupLogInfo(std::vector<std::string>& titleTbl,
                                       std::vector<std::string>& msgTbl) const
{
    titleTbl.push_back("CPU Affinity");
    msgTbl.push_back("Disabled (Windows)");
}

std::string CpuAffinityManager::show() const
{
    return "CpuAffinityManager (Windows stub - CPU affinity not supported)";
}

void CpuAffinityManager::configureCpuAffinity()
{
    // No-op on Windows
}

//-----------------------------------------------------------------------------
// AffinityManager stubs
//-----------------------------------------------------------------------------

// Static member
static std::shared_ptr<AffinityManager> sAffinityManager;

AffinityManager::AffinityManager(const unsigned desiredNumThreads,
                                 const std::string& cpuAffinityDef,
                                 const std::string& socketAffinityDef,
                                 const std::string& memAffinityDef)
{
    mCpuManager = std::make_shared<CpuAffinityManager>(desiredNumThreads, cpuAffinityDef, socketAffinityDef);
    mMemManager = std::make_shared<MemoryAffinityManager>(memAffinityDef);
    mMemManager->init(mCpuManager);
    parserConfigure();
}

bool AffinityManager::init(const unsigned desiredNumThreads,
                           const std::string& autoAffinityDef,
                           const std::string& cpuAffinityDef,
                           const std::string& socketAffinityDef,
                           const std::string& memAffinityDef)
{
    sAffinityManager = std::make_shared<AffinityManager>(
        desiredNumThreads, cpuAffinityDef, socketAffinityDef, memAffinityDef);
    return true;
}

std::shared_ptr<AffinityManager> AffinityManager::get()
{
    return sAffinityManager;
}

void AffinityManager::setupLogInfo(std::vector<std::string>& titleTbl,
                                   std::vector<std::string>& msgTbl) const
{
    titleTbl.push_back("Affinity");
    msgTbl.push_back("Disabled (Windows - no NUMA/CPU affinity support)");
}

std::string AffinityManager::show() const
{
    return "AffinityManager (Windows stub)";
}

std::string AffinityManager::showTbl(const std::string& msg, const std::vector<unsigned>& tbl)
{
    std::string result = msg + ": [";
    for (size_t i = 0; i < tbl.size(); ++i) {
        if (i > 0) result += ", ";
        result += std::to_string(tbl[i]);
    }
    result += "]";
    return result;
}

void AffinityManager::calcAutoAffinityOptions(const unsigned desiredNumThreads,
                                              std::string& cpuAffinityDef,
                                              std::string& memAffinityDef)
{
    cpuAffinityDef = "false";
    memAffinityDef = "false";
}

void AffinityManager::parserConfigure()
{
    // Basic parser setup - no-op for Windows
}

} // namespace mcrt_common
} // namespace moonray

#endif // _WIN32
