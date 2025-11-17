#!/bin/bash

# Get the request method
method="${REQUEST_METHOD}"

# Get the query string
query_string="${QUERY_STRING}"

# Parse the 'name' parameter from the query string
name="World"
IFS='&' read -ra params <<< "$query_string"
for param in "${params[@]}"; do
	key="${param%%=*}"
	value="${param#*=}"
	if [ "$key" = "name" ]; then
		name=$(printf '%b' "${value//%/\\x}")
		break
	fi
done

# Output the response
echo "Content-Type: text/html"
echo

if [ "$method" != "GET" ]; then
	echo "<html><body><h1>Method not allowed</h1></body></html>"
else
	echo "<html><body><h1>Hello, ${name}!</h1></body></html>"
fi