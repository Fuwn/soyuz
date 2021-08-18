// Copyright (C) 2021-2021 Fuwn
// SPDX-License-Identifier: GPL-3.0-only

#pragma comment(lib, "ntdll.lib")

#include <cstdio>
#include <Windows.h>

#include "soyuz/library.hh"

auto main() -> int {
  DWORD pid = find_lunar();
  if (pid == 0 || pid == 3435973836) {
    printf("could not locate lunar client\n");

    return 1;
  }
  printf("located lunar client: %lu\n\n", pid);

  for (;;) {
    if (delete_handle(pid) == 1) {
      printf("unable to close lunar client's discord ipc named pipe\n");
    }
  }

  return 0;
}
