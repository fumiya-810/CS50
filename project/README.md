# TreasureTroveMarket
#### Video Demo:  https://youtu.be/_HS98h8IL6o
#### Description:
日本語
TreasureTroveMarketはフリーマーケットサイトです。
アカウントを作成することで商品を購入したり、自分で出品したりすることもできます。
なぜこのプロジェクトにしたかというと、既存のサービスでは出品できない商品を扱えるプラットフォームが欲しいと思ったからです。
既存のサービスでは扱えない商品については学術的な場であるSC50にふさわしくないのであえて伏せておきます。
このような商品を扱えるCtoCプラットフォームがないのであれば自分で作ろうと考えこのプロジェクトを開始しました。

### 各ファイルの説明
#### app.py
このファイルは主要な関数（ルートハンドラー）を記述したものです。

#### myapp.db
このファイルはデータベースファイルです。ユーザー情報を記録するusersテーブルと商品情報を記録するitemsテーブルで構成されています。
テーブルの構成は以下の通りです。
user_idで商品と出品者を、item_idで商品と画像を関連付けています。
imagesテーブルでは画像ファイルへのパスを保管しています。
CREATE TABLE users (
     id INTEGER PRIMARY KEY AUTOINCREMENT,
     name TEXT NOT NULL,
     email TEXT NOT NULL,
     pass TEXT NOT NULL,
     post_code INTEGER NOT NULL,
     address TEXT NOT NULL,
     create_at DATETIME,
     update_at DATETIME
);
CREATE TABLE items(
item_id INTEGER PRIMARY KEY,
user_id INTEGER NOT NULL,
item_name TEXT NOT NULL,
description TEXT NOT NULL,
status BOOLEAN
, price numeric
);
CREATE TABLE images(
image_id INTEGER PRIMARY KEY,
item_id INTEGER REFERENCES items(item_id),
photo TEXT
);

#### template/buy.html
商品購入時に確認を行うページを記述したファイルです。
### template/buyed.html
このファイルは商品購入時に購入処理が正常に終わったことを知らせるページを記述したものです。

#### template/edit.html
商品情報を修正するページを記述したファイルです。
このページから商品の削除も可能です。また、削除時にJavascriptでアラートで確認を取るようにしてあります。

#### template/error.html
DB操作時などにエラーが発生した場合このページを表示します。

#### template/index.html
このアプリのトップページを記述してあるファイルです。
出品済みの商品が一覧として表示されます。
ログイン前はナビゲーションバーがログインとアカウント登録のみですが、ログインすることで、
商品の購入や出品が可能となります。

#### template/item.html
商品ページを記述するファイルです。

#### template/layout.html
書くhtmlファイルに共通するテンプレート部分を記述したファイルです。

#### template/list.html
商品出品ページを記述したファイルです。

#### tamplate/login.html
ログインページを記述したファイルです。

#### template/mypage.html
個人用のページを記述したファイルです。
このページではメールアドレスや住所などアプリに登録した情報以外に、自分が出品した商品の一覧が見られます。

#### template/register.html
アカウント登録のページです。

#### static/static.js
index.htmlとmypage.htmlはリンクをクリックすることでその商品の個別ページにアクセスできるようにしてあります。
フォームを使用していないため、JavaScriptを使って商品情報をPythonファイルに渡しています。
そのコードを記述しているファイルです。

#### static/style.css
index.htmlとmypage.htmlは商品の一覧を表示しているのですが、それらをきれいに並べるためのCSSを記述しています。

#### static/image/
このディレクトリは出品されている商品の画像を保管するためのディレクトリです。

#### その他ファイルやディレクトリ
flaskアプリが自動で作ったものです。

### 設計等の思想
WEBアプリ開発についてはとにかくシンプルにして、自力で作りきることを目的に作りました。

### その他参考にしたもの
#### SQLのクエリについて参考にしたサイト
https://products.sint.co.jp/topsic/blog/sql-insert
https://products.sint.co.jp/topsic/blog/sql-delete
https://products.sint.co.jp/topsic/blog/sql-update
https://atmarkit.itmedia.co.jp/ait/articles/1201/13/news140.html
https://www.javadrive.jp/sqlite/table/index4.html#section2

#### app.py
##### ログイン状態のみ許可するでデコレータ。
cs50ddbが提案してくれたコードを使用

##### login()関数
CS50 Problem set9 finance のlogin()と下記のサイトを参考にしました。
https://engineer-lifestyle-blog.com/code/python/flask-login-signup-function/

##### メール送信をする関数send_mail()
https://jp-seemore.com/iot/python/10295/#toc9
https://zenn.dev/daddy_yukio/articles/8c6a15fc09548a

#### template/edit.html
 https://www.sejuku.net/blog/29821

#### マークダウン記法について
https://backlog.com/ja/blog/how-to-write-markdown/

### 使用時の注意
#### app.py send_mail()について
app.py 341行目と344行目にこのようなコードがある。
必ずSender's email addressを自分のGmailのアドレス、Your passwordを自分のGmailアカウントのパスワードに置き換えること。
server.login("Sender's email address", "Your password")
server.sendmail("Sender's email address", mail_address,  msg.as_string())

また、メールアドレスとパスワードが正しいのに535エラーが出るときはいアプリパスワードを使用する。　　
アプリパスワードは2段階認証が有効になっているGoogleアカウントでないと設定できない。　　
参考サイト
https://www.howtonote.jp/google-account/2step-verify/index6.html


## 最後に
CS50の講義動画やCS50's duck debuggerにはとてもお世話になりました。
わからないことがあったら動画を見返したり、CS50's duck debuggerに相談したりしました。
おかげで自力でプロジェクトを提出可能な段階まで仕上げることができたことをとてもうれしく思います。

また、プロジェクト紹介の動画で使用したスライドも公開しておきます。
https://drive.google.com/drive/folders/12FQLDRwC1EEXV7_JCouIv0UEmevNlZzo?usp=drive_link
