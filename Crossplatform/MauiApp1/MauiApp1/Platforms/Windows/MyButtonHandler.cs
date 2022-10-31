using Microsoft.Maui.Handlers;
using Microsoft.UI.Xaml;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MauiSample.Platforms
{
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
}
