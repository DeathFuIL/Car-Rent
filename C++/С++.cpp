/*
Приветствую тех, кто нашел это!
Так как не так хорошо владею С++ и изучаю Java, Python, C#, ->
то решил написать один и тот же проект на 4 разных языках программирования.
И таких проектов будет несколько
Тема проекта: аренда автомобилей
Возможности пользователей: 
-->Добавить свое авто для последующей его сдачи в аренду
-->Арендовать авто другого пользователя
-->Цена - х долларов/час (пользователь сам ставит стоимость)
-->Деньги на аккаунте пополняются, если другой пользователь взял ваш автомобиль
-->В итоге получается симулятор аренды автомобилей
-->Вы можете посмотреть кто взял ваш автомобиль или чей автомобиль вы взяли.
P.S. Проект имеет локальную значимость. Всех "персонажей" создаете вы. Шаблон к каждому файлу:
1) Admins.txt : login:password:0
2) Bans.txt : user(login)
3) BookedAuto.txt : model:landlord:took_to_rent
4) FreeAuto.txt : model:cost:user(login)
3) Users.txt : login:password:money
*/

#include <iostream>
#include <ctime>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <chrono>
#include <sstream>
#define _CRT_SECURE_NO_WARNINGS


void tap_enter_to_continue() {
	system("pause");
}

bool check_banned(std::string match) {
	std::ifstream data("Bans.txt");
	while (!data.eof()) {
		std::string login;
		std::getline(data, login);
		if (match == login) {
			return true;
		}
	}
	return false;
}


