#!/usr/bin/python3
import cgi
import os

print("Content-Type: text/html")
print()

print("<html>\n<body>\n<table>\n")
for key, value in os.environ.items():
	print(f"""<tr><td>{key}</td><td>{value}</td></tr>\n""")
print("</table>\n</body>\n</html>")

