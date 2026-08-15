Summary for MSAL and Microsoft Graph SDK in each language (As of Sep 8 2022)

MSAL for cross-platform web browser interaction issue.

C#  has their own implementation for open browser for Windows, Linux, MacOS
The Linux implementation uses xdg-open and gnome- (graphical browser), it means it does not support other non-GUI (e.g. eLinks) or not-well known browser.

Python call built-in webbrowser module to open and exchange the token information.
(In WSL2, somehow, I don't know how, python could open default(Microsoft Edge) browser in Windows there is WSL specific code)

Java uses Selenium driver, but it looks Windows only?????? I don't see any code support in Linux?????

Microsoft Graph

C# Production Support

Java Production Support

Python forever preview T_T -> tones of third party? or miscellaneous organizations are trying to reduce this gaps.
https://github.com/vgrem/Office365-REST-Python-Client