class Person {
	bool roots = false;
	bool banned = false;
public:
	std::string login;
	int balance;
	void set_balance(int balance) {
		this->balance = balance;
	}
	void set_login(std::string login) {
		this->login = login;
	}
	void set_status() {
		banned = check_banned(login);
	}
	void set_password(std::string password) {
		this->password = password;
	}
	void get_roots(int index) {
		if (index == 2) { roots = true; }
		else if (index == 1) { roots = false; }
	}
	bool isBanned() {
		return banned;
	}
	bool isAdmin() {
		return roots;
	}
	int delete_acc() {
		std::cout << "If you sure, please, enter your current password: ";
		std::string password_to_delete;
		std::cin >> password_to_delete;
		if (password_to_delete != this->password) {
			std::cout << "Invalid password" << std::endl;
			tap_enter_to_continue();
			return 0;
		}
		std::fstream data;
		data.open("Users.txt", std::ios::in);
		std::vector<std::string> users;
		int index = 0, len = 0;
		if (data.is_open()) {
			while (!data.eof()) {
				std::string user_f;
				std::getline(data, user_f);
				users.push_back(user_f);
				if (this->login == user_f.substr(0, user_f.find(':'))) {
					index = len;
				}
				len++;
			}
		}
		data.close();
		data.open("Users.txt", std::ios::out);
		for (int i = 0; i < len - 1; i++) {
			if (index == i) {
				continue;
			}
			data << users[i] << "\n";
		}
		std::cout << "Your account has been deleted successfully";
		return 1;
	}
	int give_ban_to() {
		std::fstream data("Users.txt", std::ios::in);
		std::string username;
		std::cin >> username;
		bool is_exist = false;
		while (!data.eof()) {
			std::string user_f;
			std::getline(data, user_f);
			if (username == user_f.substr(0, user_f.find(':'))) {
				is_exist = true;
				break;
			}
		}
		if (!is_exist) {
			std::cout << "The user you have input doesn't exist. Please, try again" << std::endl;
			data.close();
			tap_enter_to_continue();
			return 0;
		}
		data.close();
		data.open("Bans.txt", std::ios::out | std::ios::app | std::ios::ate);
		data << username << "\n";
		std::cout << "The user " << username << " has been banned by you" << std::endl;
		tap_enter_to_continue();
		return 1;
	}
	int to_unban_user() {
		int index = 0;
		std::string username;
		std::cin >> username;
		std::vector<std::string> users;
		std::fstream data("Bans.txt", std::ios::in);
		bool is_exist = false;
		int len = 0;
		while (!data.eof()) {
			std::string user_f;
			std::getline(data, user_f);
			users.push_back(user_f.substr(0, user_f.find(':')));
			if (username == user_f.substr(0, user_f.find(':'))) {
				index = len;
				is_exist = true;
			}
			len++;
		}
		if (!is_exist) {
			std::cout << "The user you gonna unban isn't banned or doesn't exist. Please, try again" << std::endl;
			data.close();
			tap_enter_to_continue();
			return 0;
		}
		data.close();
		data.open("Bans.txt", std::ios::out);
		for (int i = 0; i < len - 1; i++) {
			if (i == index) {
				continue;
			}
			data << users[i] << "\n";
		}
		data.close();
		std::cout << "The user " << username << " has been unbanned by you" << std::endl;
		tap_enter_to_continue();
		return 1;
	}
	int create_new_user() {
		std::string login;
		std::cin >> login;
		std::string password = "qwerty5";
		std::fstream data;
		data.open("Users.txt", std::ios::in);
		if (data.is_open()) {
			while (!data.eof()) {
				std::string match;
				std::getline(data, match);
				if (match.substr(0, match.find(':')) == login) {
					std::cout << "This name is unvailable. Please, try another" << std::endl;
					data.close();
					tap_enter_to_continue();
					return 0;
				}
			}
		}
		data.close();
		data.open("Users.txt", std::ios::app | std::ios::ate);
		if (data.is_open()) {
			data << login << ":" << password << ":" << 0 << "\n";
			std::cout << "The user was created successfully" << std::endl;
		}
		else {
			std::cout << "The proccess has a error";
		}
		tap_enter_to_continue();
		data.close();
		return 1;
	}
	std::string change_password(std::string old_password, std::string new_password) {
		if (old_password != password) {
			return "The current password is not correct\n";
		}
		else if (old_password == new_password) {
			return "The current password and new password are identical\n";
		}
		else if (new_password.find(' ') != std::string::npos) {
			return "You can't use space bar in password\n";
		}
		std::string path = isAdmin() ? "Admins.txt" : "Users.txt";
		std::fstream data(path, std::ios::in);
		std::vector<std::string> users;
		int num_curr_user = 0;
		bool to_count = true;

		std::ostringstream str_balance;
		str_balance << this->balance;
		std::string str_balance_f = str_balance.str();

		if (data.is_open()) {
			while (!data.eof()) {
				std::string user_f;
				std::getline(data, user_f);
				if (user_f.empty()) {
					continue;
				}
				users.push_back(user_f);
				if (to_count) {
					if (user_f != (this->login + ":" + this->password + ":" + str_balance_f)) {
						num_curr_user++;
					}
					else { to_count = false; }
				}
			}
		}
		for (std::string x : users) {
			std::cout << x << std::endl;
		}
		data.close();
		data.open(path, std::ios::out);
		std::string change = this->login + ":" + new_password + ":" + str_balance_f;
		users[num_curr_user] = change;
		for (std::string user : users) {
			data << user << "\n";
		}
		data.close();
		return "The password has been changed successfully\n";
	}
private:
	std::string password;
	void show_password() {
		std::cout << password;
	}
};


Person user;

int add_auto() {
	std::string brand, model;
	int price;
	std::cout << "Please, enter auto's brand and model and your price: ";
	std::cin >> brand;
	std::cin >> model;
	std::cin >> price;
	std::fstream data("FreeAuto.txt", std::ios::in);
	int len = 0;
	if (data.is_open()) {
		while (!data.eof()) {
			std::string none;
			std::getline(data, none);
			len++;
		}
	}
	data.close();
	data.open("FreeAuto.txt", std::ios::app | std::ios::ate);
	data << brand + " " + model + ":" << price << ":" << user.login << "\n";
	data.close();
	std::cout << "Your auto was added successfully" << std::endl;
	tap_enter_to_continue();
	return 1;
}


