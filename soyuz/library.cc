// Copyright (C) 2021-2021 Fuwn
// SPDX-License-Identifier: GPL-3.0-only

#include <cstdio>
#include <memory>
#include <string>
#include <Windows.h>

#include "soyuz/library.hh"
#include "soyuz/soyuz.hh"
#include "soyuz/windows.hh"

static auto enum_windows_proc(HWND hwnd, LPARAM lparam) -> BOOL {
  int length = GetWindowTextLength(hwnd);
  auto title = new CHAR[length + 1];
  GetWindowText(hwnd, title, length);

  if (strstr(title, LUNAR_WINDOW_NAME_BASE)) {
    *((HWND*)lparam) = hwnd;

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
    printf("could not open handle to lunar client: %lu\n", GetLastError());

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

    printf("could not enumerate handle\n");

    return 1;
  }

  auto *info = reinterpret_cast<PROCESS_HANDLE_SNAPSHOT_INFORMATION*>(buffer.get());
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

    auto *name = reinterpret_cast<UNICODE_STRING*>(name_buffer);
    if (name->Buffer && _wcsnicmp(name->Buffer, target_name, length) == 0) {
      printf("found lunar client's discord ipc named pipe\n");

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

      printf("closed lunar client's discord ipc named pipe\n");

      return 0;
    }
  }

  return 0;
}
