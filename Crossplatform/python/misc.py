
def is_wsl():
    # "Official" way of detecting WSL: https://github.com/Microsoft/WSL/issues/423#issuecomment-221627364
    # Run `uname -a` to get 'release' without python
    #   - WSL 1: '4.4.0-19041-Microsoft'
    #   - WSL 2: '4.19.128-microsoft-standard'
    import platform
    uname = platform.uname()
    platform_name = getattr(uname, 'system', uname[0]).lower()
    release = getattr(uname, 'release', uname[2]).lower()
    return platform_name == 'linux' and 'microsoft' in release


some_uri="https://google.ca"
after_format='Start-Process "{}"'.format(some_uri)
#print(after_format)

if is_wsl():
    print("WSL!!")
    try:
        import subprocess
        # https://docs.microsoft.com/en-us/powershell/module/microsoft.powershell.core/about/about_powershell_exe
        # Ampersand (&) should be quoted
        exit_code = subprocess.call(
            ['powershell.exe', '-NoProfile', '-Command', 'Start-Process "{}"'.format(some_uri)])
        browser_opened = exit_code == 0
    except FileNotFoundError:  # WSL might be too old
        pass
else:
    print("Native")
    import webbrowser
    webbrowser.open_new(some_uri)