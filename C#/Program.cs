using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;




namespace CSharp
{
    internal class Program
    {
        public static void tap_enter_to_continue()
        {
            Console.Write("Tap enter to continue . . .");
            Console.ReadLine();
        }
        public static void clear_console()
        {
            Console.Clear();
        }
        public static bool isdigit(string str)
        {
            foreach (char c in str)
            {
                if (!(c >= '0' && c <= '9'))
                {
                    return false;
                }
            }
            return true;
        }
        class Person
        {
            private string login;
            private string password;
            private bool roots;
            private bool banned;
            private int balance;

            private bool check_banned(string match)
            {
                string[] users = File.ReadAllLines("Bans.txt");
                return users.Contains(match) ? true : false;
            }

            public Person (string login, string password, bool roots, int balance)
            {
                this.login = login;
                this.password = password;
                this.roots = roots;
                this.banned = check_banned(this.login);
                this.balance = balance;
            }

            public int get_balance()
            {
                return balance;
            }
            public void set_balance(int balance)
            {
                this.balance = balance;
            }
            public string get_login()
            {
                return login;
            }
            public bool isAdmin()
            {
                return roots;
            }
            public bool isBanned()
            {
                return banned;
            }
            public int give_ban_to()
            {
                Console.Write("Enter user you gonna ban: ");
                string user_to_ban = Console.ReadLine();
                string[] users = File.ReadAllLines("Users.txt");
                foreach (string user in users)
                {
                    if (user.Split(':')[0] == user_to_ban)
                    {
                        using (StreamWriter writer = new StreamWriter("Bans.txt", true))
                        {
                            writer.WriteLine(user_to_ban);
                            Console.WriteLine($"The user {user_to_ban} has been banned");
                            return 1;
                        }
                    }
                }
                Console.WriteLine("This user doesn't exist");
                return 0;
            }
            public int to_unban_user()
            {
                Console.Write("Enter user you gonna unban: ");
                string user_to_unban = Console.ReadLine();
                string[] users = File.ReadAllLines("Bans.txt");
                if (Array.IndexOf(users, user_to_unban) == -1)
                {
                    Console.WriteLine("This user is not in list");
                    return 0;
                }
                using (StreamWriter writer = new StreamWriter("Bans.txt"))
                {
                    foreach (string user in users)
                    {
                        if (user == user_to_unban)
                        {
                            continue;
                        }
                        writer.WriteLine(user);
                    }
                    Console.WriteLine("User has been ubanned");
                    return 1;
                }
            }
            public int create_new_user()
            {
                string login, password = "qwerty5";
                login = Console.ReadLine();
                using (StreamReader data = new StreamReader("Users.txt"))
                {
                    if (data.ReadLine().Split(':')[0] == login)
                    {
                        Console.WriteLine("The user with this login already exist");
                        return 0;
                    }
                }
                using (StreamWriter write = new StreamWriter("Users.txt", true))
                {
                    write.WriteLine($"{login}:{password}:0");
                    write.Close();
                    Console.WriteLine($"User {login} was created successfully");
                }
                return 0;
            }
            public int delete_acc()
            {
                Console.Write("If you are sure, please, enter your current password: ");
                string password = Console.ReadLine();
                if (password != this.password)
                {
                    Console.WriteLine("Invalid password");
                    tap_enter_to_continue();
                    return 0;
                }
                string[] users = File.ReadAllLines("Users.txt");
                StreamWriter data = new StreamWriter("Users.txt");
                foreach (string user in users)
                {
                    Console.WriteLine(user);
                    if (user.Substring(0,user.IndexOf(':')) == login)
                    {
                        continue;
                    }
                    data.WriteLine(user);
                }
                data.Close();
                return 1;
            }
            public string change_password(string old_password, string new_password)
            {
                if (old_password != password)
                {
                    return "The current password is not correct!\n";
                }
                if (old_password == new_password)
                {
                    return "The new password and current password are identical\n";
                }
                if (string.IsNullOrEmpty(new_password))
                {
                    return "You didn't write a new password\n";
                }
                if (new_password.IndexOf(' ') != -1)
                {
                    return "You can't use space bar in password\n";
                }
                string path = isAdmin() ? "Admins.txt" : "Users.txt";
                string[] users = File.ReadAllLines(path);
                using (StreamWriter write = new StreamWriter(path))
                {
                    foreach (string user in users)
                    {
                        if (user.Split(':')[0] == login)
                        {
                            write.WriteLine($"{login}:{new_password}:{balance}");
                        }
                        else
                        {
                            write.WriteLine(user);
                        }
                    }
                    return "The password was changed successfully";
                }

            }
            public int addauto()
            {
                Console.WriteLine("Write auto's brand and model and your price");
                string[] settings = Console.ReadLine().Split(' ');
                using (StreamWriter writer = new StreamWriter("FreeAuto.txt", true))
                {
                    writer.WriteLine($"{settings[0]} {settings[1]}:{settings[2]}:{login}");
                }
                Console.WriteLine("Your auto was added successfully");
                return 1;
            }  
            public int free_auto_list()
            {
                string[] data = File.ReadAllLines("FreeAuto.txt");
                string result = string.Concat(Enumerable.Repeat(" ", 20));
                Console.WriteLine("  Car" + result + "Price (per hour)" + result + "Landlord");
                int count = 1;
                foreach(string item in data)
                {
                    string[] line = item.Split(':');
                    Console.Write(count + "." + line[0] + string.Concat(Enumerable.Repeat(" ", 20 - line[0].Length + 3)));
                    Console.Write(line[1] + string.Concat(Enumerable.Repeat(" ", 20 - line[1].Length + 16)));
                    Console.WriteLine(line[2] == login ? line[2] +"(you)" : line[2]);
                    count++;
                }
                return 0;
            }
            public int booked_auto_list()
            {
                string[] data = File.ReadAllLines("BookedAuto.txt");
                string[] user_is_renting = new string[data.Length];
                string[] user_is_having = new string[data.Length];
                int count_r = 0, count_h = 0;
                foreach (string auto in data)
                {
                    string[] splitted = auto.Split(':');
                    if (splitted[1] == login)
                    {
                        user_is_renting[count_r++] = (splitted[0] + "(took by " + splitted[2] + ")");
                    }
                    if (splitted[2] == login)
                    {
                        user_is_having[count_h++] = (splitted[0] + "(took from " + splitted[1] + ")");
                    }
                }
                string result = string.Concat(Enumerable.Repeat(" ", 20));
                Console.WriteLine("  You are renting:" + result + "You took:");
                int count = 1;
                for (int i = 0; i < data.Length; i++)
                {
                    
                    if (string.IsNullOrEmpty(user_is_renting[i]) && string.IsNullOrEmpty(user_is_having[i]))
                    {
                        break;
                    }
                    if (!string.IsNullOrEmpty(user_is_renting[i]))
                    {
                        Console.Write($"{i + 1}.{user_is_renting[i]}{string.Concat(Enumerable.Repeat(" ", 20 - user_is_renting[i].Length + 3 + 13))}");
                    }
                    else
                    {
                        Console.Write(string.Concat(Enumerable.Repeat(" ", 20 + 18)));
                    }
                    if (!string.IsNullOrEmpty(user_is_having[i]))
                    {
                        Console.WriteLine($"{count++}.{user_is_having[i]}");
                    }
                    else
                    {
                        Console.WriteLine("");
                    }
                }
                return 1;
            }
            public int do_booking_auto()
            {
                bool correctValue = false;
                int count_error = 0;
                string landlord;
                string[] selected_car = new string[3];
                int num = 0;
                do
                {
                    string command;
                    if (count_error >= 4)
                    {
                        Console.WriteLine("If you wanna back, enter \'back\'");
  
                    }
                    Console.Write("Enter a number of auto you need: ");
                    command = Console.ReadLine();
                    if (command == "back")
                    {
                        return 0;
                    }
                    else if (command == "")
                    {
                        Console.WriteLine("Empty string!");
                    }
                    else
                    {
                        if (isdigit(command))
                        {
                            string[] data = File.ReadAllLines("FreeAuto.txt");
                            num = Convert.ToInt16(command);
                            if (num - 1 <= data.Length)
                            {
                                selected_car = data[num - 1].Split(':');
                                if (selected_car[2] == login)
                                {
                                    Console.WriteLine("You can't take your own car");
                                }
                                else
                                {
                                    correctValue = true;
                                }
                            }
                            else
                            {
                                Console.WriteLine("You printed incorrect value\nPlease, try again");
                            }
                        }
                        else
                        {
                            Console.WriteLine("You printed incorrect value\nPlease, try again");
                        }
                    }
                } while (!correctValue);
                Console.Write("Enter a count of hours: ");
                int hours = Convert.ToInt16(Console.ReadLine());
                if (Convert.ToInt16(selected_car[1]) * hours > balance)
                {
                    Console.WriteLine("You don't have enough money");
                    return 0;
                }
                else
                {
                    landlord = selected_car[2];
                    set_balance(balance - Convert.ToInt16(selected_car[1]) * hours);
                }
                string[] users = File.ReadAllLines("Users.txt");
                using (StreamWriter writer = new StreamWriter("Users.txt"))
                {
                    for (int i = 0; i < users.Length; i++)
                    {
                        string[] user = users[i].Split(':');
                        if (user[0] == login)
                        {
                            writer.WriteLine($"{login}:{password}:{balance}");
                        }
                        else if (user[0] == landlord)
                        {
                            writer.WriteLine($"{landlord}:{user[1]}:{Convert.ToInt16(user[2]) + Convert.ToInt16(selected_car[1]) * hours}");
                        }
                        else
                        {
                            writer.WriteLine(user[0] + ":" + user[1] + ":" + user[2]);
                        } 
                    }
                }
                using (StreamWriter writer = new StreamWriter("BookedAuto.txt",true))
                {
                    writer.WriteLine($"{selected_car[0]}:{landlord}:{login}");
                }
                string[] rewrite = File.ReadAllLines("FreeAuto.txt");
                using (StreamWriter writer = new StreamWriter("FreeAuto.txt"))
                {
                    for (int i = 0; i < rewrite.Length; i++)
                    {
                        if (i == num - 1)
                        {
                            continue;
                        }
                        writer.WriteLine(rewrite[i]);
                    }
                }
                Console.WriteLine("You rent a car successfully");
                return 1;
            }
        }

