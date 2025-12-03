#include <iostream>
#include <string>
#include <fstream>
#include <filesystem>
std::string user;
namespace fs = std::filesystem;

class Get_info {
public:
    std::string osinfo();
};

class Shell {
private:
    const std::string &username;
    const std::string &infoos;

public:
    Shell(const std::string &user, const std::string &os);
    void run();

private:
    void dispatcher(const std::string &cmd);
    void clearScreen();
    void help_main();
    void df_main(const char *path);
    void whoami(const std::string &username);
    void change_nick(const std::string &username);
};

class Auth {
public:
    void login();
};

std::string Get_info::osinfo() {
    std::string path = "info.txt";
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "[Error]: can't get info from " << path << std::endl;
        return "UnknownOS";
    }

    std::string firstLine;
    if (std::getline(file, firstLine)) {
        file.close();
        return firstLine;
    } else {
        file.close();
        return "UnknownOS";
    }
}


Shell::Shell(const std::string &user, const std::string &os)
    : username(user), infoos(os) {}

void Shell::run() {
    std::cout << "Hello World!" << std::endl;

    while (true) {
        std::cout << "\033[32m"
                  << username
                  << "@"
                  << infoos
                  << " $"
                  << "\033[0m ";
        std::string cmd;
        std::getline(std::cin, cmd);
        dispatcher(cmd);
    }
}

void Shell::dispatcher(const std::string &cmd) {
    if (cmd == "help") { help_main(); return; }
    if (cmd == "clear") { clearScreen(); return; }
    if (cmd == "df -h") { df_main("/"); return; }
    if (cmd == "whoami") { whoami(username); return; }
    if (cmd.substr(0, 9) == "username ") { change_nick(cmd.substr(9)); return; }
    std::cout << "invalid command \"" << cmd << "\"" << std::endl;
}

void Shell::clearScreen() { std::cout << "\033[2J\033[1;1H"; }

void Shell::help_main() {
    std::cout << "help - shows this page" << std::endl;
    std::cout << "clear - clears the screen" << std::endl;
    std::cout << "df -h - shows disk space" << std::endl;
    std::cout << "whoami - prints current user" << std::endl;
    std::cout << "username <username> - temp. changes current username" << std::endl;
}
void Shell::change_nick(const std::string &username) {
    user = username;
}
void Shell::df_main(const char *path) {
    fs::directory_entry Directory{path};
    if (!Directory.exists()) {
        std::cout << "du: " << path << ": No such file or directory (os error 2)." << std::endl;
        return;
    }

    auto [capacity, free, available]{ fs::space(path) };
    constexpr int bytesInGB{1024 * 1024 * 1024};

    std::cout << "Capacity: " << capacity / bytesInGB << "GB"
              << "\nFree: " << free / bytesInGB << "GB"
              << "\nAvailable: " << available / bytesInGB << "GB" << std::endl;
}

void Shell::whoami(const std::string &username) { std::cout << username << std::endl; }


void Auth::login() {
    Get_info get_info;
    std::string osinfo = get_info.osinfo();


    std::string pass;

    for (int i = 1; i <= 3; i++) {
        std::cout << "User# ";
        std::cin >> user;

        if (user == "Mia") {
            std::cout << "Pass# ";
            std::cin >> pass;

            if (pass == "pass") {
                std::cout << "Auth: ok" << std::endl;
                Shell shell(user, osinfo);
                shell.run();
                return;
            } else {
                std::cout << "Invalid Password!" << std::endl;
            }
        } else {
            std::cout << "Invalid Username!" << std::endl;
        }

        std::cout << i << " out of 3 attempts" << std::endl;
    }

    std::cout << "Exiting program.." << std::endl;
    std::cout << "Reason: too many attempts" << std::endl;
}

int main() {
    Auth auth;
    auth.login();

    return 0;
}
