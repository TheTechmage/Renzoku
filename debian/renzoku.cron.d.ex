#
# Regular cron jobs for the renzoku package
#
0 4	* * *	root	[ -x /usr/bin/renzoku_maintenance ] && /usr/bin/renzoku_maintenance
