Aspx view

<%: Output HTML Escape  %>
@__w.Write(System.Web.HttpUtility.HtmlEncode("Encode Output"));
<%= Output Raw  %>
@__w.Write("Raw Output");