import sqlite3


def get_user(username, password):
    conn = sqlite3.connect('users.db')
    cursor = conn.cursor()
    query = "SELECT * FROM users WHERE username = '" + username + "' AND password = '" + password + "'"
    print(query)
    cursor.execute(query)
    return cursor.fetchall()



conn = sqlite3.connect('users.db')
cursor = conn.cursor()
query = "CREATE TABLE users (username TEXT, password TEXT)"
cursor.execute(query)
query1="INSERT INTO users (username, password) VALUES ('1','ABCD')"
query2="INSERT INTO users (username, password) VALUES ('2','BCDE')"
query3="INSERT INTO users (username, password) VALUES ('3','CDEF')"
query4="select * from users"
cursor.execute(query1)
cursor.execute(query2)
cursor.execute(query3)
cursor.execute(query4)
conn.commit()
print(cursor.fetchone())


print("Input the username")
username = input("Enter username:")
print("Input the password")
password = input("Enter password:")
data=get_user(username,password )
print(data)    
    