        public static string[] log_in(string login, string password)
        {
            string[] files = { "Users.txt", "Admins.txt" };
            foreach (string file in files) {
                using (FileStream data = File.OpenRead(file))
                {
                    string[] users = File.ReadAllLines(file);
                    foreach (string user in users)
                    {
                        string[] user_data = user.Split(':');
                        if (user_data[0] == login && user_data[1] == password)
                        {
                            string[] return_data = { login, password, Convert.ToString(Array.IndexOf(files, file)), user_data[2] };
                            return return_data;
                        }
                    }
                }
            }
            return null;
        }

        public static int sing_up()
        {
            string[] users = File.ReadAllLines("Users.txt");
            bool correctLogin = false;
            string login = "", password;
            do
            {
                Console.Write("Enter a login: ");
                login = Console.ReadLine();
                if (login == "")
                {
                    Console.WriteLine("The input is empty");
                    tap_enter_to_continue();
                    continue;
                }
                if (login.IndexOf(' ') != -1)
                {
                    Console.WriteLine("Login may not contain a space bar");
                    tap_enter_to_continue();
                    continue;
                }
                correctLogin = true;
                foreach (string user in users)
                {
                    string login_f = user.Split(':')[0];
                    if (login == login_f)
                    {
                        Console.WriteLine("This login is unvailable");
                        correctLogin = false;
                        break;
                    }
                }
            } while (!correctLogin);
            Console.Write("Enter a password: ");
            password = Console.ReadLine();
            using (StreamWriter writer = new StreamWriter("Users.txt", true))
            {
                writer.WriteLine($"{login}:{password}:0");
            }
            Console.WriteLine("You signed up successfully");
            return 1;
        }

