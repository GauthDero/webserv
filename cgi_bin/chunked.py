#!/usr/bin/python3
import os
import sys

method = os.environ.get("REQUEST_METHOD", "")

print("Content-Type: text/html")
print()
if (method != "POST"):
	print("<html><body><h1> Method not allowed </h1></body></html>")
else:
	content_length = os.environ.get("CONTENT_LENGTH", "")
	if (content_length == ""):
		print("<html><body><h1> No content length has been provided </h1></body></html>")
	else:
		print("<html><body><table>")
		for key, value in os.environ.items():
			print(f"""<tr><td>{key}</td><td>{value}</td></tr>\n""")
		print("</table><br>")

		print("Script: Reading...", file=sys.stderr)
		chunk = sys.stdin.read(int(content_length))
		print("Script: Read", file=sys.stderr)
		if not chunk:  # EOF
			print("Script: No data sent", file=sys.stderr)
		else:
			print("Script: We got data baby", file=sys.stderr)
			print("<h1>CGI Script Output</h1>")
			print("<p><strong>Data received:</strong></p>")
			print("<pre style='white-space: pre-wrap; word-wrap: break-word;'>")
			print(chunk)
			print("</pre>")
			print(f"Script: Read: {chunk.__len__()}", file=sys.stderr)
		print("Script: End script", file=sys.stderr)
		print("</body></html>")
		sys.exit(0)