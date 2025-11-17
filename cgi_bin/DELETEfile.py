#!/usr/bin/python3

import os
import cgi
import os

method = os.environ.get("REQUEST_METHOD", "")

if (method != "DELETE"):
	print("Content-Type: text/html")
	print()
	print("<html><body><h1> Method not allowed </h1></body></html>")
else:
	print("Content-Type: text/html\n")

	form = cgi.FieldStorage()
	filename = form.getvalue("filename")
	query_string = os.environ.get("PATH_TRANSLATED", "")

	if not filename:
		print("<html><body><h3>Error: No filename provided.</h3></body></html>")
	else:
		try:
			os.remove(query_string)
			print(f"<html><body><h3>File '{filename}' removed successfully.</h3></body></html>")
		except Exception as e:
			print(f"<html><body><h3>Error removing file: {e}</h3></body></html>")
