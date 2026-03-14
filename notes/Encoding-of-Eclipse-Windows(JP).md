Eclipse の言語 Encoding 

日本語版のeclipseで、workspaceを作成するとデフォルトの 文字エンコードがすべてMS932(SJIS)になってしまい、英語版のworkspace 
はUTF-8になっているので、エンコードの違いによりコンパイルできない。 この場合は英語版のeclipseでも元のworkspaceとは別にworkspaceを 
作成し、そのworkspaceのデフォルトエンコードをMS932に変更する。 srcや各フォルダごとにエンコードを設定できるようにはなっているが、 
各ファイルのエンコードはworkspaceのエンコードに固定されてしまうので 注意が必要。 

http://www.eclipse.org/pdt/downloads 
http://code.google.com/p/zend-sdk/downloads/detail?name=eclipse-php-3.0.2.v2011102768-Win32.zip&can=2&q= 