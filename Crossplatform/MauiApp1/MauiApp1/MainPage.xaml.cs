namespace MauiApp1;

public partial class MainPage : ContentPage
{
	int count = 0;

	public MainPage()
	{
		InitializeComponent();
        var btn = new Button();
        btn.Text = "Button " + count + " added";
        btn.Clicked += OnCounterClicked;
        btn.HorizontalOptions = LayoutOptions.Center;
        btn.HandlerChanging += Btn_HandlerChanging;
        btn.HandlerChanged += Btn_HandlerChanged;
        VLayout.Add(btn);
    }

    private void OnCounterClicked(object sender, EventArgs e)
	{
		count++;

		if (count == 1)
			CounterBtn.Text = $"Clicked {count} time";
		else
			CounterBtn.Text = $"Clicked {count} times";

		SemanticScreenReader.Announce(CounterBtn.Text);
	}

    private void Btn_HandlerChanging(object sender, HandlerChangingEventArgs e)
    {
        View btn = sender as View;
        if (btn.Handler == null)
            return;
#if IOS || MACCATALYST
        System.Diagnostics.Debug.WriteLine((btn.Handler.PlatformView as UIKit.UIButton).TitleColor(UIKit.UIControlState.Normal).ToString());
#endif
    }

    private void Btn_HandlerChanged(object sender, EventArgs e)
    {

        View btn = sender as View;
        if (btn.Handler == null)
            return;

#if MACCATALYST
        System.Diagnostics.Debug.WriteLine((btn.Handler.PlatformView as UIKit.UIButton).Title(UIKit.UIControlState.Normal).ToString());
        System.Diagnostics.Debug.Write(Environment.StackTrace);
#endif
    }


    void ContentPage_HandlerChanged(System.Object sender, System.EventArgs e)
    {
        ContentPage page = sender as ContentPage;
        if (page.Handler == null)
            return;

#if  MACCATALYST
        var btn = new UIKit.UIButton(UIKit.UIButtonType.System);
        btn.BackgroundColor = UIKit.UIColor.Black;
        btn.SetTitleColor(UIKit.UIColor.White, UIKit.UIControlState.Normal);
        btn.SetTitle("UIButton!!", UIKit.UIControlState.Normal);
        btn.Frame = new CoreGraphics.CGRect(10, 50, 100, 50);


        (page.Handler.PlatformView as UIKit.UIView).Add(btn);
#endif
    }

}

