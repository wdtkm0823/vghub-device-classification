from flask import Flask, render_template
import run

app = Flask(__name__)

@app.route('/')
def index():
    return render_template('index.html', device= run.class_name)