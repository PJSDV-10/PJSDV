# Website

## Configuring the pi
1. $ apt install apache2 php libapache2-mod-php php-mycrypt php-cli php-mysql php-gd php-imagick php-recode php-tidy php-xmlrpc
2. Question yourself for installing all these programs, probably only the first 2 are essential. This is unless we add a database later on, then the mysql one might come in handy.
3. Create a symlink in /var/www to ~/PJSVD/html using the following commands:
	1. $ rm -r /var/www/html
	2. $ ln -s ~/PJVD/html /var/www
4. enjoy our glorious site which automagically updates when the git repo get's pulled

