# Project: WinAPIOverride32 - Version 5.5.3

This repository contains the source code for **WinAPIOverride32**, version 5.5.3, originally created by **Jacquelin Potier** ([http://jacquelin.potier.free.fr/winapioverride32/](http://jacquelin.potier.free.fr/winapioverride32/)). It has been slightly modified to ensure compatibility and successful compilation on a **Windows XP SP3 32-bit Virtual Machine** using **Visual Studio 2010**. These modifications include adjustments for paths, dependencies, project settings, and fixes for type-related errors, allowing the code to compile correctly in this specific environment.

## Compilation Order
The solution files should be built in this order (in Release mode, except for distorm) :
1.	FakeExeInternal
2.	GetDriveType
3.	HideMe
4.	FakeIsDebuggerPresent
5.	FakeMsgBox
6.	PrePostHooksMsgBox
7.	EmulatedRegistryCreator
8.	EmulatedRegistryEditor
9.	Launcher
10.	RegistryReplacement
11.	AsmExeInternal
12.	COMObjectCreationSpy
13.	COMQueryInterfaceRestriction
14.	FakeNET
15.	Example
16.	HookChildProcesses
17.	MultiConfigManager
18.	DllExportFinder
19.	Dumper
20.	HeapWalker
21.	MonitoringFileBuilder
22.	StaticImportFinder
23.	StubResolver
24.	DebugInfos
25.	dia2dump
26.	distorm
27.	disasm
28.	HookNet
29.	IATLoader
30.	Injlib
31.	ProcMonUI
32.	SetEnvVarProc
33.	x86CrossCompatibility
34.	APIOverride
    
## Executable
The executable produced will be located in `winapioverride32_src\WinAPIOverride32\output\release` .