        public static void say_goodbye()
        {
            Console.WriteLine("Bye-bye, see you later");
        }
        static int Main(string[] args)
        {
            string[] commands = { "logout", "chpass", "status", "lfauto", "lbauto", "rauto", "addauto", "rbalance", "delete" };
            string[] description_commands = { "TO LOGOUT FROM ACCOUNT", "TO CHANGE PASSWORD", "TO CHECK STATUS OF BAN", "TO LOOK FREE AUTO", "TO LOOK BOOKED AUTO", "TO RENT AUTO", "TO RENT OWN AUTO", "TO REPLENISH YOUR BALANCE", "TO DELETE YOUR ACCOUNT FOREVER(IF BALANCE >= 0). MONEY WILL BE RETURNED, IF YOUR BALANCE > 0" };
            string[] login_data = new string[4];//login, password, root, balance
            bool welcome = true;
            while(welcome is true)
            {
                Console.Write("What do you want: \"sign up\", \"sign in\" or \"exit\": ");
                string first_command = Console.ReadLine();
                switch (first_command)
                {
                    case "sign up":
                        sing_up();
                        break;
                    case "sign in":
                        Console.Write("Enter login: ");
                        string login = Console.ReadLine();
                        Console.Write("Enter password: ");
                        string password = Console.ReadLine();
                        login_data = log_in(login, password);
                        if (login_data != null)
                        {
                            welcome = false;
                            Console.WriteLine($"Welcome back, {login}");
                            tap_enter_to_continue();
                        }
                        else
                        {
                            Console.WriteLine("Invalid login or password");
                            tap_enter_to_continue();
                            clear_console();
                        }
                        break;
                    case "exit":
                        clear_console();
                        say_goodbye();
                        return 0;
                    default:
                        Console.WriteLine("Incorrect command");
                        break;
                }
            }
            Person user = new Person(login_data[0], login_data[1], login_data[2] == "1", Convert.ToInt16(login_data[3]));
            string user_command = "";    
            while (user_command != "exit")
            {
                clear_console();
                //Дата и время

                Console.Write($"User -> {user.get_login()}\nBalance -> {user.get_balance()}$\nEnter a command: ");
                user_command = Console.ReadLine();

                switch (user_command)
                {
                    case "cuser":
                        if (!user.isAdmin())
                        {
                            Console.WriteLine("You don't have roots");
                        }
                        else
                        {
                            user.create_new_user();
                            tap_enter_to_continue();
                        }
                        break;
                    case "delete":
                        if (user.isAdmin())
                        {
                            Console.WriteLine("Admins can't delete own account");
                            tap_enter_to_continue();                         
                        }
                        else
                        {
                            if (user.delete_acc() == 1)
                            {
                                Main(null);
                                return 0;
                            }
                        }
                        break;
                    case "ban":
                        if (!user.isAdmin())
                        {
                            Console.WriteLine("You don't have roots");
                            tap_enter_to_continue();
                        }
                        else
                        {
                            user.give_ban_to();
                        }
                        tap_enter_to_continue();
                        break;
                    case "unban":
                        if (!user.isAdmin())
                        {
                            Console.WriteLine("You don't have roots");
                        }
                        else
                        {
                            user.to_unban_user();
                        }
                        tap_enter_to_continue();
                        break;
                    case "chpass":
                        Console.Write("Enter a current password: ");
                        string old_password = Console.ReadLine();
                        Console.Write("Enter a new password: ");
                        string new_password = Console.ReadLine();
                        Console.WriteLine(user.change_password(old_password, new_password)); 
                        tap_enter_to_continue();
                        break;
                    case "status":
                        if (user.isBanned())
                        {
                            Console.WriteLine("Banned");
                        }
                        else
                        {
                            Console.WriteLine("You don't have a ban");
                        }
                        tap_enter_to_continue();
                        break;
                    case "addauto":
                        if (user.isBanned())
                        {
                            Console.WriteLine("You cant do it (you are banned)!");
                            break;
                        }
                        else
                        {
                            user.addauto();
                        }
                        tap_enter_to_continue();
                        break;
                    case "lfauto":
                        user.free_auto_list();
                        tap_enter_to_continue();
                        break;
                    case "lbauto":
                        user.booked_auto_list();
                        tap_enter_to_continue();
                        break;
                    case "rauto":
                        if (user.isBanned())
                        {
                            Console.WriteLine("You cant do it (you are banned)!");
                            break;
                        } 
                        else
                        {
                            user.free_auto_list();
                            user.do_booking_auto();
                        }
                        tap_enter_to_continue();
                        break;
                    case "rbalance":
                        Console.WriteLine("Replenishment of the balance is not available for users from Russia. :(");
                        tap_enter_to_continue();
                        break;
                    case "list":
                        for (int i = 0; i < commands.Length; i++)
                        {
                            Console.WriteLine(commands[i] + " - " + description_commands[i]);
                        }
                        tap_enter_to_continue();
                        break;
                    case "exit":
                        clear_console();
                        say_goodbye();
                        break;
                    case "logout":
                        clear_console();
                        Main(null);
                        return 0;
                    default:
                        Console.WriteLine($"The command '{user_command}' doesn't.\nIf you want to get list of commands, write \"list\"");
                        tap_enter_to_continue();
                        break;
                }
            }
            tap_enter_to_continue();
            clear_console();
            return 0;
        }
    }
}
