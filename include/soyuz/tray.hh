// Copyright (C) 2021-2023 Fuwn
// SPDX-License-Identifier: GPL-3.0-only

#ifndef SOYUZ_TRAY_HH
#define SOYUZ_TRAY_HH
#pragma once

#include <Windows.h>
#include <string>
#include <vector>


#include <soyuz/library.hh>

auto CALLBACK WindowProcedure(HWND, UINT, WPARAM, LPARAM) -> LRESULT;
auto minimize() -> void;
auto restore() -> void;
auto InitNotifyIconData() -> void;

namespace soyuz {

[[maybe_unused]] auto log(const std::string &) -> void;
auto log(log_level, const std::string &) -> void;

} // namespace soyuz

#endif // SOYUZ_TRAY_HH
