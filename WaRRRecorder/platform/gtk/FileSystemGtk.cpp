/*
 * Copyright (C) 2007, 2009 Holger Hans Peter Freyther
 * Copyright (C) 2008 Collabora, Ltd.
 * Copyright (C) 2008 Apple Inc. All rights reserved.
 * Portions Copyright (c) 2010 Motorola Mobility, Inc.  All rights reserved.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

#include "config.h"
#include "FileSystem.h"

#include "GOwnPtr.h"
#include "PlatformString.h"
#include "UUID.h"
#include <gio/gio.h>
#include <glib.h>
#include <glib/gstdio.h>
#include <wtf/gobject/GRefPtr.h>
#include <wtf/text/CString.h>

namespace WebCore {

/* On linux file names are just raw bytes, so also strings that cannot be encoded in any way
 * are valid file names. This mean that we cannot just store a file name as-is in a String
 * but we have to escape it.
 * On Windows the GLib file name encoding is always UTF-8 so we can optimize this case. */
String filenameToString(const char* filename)
{
    if (!filename)
        return String();

#if OS(WINDOWS)
    return String::fromUTF8(filename);
#else
    gchar* escapedString = g_uri_escape_string(filename, "/:", false);
    String string(escapedString);
    g_free(escapedString);
    return string;
#endif
}

CString fileSystemRepresentation(const String& path)
{
#if OS(WINDOWS)
    return path.utf8();
#else
    char* filename = g_uri_unescape_string(path.utf8().data(), 0);
    CString cfilename(filename);
    g_free(filename);
    return cfilename;
#endif
}

// Converts a string to something suitable to be displayed to the user.
String filenameForDisplay(const String& string)
{
#if OS(WINDOWS)
    return string;
#else
    CString filename = fileSystemRepresentation(string);
    gchar* display = g_filename_to_utf8(filename.data(), 0, 0, 0, 0);
    if (!display)
        return string;

    String displayString = String::fromUTF8(display);
    g_free(display);

    return displayString;
#endif
}

bool fileExists(const String& path)
{
    bool result = false;
    CString filename = fileSystemRepresentation(path);

    if (!filename.isNull())
        result = g_file_test(filename.data(), G_FILE_TEST_EXISTS);

    return result;
}

bool deleteFile(const String& path)
{
    bool result = false;
    CString filename = fileSystemRepresentation(path);

    if (!filename.isNull())
        result = g_remove(filename.data()) == 0;

    return result;
}

bool deleteEmptyDirectory(const String& path)
{
    bool result = false;
    CString filename = fileSystemRepresentation(path);

    if (!filename.isNull())
        result = g_rmdir(filename.data()) == 0;

    return result;
}

bool getFileSize(const String& path, long long& resultSize)
{
    CString filename = fileSystemRepresentation(path);
    if (filename.isNull())
        return false;

    struct stat statResult;
    gint result = g_stat(filename.data(), &statResult);
    if (result != 0)
        return false;

    resultSize = statResult.st_size;
    return true;
}

bool getFileModificationTime(const String& path, time_t& modifiedTime)
{
    CString filename = fileSystemRepresentation(path);
    if (filename.isNull())
        return false;

    struct stat statResult;
    gint result = g_stat(filename.data(), &statResult);
    if (result != 0)
        return false;

    modifiedTime = statResult.st_mtime;
    return true;

}

String pathByAppendingComponent(const String& path, const String& component)
{
    if (path.endsWith(G_DIR_SEPARATOR_S))
        return path + component;
    else
        return path + G_DIR_SEPARATOR_S + component;
}

bool makeAllDirectories(const String& path)
{
    CString filename = fileSystemRepresentation(path);
    if (filename.isNull())
        return false;

    gint result = g_mkdir_with_parents(filename.data(), S_IRWXU);

    return result == 0;
}

String homeDirectoryPath()
{
    return filenameToString(g_get_home_dir());
}

String pathGetFileName(const String& pathName)
{
    if (pathName.isEmpty())
        return pathName;

    CString tmpFilename = fileSystemRepresentation(pathName);
    char* baseName = g_path_get_basename(tmpFilename.data());
    String fileName = String::fromUTF8(baseName);
    g_free(baseName);

    return fileName;
}

CString applicationDirectoryPath()
{
#if OS(LINUX)
    // Handle the /proc filesystem case.
    char pathFromProc[PATH_MAX] = {0};
    if (readlink("/proc/self/exe", pathFromProc, sizeof(pathFromProc) - 1) == -1)
        return CString();

    GOwnPtr<char> dirname(g_path_get_dirname(pathFromProc));
    return dirname.get();
#elif OS(UNIX)
    // If the above fails, check the PATH env variable.
    GOwnPtr<char> currentExePath(g_find_program_in_path(g_get_prgname()));
    if (!currentExePath.get())
        return CString();

    GOwnPtr<char> dirname(g_path_get_dirname(currentExePath.get()));
    return dirname.get();
#else
    return CString();
#endif
}

