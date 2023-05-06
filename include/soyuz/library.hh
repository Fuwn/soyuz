// Copyright (C) 2021-2023 Fuwn
// SPDX-License-Identifier: GPL-3.0-only

#ifndef SOYUZ_LIBRARY_HH
#define SOYUZ_LIBRARY_HH
#pragma once

#include <Windows.h>
#include <string>
#include <vector>


#define NT_SUCCESS(status) (status >= 0)
#define STATUS_INFO_LENGTH_MISMATCH ((NTSTATUS)0xC0000004L)

namespace soyuz {

const std::string numbers_as_string[] = {
    "zero", "one", "two", "three", "four",
};

enum log_level {
  LOG_LEVEL_TRACE,
  LOG_LEVEL_DEBUG,
  LOG_LEVEL_INFO,
  LOG_LEVEL_WARN,
  LOG_LEVEL_ERROR,
};

struct log_t {
  log_level level;
  std::string value;

  log_t(log_level level, std::string value)
      : level(level), value(std::move(value)) {}

  [[nodiscard]] auto to_colorref() const -> COLORREF;
};

static auto CALLBACK enum_windows_proc(HWND, LPARAM) -> BOOL;
auto find_lunar() -> DWORD;
auto delete_handle(DWORD) -> int;
auto write_log_file(const std::string &) -> void;
auto init_log_file() -> void;
auto close_log_file() -> void;
auto exit(int) -> void;
// https://stackoverflow.com/a/10467633/14452787
auto current_date_time() -> std::string;

} // namespace soyuz

#endif // SOYUZ_LIBRARY_HH
