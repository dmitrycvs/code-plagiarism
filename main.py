import sqlite3
import os

inputs_path = os.path.join(os.path.dirname(__file__), 'inputs')
file_paths = []

for input_file in os.listdir(inputs_path):
    filepath = os.path.join(inputs_path, input_file)
    file_paths.append(filepath)

conn = sqlite3.connect('inputs.db')
cursor = conn.cursor()

cursor.execute("CREATE TABLE IF NOT EXISTS files (id INTEGER PRIMARY KEY, content TEXT)")

cursor.execute("DELETE FROM files")

conn.commit()

for path in file_paths:
    with open(path, "r") as file:
        content = file.read()
        if content:
            content = content.replace("\n", " ").replace("\t", " ")
            cursor.execute("INSERT INTO files (content) VALUES (?)", (content,))

conn.commit()

conn.close()