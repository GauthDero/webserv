#!/usr/bin/php
<?php
	// Print the HTTP headers and HTML response
	echo 'Content-Type: text/html';
	echo "\n\n";
	echo "<html>\n<body>\n<table>\n";
	foreach ($_SERVER as $key => $value)
	{
		echo "<tr><td>" . $key . "</td><td>" . $value . "</td></tr>\n";
	}
	echo "</table>\n</body>\n</html>";
?>
