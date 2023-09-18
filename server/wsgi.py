
from flask import Flask , make_response


app = Flask(__name__)


@app.route("/x")
def x():
    print("access")
    return "make_response"


if __name__ == '__main__':
    app.run(debug=True, host="0.0.0.0", port=80)