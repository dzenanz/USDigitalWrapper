https://www.usdigital.com/support/software/sei

Assemblies need to be registered:
RegAsm USDigital.dll
RegAsm USDSEI.dll
Note: use 32-bit version of RegAsm.exe for 32-bit version of DLLs, and 64-bit RegAsm.exe for 64-bit DLLs. Appropriate bitness of Visual Studio command prompt drags in correct RegAsm to the command line.

USDSEI.dll and USDigital.dll need to be next to the executable at run time, and next to .lib or .exe file at compile time (e.g. in Debug folder).
