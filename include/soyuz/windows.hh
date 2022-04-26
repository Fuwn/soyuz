// Copyright (C) 2021-2022 Fuwn
// SPDX-License-Identifier: GPL-3.0-only

/**
 * @file   windows.hh
 * @author Fuwn
 * @date   2021. August. 18.
 */

#ifndef SOYUZ_WINDOWS_HH
#define SOYUZ_WINDOWS_HH
#pragma once

#include <Windows.h>

enum PROCESSINFOCLASS {
  ProcessHandleInformation = 51
};

typedef struct _PROCESS_HANDLE_TABLE_ENTRY_INFO {
  HANDLE    HandleValue;
  ULONG_PTR HandleCount;
  ULONG_PTR PointerCount;
  ULONG     GrantedAccess;
  ULONG     ObjectTypeIndex;
  ULONG     HandleAttributes;
  ULONG     Reserved;
} PROCESS_HANDLE_TABLE_ENTRY_INFO, * PPROCESS_HANDLE_TABLE_ENTRY_INFO;

typedef struct _PROCESS_HANDLE_SNAPSHOT_INFORMATION {
  ULONG_PTR                       NumberOfHandles;
  ULONG_PTR                       Reserved;
  PROCESS_HANDLE_TABLE_ENTRY_INFO Handles[1];
} PROCESS_HANDLE_SNAPSHOT_INFORMATION, * PPROCESS_HANDLE_SNAPSHOT_INFORMATION;

extern "C" NTSTATUS NTAPI NtQueryInformationProcess(
  _In_ HANDLE                                        ProcessHandle,
  _In_ PROCESSINFOCLASS                              ProcessInformationClass,
  _Out_writes_bytes_(ProcessInformationLength) PVOID ProcessInformation,
  _In_ ULONG                                         ProcessInformationLength,
  _Out_opt_ PULONG                                   ReturnLength
);

typedef enum _OBJECT_INFORMATION_CLASS {
  ObjectNameInformation = 1
} OBJECT_INFORMATION_CLASS;

typedef struct _UNICODE_STRING {
  USHORT Length;
  USHORT MaximumLength;
  PWSTR  Buffer;
} UNICODE_STRING;

typedef struct _OBJECT_NAME_INFORMATION {
  UNICODE_STRING Name;
} OBJECT_NAME_INFORMATION, * POBJECT_NAME_INFORMATION;

extern "C" NTSTATUS NTAPI NtQueryObject(
  _In_opt_ HANDLE                                       Handle,
  _In_ OBJECT_INFORMATION_CLASS                         ObjectInformationClass,
  _Out_writes_bytes_opt_(ObjectInformationLength) PVOID ObjectInformation,
  _In_ ULONG                                            ObjectInformationLength,
  _Out_opt_ PULONG                                      ReturnLength
);

#endif // SOYUZ_WINDOWS_HH
