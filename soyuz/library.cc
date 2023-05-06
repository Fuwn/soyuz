// Copyright (C) 2021-2023 Fuwn
// SPDX-License-Identifier: GPL-3.0-only

#include <soyuz/library.hh>

#include <Windows.h>
#include <cstdio>
#include <fmt/format.h>
#include <fmt/os.h>
#include <memory>
#include <string>


#include <soyuz/soyuz.hh>
#include <soyuz/tray.hh>
#include <soyuz/windows.hh>

namespace soyuz {

fmt::ostream log_file = fmt::output_file("soyuz.log");

static auto CALLBACK enum_windows_proc(HWND hwnd, LPARAM lparam) -> BOOL {
  int length = GetWindowTextLength(hwnd);
  auto title = new CHAR[length + 1];

  GetWindowText(hwnd, title, length);

  if (strstr(title, LUNAR_WINDOW_NAME_BASE)) {
    *((HWND *)lparam) = hwnd;

    delete[] title;

    return FALSE;
  }

  delete[] title;

  return TRUE;
}

auto find_lunar() -> DWORD {
  HWND window = nullptr;
  DWORD pid;

  EnumWindows(enum_windows_proc, (LPARAM)&window);

  int length = GetWindowTextLength(window);
  auto title = new CHAR[length + 1];

  GetWindowText(window, title, length);

  GetWindowThreadProcessId(window, &pid);

  return pid;
}

auto delete_handle(DWORD pid) -> int {
  HANDLE lunar =
      OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_DUP_HANDLE, FALSE, pid);
  ULONG size = 1 << 10;
  std::unique_ptr<BYTE[]> buffer;

  if (!lunar) {
    soyuz::log(soyuz::log_level::LOG_LEVEL_WARN,
               fmt::format("could not open handle to lunar client: {}",
                           GetLastError()));

    return 1;
  }

  for (;;) {
    buffer = std::make_unique<BYTE[]>(size);

    NTSTATUS status = NtQueryInformationProcess(lunar, ProcessHandleInformation,
                                                buffer.get(), size, &size);

    if (NT_SUCCESS(status)) {
      break;
    }
    if (status == STATUS_INFO_LENGTH_MISMATCH) {
      size += 1 << 10;
      continue;
    }

    soyuz::log(soyuz::log_level::LOG_LEVEL_DEBUG,
               "could not enumerate handle, skipping");

    return 1;
  }

  auto *info =
      reinterpret_cast<PROCESS_HANDLE_SNAPSHOT_INFORMATION *>(buffer.get());

  for (ULONG i = 0; i < info->NumberOfHandles; ++i) {
    HANDLE h = info->Handles[i].HandleValue;
    HANDLE target;
    BYTE name_buffer[1 << 10];
    WCHAR target_name[256];
    DWORD session_id;

    if (!DuplicateHandle(lunar, h, GetCurrentProcess(), &target, 0, FALSE,
                         DUPLICATE_SAME_ACCESS)) {
      continue;
    }

    NTSTATUS status = NtQueryObject(target, ObjectNameInformation, name_buffer,
                                    sizeof(name_buffer), nullptr);

    CloseHandle(target);

    if (!NT_SUCCESS(status)) {
      continue;
    }

    ProcessIdToSessionId(pid, &session_id);
    swprintf_s(target_name, DISCORD_IPC_NAMED_PIPE_NAME);

    size_t length = wcslen(target_name);
    auto *name = reinterpret_cast<UNICODE_STRING *>(name_buffer);

    if (name->Buffer && _wcsnicmp(name->Buffer, target_name, length) == 0) {
      soyuz::log(soyuz::log_level::LOG_LEVEL_INFO,
                 "found lunar client's discord ipc named pipe");
      DuplicateHandle(lunar, h, GetCurrentProcess(), &target, 0, FALSE,
                      DUPLICATE_CLOSE_SOURCE);
      CloseHandle(target);
      soyuz::log(soyuz::log_level::LOG_LEVEL_INFO,
                 "closed lunar client's discord ipc named pipe");

      return 0;
    }
  }

  return 0;
}

auto write_log_file(const std::string &message) -> void {
  log_file.print("{}\n", message);
}

auto init_log_file() -> void {
  //  if (!log_file.is_open()) {
  //    soyuz::log("could not open 'soyuz.log'");
  //    soyuz::log("proceeding without logging to file");
  //
  //    return;
  //  }

  soyuz::log(soyuz::log_level::LOG_LEVEL_DEBUG, "opened 'soyuz.log'");
}

auto close_log_file() -> void {
  soyuz::log(soyuz::log_level::LOG_LEVEL_DEBUG, "closing 'soyuz.log'");
  log_file.close();
}

auto exit(int exit_code) -> void {
  // if (log_file.is_open()) { close_log_file(); }

  close_log_file();
  std::exit(exit_code);
}

auto current_date_time() -> std::string {
  time_t now = time(nullptr);
  struct tm t_struct {};
  char buffer[80];

  localtime_s(&t_struct, &now); // t_struct = *localtime(&now);
  strftime(buffer, sizeof(buffer), "%Y-%m-%d.%X", &t_struct);

  return buffer;
}

auto log_t::to_colorref() const -> COLORREF {
  switch (this->level) {
  case LOG_LEVEL_TRACE: {
    return 0x00FF0000;
  } // blue
  case LOG_LEVEL_DEBUG: {
    return 0x0000FF00;
  } // green
  case LOG_LEVEL_INFO: {
    return 0x00000000;
  } // black
  case LOG_LEVEL_WARN: {
    return 0x000080FF;
  } // orange
  case LOG_LEVEL_ERROR: {
    return 0x000000FF;
  } // red
  default: {
    return 0x00000000;
  } // black
  }
}

} // namespace soyuz
