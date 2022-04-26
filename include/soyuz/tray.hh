// Copyright (C) 2021-2022 Fuwn
// SPDX-License-Identifier: GPL-3.0-only

/**
 * @file   tray.hh
 * @author Fuwn
 * @date   2021. August. 18.
 */

#ifndef SOYUZ_TRAY_HH
#define SOYUZ_TRAY_HH
#pragma once

#include <string>
#include <vector>
#include <Windows.h>

#include <soyuz/library.hh>

auto CALLBACK WindowProcedure(HWND, UINT, WPARAM, LPARAM) -> LRESULT;
auto minimize() -> void;
auto restore() -> void;
auto InitNotifyIconData() -> void;

namespace soyuz {

[[maybe_unused]] auto log(const std::string &) -> void;
auto log(log_level, const std::string &) -> void;

}

#endif // SOYUZ_TRAY_HH
