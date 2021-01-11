# Website

## Configuring the pi
### php server
1. $ apt install apache2 php libapache2-mod-php php-mycrypt php-cli php-mysql php-gd php-imagick php-recode php-tidy php-xmlrpc
2. Question yourself for installing all these programs, probably only the first 2 are essential. This is unless we add a database later on, then the mysql one might come in handy.
3. Create a symlink in /var/www to ~/PJSVD/html using the following commands:
	1. $ rm -r /var/www/html
	2. $ ln -s ~/PJVD/html /var/www
4. enjoy our glorious site which automagically updates when the git repo get's pulled

### SQL database
1. $ apt install mariadb
2. $ sudo mysql
3. > CREATE USER 'PJSDV'@localhost IDENTIFIED BY 'gaatjeniksaan';
4. > GRANT ALL PRIVILEGES ON 'PJSDV'.* TO 'PJSDV'@localhost IDENTIFIED BY 'gaatjeniksaan';
5. > FLUSH PRIVILEGES;
6. > [plak inhoud bestand db.sql]
7. > quit

## Structure of the site
First of all the page the site is on is managed by the variable "$\_GET['p']" to create a more clean experience without showing any .php files in the url.
To add an additional page you can simply add it to the pages folder and to reach it you need to link to the page on another page or you can simply add "?p=[page name]" at the end of the url. The page will automagically link to a stylesheet with the same name as the page so you can get custom styling for elements on the specific page.