void free_auto_list() {
	std::ifstream data("FreeAuto.txt", std::ios::in);
	std::vector <std::string> model;
	std::vector <std::string> price; 
	std::vector<std::string> landlords;
	int count = 0;
	if (data.is_open()) {
		std::string line, model_f, price_f, landlord_f;
		while (!data.eof()) {
			std::getline(data, line);
			if (line == "") {
				break;
			}
			model_f = line.substr(0, line.find(':'));
			price_f = line.substr(line.find(':') + 1, line.rfind(':') - line.find(':') - 1);
			landlord_f = line.substr(line.rfind(':') + 1, line.size());
			model.push_back(model_f);
			price.push_back(price_f);
			landlords.push_back(landlord_f);
			count++;
		}
	}
	std::cout << "  Car";
	for (int i = 0; i < 20; i++) {
		std::cout << " ";
	}
	std::cout << "Price (per hour)";
	for (int i = 0; i < 10; i++) {
		std::cout << " ";
	}
	std::cout << "Landlord" << std::endl;
	for (int i = 0; i < count; i++) {
		std::cout << i+1 << "." << model[i];
		for (int j = 0; j < 20 - model[i].size() + 3; j++) {
			std::cout << " ";
		}
		std::cout << price[i] << "$";
		for (int j = 0; j < 10 - price[i].size() + 15; j++) {
			std::cout << " ";
		}
		if (landlords[i] == user.login) {
			std::cout << landlords[i] << "(you)" << std::endl;
		}
		else { std::cout << landlords[i] << std::endl; }
	}
 }

void booked_auto_list() {
	std::ifstream data("FreeAuto.txt");
	if (data.is_open()) {
		std::string model;
		std::vector<std::string> user_is_rentting;
		std::vector<std::string> user_is_having;
		int count = 1;
		while (!data.eof()) {
			std::string landlord;
			std::getline(data, model);
			landlord = model.substr(model.rfind(':') + 1, model.size());
			if (landlord == user.login) {
				user_is_rentting.push_back(model.substr(0, model.find(':')) + "(free)");
				continue;
			}
		}
		data.close();
		data.open("BookedAuto.txt");
		while (!data.eof()) {
			std::string landlord, took;
			std::getline(data, model);
			landlord = model.substr(model.find(':') + 1, model.rfind(':') - model.find(':') - 1);
			took = model.substr(model.rfind(':') + 1, model.size());
			if (landlord == user.login) {
				user_is_rentting.push_back(model.substr(0, model.find(':')) + "(took by " + took + ")");
				continue;
			}
			if (user.login == took) {
				user_is_having.push_back(model.substr(0, model.find(':')) + "(took from " + landlord + ")");
				continue;
			}
   		}
		std::cout << "  You are renting:";
		for (int i = 0; i < 20; i++) {
			std::cout << " ";
		}
		std::cout << "You took:" << std::endl;
		int max = user_is_rentting.size() >= user_is_having.size() ? user_is_rentting.size() : user_is_having.size();
		for (int i = 0; i < max; i++) {
			if (i < user_is_rentting.size()) {
				std::cout << i+1 << ".";
				std::cout << user_is_rentting[i];
				for (int j = 0; j < 20 - user_is_rentting[i].size() + 3 + 13; j++) {
					std::cout << " ";
				}
			}
			else {
				for (int j = 0; j < 20 + 18; j++) {
					std::cout << " ";
				}
			}
			if (i < user_is_having.size()) {
				std::cout << count++ << ".";
				std::cout << user_is_having[i] << std::endl;
			}
			else {
				std::cout << std::endl;
			}
		}
	}
	data.close();
}


