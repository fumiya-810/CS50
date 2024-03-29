# https://engineer-lifestyle-blog.com/code/python/flask-login-signup-function/
# このサイトを参考にログイン機能を作成
import os
import smtplib
# cs50 第9回課題financeのapp.pyと同様にインポートただし、helpers以外
from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from werkzeug.security import check_password_hash, generate_password_hash
from flask_sqlalchemy import SQLAlchemy
import traceback
from email.mime.multipart import MIMEMultipart
from email.mime.text import MIMEText


app = Flask(__name__)

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

db = SQL("sqlite:///myapp.db")

# ログイン状態のみ許可するでデコレータ。cs50ddbの提案コードを使用
from functools import wraps
from flask import session, redirect, url_for



def login_required(f):
    @wraps(f)
    def decorated_function(*args, **kwargs):
        if "user_id" not in session:
            return redirect(url_for('login', next=request.url))
        return f(*args, **kwargs)
    return decorated_function


@app.route('/')
def index():
    try:
        items = db.execute("SELECT * FROM items")
        images = db.execute("SELECT * FROM images")

    except Exception as e:
        message = "処理に失敗しました。"
        message = e
        return render_template("error.html", message=message)

    return render_template("index.html", items=items, photos=images)


@app.route('/register', methods=['GET', 'POST'])
def register():
    if request.method == "POST":
        name = request.form.get('username')
        email = request.form.get('email')
        password = request.form.get('password')
        password2 = request.form.get('password2')
        post_code = request.form.get('post_code')
        address = request.form.get('address')
        if not name or not email or not password or not password2 or not post_code or not address:
            message = "すべての項目を正しく入力してください。"
            return render_template('error.html', message=message)

        if password == password2:
            message = "パスワードが一致しません。"
            render_template('error.html', nessage=message)
        if not password or not password2:
            message = "パスワードが入力されていません。"
            render_template("error.html", message=message)

        result = db.execute("SELECT * FROM users WHERE email = ?", email)
        if result:
            message = "すでにメールアドレスが登録されています。"
            return render_template("error.html", message=message)
        try:
            hash = generate_password_hash(password)
            db.execute("INSERT INTO users (name, email, pass, post_code, address, create_at, update_at) VALUES (?, ?, ?, ?, ?,CURRENT_TIMESTAMP, CURRENT_TIMESTAMP)",
                       name, email, hash, post_code, address)
        except:
            message = "登録に失敗しました。"
            return render_template("error.html", message=message)

        return redirect("/")
    else:
        return render_template('register.html')


@app.route('/login', methods=['GET', 'POST'])
def login():
    session.clear()
    if request.method == "POST":
        email = request.form.get('email')
        password = request.form.get('password')
        if not email:
            message = "メールアドレスが入力されていません。"
            return render_template("error.html", message=message)

        if not password:
            message = "パスワードが入力されていません。"
            return render_template("error.html", message=message)

        rows = db.execute(
            "SELECT * FROM users WHERE email = ?", request.form.get("email")
        )

        # ここから　cs50 week9 finance app.pyより
        if len(rows) != 1 or not check_password_hash(
            rows[0]["pass"], request.form.get("password")
        ):
            message = "メールアドレスもしくはパスワードが無効です。"
            return render_template("error.html", message=message)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]
        return redirect('/')
        # ここまで
    else:
        return render_template('login.html')

# ログアウト機能sc50 week9 financeより


@app.route("/logout")
@login_required
def logout():
    session.clear()
    return redirect("/")


@app.route("/mypage")
@login_required
def mypage():
    user_id = session["user_id"]
    print(f"user_id :{user_id}")
    photos = None
    try:
        # ユーザーの情報を取得
        user = db.execute("SELECT * FROM users WHERE id = ?", user_id)
        # 商品情報を取得
        myitems = db.execute("SELECT * FROM items WHERE user_id = ?", user_id)
        for item in myitems:
            photos = db.execute("SELECT photo FROM images WHERE item_id = ?", item['item_id'])
    except Exception as e:
       # message="処理に失敗しました。"
        message = e
        return render_template("error.html", message=message)

    name = user[0]["name"]
    email = user[0]["email"]
    post_code = user[0]["post_code"]
    address = user[0]["address"]
    return render_template("mypage.html", name=name, email=email, post_code=post_code, address=address, items=myitems, photos=photos)


