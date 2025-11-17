#!/usr/bin/php
<?php
	for ($i = 0; $i < 15; $i++)
	{
		fwrite(STDERR, "Sleeping... {$i}\n");
		sleep(1);
	}

	echo "Content-Type: text/html";
	echo "\n\n";
	echo "<html><body><t1> What a good nap! </t1></body></html>"
?>