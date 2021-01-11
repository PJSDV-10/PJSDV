CREATE SCHEMA PJSDV;

CREATE TABLE users (
	usr_ID int NOT NULL AUTO_INCREMENT,
	username varchar(255) NOT NULL,
	password varchar(255) NOT NULL,
	room_number int DEFAULT 0,
	PRIMARY KEY (usr_ID),
	UNIQUE (usr_ID, username)
);
