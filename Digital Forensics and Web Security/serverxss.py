from markupsafe import escape
from flask import Flask, request, render_template_string

app = Flask(__name__)

@app.route('/', methods=['GET', 'POST'])
def login():
    if request.method == 'POST':
        username = request.form.get('username', '')
	safe_username = escape(username)
        # Vulnerable: Directly embedding user input into HTML
        message = f'Welcome, {safe_username}!'
        return render_template_string('''
            <h1>Login Result</h1>
            <p>{{ message }}</p>
            <a href="/">Back to Login</a>
        ''', message=message)
    return '''
        <h1>Login Form</h1>
        <form method="post">
            <label>Username: </label>
            <input type="text" name="username"><br>
            <input type="submit" value="Login">
        </form>
    '''

if __name__ == '__main__':
    app.run(debug=True, host='0.0.0.0', port=5000)