int do_booking_auto() {
	int num;
	std::string landlord;
	std::fstream data;
	std::vector <std::string> list;
	data.open("FreeAuto.txt", std::ios::in);
	int count = 0;
	if (data.is_open()) {
		std::string free_car;
		while (!data.eof()) {
			std::getline(data, free_car);
			list.push_back(free_car);
			count++;
		}
	}
	bool correctValue = false;
	int count_error = 0;
	do {
		std::string command;
		if (count_error >= 4) {
			std::cout << "If you wanna back, enter \'back\'" << std::endl;
		}
		std::cout << "Enter a number auto you need: ";
		std::cin >> command;
		if (command == "back") {
			return 0;
		}
		else {
			bool str_is_int = true;
			for (int i = 0; i < command.size(); i++) {
				if (!isdigit(command[i])) {
					str_is_int = false;
					break;
				}
			}
			if (str_is_int) {
				num = std::stoi(command);
				if (num <= count) {
					if (user.login != list[num - 1].substr(list[num - 1].rfind(':') + 1, list[num - 1].size())) {
						correctValue = true;
					}
					else {
						std::cout << "You can't take your own car" << std::endl;
					}
				}
				else {
					count_error++;
					std::cout << "You printed incorrect value" << "\n" << "Please, try again" << std::endl;
				}
			}
			else {
				count_error++;
				std::cout << "You printed incorrect value" << "\n" << "Please, try again" << std::endl;
			}
		}
	} while (!correctValue);
	int hours;
	std::cout << "Enter a count of hours: ";
	std::cin >> hours;
	if (std::stoi(list[num - 1].substr(list[num - 1].find(':')+1, list[num - 1].rfind(':') - list[num - 1].find(':') + 1)) * hours > user.balance) {
		std::cout << "You don't have enough money" << std::endl;
		return 0;
	}
	else {
		landlord = list[num - 1].substr(list[num - 1].rfind(':') + 1, list[num - 1].size());
		user.set_balance(user.balance - std::stoi(list[num - 1].substr(list[num - 1].find(':') + 1, list[num - 1].size())) * hours);
	}

	data.close();
	data.open("Users.txt", std::ios::in);
	std::vector<std::string> users;
	if (data.is_open()) {
		while (!data.eof()) {
			std::string user_f;
			std::getline(data, user_f);
			if (user_f.empty()) {
				continue;
			}
			users.push_back(user_f);
		}
	}
	data.close();
	data.open("Users.txt", std::ios::out);
	if (data.is_open()) {
		std::string login_f, password_f;
		char del = ':';
		int balance_f;

		for (std::string x : users) {
			std::string line[3] = { "0","0","0" };
			int start_pos = 0, end_pos = 0;
			int index = 0;
			while (index != 2) {
				if (x[end_pos] != del) {
					end_pos++;
				}
				else {
					line[index++] = x.substr(start_pos, end_pos - start_pos);
					start_pos = ++end_pos;
				}
			}
			line[index] = x.substr(start_pos, x.size() - start_pos);
			login_f = line[0];
			password_f = line[1];
			if (login_f == user.login) {
				balance_f = user.balance;
			}
			else if (login_f == landlord) {
				balance_f = std::stoi(line[2]) + std::stoi(list[num - 1].substr(list[num - 1].find(':') + 1, list[num - 1].size())) * hours;
			}
			else {
				balance_f = std::stoi(line[2]);
			}
			data << login_f << ":" << password_f << ":" << balance_f << "\n";
		}
	}
	data.close();
	data.open("BookedAuto.txt", std::ios::app | std::ios::ate);
	if (data.is_open()) {
		std::string car = list[num - 1].substr(0, list[num - 1].find(':')) + ":" + landlord + ":" + user.login;
		data << "\n" << car;
		std::cout << "The car was booked successfully" << std::endl;
	}
	else {
		std::cout << "Something went wrong..." << std::endl;
		return 0;
	}
	return 0;
	list.erase(list.begin()+num-1);
	data.close();
	data.open("FreeAuto.txt", std::ios::out);
	if (data.is_open()) {
		for (int i = 0; i < --count; i++) {
			data << list[i] << "\n";
		}
	}
	return 0;
}



void clear_console() {
	system("cls");
}


void print_goodbye() {
	std::cout << "Bye-bye, see you later" << std::endl;
}




int log_in(std::string login, std::string password) {
	std::string login_f, password_f;
	char del = ':';
	std::ifstream data;
	std::string files[2] = { "Admins.txt", "Users.txt" };
	for (std::string x : files) {
		data.open(x);
		while (!data.eof()) {
			std::string str;
			std::string line[3] = { "0","0","0" };
			int balance_f;
			std::getline(data, str);

			int start_pos = 0, end_pos = 0;
			int index = 0;
			if (!str.empty()) {
				while (index != 2) {
					if (str[end_pos] != del) {
						end_pos++;
					}
					else {
						line[index++] = str.substr(start_pos, end_pos - start_pos);
						start_pos = ++end_pos;
					}
				}
			}
			line[index] = str.substr(start_pos, str.size()-start_pos);
			login_f = line[0];
			password_f = line[1];
			balance_f = std::stoi(line[2]);
			if (login == login_f && password == password_f) {
				user.set_login(login);
				user.set_password(password);
				user.set_balance(balance_f);
				data.close();
				if (x == "Admins.txt") {
					return 2;
				}
				else {
					return 1;
				}
			}
		}
		data.close();
	}
	return 0;
}




