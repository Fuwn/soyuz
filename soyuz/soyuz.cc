// Copyright (C) 2021-2021 Fuwn
// SPDX-License-Identifier: GPL-3.0-only

/**
 * @file   soyuz.cc
 * @author Fuwn
 * @date   2021. August. 18.
 */

#include <soyuz/soyuz.hh>

#pragma comment(lib, "ntdll.lib")

#include <fmt/format.h>
#include <thread>
#include <Windows.h>

#include <soyuz/library.hh>
#include <soyuz/resource.hh>
#include <soyuz/tray.hh>

extern UINT WM_TASKBAR;
extern HWND window;
extern HMENU menu;
extern NOTIFYICONDATA data;
extern TCHAR tip[64];
extern char class_name[];
extern std::vector<std::string> logs;

int WINAPI WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int show) {
  soyuz::init_log_file();

  MSG messages;
  WNDCLASSEX wincl;

  WM_TASKBAR = RegisterWindowMessageA("TaskbarCreated");
  wincl.hInstance = instance;
  wincl.lpszClassName = class_name;
  wincl.lpfnWndProc = WindowProcedure;
  wincl.style = CS_DBLCLKS;
  wincl.cbSize = sizeof(WNDCLASSEX);

  wincl.hIcon = LoadIcon(GetModuleHandle(nullptr), MAKEINTRESOURCE(ICO1));
  wincl.hIconSm = LoadIcon(GetModuleHandle(nullptr), MAKEINTRESOURCE(ICO1));
  wincl.hCursor = LoadCursor(nullptr, IDC_ARROW);
  wincl.lpszMenuName = nullptr;
  wincl.cbClsExtra = 0;
  wincl.cbWndExtra = 0;
  wincl.hbrBackground = (HBRUSH)(CreateSolidBrush(RGB(255, 255, 255)));
  if (!RegisterClassEx(&wincl)) { return 0; }

  window = CreateWindowEx(
    0,
    class_name,
    class_name,
    WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME,
    CW_USEDEFAULT,
    CW_USEDEFAULT,
    544,
    375,
    HWND_DESKTOP,
    nullptr,
    instance,
    nullptr
  );
  InitNotifyIconData();
  ShowWindow(window, show);

  /**
   * Launch a new thread to take care of everything important
   *
   * https://medium.com/@vgasparyan1995/a-new-thread-in-c-20-jthread-ebd121ae8906
   */
  std::jthread soyuz {[](const std::stop_token &stop) -> void {
    // Check if Lunar Client is open, if not; close Soyuz
    DWORD pid = soyuz::find_lunar();
    if (pid == 0 || pid == 3435973836) {
      soyuz::log("could not locate lunar client");
      soyuz::log("this window will close in five seconds");
      for (int i = 4; i > -1; --i) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        soyuz::log(fmt::format("> {} second{}", soyuz::numbers_as_string[i], i != 1 ? "s" : ""));
      }

      soyuz::exit(1);
    }
    soyuz::log(fmt::format("located lunar client: pid {}", GetLastError()));
    soyuz::log("hooked lunar client"); soyuz::log("you may now close this window");

    while (!stop.stop_requested()) {

      /**
       * Check if Lunar Client is open before every `delete_handle` run, if not; timeout
       *
       * Thanks, @LorenzoHanssens (#1)
       */
      pid = soyuz::find_lunar();
      if (pid == 0 || pid == 3435973836) {
        soyuz::log("could not locate lunar client, waiting 10 seconds");
        std::this_thread::sleep_for(std::chrono::seconds(10));
      }

      // If Lunar Client **is** open, close it's Discord IPC Named Pipe
      if (soyuz::delete_handle(pid) == 1) {
        soyuz::log("unable to close lunar client's discord ipc named pipe");
      }
    }
  }};

  while (GetMessage(&messages, nullptr, 0, 0)) {
    TranslateMessage(&messages);
    DispatchMessage(&messages);
  }

  soyuz::log("requesting exit");
  soyuz.request_stop(); soyuz.join();

  soyuz::log("exiting");
  soyuz::close_log_file();
  return (int)messages.wParam;
}
