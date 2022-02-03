// Copyright (C) 2021-2021 Fuwn
// SPDX-License-Identifier: GPL-3.0-only

/**
 * @file   library.hh
 * @author Fuwn
 * @date   2021. August. 18.
 */

#ifndef SOYUZ_LIBRARY_HH
#define SOYUZ_LIBRARY_HH
#pragma once

#include <string>
#include <Windows.h>

#define NT_SUCCESS(status) (status >= 0)
#define STATUS_INFO_LENGTH_MISMATCH ((NTSTATUS)0xC0000004L)

namespace soyuz {

// enum log_level {
//   low,    // blue
//   medium, // orange
//   high,   // red
// };

static BOOL CALLBACK enum_windows_proc(HWND, LPARAM);
auto find_lunar() -> DWORD;
auto delete_handle(DWORD) -> int;
auto write_log_file(const std::string &) -> void;
auto init_log_file() -> void;
auto close_log_file() -> void;
auto exit(int) -> void;
// https://stackoverflow.com/a/10467633/14452787
auto current_date_time() -> std::string;

}

#endif //SOYUZ_LIBRARY_HH