String directoryName(const String& path)
{
    /* No null checking needed */
    GOwnPtr<char> dirname(g_path_get_dirname(fileSystemRepresentation(path).data()));
    return String::fromUTF8(dirname.get());
}

Vector<String> listDirectory(const String& path, const String& filter)
{
    Vector<String> entries;

    CString filename = fileSystemRepresentation(path);
    GDir* dir = g_dir_open(filename.data(), 0, 0);
    if (!dir)
        return entries;

    GPatternSpec *pspec = g_pattern_spec_new((filter.utf8()).data());
    while (const char* name = g_dir_read_name(dir)) {
        if (!g_pattern_match_string(pspec, name))
            continue;

        GOwnPtr<gchar> entry(g_build_filename(filename.data(), name, NULL));
        entries.append(filenameToString(entry.get()));
    }
    g_pattern_spec_free(pspec);
    g_dir_close(dir);

    return entries;
}

CString openTemporaryFile(const char* prefix, PlatformFileHandle& handle)
{
    GOwnPtr<gchar> filename(g_strdup_printf("%s%s", prefix, createCanonicalUUIDString().utf8().data()));
    GOwnPtr<gchar> tempPath(g_build_filename(g_get_tmp_dir(), filename.get(), NULL));
    GRefPtr<GFile> file = adoptGRef(g_file_new_for_path(tempPath.get()));

    handle = g_file_create_readwrite(file.get(), G_FILE_CREATE_NONE, 0, 0);
    if (!isHandleValid(handle))
        return CString();
    return tempPath.get();
}

PlatformFileHandle openFile(const String& path, FileOpenMode mode)
{
    CString fsRep = fileSystemRepresentation(path);
    if (fsRep.isNull())
        return invalidPlatformFileHandle;

    GRefPtr<GFile> file = adoptGRef(g_file_new_for_path(fsRep.data()));
    GFileIOStream* ioStream = 0;
    if (mode == OpenForRead)
        ioStream = g_file_open_readwrite(file.get(), 0, 0);
    else if (mode == OpenForWrite) {
        if (g_file_test(fsRep.data(), static_cast<GFileTest>(G_FILE_TEST_EXISTS | G_FILE_TEST_IS_REGULAR)))
            ioStream = g_file_open_readwrite(file.get(), 0, 0);
        else
            ioStream = g_file_create_readwrite(file.get(), G_FILE_CREATE_NONE, 0, 0);
    }

    return ioStream;
}

void closeFile(PlatformFileHandle& handle)
{
    if (!isHandleValid(handle))
        return;

    g_io_stream_close(G_IO_STREAM(handle), 0, 0);
    g_object_unref(handle);
    handle = invalidPlatformFileHandle;
}

long long seekFile(PlatformFileHandle handle, long long offset, FileSeekOrigin origin)
{
    GSeekType seekType = G_SEEK_SET;
    switch (origin) {
    case SeekFromBeginning:
        seekType = G_SEEK_SET;
        break;
    case SeekFromCurrent:
        seekType = G_SEEK_CUR;
        break;
    case SeekFromEnd:
        seekType = G_SEEK_END;
        break;
    default:
        ASSERT_NOT_REACHED();
    }

    if (!g_seekable_seek(G_SEEKABLE(g_io_stream_get_input_stream(G_IO_STREAM(handle))),
                         offset, seekType, 0, 0))
        return -1;
    return g_seekable_tell(G_SEEKABLE(g_io_stream_get_input_stream(G_IO_STREAM(handle))));
}

int writeToFile(PlatformFileHandle handle, const char* data, int length)
{
    gsize bytesWritten;
    g_output_stream_write_all(g_io_stream_get_output_stream(G_IO_STREAM(handle)),
                              data, length, &bytesWritten, 0, 0);
    return bytesWritten;
}

int readFromFile(PlatformFileHandle handle, char* data, int length)
{
    GOwnPtr<GError> error;
    do {
        gssize bytesRead = g_input_stream_read(g_io_stream_get_input_stream(G_IO_STREAM(handle)),
                                               data, length, 0, &error.outPtr());
        if (bytesRead >= 0)
            return bytesRead;
    } while (error && error->code == G_FILE_ERROR_INTR);
    return -1;
}

bool unloadModule(PlatformModule module)
{
    return g_module_close(module);
}
}
