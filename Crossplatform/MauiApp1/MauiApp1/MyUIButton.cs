using System;
using Microsoft.Maui.Handlers;

namespace MauiApp1
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
}

