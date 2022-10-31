#if WINDOWS
using Microsoft.UI;
using Microsoft.UI.Windowing;
using Windows.Graphics;
#elif MACCATALYST
using UIKit;
using CoreGraphics;
#endif

namespace MauiApp1;

public partial class App : Application
{
	public App()
	{
		InitializeComponent();

		MainPage = new AppShell();
        const int WindowWidth = 400;
        const int WindowHeight = 300;
        Microsoft.Maui.Handlers.WindowHandler.Mapper.AppendToMapping(nameof(IWindow), (handler, view) =>
        {
#if WINDOWS
            var mauiWindow = handler.VirtualView;
            var nativeWindow = handler.PlatformView;
            nativeWindow.Activate();
            IntPtr windowHandle = WinRT.Interop.WindowNative.GetWindowHandle(nativeWindow);
            WindowId windowId = Microsoft.UI.Win32Interop.GetWindowIdFromWindow(windowHandle);
            AppWindow appWindow = Microsoft.UI.Windowing.AppWindow.GetFromWindowId(windowId);
            appWindow.Resize(new SizeInt32(WindowWidth, WindowHeight));
#elif MACCATALYST
            var mauiWindow = handler.VirtualView;
            UIWindow nativeWindow = handler.PlatformView;
            nativeWindow.WindowScene.SizeRestrictions.MinimumSize = new CGSize(WindowWidth, WindowHeight);

#endif
        });

    }
}