int get_data() {
	std::string login, password;
	while (true) {
		std::cout << "Enter a login: ";
		std::cin >> login;
		std::cout << "Enter a password: ";
		std::cin >> password;
		std::cout << std::endl;
		int root = log_in(login, password);
		if (root == 2) {
			std::cout << "Welcome back, " << login << std::endl;
			tap_enter_to_continue();
			return 2;
		}
		else if (root == 1) {
			std::cout << "Log in was succesfully" << std::endl;
			tap_enter_to_continue();
			return 1;
		}
		else {
			std::cout << "Invalid login or password" << std::endl << "Try again" << std::endl;
		}
	}
}




void sign_up() {
	std::fstream data;
	std::string login = "", password;
	bool correctLogin = false;
	data.open("Users.txt");
	do {
		std::cout << "Login: ";
		std::getline(std::cin, login);
		if (login == "") {
			std::cout << "The input is empty" << std::endl;
			tap_enter_to_continue();
			continue;
		}
		if (login.find(' ') != std::string::npos) {
			std::cout << "The login may not contain a space bar" << std::endl;
			tap_enter_to_continue();
			continue;
		}
		while (!data.eof()) {
			std::string login_f;
			std::getline(data, login_f);
			if (login != login_f.substr(0, login_f.find(':'))) {
				if (data.eof()) {
					correctLogin = true;
					data.close();
				}
			}
			else {
				std::cout << "This login is invailable!" << std::endl;
				tap_enter_to_continue();
				break;
			}
		}
	} while (!correctLogin);
	std::cout << "Password: ";
	std::cin >> password;
	data.open("Users.txt", std::ios::app | std::ios::ate);
	if (data.is_open()) {
		data << "\n" << login << ":" << password;
		std::cout << "The sign up was succesfully" << std::endl;
	}
}





