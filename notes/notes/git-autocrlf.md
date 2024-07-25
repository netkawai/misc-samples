This is a note of thinking autocrlf in WSL2 environment.
(WSL means WSL2 in this text)

WSL2 is a special Linux environment seamlessly hosted in Windows
Historically, The default end-of-line (eol) of Windows is CRLF
Mac and Unix(Linux) are LF.

In my understanding autocrlf was implemented to convert CRLF to LF between workspace and staging(Blob). So that in Windows, when the git user checks out repo, the files in workspace are CRLF and principally, staging(blob) in repo are LF, when you can see the files in web-view or Unix. the EOL is LF.

But this holds a big caveat. When git user creates a git-formatted patch, since those patches are created from blob, the EOL of those patch files are LF. If you apply those patches without proper command parameters, git user will create CRLF/CR mixing or you cannot apply since the EOL are different (workspace is CRLF, the patch is LF)

A simple solution was, if the source codes were a specifically for Windows, e.g. NET Framework, or DirectX, turn off autocrlf, then, staging/blob should have CRLF. Since, those codes cannot be used in other environments. It had done.

However Microsoft deeply started to involve and integrate with Linux environment for Azure (Cloud) due to that cloud computing is dominated in Linux. More over, Microsoft starts to support cross platform development for smartphone(Android/iOS) due to failure of Microsoft Smartphone/Sliverlight/UWP largely.  The Notepad (WIN32 TEXT COMPONENT) finally adopted LF as EOL, and introduced Visual Studio Code based on Atom developed by Github acquired by Microsoft. Now, there is no necessity to keep CRLF even in Windows. It is similar of ISO8859, superseded by UTF-8, in my opinion, CRLF became obsolete finally, superseded? by LF. (at 2023-01-10 Windows 8/8.1 end of life, Windows 7 end of extended security update)

I think, now time has come to renormalize all EOL(end-of-line) to LF for text files

In nutshell, core.autocrlf=input. all CRLF should convert to LF, when I add new files whenever add staging, should update all EOL to LF, either dos2unix or simply reset --hard after commit
