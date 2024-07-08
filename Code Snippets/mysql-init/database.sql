create database code_snippets;
use code_snippets;

create table users (
	id int auto_increment primary key,
    name varchar(100) not null,
    age int not null,
    email varchar(100) not null,
    password varchar(32) not null
);

create table snippets (
	id int auto_increment primary key,
    user_id int,
    title varchar(255),
    code TEXT not null,
    type varchar(12) not null,
    created_at timestamp default current_timestamp,
    foreign key (user_id) references users(id)
);