import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    user_id = session["user_id"]
    # ユーザー情報を取得
    money = db.execute("SELECT cash FROM users WHERE id = ?", user_id)
    cash = float(money[0]["cash"])

    # ユーザーの取引履歴を取得
    transactions = db.execute("SELECT * FROM transactions WHERE user_id = ?", user_id)

    # 各株の現在の価格を取得する
    current_prices = {}
    total_shares = {}
    for transaction in transactions:
        symbol = transaction["stock_symbol"]
        stock_info = lookup(symbol)
        current_price = stock_info["price"]
        current_prices[symbol] = current_price
        if symbol not in total_shares:
            total_shares[symbol] = 0
        # 所有している株の数をカウント
        total_shares[symbol] += transaction["num_shares"]

    total_values = {}# 所有している株の数をカウントする辞書型変数
    grand_total = 0 # 所有している株の金額の合計と残高の総計を入れる変数
    check_shares = set()
    # 所有している株の合計をそれぞれ求める
    for transaction in transactions:
        symbol = transaction["stock_symbol"]
        total_values[symbol] = total_shares[symbol] * current_prices[symbol] # その銘柄の所有額を計算
        grand_total = total_values[symbol] + grand_total # 総計に加算

    cash_float = float(cash)
    cash = "{:.2f}".format(cash)
    grand_total = cash_float + grand_total
    grand_total = "{:.2f}".format(grand_total)
    # 個人用画面を表示する。
    return render_template("index.html",cash=cash, transactions=transactions, current_prices=current_prices, total_values=total_values, grand_total=grand_total, total_shares=total_shares)
    # return apology("TODO")


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
        # POSTによる要求なら
    if request.method == "POST":
        # 受け取ったシンボルを変数に格納し、APIで検索する。
        symbol = request.form.get("symbol")
        number = request.form.get("shares")
        try:
            num_check = float(number)
        except ValueError:
            return apology("Please enter an integer.")
        number = int (float(number)) # htmlからのデータは文字列型で送信されるのでキャストする。
        if (num_check - number) > 0:
            return apology("Please enter an integer.")
        if number < 0:
            return apology("Please enter a positive number.")
        result = lookup(symbol)
        if result == None: # 見つからなかったらエラーを表示
            return apology("symbol not found")

        user_id = session["user_id"]
        money = db.execute("SELECT cash FROM users WHERE id = ?", user_id)

        # 所持金が１株の金額より低かったら
        if money[0]["cash"] < result["price"]:
            return apology("Insufficient cash")

        # 個数が整数以外だったら
        try:
            cash = int(money[0]["cash"])
        except ValueError:
            return apology("Please enter the correct number")

        # 支払う金額を計算
        amount = result["price"] * number
        #支払い後の所持金を計算
        cash = cash - amount
        try:
            db.execute("UPDATE users SET cash = ? WHERE id = ?", cash, user_id)
            db.execute("INSERT INTO transactions (user_id, stock_symbol, num_shares, transaction_price, transaction_time) VALUES (?, ?, ?, ?, CURRENT_TIMESTAMP)", user_id,  result["symbol"], number, result["price"])

        except Exception as e:
            return apology(f"An unexpected error occurred: {str(e)}")
            #return apology("Transaction failed.")
        cash2 = "{:.2f}".format(cash)
        #return render_template("buy.html", cash=cash2)
        return redirect("/")
    else:
        # GETによる要求ならbuy.htmlをレンダリング
        user_id = session["user_id"]
        money = db.execute("SELECT cash FROM users WHERE id = ?", user_id)
        cash = int(money[0]["cash"])
        cash2 = "{:.2f}".format(cash)
        return render_template("buy.html", cash=cash2)


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    user_id = session["user_id"]
    logs = db.execute("SELECT * FROM transactions WHERE user_id = ?", user_id)
    for log in logs:
        print(type(log["num_shares"]))
    return render_template("history.html", logs=logs)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute(
            "SELECT * FROM users WHERE username = ?", request.form.get("username")
        )

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(
            rows[0]["hash"], request.form.get("password")
        ):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    if request.method == "POST":
        symbol = request.form.get("symbol")
        result = lookup(symbol)
        # 結果が無かったら謝辞を表示
        if result == None:
            return apology("symbol not found")
        # 小数第二位まで表示するようにフォーマット
        price = "{:.2f}".format(result["price"])
        return render_template("quoted.html", price = price, symbol = result["symbol"])

    else:
         return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    if request.method == "POST": # POSTメソッドのリクエストだったら
        # 送信されたデータをpythonの変数に格納
        user_name = request.form.get("username")

        pass_word = request.form.get("password")
        pass_word2 =request.form.get("confirmation")

        # ユーザー名が空なら謝辞を表示
        if not user_name:
            return apology("名前が入力されていません")

        # 入力された名前でDBを検索して、ヒットしたら謝辞を表示
        result = db.execute("SELECT * FROM users WHERE username = ?", user_name)
        if result:
            return apology("Username already taken")

        # パスワードが入力されていなかったら謝辞を表示
        if not pass_word or not pass_word2:
            return apology("パスワードが入力されていません")

        # 入力されたパスワードが一致しなかったら謝辞を表示
        if pass_word != pass_word2:
            return apology("パスワードが一致しません")

        #パスワードをハッシュ化
        hash = generate_password_hash(pass_word)

        # DBに名前とパスワードを登録する。エラー発生時は例外処理で謝辞を返す
        try:
            db.execute("INSERT INTO users (username, hash) VALUES(?, ?)", user_name, hash)
        except:
            return apology("登録に失敗しました。")

        return redirect("/")
    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    if request.method == "POST":
        user_id = session["user_id"]
        sell_symbol = request.form.get("symbol")
        number = float(request.form.get("shares"))
        #number = float(number)
        if not number:
            return apology("Please enter the correct number.")
        if number < 0: # 売りたい数が負の数だったら
            return apology("Please enter a positive number.")
        transactions = db.execute("SELECT * FROM transactions WHERE user_id = ? AND stock_symbol = ?", user_id, sell_symbol)
        sum_shares = 0
        for transaction in transactions:
            sum_shares += int(transaction['num_shares'])

        if number > sum_shares: # 売りたい株の数が所有している数より多かったら
            return apology("I'm trying to sell more than I own.")
        money = db.execute("SELECT cash FROM users WHERE id = ?", user_id)
        cash = float(money[0]["cash"])
        stock_info = lookup(sell_symbol)
        if not stock_info:
            return apology("Please enter a valid symbol.")
        price = float(stock_info["price"])
        cash = cash + (number * price)
        try:
            db.execute("UPDATE users SET cash = ? WHERE id = ?", cash, user_id)
            db.execute("INSERT INTO transactions(user_id, stock_symbol, num_shares, transaction_price, transaction_time) VALUES (?, ?, ?, ?, CURRENT_TIMESTAMP)", user_id, stock_info["symbol"], -number, price)
        except Exception as e:
            print("エラーは")
            print(e)
            return apology("Processing failed.")

        return redirect("/")
    else:
        # ユーザIDを取得し、それをもとにDBから所有している株の情報を取得する
        user_id = session["user_id"]
        symbols = db.execute("SELECT stock_symbol FROM transactions WHERE user_id = ?", user_id)
        stocks = set([symbol['stock_symbol'] for symbol in symbols])
        return render_template("sell.html",stocks=stocks)


@app.route("/change_password", methods=["GET", "POST"])
@login_required
def change_password():
    if request.method == "POST":
        user_id = session["user_id"]
        pass_word = request.form.get("password")
        pass_word2 =request.form.get("password2")
         # パスワードが入力されていなかったら謝辞を表示
        if not pass_word or not pass_word2:
            return apology("パスワードが入力されていません")

        # 入力されたパスワードが一致しなかったら謝辞を表示
        if pass_word != pass_word2:
            return apology("パスワードが一致しません")

        #パスワードをハッシュ化
        hash = generate_password_hash(pass_word)

        try:
            db.execute("UPDATE users SET hash = ? WHERE id = ?", hash, user_id)
        except:
            return apology("Processing failed.")
        return redirect("/")
    else:
        return render_template("change.html")
