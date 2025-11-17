#!/usr/bin/python3
import cgi
import os

# Get the query string from the environment variable
query_string = os.environ.get("QUERY_STRING", "")

method = os.environ.get("REQUEST_METHOD", "")

if (method != "GET"):
	print("Content-Type: text/html")
	print()
	print("<html><body><h1> Method not allowed </h1></body></html>")
else:
	# Parse the query string using cgi.FieldStorage
	form = cgi.FieldStorage()

	# Get the value of 'name' parameter, defaulting to 'World' if not provided
	name = form.getvalue("name", "World")

	# Print the HTTP headers
	print("Content-Type: text/html")
	print()  # Blank line separating headers from body

	# Print the HTML response with variable substitution
	print(f"<html><body><h1>Hello, {name}!</h1></body></html>")

