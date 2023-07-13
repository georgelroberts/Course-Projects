from sql import SQL
from flask import Flask
from passlib.apps import custom_app_context as pwd_context
import os

from helpers import *

# configure application
app = Flask(__name__)

# ensure responses aren't cached
if app.config["DEBUG"]:
    @app.after_request
    def after_request(response):
        response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
        response.headers["Expires"] = 0
        response.headers["Pragma"] = "no-cache"
        return response

# custom filter
app.jinja_env.filters["usd"] = usd

# create a random key for the session
app.secret_key = os.urandom(24)

# configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

@app.route("/")
@login_required
def index():
    """List all of the items from the portfolio database, alongside current funds and the total worth"""
    symbols = db.execute("SELECT symbol FROM portfolio WHERE id = :idsession GROUP BY symbol",
                         idsession=session["user_id"])
    indexinfo = []
    currentcash = 0
    grandtotal=0

    if symbols:
        counter = 0
        for symbol in symbols:
            # uses yahoo to search for the company based on the symbol
            currentstockinfo = lookup(symbol['symbol'])
            noshares = db.execute("SELECT SUM(noShares) FROM portfolio where symbol=:symbol AND id=:idsession",
                                  symbol=symbol["symbol"], idsession=session["user_id"])
            if noshares[0]['SUM(noShares)'] > 0:
                counter += 1
                currentstock = {'symbol': currentstockinfo['symbol'], 'name': currentstockinfo['name'],
                                'shares': noshares[0]['SUM(noShares)'], 'price': currentstockinfo['price']}
                currentstock['total'] = currentstock['price'] * currentstock['shares']

                indexinfo.append(currentstock)

        usercash = db.execute("SELECT cash FROM users where id=:idsession", idsession=session["user_id"])
        currentcash = usercash[0]["cash"]
        grandtotal = usercash[0]["cash"]

        for i in range(counter):
            grandtotal += indexinfo[i]['total']

        for i in range(counter):
            indexinfo[i]['price'] = usd(indexinfo[i]['price'])
            indexinfo[i]['total'] = usd(indexinfo[i]['total'])

    return render_template("index.html", holdings=indexinfo, currentCash=usd(currentcash), grandTotal=usd(grandtotal))

@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Allow user to purchase shares of real companies"""
    if request.method != "POST":
        return render_template("buy.html")
    if request.form.get("symbol") == "":
        return render_template("apology.html", message="Please enter a symbol!")
    if request.form.get("shares") == "" or request.form.get("shares").isdigit() is False or int(
                request.form.get("shares")) < 1:
        return render_template("apology.html", message="Please enter number of shares!")
    sharedetails = lookup(request.form.get("symbol"))
    if sharedetails is None:
        return render_template("apology.html", message="Enter a correct symbol")
    currentcash = db.execute("SELECT cash FROM users WHERE id = :idsession", idsession=session["user_id"])

    if (
        float(request.form.get("shares")) * sharedetails["price"]
        >= currentcash[0]["cash"]
    ):
        return render_template("apology.html", message="Insufficient funds available")
    db.execute(
        "INSERT INTO portfolio (id,symbol,sharePrice,noShares,totalCost) VALUES (:id,:symbol,:sharePrice,:noShares,:totalCost)",
        id=session["user_id"], symbol=sharedetails["symbol"], sharePrice=sharedetails["price"],
        noShares=float(request.form.get("shares")),
        totalCost=float(request.form.get("shares")) * sharedetails["price"])
    db.execute("UPDATE users SET cash = cash - :totalCost where id=:idsession",
               totalCost=float(request.form.get("shares")) * sharedetails["price"],
               idsession=session["user_id"])
    return redirect(url_for("index"))


@app.route("/history")
@login_required
def history():
    """Return all user activity"""
    userhistory = db.execute("SELECT symbol,noShares,sharePrice,time FROM portfolio WHERE id=:sessionid",
                             sessionid=session["user_id"])
    return render_template("history.html", userHistory=userhistory)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in."""

    # forget any user_id
    session.clear()

    if request.method != "POST":
        return render_template("login.html")
    # ensure username was submitted
    if not request.form.get("username"):
        return render_template("apology.html", message="Must provide username")

    # ensure password was submitted
    elif not request.form.get("password"):
        return render_template("apology.html", message="must provide password")

    # query database for username
    rows = db.execute("SELECT * FROM users WHERE username = :username", username=request.form.get("username"))

    # ensure username exists and password is correct
    if len(rows) != 1 or not pwd_context.verify(request.form.get("password"), rows[0]["hash"]):
        return render_template("apology.html", message="invalid username and/or password")

    # remember which user has logged in
    session['user_id'] = rows[0]["id"]

    # redirect user to home page
    return redirect(url_for("index"))


