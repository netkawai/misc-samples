Passenger-Nginx And RVM And Redmine2.3.3のインストール

現在、Apache Passenger Moduleを使って、Rail2のredmine1.4が動作しているが、Redmine2.3のテストのため、これとは別にRail3をインストールする必要があって、かなり迷ったので、一応メモ書きを残しておく。
特にこのRailsは、本家・本元で開発されているRubyのみで動作するものと、これをWebサーバに組み込んだPassengerというものがある。
同じRailsだが、動作環境・速度・制限が異なる。しかも、Rubyのバージョンも違うため、これを共存させるため、RVM(Ruby Version Manager)というものがある。
1. apt-get install rvm
とりあえず、APTパッケージでrvmを入れた。現在のrubyの最新がver2.0になってしまっており、現時点でRedmineがサポートしているのが、Ruby1.9系と、互換性がないため。
2. rvm use ruby1.9をして、デフォルトのrubyを1.9に切り替えて、gem install bundlerでバンドルを入れておく。
3.Apache Passenger Moduleは既にRails2相当のものが入っているが、これとは別にngnixを入れる必要がある。しかも、このWebサーバはモジュール構造を持たないため、コンパイルし直す必要がある。しかも、コンパイル時に
apt-get install nginx-full
でUbuntuのnginxを入れておいた方がよい公式ページに書いてある。
おかげで、コンパイルし直した後、間違って２時間ぐらいUbuntuのnginxを使って、動作テストしたため、動かなかった。
Passenger自体のインストールは
gem install passenger
必要なパッケージが入っていれば、問題ない。
このとき、デフォルトのインストール先が/opt/nginxになっているが、/usr/local/nginxに変えたが、
これは/opt/nginxのほうが良かったと思う。
インストールが終わったら、***** 必ず apt-get remove nginx-full すること ****。
今回はこれを忘れたために、不必要なところで手間取った。
後、当然/etc/nginxにあるのは、ubuntuの設定ファイルなので、ここではなく、/opt/nginx/conf内
を変更すること。
nginx.confで、PassengerとRedmineで必要なのは下記のもの。
# rvmを使っているので、そのユーザに合わせる。
user  redmine2;
#これはコンパイル時に自動で入った。(rvmのものが使用されている。)
http {
passenger_root /usr/local/rvm/gems/ruby-1.9.3-p448/gems/passenger-4.0.20;
passenger_ruby /usr/local/rvm/wrappers/ruby-1.9.3-p448/ruby;
location / {
root   html;
index  index.html index.htm;
passenger_enabled on;
passenger_base_uri /redmine;
}
今回、別のサイトでredmine/publicにリンクを張れば十分ということが書いてあったので、
ln -s /opt/redmine-2.3.3/public /usr/local/nginx/html/redmine
という形で、リンクを張った。
後、passenger_enabled onをするのを忘れずに。
これで、基本はredmineのインストールサイトに記述がある。
bundle install --without test
で、ymlを設定していけば、終わり。
同じソフトで、異なるバージョンがたくさんあるのは、やっぱりややこしい。
後間違って、Passengerを入れた後にRailsも入れたけど、これは必要ないみたい。ようやく、移行の仕方が分かったので、ここに手順を示す。
0.
mysqldump -u redmine -predmine -n redmine_default | mysql -u redmine -predmine redmine2
データベースをコピー。最初のインストール時のDB名を変更できないようなので、インストール時に決める
DB名には注意。
1. worktime pluginを展開。
cd /opt/redmine-2.3.3/plugins; tar xvfz /home/redmine2/redmine_work_time-0.2.14.zip
ここからはrvmを使用する為、rvmを使用できるredmine2でログインする。
1.5
rake generate_secret_token
2.Upgrade DB
rake db:migrate RAILS_ENV=production
3.Plugin
rake redmine:plugins:migrate RAILS_ENV=production
4. cealn
rake tmp:cache:clear
rake tmp:sessions:clear
これで現在の状態。
次にbacklog pluginをインストールする。
5. backlog pluginを展開。
cd /opt/redmine-2.3.3/plugins; tar xvfz /home/redmine2/redmine_backlogs-1.0.6.tar.gz
mv redmine_backlogs-1.0.6 redmine_backlogs
3つ目は重要(これがないとエラーになる。)
6. Plugin DB Migrate。既存データがある状態なので、先にこれを実施。
rake redmine:plugins:migrate RAILS_ENV=production
ここから全てのチケットに対するレコードの変更が行われる。LinuxがFreezeしないかを
良く見守ること。
7. RAILS_ENVをexport
RAILS_ENV=production
export RAILS_ENV
bundle exec rake redmine:backlogs:install
ここで、Product BackLogにするTackerを聞かれるので、
とりあえず、Bugだけにする。後で変更可能。
8. RoleとConfigurationのところで、Applyを押すと使用できるようになる。
ここまでお疲れ様でした。
現在、インストールしたplugins
Migrating redmine_assets_plugin (Redmine Assets plugin)... redmine_assets_plugin_master(git version:20131026)
Migrating redmine_backlogs (Redmine Backlogs)...           redmine_backlogs(version1.0.6)
Migrating redmine_work_time (Redmine Work Time plugin)...  redmine_worktime(0.2.14)
後、システムのrubyを消したら、nginxのdeamonが正しく停止しなくなった。
使用上は問題ないので、とりあえず、置いておくが、そのうち、調査する。
restartしても、processが止まらないので、8080:can not bindがでるが、killすれば、大丈夫。
From <http://www.modrails.com/documentation/Users%20guide%20Nginx.html>