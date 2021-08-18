// Copyright (C) 2021-2021 Fuwn
// SPDX-License-Identifier: GPL-3.0-only

#ifndef SOYUZ_LIBRARY_HH
#define SOYUZ_LIBRARY_HH

#pragma once

#include <Windows.h>

#define NT_SUCCESS(status) (status >= 0)
#define STATUS_INFO_LENGTH_MISMATCH ((NTSTATUS)0xC0000004L)

static auto enum_windows_proc(HWND hwnd, LPARAM lparam) -> BOOL;
auto find_lunar() -> DWORD;
auto delete_handle(DWORD pid) -> int;

#endif //SOYUZ_LIBRARY_HH
