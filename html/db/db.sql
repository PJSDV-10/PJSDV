CREATE SCHEMA PJSDV;

CREATE TABLE users (
	usr_ID int NOT NULL AUTO_INCREMENT,
	username varchar(255) NOT NULL,
	password varchar(255) NOT NULL,
	room_number int DEFAULT 0,
	PRIMARY KEY (usr_ID),
	UNIQUE (usr_ID, username)
);

CREATE TABLE rooms (
	room_number int NOT NULL,
	types int NOT NULL,
	PRIMARY KEY (room_number),
	UNIQUE (room_number)
);

CREATE TABLE devices (
	type_number int NOT NULL,
	device_name varchar(255) NOT NULL,
	PRIMARY KEY (type_number),
	UNIQUE (type_number)
);
