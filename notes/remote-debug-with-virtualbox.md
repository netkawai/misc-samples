I have faced pretty frequently to need to runWindows app on Virtual Machine for development.
The reasons were:
1. The app's source code relied on  out-dated 3rd party libraries which required a license key, but since the 3rd party library business went out, there is no way to buy a new license.
   To run the App, I needed to reset the Windows environnement every 90 days, once the trial period had passed.
2. The app needed a specific Windows driver or version, which was not provided by the customer due to legal? or their policy reason, so I need to run the app on a specific LTSC Windows 10 version which contained the specific drivers.

Those reasons are very common in Windows dev in the cooperation settings, However, I did not note down the setup steps, so I noted down here.
NOTE: For the reason No.1, now I can use Sandbox, which is bit easier than VirtualBox setup if the customer don't specific Windows version.


1. Install VirtualBox (prefer to the latest). Since v7.0, VirtualBox finally included USB device into the core open source, so now on Windows, accessing USB device become easier.
2. Get Windows Installation image and Install it. Or, get Installed Windows image as VHD(VDI).
3. Create virtual machine settings, if the customer specified a specific CPU cores or memory, just follow.
4. Check that image will boot properly, if not LTSC version, Update Windows to the latest update.
5. Change the network type from NAT to Host-only network. (So this allow Visual Studio connect to Remote debugger seamlessly, otherwise, we need port forward)
6. Reboot the guest
7. In host, select the folder which contains repos(source codes) and click Properties and click Sharing on Properties tab and Click Share (not Advanced Share)
8. Make sure only logged-in user is selected, Click Share (Now this folder is accessible from other computer for the login user)
9. In guest, vboxsrv or check IP address of guest, if 192.168.52.xx, then host is 192.168.52.1 (unless changed from command line)
10. Type \\hostip(or host name) in Windows Explorer file path. Then usually Windows pop up credential input.
11. Type credential the HOST logged in username and password. As usual, If the host is WorkGroup, start from the Computer name .e.g \\(Computer Name)\(user name), if the computer belong to AD (not AAD), 
Start from Directory domain \\(AD domain name)\(user name) and password.
12. The Widows Explorer shows the Shared folder in Step 8. If not check password and user name. (I know this is bit complex due to Windows)
13. Map the Shared folder e.g. to Z: drive.
14. Download Remote Tools matching with Visual Studio version for development. Copy from Host to Guest, and Install it in Windows in VM
15. Search the installed path (usually "C:\Program Files\Microsoft Visual Studio 17.0"  VS2022) and go through Common7\IDE\Remote Debugger\x64\
16. Type below command: This allow to connect any user without warning. (Microsoft discouraged, but since We run Windows Host only in VirtualBox really does not harm)
```
msvsmon /noauth /nosecuritywarn /anyuser
```
17. In Host, mount the shared folder to the same drive path which has done in Step 13 (This case is Z:)
18. Open the sln through that mapped drive with Visual Studio.
19. Open proj file for the App, go to Debug and check "Use Remote Machine", and type the computer name of the Windows VM (since Host only network, Host can see the guest computer name) 
and and change the Authentication type to No Authentication.
20. Build app and Click run, Check the remote debugger window, either the remote debugger connected from the loggined machine and user. App shows up on Windows guest in VirtualBox.


