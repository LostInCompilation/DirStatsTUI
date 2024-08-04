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
/*  FileSystem.hpp                                                  */
/*  Created: 30.07.2024                                             */
/*------------------------------------------------------------------*/

#ifndef FileSystem_hpp
#define FileSystem_hpp

class FileSystem
{
public:
    using Path = std::filesystem::path;
    
    struct DirectoryEntry
    {
        Path                            path = "";
        uintmax_t                       fileSize = 0;
        std::filesystem::file_time_type lastWriteTime = std::filesystem::file_time_type();
        std::filesystem::perms          permissions = std::filesystem::perms::none;
        
        bool        exists = false; // Can stay false for broken symbolic links
        bool        isDirectory = false;
        bool        isSymbolicLink = false;
        bool        isRegularFile = false;
    };
    
private:
    Error   m_LastError;
    
    template<typename IteratorType>
    bool IterateDirectoryT(const Path& path, std::vector<DirectoryEntry>& out_iteratedDirectoryInfo);
    
    void DebugPrintDirectoryEntry(const DirectoryEntry& entry);
    
public:
    FileSystem() = default;
    //~FileSystem();
    
    Error   GetLastError() const noexcept { return m_LastError; }
    
    bool    GetSpaceInfo(const Path& path, uintmax_t& out_capacity, uintmax_t& out_free, uintmax_t& out_available) noexcept;
    
    bool    IterateDirectory(const Path& path, std::vector<DirectoryEntry>& out_iteratedDirectoryInfo); // May throw std::bad_alloc
    bool    IterateDirectoryRecursively(const Path& path, std::vector<DirectoryEntry>& out_iteratedDirectoryInfo); // May throw std::bad_alloc
    
    bool    GetSizesOfDirectoryRecursively(const Path& path, std::unordered_map<Path, uintmax_t>& out_directorySizes, uintmax_t& out_totalSize);
};

#endif /* FileSystem_hpp */
