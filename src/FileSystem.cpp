/*
    The zlib License

    Copyright (C) 2024 Marc Schöndorf
 
This software is provided 'as-is', without any express or implied warranty. In
no event will the authors be held liable for any damages arising from the use of
this software.

Permission is granted to anyone to use this software for any purpose, including
commercial applications, and to alter it and redistribute it freely, subject to
the following restrictions:

1.  The origin of this software must not be misrepresented; you must not claim
    that you wrote the original software. If you use this software in a product,
    an acknowledgment in the product documentation would be appreciated but is
    not required.

2.  Altered source versions must be plainly marked as such, and must not be
    misrepresented as being the original software.

3.  This notice may not be removed or altered from any source distribution.
*/

/*------------------------------------------------------------------*/
/*                                                                  */
/*                      (C) 2024 Marc Schöndorf                     */
/*                            See license                           */
/*                                                                  */
/*  FileSystem.cpp                                                  */
/*  Created: 30.07.2024                                             */
/*------------------------------------------------------------------*/

#include "Main.hpp"

bool FileSystem::GetSpaceInfo(const Path& path, uintmax_t& out_capacity, uintmax_t& out_free, uintmax_t& out_available) noexcept
{
    std::filesystem::space_info spaceInfo = std::filesystem::space(path, m_LastError);
    
    out_capacity = spaceInfo.capacity;
    out_free = spaceInfo.free;
    out_available = spaceInfo.available;
    
    if(m_LastError)
        return false;
    
    return true;
}

template<typename IteratorType>
bool FileSystem::IterateDirectoryT(const Path& path, std::vector<DirectoryEntry>& out_iteratedDirectoryInfo)
{
    const std::filesystem::directory_options directoryOptions = std::filesystem::directory_options::skip_permission_denied;
    Path currentIteratedPath = "";
    
    for(std::filesystem::directory_entry i : IteratorType(path, directoryOptions, m_LastError)) // May throw std::bad_alloc
    {
        if(m_LastError)
            return false;
        
        DirectoryEntry dirEntry;
        dirEntry.path = i.path();
        
        // Does the file exist?
        dirEntry.exists = i.exists(m_LastError);
        if(m_LastError)
        {
            if(m_LastError.GetCode() != ENOENT) // Ignore "No such file or directory" error
                return false;
            
            m_LastError.Clear();
        }
        
        // Is it a symbolic link?
        dirEntry.isSymbolicLink = i.is_symlink(m_LastError);
        if(m_LastError)
            return false;
        
        if(dirEntry.exists)
        {
            // Get file stats for permissions
            std::filesystem::file_status status = i.status(m_LastError);
            if(m_LastError)
                return false;
            
            dirEntry.permissions = status.permissions();
            
            // Get last write time
            dirEntry.lastWriteTime = i.last_write_time(m_LastError);
            if(m_LastError)
                return false;
            
            // Is it a directory?
            dirEntry.isDirectory = i.is_directory(m_LastError);
            if(m_LastError)
                return false;
            
            // Is it a regular file?
            dirEntry.isRegularFile = i.is_regular_file(m_LastError);
            if(m_LastError)
                return false;
            
            // Get file size if not a directory
            if(!dirEntry.isDirectory && dirEntry.isRegularFile)
            {
                dirEntry.fileSize = i.file_size(m_LastError);
                if(m_LastError)
                    return false;
            }
        }
        
        // Save entry
        out_iteratedDirectoryInfo.push_back(dirEntry);
        
        // Debug print
        //DebugPrintDirectoryEntry(dirEntry);
    }
    
    return true;
}

bool FileSystem::IterateDirectory(const Path& path, std::vector<DirectoryEntry>& out_iteratedDirectoryInfo)
{
    return IterateDirectoryT<std::filesystem::directory_iterator>(path, out_iteratedDirectoryInfo);
}

bool FileSystem::IterateDirectoryRecursively(const Path& path, std::vector<DirectoryEntry>& out_iteratedDirectoryInfo)
{
    return IterateDirectoryT<std::filesystem::recursive_directory_iterator>(path, out_iteratedDirectoryInfo);
}

bool FileSystem::GetSizesOfDirectoryRecursively(const Path& path, std::unordered_map<Path, DirectoryStats>& out_directorySizes, uintmax_t& out_totalSize)
{
    out_totalSize = 0;
    
    // First get all elements in the requested path
    std::vector<DirectoryEntry> currentPathElements;
    if(!IterateDirectory(path, currentPathElements))
        return false;
    
    uintmax_t totalSubDirSize = 0;
    std::vector<DirectoryEntry> subDirElements;
    
    // Iterate all elements of the requested path
    for(DirectoryEntry& i : currentPathElements)
    {
        // Iterate element recursively if it is a directory
        if(i.isDirectory)
        {
            if(!IterateDirectoryRecursively(i.path, subDirElements))
                return false;
            
            // Sum up all sizes
            for(DirectoryEntry& sub : subDirElements)
            {
                if(sub.fileSize != UINTMAX_MAX)
                    totalSubDirSize += sub.fileSize;
            }
            
            // Add size of directory to unordered_map
            DirectoryStats stats;
            stats.isDirectory = true;
            stats.size = totalSubDirSize;
            stats.count = subDirElements.size();
            out_directorySizes[i.path] = stats;
            
            out_totalSize += totalSubDirSize;
            
            totalSubDirSize = 0;
            subDirElements.clear();
        }
        else
        {
            // Add size of file to unordered_map
            DirectoryStats stats;
            stats.isDirectory = false;
            stats.size = i.fileSize;
            stats.count = 1;
            out_directorySizes[i.path] = stats;
            
            out_totalSize += i.fileSize;
        }
        
        // Debug print
        std::cout << i.path << ": " << out_directorySizes[i.path].size << "B (count: " << out_directorySizes[i.path].count << ")" << std::endl;
    }
    
    return true;
}

#ifndef NDEBUG
void FileSystem::DebugPrintDirectoryEntry(const DirectoryEntry& entry)
{
    // Lambda to print permission
    auto printPermissions = [](const DirectoryEntry& entry)
    {
        // Helper lambda
        auto printOnePermission = [&entry](const std::filesystem::perms permissionToCheck, const char associatedPermissionLetter)
        {
            std::cout << ((entry.permissions & permissionToCheck) != std::filesystem::perms::none ? associatedPermissionLetter : '-');
        };
        
        // Owner
        printOnePermission(std::filesystem::perms::owner_read, 'r');
        printOnePermission(std::filesystem::perms::owner_write, 'w');
        printOnePermission(std::filesystem::perms::owner_exec, 'x');
        std::cout << " ";
        
        // Group
        printOnePermission(std::filesystem::perms::group_read, 'r');
        printOnePermission(std::filesystem::perms::group_write, 'w');
        printOnePermission(std::filesystem::perms::group_exec, 'x');
        std::cout << " ";
        
        // Others
        printOnePermission(std::filesystem::perms::others_read, 'r');
        printOnePermission(std::filesystem::perms::others_write, 'w');
        printOnePermission(std::filesystem::perms::others_exec, 'x');
    };
    
    // Print
    printPermissions(entry);
    std::cout << " " << std::setw(9) << entry.fileSize << " B  "
        << std::format("{:%d-%b-%Y %T}", std::chrono::floor<std::chrono::seconds>(entry.lastWriteTime)) << "  "
        << "[Exist: " << entry.exists << "] "
        << "[Sym link: " << entry.isSymbolicLink << "] "
        << "[Dir: " << entry.isDirectory << "] "
        << "[Regular: " << entry.isRegularFile << "]  "
        << entry.path
        << std::endl;
}
#endif
