In the beginning, Windows NT API was created in mind of nomenclature of the original Windows 16bit. 
However at the time, Unicode was about standarized but UTF-8 was not existed. So Microsoft define two APIs for one function with A and W.

The function with W has argument w_char (16bit), function with A has char (8bit with multibyte character set).

Windows NT evolved in Windows 2000 with the integration of Windows 9x line. All APIs are integrated which eventually called as Win32 API.
At the time, Windows NT equals Windows 32bit version. Even After Windows 2003 64it version came out, most API manner or rules did not change.

From Windows 8, Microsoft introduced Windows Runtime and Microsoft Store. Those are analogy of what smartphone platform appeared late 2000s.
Basically the app runs on App Contains (the evolution of APP-V - Office app) with new App package format and provide mechanism to install without Admin (not Program Files),
and Settings can control certain aspect such as the local file access, the Internet, or some resources, and auto-upate and cataloged through Microsoft Store App.

In design, the mechanisms were introduced completely different layers as a new COM (WinRT COM) (other than DirectX Runtime) due to the purpose or goal of this mechnism,
those two world are isolated, originally if the developer want to create Microsoft Store App, they have create a completely different stuff not Win32 API.


Unlike iPhone or Android, those completely new device or new platform, Microsoft could not compete those software platform and hardware rapid revolution.
Microsoft failed to grab smartphone platform, the original intention of Windows Runtime and Store became garbage.

However, Microsoft did not expand the already legacy component such as WPF/Winform. Instead of that, since Microsoft focused on Azure with Linux, abandon the original ASP.NET which
are tightly integrated with Windows Server, they de-cluttered (de-coupled) .NET Platform.

And for Microsoft, App Container is useful in a certain situation, yes, historically, most App is expected into Program File in classic installer without any limitation except ACL,
However, if they can split new UIs (UI mechanism), App Container(Publish), and make Win32(original .NET Framework API) can communicate to them. Natually, some developers start to think,
Windows does not need rigid App framework, rather developers need them as components to pick by developers.

Now, Windows support three major executable ways.
1. The single EXE (no package ID): Most traditional load PE file as execute in a process) -> More: Run as Administrator, Open File Location. (No App Settings, Share)  
1a. The single EXE with Package ID through sparse library (The context menu in app is identical to No.1? Some call sparse library. I am not sure whether this is still officially supported or not. At least Windows 11 added an API for this). EXE resides in Program Files. The user can execute EXE through double-click. For example, Microsoft Edge has EXE in C:\Program Files (x86)\Microsoft\Edge\Application, simultaneously, there is a package WITHOUT EXE in e.g. C:\Program Files\WindowsApps\Microsoft.MicrosoftEdge.Stable_111.0.1661.54_neutral__8wekyb3d8bbwe and signed kind is Developer (Not Store)

2. Packaged EXE with UWP conformant (i.e. UWP app, use only UWP api to support multiple type device on Windows/Microsoft Store,but dead) -> More: App Settings, Share, (No Run as Administrator) and the user cannot run the EXE directly with double-click.  

3. Packaged EXE with Transitional Win32/.NET App -> More: App Settings, Run as Administrator, Share, but still the user cannot run the EXE directly with double-click.Since the start menu icon is NOT a simple shortcut for EXE.  

No.3 is still packaged, but only Windows 10 (1904) or Later, not necessary for Microsoft Store. It can be elevated. 
In default the EXE/Packaged EXE can access all resources on Windows. They still are called to desktop app (meh) 

Windows App SDK is not an open-source project, rather to open new UIs (UI mechanism) of WinRT to open up to the single Exe(traditional EXE).
WPF/Winform in .NET are now MIT, it means, Microsoft will not add any new UI components (only improve performance and compatibility) at least in WPF.

As of 2021-12-20, Microsoft does not have any killer app as Store App(MSIX package APP)s, even Desktop Office are single EXE due to compatibility and plugin. basically smartphone app will not have plugin/add-on nor non-connected environment. Games need lots of protection to prevent cheat and resource extractions. Either way, Windows package does not help anything.

Windows 10 Mobile is dead, Ad Monetization platform is dead, Cortana is dead.. basically current Windows system functions are Windows 7 + OneDrive(App and NTFS support) + DPI scaler. Since Packaged Apps have default app update mechanism, at least, PC maker and chip makers are using them. However, Microsoft already distributed the basic drivers update through Windows Update, basically duplicated, unnecessary works.