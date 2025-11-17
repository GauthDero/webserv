#!/usr/bin/python3
import cgi
import os

# Create an instance of FieldStorage to parse the POST data
form = cgi.FieldStorage()

method = os.environ.get("REQUEST_METHOD", "")

if (method != "POST"):
	print("Content-Type: text/html")
	print()
	print("<html><body><h1> Method not allowed </h1></body></html>")
else:
# Retrieve parameters from the form
	name = form.getvalue("name", "World")  # Default to "World" if not provided
	age = form.getvalue("age", "Unknown")

	# Print the required HTTP header
	print("Content-Type: text/html")
	print()  # Blank line separating headers from body

	# Print the HTML response
	print(f"<html><body><h1>Hello, {name}! You are {age} years old.</h1></body></html>")