# 出品ページのルーティング
@app.route('/list', methods=['GET', 'POST'])
@login_required
def list():
    if request.method == "POST":
        user_id = session["user_id"]
        item_name = request.form.get('item_name')
        description = request.form.get('description')
        image = request.files.get('image')
        price = request.form.get('price')
        # もし画像が送られてきていたら
        if image:
            filepath = os.path.join('./static/image', image.filename)
            image.save(filepath)  # 画像を保存
        else:
            message = "画像を選択してください。"
            return render_template("error.html", message=message)
        if not price:
            message = "値段を入力してください。"
            return render_template("error.html", message=message)
        try:
            # 商品の商品のデータを保存
            db.execute("INSERT INTO items (user_id, item_name, description, status, price) VALUES (?, ?, ?, true, ?)",
                       user_id, item_name, description, price)
            # 今保存した商品のidを取得
            result = db.execute("SELECT last_insert_rowid()")
            item_id = result[0]['last_insert_rowid()']
            # 画像のパスをDBに追加
            db.execute("INSERT INTO images (item_id, photo) VALUES (?, ?)", item_id, filepath)

        except Exception as e:
            message = "出品処理に失敗しました。"
            return render_template("error.html", message=e)
        # render_templateだとセッション情報が更新される前にページが読み込まれる。
        # そのためマイページで情報が表示されなくなる。
        return redirect("/mypage")
    else:
        return render_template("list.html")


@app.route('/item', methods=['GET', 'POST'])
@login_required
def item():
    user_id = session["user_id"]
    if request.method == "POST":
        item_id = request.form.get('item_id')
        try:
            item = db.execute("SELECT * FROM items WHERE item_id = ?", item_id)
            item_images = db.execute("SELECT * FROM images WHERE item_id = ?", item_id)
            # db.executeの返り値は辞書型配列なので０番目（最初）の要素を指定し、
            # 通常の辞書型変数として格納しなおす
            item_info = item[0]
        except Exception as e:
            message = "商品画面から確認画面への遷移の途中で処理が失敗しました。"
            return render_template("error.html", message=message)

        return render_template("buy.html", item_info=item_info, item_images=item_images, user_id=user_id)

    else:
        item_id = None
        item_id = request.args.get('item_id')
        try:
            item = db.execute("SELECT * FROM items WHERE item_id = ?", item_id)
            item_images = db.execute("SELECT * FROM images WHERE item_id = ?", item_id)
            item_info = item[0]
        except Exception as e:
            message = "処理に失敗しました。"
            message = e
            return render_template("error.html", message=message)

        return render_template("item.html", item_info=item_info, item_images=item_images, user_id=user_id)


