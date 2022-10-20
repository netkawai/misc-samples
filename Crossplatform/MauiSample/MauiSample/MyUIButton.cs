using System;
using Microsoft.Maui.Handlers;
#if WINDOWS
using Microsoft.UI.Xaml;
#endif

namespace MauiSample
{

    public interface IMyButton : IView
    {
        public string Text { get; set; }
    }

    public class MyButton : View, IMyButton
    {
		public MyButton()
		{

		}

        public string Text { get; set; }
    }
#if IOS || MACCATALYST
    public partial class MyButtonHandler : ViewHandler<IMyButton, UIKit.UIButton>
    {

        public static IPropertyMapper<IMyButton, MyButtonHandler> PropertyMapper = new PropertyMapper<IMyButton, MyButtonHandler>(ViewHandler.ViewMapper)
        {
            ["Text"] = MapText
        };

        public MyButtonHandler() : base(PropertyMapper)
        {
            System.Diagnostics.Debug.WriteLine("test");
        }

        protected override UIKit.UIButton CreatePlatformView() {
            var btn = new UIKit.UIButton(UIKit.UIButtonType.System);
            btn.SetTitle("UIButton!!", UIKit.UIControlState.Normal);
            btn.Frame = new CoreGraphics.CGRect(10, -50, 100, 50);
            return btn; }


        public static void MapText(MyButtonHandler handler, IMyButton button)
        {
            
        }
    }
#elif WINDOWS
    public partial class MyButtonHandler : ViewHandler<IMyButton, FrameworkElement>
    {
        public MyButtonHandler(IPropertyMapper mapper, CommandMapper commandMapper = null) : base(mapper, commandMapper)
        {
        }

        protected override FrameworkElement CreatePlatformView()
        {
            return new Microsoft.UI.Xaml.Controls.Button();
        }
    }
#endif
}

