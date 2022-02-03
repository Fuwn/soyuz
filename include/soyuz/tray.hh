// Copyright (C) 2021-2021 Fuwn
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

#define LOG(message) logs.emplace_back(message);

LRESULT CALLBACK WindowProcedure(HWND, UINT, WPARAM, LPARAM);
auto minimize() -> void;
auto restore() -> void;
auto InitNotifyIconData() -> void;

namespace soyuz {

auto log(const std::string &message) -> void;

}

#endif //SOYUZ_TRAY_HH
