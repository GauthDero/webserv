#!/usr/bin/php
<?php

	parse_str($_SERVER['QUERY_STRING'], $_GET);

	// Get the request method
	$method = $_SERVER['REQUEST_METHOD'];

	if ($method !== 'GET')
	{
		echo 'Content-Type: text/html';
		echo "\n\n";
		echo "<html><body><h1>Method not allowed</h1></body></html>";
		exit;
	}

	// Get the 'name' parameter from the query string, default to 'World'
	$name = isset($_GET['name']) ? htmlspecialchars(string: $_GET['name']) : 'World';

	// Print the HTTP headers and HTML response
	echo 'Content-Type: text/html';
	echo "\n\n";
	echo "<html><body><h1>Hello, {$name}!</h1></body></html>";
?>
