Co-install php-cgi and php5_modules in Apache

メモ 

apache 2.2 Fedora 11でphp5_moduleと php-cgiを同じサーバで同居させる方法。 
(発端はテストサーバにzabbixをインストール際に、 frontendのphpがbcmath対応、gd対応を要求。module版には 
入っていないが、php-cgi(CLI)はパッケージをyumからインストールする だけのため、対応可能だが、今更全体の設定を変えたくないので、 
zabbixのフォルダだけphp-cgiにする。さらにデフォルトのphp.iniは memory_limitを32Mまで制限しているので、これも分ける。) 

1. httpd.conf CGIの実行を許可 
zabbixのディレクトリに対するオプションで、ExecCGIを追加 
'''
<Directory /var/www/html/zabbix> 
    AllowOverride All 
    Options FollowSymLinks Includes ExecCGI 
</Directory> 
'''
 

2. 起動ファイルの設置 
php-cgiは/usr/bin/php-cgiにあるが、これは通常httpd.confの 
設定によりapacheからは見えないため、/var/www/cgi-binに phpを実行するためのshスクリプト(php-cgi.sh)を置く。 
(なお、php.iniをmodule版と分ける必要がない場合はシンボリックリンクでもよい。) 

#!/bin/sh 
exec /usr/bin/php-cgi -c /var/www/cgi-bin/php.ini 


なお、今回はここにphp-cgiのphp.iniも置く。 
3. .htaccessに特定の拡張子のファイルをphp-cgi.shに実行してもらう。 
.htaccessに下記２行を追加 
AddHandlerでどのファイルをphp-cgi.shで実行するか指定し、 
その実行するハンドラをActionで定義。httpd.confのルートは 
/var/wwwのため、そこからのパスでしているため、今回の 
実行ファイルは/cgi-bin/php-cgi.shとなる。 
このファイルに実行権を与えるのを忘れずに。 
'''
Action php-cgi /cgi-bin/php-cgi.sh 
AddHandler php-cgi .php .phps 
'''