@app.route("/logout")
def logout():
    """Log user out."""

    # forget any user_id
    session.clear()

    # redirect user to login form
    return redirect(url_for("login"))


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """ Find current share price of company"""
    if request.method != "POST":
        return render_template("quote.html")
    if request.form.get("symbol") == "":
        return render_template("apology.html", message="Please enter a symbol!")
    sharedetails = lookup(request.form.get("symbol"))
    return (
        render_template("apology.html", message="Enter a correct symbol")
        if sharedetails is None
        else render_template(
            "quoted.html",
            name=sharedetails["name"],
            symbol=sharedetails["symbol"],
            price=sharedetails["price"],
        )
    )


@app.route("/register", methods=["GET", "POST"])
def register():
    """ Register a username and password"""
    if request.method == "POST":
        if request.form.get("username") == "" or request.form.get("password") == "" or request.form.get(
                "confirmation") == "":
            return render_template("apology.html", message="Must enter a username, password and confirmation!")
        elif request.form.get("password") != request.form.get("confirmation"):
            return render_template("apology.html", message="Passwords must match!")
        else:
            # store a hash of the password for security
            passwordhash = pwd_context.encrypt(request.form.get("password"))
            rows = db.execute("SELECT * FROM users WHERE username = :username", username=request.form.get("username"))
            if len(rows) != 0:
                return render_template("apology.html", message="User already exists!")

            db.execute("INSERT INTO users (username,hash) VALUES (:username,:passwordHash)",
                       username=request.form.get("username"), passwordHash=passwordhash)
            session["user_id"] = \
                db.execute("SELECT * FROM users WHERE username = :username", username=request.form.get("username"))[0][
                    "id"]
            session["user_name"] = \
                db.execute("SELECT * FROM users WHERE username = :username", username=request.form.get("username"))[0][
                    "username"]
    return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """ Let user sell shares"""
    if request.method != "POST":
        return render_template("sell.html")
    if request.form.get("symbol") == "":
        return render_template("apology.html", message="Please enter a symbol!")
    if request.form.get("shares") == "" or request.form.get("shares").isdigit() is False or int(
                request.form.get("shares")) < 1:
        return render_template("apology.html", message="Please enter number of shares!")
    sharedetails = lookup(request.form.get("symbol"))
    if sharedetails is None:
        return render_template("apology.html", message="This company doesn't exist. Please try again")
    noshares = db.execute(
        "SELECT SUM(noShares) FROM portfolio where id=:idsession and symbol=:currentSymbol GROUP BY symbol",
        idsession=session["user_id"], currentSymbol=sharedetails["symbol"])
    if int(request.form.get("shares")) > noshares[0]["SUM(noShares)"]:
        return render_template("apology.html",
                               message="You have " + str(noshares[0]["SUM(noShares)"]) + " share(s) in " +
                                       sharedetails["name"] + " and are trying to sell " + request.form.get(
                                   "shares") + " shares. Please try again.")

    db.execute(
        "INSERT INTO portfolio (id,symbol,sharePrice,noShares,totalCost) VALUES (:id,:symbol,:sharePrice,:noShares,:totalCost)",
        id=session["user_id"], symbol=sharedetails["symbol"], sharePrice=sharedetails["price"],
        noShares=-1 * int(request.form.get("shares")),
        totalCost=float(request.form.get("shares")) * sharedetails["price"])
    db.execute("UPDATE users SET cash = cash + :totalCost where id=:sessionid",
               totalCost=float(request.form.get("shares")) * sharedetails["price"],
               sessionid=session["user_id"])
    return redirect(url_for("index"))


@app.route("/addcash", methods=["GET", "POST"])
@login_required
def addcash():
    """Let user add funds to their account"""
    if request.method != "POST":
        return render_template("addcash.html")
    if request.form.get("money") == "" or request.form.get("money").isnumeric() is False or int(
                request.form.get("money")) < 0:
        return render_template("apology.html", message="Please enter some money!")
    db.execute("UPDATE users SET cash = cash + :money where id=:sessionid",
               money=float(request.form.get("money")), sessionid=session["user_id"])
    return redirect(url_for("index"))

if __name__ == "__main__":
    app.debug = True
    app.run()

