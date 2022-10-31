using Microsoft.Maui.Handlers;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MauiApp1.Platforms
{
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

        protected override UIKit.UIButton CreatePlatformView()
        {
            var btn = new UIKit.UIButton(UIKit.UIButtonType.System);
            btn.SetTitle("UIButton!!", UIKit.UIControlState.Normal);
            btn.Frame = new CoreGraphics.CGRect(10, -50, 100, 50);
            return btn;
        }


        public static void MapText(MyButtonHandler handler, IMyButton button)
        {

        }
    }
}
