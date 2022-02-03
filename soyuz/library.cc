// Copyright (C) 2021-2021 Fuwn
// SPDX-License-Identifier: GPL-3.0-only

/**
 * @file   library.cc
 * @author Fuwn
 * @date   2021. August. 18.
 */

#include <soyuz/library.hh>

#include <cstdio>
#include <fmt/format.h>
#include <fmt/os.h>
#include <memory>
#include <string>
#include <Windows.h>

#include <soyuz/soyuz.hh>
#include <soyuz/tray.hh>
#include <soyuz/windows.hh>

namespace soyuz {

fmt::ostream log_file = fmt::output_file("soyuz.log");

static BOOL CALLBACK enum_windows_proc(HWND hwnd, LPARAM lparam) {
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
  EnumWindows(enum_windows_proc, (LPARAM)&window);

  int length = GetWindowTextLength(window);
  auto title = new CHAR[length + 1];
  GetWindowText(window, title, length);

  DWORD pid;
  GetWindowThreadProcessId(window, &pid);
  return pid;
}

auto delete_handle(DWORD pid) -> int {
  HANDLE lunar = OpenProcess(
    PROCESS_QUERY_INFORMATION | PROCESS_DUP_HANDLE,
    FALSE,
    pid
  );
  if (!lunar) {
    soyuz::log(fmt::format("could not open handle to lunar client: {}", GetLastError()));

    return 1;
  }

  ULONG size = 1 << 10;
  std::unique_ptr<BYTE[]> buffer;
  for (;;) {
    buffer = std::make_unique<BYTE[]>(size);

    NTSTATUS status = NtQueryInformationProcess(
      lunar,
      ProcessHandleInformation,
      buffer.get(),
      size,
      &size
    );

    if (NT_SUCCESS(status)) { break; }
    if (status == STATUS_INFO_LENGTH_MISMATCH) { size += 1 << 10; continue; }

    soyuz::log("could not enumerate handle, skipping");

    return 1;
  }

  auto *info = reinterpret_cast<PROCESS_HANDLE_SNAPSHOT_INFORMATION *>(buffer.get());
  for (ULONG i = 0; i < info->NumberOfHandles; ++i) {
    HANDLE h = info->Handles[i].HandleValue;
    HANDLE target;
    if (!DuplicateHandle(
      lunar,
      h,
      GetCurrentProcess(),
      &target,
      0,
      FALSE,
      DUPLICATE_SAME_ACCESS
    )) { continue; }

    BYTE name_buffer[1 << 10];
    NTSTATUS status = NtQueryObject(
      target,
      ObjectNameInformation,
      name_buffer,
      sizeof(name_buffer),
      nullptr
    );
    CloseHandle(target);
    if (!NT_SUCCESS(status)) { continue; }

    WCHAR target_name[256];
    DWORD session_id;
    ProcessIdToSessionId(pid, &session_id);
    swprintf_s(target_name, DISCORD_IPC_NAMED_PIPE_NAME);
    size_t length = wcslen(target_name);

    auto *name = reinterpret_cast<UNICODE_STRING *>(name_buffer);
    if (name->Buffer && _wcsnicmp(name->Buffer, target_name, length) == 0) {
      soyuz::log("found lunar client's discord ipc named pipe");

      DuplicateHandle(
        lunar,
        h,
        GetCurrentProcess(),
        &target,
        0,
        FALSE,
        DUPLICATE_CLOSE_SOURCE
      );
      CloseHandle(target);

      soyuz::log("closed lunar client's discord ipc named pipe");

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

  soyuz::log("opened 'soyuz.log'");
}

auto close_log_file() -> void {
  soyuz::log("closing 'soyuz.log'"); log_file.close();
}

auto exit(int exit_code) -> void {
  // if (log_file.is_open()) { close_log_file(); }
  close_log_file();
  ::exit(exit_code);
}

auto current_date_time() -> std::string {
  time_t now = time(nullptr);
  struct tm t_struct{};
  char buffer[80];
  localtime_s(&t_struct, &now); // t_struct = *localtime(&now);
  strftime(buffer, sizeof(buffer), "%Y-%m-%d.%X", &t_struct);

  return buffer;
}

}