@app.route('/buyed', methods=['GET', 'POST'])
@login_required
def buyed():
    item_id = request.form.get('item_id')
    buyer_id = request.form.get('user_id')
    print(f"item_id :{item_id}")
    print(f"buyer_id{buyer_id}")
    try:
        # 購入した商品の情報を取得
        item = db.execute("SELECT * FROM items WHERE item_id = ?", item_id)
        item_images = db.execute("SELECT * FROM images WHERE item_id = ?", item_id)

        # 購入された商品の状態を売り切れ状態にする
        db.execute("UPDATE items SET status = false WHERE item_id = ?", item_id)
        # db.executeの返り値は辞書型配列なので０番目（最初）の要素を指定し、
        # 通常の辞書型変数として格納しなおす
        item_info = item[0]

        # 出品者のメールアドレスを取得し変数に格納
        sellser_address = db.execute("SELECT email FROM users WHERE id = ?", item_info["user_id"])
        mail_address = sellser_address[0]["email"]

        # 購入者の情報を取得して変数に格納
        buyer_info = db.execute("SELECT name, email, address FROM users WHERE id = ?", buyer_id)
        buyer_name = buyer_info[0]["name"]
        buyer_mail = buyer_info[0]["email"]
        buyer_address = buyer_info[0]["address"]
        text = "商品が購入されました。発送作業を行ってください。\n"+"購入者 :" + buyer_name + "\n" + \
            "商品 :" + item_info["item_name"] + "\n" + "発送先 :" + buyer_address

        # 出品者にメール送信
        subject = "商品が購入されました。"
        send_mail(mail_address, text, subject)
        # 購入者にメール送信
        subject = "商品を購入しました。"
        text = "商品を購入しました。\n" + "購入した商品 :" + item_info["item_name"]
        send_mail(buyer_mail, text, subject)

    except Exception as e:
        message = "確認画面から購入後画面への遷移の途中で処理が失敗しました。"
        print(e)
        traceback.print_exc()
        return render_template("error.html", message=message)

    return render_template("buyed.html", item_info=item_info, item_images=item_images)


@app.route('/edit', methods=['GET', 'POST'])
@login_required
def edit():
    item_info = None
    item_images = None
    user_id = session["user_id"]
    if request.method == "POST":
        item_id = request.form.get('item_id')
        item_name = request.form.get('item_name')
        description = request.form.get('description')
        price = request.form.get('price')
        try:
            # 商品情報を更新
            db.execute("UPDATE items SET item_name = ?, description = ?, price = ? WHERE item_id = ?", item_name,  description, price, item_id)
        except:
            message = "商品情報の更新に失敗しました。"
        return redirect("/mypage")
    else:
        item_id = request.args.get('item_id')
        try:
            item_info = db.execute("SELECT * FROM items WHERE item_id = ?", item_id)
            item_images = db.execute("SELECT * FROM images WHERE item_id = ?", item_id)
        except:
            message = "商品情報の取得に失敗しました。"
            return render_template("error.html", message=message)
        # item_infoとitem_imagesに値がある場合は編集画面を
        if item_info and item_images:
            return render_template("edit.html", item_info=item_info[0], item_images=item_images, user_id=user_id)
        else:
            message = "商品が見つかりません"
            return render_template("error.html", message=message)


@app.route('/delete', methods=['GET', 'POST'])
@login_required
def delete():
    try:
        item_id = request.form.get('item_id')
        db.execute("DELETE FROM images WHERE item_id = ?", item_id)
        db.execute("DELETE FROM items WHERE item_id = ?", item_id)
        return redirect("/mypage")
    except:
        message = "商品の削除に失敗しました。"
        return render_template("error.html", message=message)

    # メールを作成して送信する関数
    # 参考サイト
    # https://jp-seemore.com/iot/python/10295/#toc9
    # https://zenn.dev/daddy_yukio/articles/8c6a15fc09548a
    # エラーメモ
    # 535エラーが出てメールが送信できない時はアプリパスワードを設定する
    # アプリパスワードは2段階認証が有効になっているGoogleアカウントでないと設定できない。
def send_mail(mail_address, text, subject):
    try:
        msg = MIMEMultipart()
        # メールを作成
        msg['From'] = 'fumiya.sute01@gmail.com'
        msg['To'] = mail_address
        msg['Subject'] = subject
        body = text
        msg.attach(MIMEText(body, 'plain'))
        # SMTPサーバーを設定してログインする
        server = smtplib.SMTP('smtp.gmail.com', 587)
        server.starttls()
        # 送信者のアドレスとパスワード
        # コードを共有するときは必ず隠すこと
        server.login("Sender's email address", "Your password")

        # 本文を作成したメールに付け加える
        server.sendmail("Sender's email address", mail_address,  msg.as_string())

        # サーバーへの接続を閉じる
        server.quit()
    except smtplib.SMTPException as e:
        print("Error: unable to send email. Details:", e)