int main() {

	//Интрфейс входа
	bool first = true;
	while (first) {
		std::cout << "What do you want: \"sign up\", \"sign in\" or \"exit\": ";
		std::string first_command;
		std::getline(std::cin, first_command);
		if (first_command == "sign up") {
			//Регистрация
			sign_up(); 
		}
		else if (first_command == "sign in") {
			//Вход
			first = false;
			break;
		}
		else if (first_command == "exit") {
			//Выход из программы
			print_goodbye();
			return 0;
		}
		else {
			//Другая команда
			std::cout << "Incorrect command" << std::endl;
			tap_enter_to_continue();
		}
	}


	user.get_roots(get_data()); //Получаем статус аккаунта(Администратор или обыный) + вход в системы при правильном логине и пароле
	user.set_status(); //Устанавливаем статус

	//Списки команд и их описание
	std::string commands[] = { "logout","chpass", "status", "lfauto","lbauto", "rauto","addauto", "rbalance", "delete"};
	std::string description_command[] = { "TO LOGOUT FROM ACCOUNT", "TO CHANGE PASSWORD", "TO CHECK STATUS OF BAN", "TO LOOK FREE AUTO", "TO LOOK BOOKED AUTO", "TO RENT AUTO", "TO RENT OWN AUTO", "TO REPLENISH YOUR BALANCE", "TO DELETE YOUR ACCOUNT FOREVER(IF BALANCE >= 0). MONEY WILL BE RETURNED, IF YOUR BALANCE > 0"};
	
	//Управление с помощью команд (все процедуры написаны по порядку из в файле, кроме методов класса)
	std::string user_command;
	while (user_command != "exit" && user_command != "quit") {
		clear_console();

		//Дата и время
		time_t now = time(0);
		tm* ltm = localtime(&now);
		std::cout << "Date: " << ltm->tm_mday << "." << 1 + ltm->tm_mon << "." << 1900 + ltm->tm_year << "\nTime: " << ltm->tm_hour << ":" << ltm->tm_min << std::endl;
		
		std::cout << "User: " << user.login << std::endl;
		std::cout << "Balance: " << user.balance << "$" << std::endl;
		std::cout << "Enter a command: ";
		std::cin >> user_command;

		//Добавления нового аккаунта со стандартным паролем (только с правами админа) - \\метод класса//
		if (user_command == "cuser") {
			//Проверка статуса аккаунта
			if (!user.isAdmin()) {
				std::cout << "You dont have roots" << std::endl;
				tap_enter_to_continue();
			}
			else {
				user.create_new_user();
			}
		}
		//Удаление аккаунта -> при удалении переход в входное меню - \\метод класса//
		else if (user_command == "delete") {
			//Проверка статуса аккаунта (админ не может удалить собственный аккаунт)
			if (user.isAdmin()) {
				std::cout << "The admins can't delete own account" << std::endl;
				tap_enter_to_continue();
			}
			else {
				if (user.delete_acc() == 1) {
					std::cin.ignore();
					std::cout << std::endl;
					main();
					return 0;
				}
			}
		}

		//Заблокировать или разблокировать аккаунт (блокировка не позволяет сдать, взять автомобиль в аренду) - \\методы класса//
		else if (user_command == "ban") {
			if (!user.isAdmin()) {
				std::cout << "You don't have enough roots" << std::endl;
				tap_enter_to_continue();
			}
			else {
				user.give_ban_to();
			}
		}
		else if (user_command == "unban") {
			if (!user.isAdmin()) {
				std::cout << "You don't have enough roots" << std::endl;
				tap_enter_to_continue();
			}
			else {
				user.to_unban_user();
			}
		}

		//Смена пароля - \\метод класса//
		else if (user_command == "chpass") {
			std::string old_password, new_password;
			std::cout << "Enter a current password: ";
			std::cin >> old_password;
			std::cout << "Enter a new password: ";
			std::cin >> new_password;
			std::cout << user.change_password(old_password, new_password);
			tap_enter_to_continue();
		}

		//Узнать статус блокировки - \\метод класса//
		else if (user_command == "status") {
			if (user.isBanned()) {
				std::cout << "Banned" << std::endl;
			}
			else {
				std::cout << "This account doesn't have a ban" << std::endl;
			}
			tap_enter_to_continue();
		}

		//Добавление собственного авто
		else if (user_command == "addauto") {
			if (user.isBanned()) {
				std::cout << "You cant do it (you are banned)" << std::endl;
			}
			else {
				add_auto();
			}
			tap_enter_to_continue();
		}

		//Просмотр списка свободных машин
		else if (user_command == "lfauto") {
			free_auto_list();
			tap_enter_to_continue();
		}
		//Просмотр списка машин, который пользователь взял и сдал (или собираештся сдавать) ( при наличии )
		else if (user_command == "lbauto") {
			booked_auto_list();
			tap_enter_to_continue();
		}

		//Аренда автомобиля из свободных для сдачи машин
		else if (user_command == "rauto") {
			if (user.isBanned()) {
				std::cout << "You cant do it (you are banned)" << std::endl;
			} 
			else {
				free_auto_list();
				do_booking_auto();
			}
			tap_enter_to_continue();
		}

		//Пополнение баланса - в разработке)
		else if (user_command == "rbalance") {
			std::cout << "Replenishment of the balance is not available for users from Russia. :(" << std::endl;
			tap_enter_to_continue();
		}	

		//Список команд
		else if (user_command == "list") {
			for (int i = 0; i < size(commands); i++) {
				std::cout << commands[i] << " - " << description_command[i] << std::endl;
			}
			tap_enter_to_continue();
		}


		//Выход из системы
		else if (user_command == "exit" || user_command == "quit") {
			clear_console();
			print_goodbye();
		}

		//Выход из аккаунта
		else if (user_command == "logout") {
			std::cin.ignore();
			clear_console();
			main();
			return 0;
		}

		else {
			std::cout << "The command \'" << user_command << "\' doesn't exist.\nIf you want to get list of commands, write \"list\"" << std::endl;
			tap_enter_to_continue();
		}
	}
	return 0;
}