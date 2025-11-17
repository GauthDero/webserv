#!/usr/bin/python3

import time
import sys

for i in range(15):
	print(f"Sleeping... {i}", file=sys.stderr)
	time.sleep(1)