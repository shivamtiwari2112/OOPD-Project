#include <iostream>
#include <fstream>
#include <filesystem>
#include <regex>
#include <vector>
#include <sstream>
#include <iterator>
#include <chrono>

using namespace std;
namespace fs = filesystem;

class Shell {
private:
    string current_directory;

public:
    Shell() {
        current_directory = fs::current_path().string();
    }

    void execCommand(const string& command) {
        // Split command into tokens
        istringstream iss(command);
        vector<string> tokens{istream_iterator<string>{iss}, istream_iterator<string>{}};

        // Check if there are any tokens
        if (!tokens.empty()) {
            string cmd = tokens[0];

            // Extract options and arguments
            vector<string> options;
            vector<string> args;

            for (size_t i = 1; i < tokens.size(); ++i) {
                if (tokens[i].find('-') == 0) {
                    // Token is an option
                    options.push_back(tokens[i]);
                } else {
                    // Token is an argument
                    args.push_back(tokens[i]);
                }
            }

            // Execute the command based on cmd, options, and args
            if (options.size() == 1 && options[0] == "-h") {
                // Display help for the command
                displayHelp(cmd);
            } else {
                executeCommand(cmd, options, args);
            }
        }
    }

    void displayHelp(const string& cmd) {
        cout << "Help for command: " << cmd << endl;

        if (cmd == "cd") {
            cout << "Usage: cd <directory>" << endl;
        } else if (cmd == "mv") {
            cout << "Usage: mv <source> <destination>" << endl;
        } else if (cmd == "rm") {
            cout << "Usage: rm [-r] <file/directory>" << endl;
            cout << "Options:" << endl;
            cout << "  -r: Remove directories and their contents recursively" << endl;
        } else if (cmd == "ls") {
            cout << "Usage: ls [options] [directories]" << endl;
            cout << "Options:" << endl;
            cout << "  -l: Display detailed information" << endl;
            cout << "  -h: Display help" << endl;
        } else if (cmd == "cp") {
            cout << "Usage: cp <source> <destination>" << endl;
        } else if (cmd == "clear") {
            cout << "Usage: clear" << endl;
        } else if (cmd == "mkdir") {
            cout << "Usage: mkdir <directory>" << endl;
        } else {
            cout << "Command not recognized." << endl;
        }
    }

    void executeCommand(const string& cmd, const vector<string>& options, const vector<string>& args) {
        if (cmd == "cd") {
            changeDir(args);
        } else if (cmd == "mv") {
            moveFile(args);
        } else if (cmd == "rm") {
            removeFile(options, args);
        } else if (cmd == "ls") {
            listDir(options, args);
        } else if (cmd == "cp") {
            copyFile(args);
        } else if (cmd == "clear") {
            clearConsole();
        } else if (cmd == "mkdir") {
            createDirectory(args);
        } else {
            cout << "Command not recognized." << endl;
        }
    }

    void createDirectory(const vector<string>& args) {
        if (!args.empty()) {
            try {
                // Create a new directory
                fs::create_directory(args[0]);

                cout << "Directory created: " << args[0] << endl;
            } catch (const exception& e) {
                cout << "Error: " << e.what() << endl;
            }
        } else {
            cout << "Usage: mkdir <directory>" << endl;
        }
    }

    void clearConsole() {
        #ifdef _WIN32
                // For Windows
                system("cls");
        #else
                // For Unix-like systems
                system("clear");
        #endif
    }

    void changeDir(const vector<string>& args) {
        if (!args.empty()) {
            fs::current_path(args[0]);
            current_directory = fs::current_path().string();
        } else {
            cout << "Usage: cd <directory>" << endl;
        }
    }

    void moveFile(const vector<string>& args) {
        if (args.size() == 2) {
            string source = args[0];
            string destination = args[1];

            try {
                // Check if the destination is a directory
                if (fs::is_directory(destination)) {
                    // If it's a directory, perform a move operation
                    fs::create_directories(fs::path(destination).parent_path());
                    fs::rename(source, fs::path(destination) / fs::path(source).filename());
                } else {
                    // If it's not a directory, perform a rename operation
                    fs::rename(source, destination);
                }
            } catch (const exception& e) {
                cout << "Error: " << e.what() << endl;
            }
        } else {
            cout << "Usage: mv <source> <destination>" << endl;
        }
    }

    void removeFile(const vector<string>& options, const vector<string>& args) {
        if (!args.empty()) {
            try {
                // Check for the presence of the recursive option
                bool recursive = find(options.begin(), options.end(), "-r") != options.end();

                if (recursive) {
                    // Remove directory and its contents recursively
                    fs::remove_all(args[0]);
                } else {
                    // Remove file or empty directory
                    fs::remove(args[0]);
                }
            } catch (const exception& e) {
                cout << "Error: " << e.what() << endl;
            }
        } else {
            cout << "Usage: rm [-r] <file/directory>" << endl;
        }
    }

    void listDir(const vector<string>& options, const vector<string>& args) {
        if (args.empty()) {
            // List the current directory if no directory is specified
            listDirectory(current_directory, options);
        } else {
            // List the specified directories
            for (const auto& arg : args) {
                listDirectory(arg, options);
            }
        }
    }

    // void listDirectory(const string& path, const vector<string>& options) {
    //     try {
    //         for (const auto& entry : fs::directory_iterator(path)) {
    //             cout << entry.path().filename();
                
    //             // Check for the presence of the option to display details (ls -l)
    //             if (find(options.begin(), options.end(), "-l") != options.end()) {
    //                 cout << " (" << fs::file_size(entry) << " bytes)";
    //             }

    //             cout << endl;
    //         }
    //     } catch (const exception& e) {
    //         cout << "Error: " << e.what() << endl;
    //     }
    // }
    void listDirectory(const string& path, const vector<string>& options) {
        try {
            for (const auto& entry : fs::directory_iterator(path)) {
                // Check for the presence of the option to display hidden entries (ls -a)
                if (find(options.begin(), options.end(), "-a") != options.end() || entry.path().filename().string().find('.') != 0) {
                    cout << entry.path().filename();

                    // Check for the presence of the option to display detailed information (ls -l)
                    if (find(options.begin(), options.end(), "-l") != options.end()) {
                        cout << " (" << fs::file_size(entry) << " bytes)";
                    }

                    cout << endl;
                }
            }
        } catch (const exception& e) {
            cout << "Error: " << e.what() << endl;
        }
    }

    void copyFile(const vector<string>& args) {
        if (args.size() == 2) {
            string source = args[0];
            string destination = args[1];

            try {
                // Check if the source is a directory
                if (fs::is_directory(source)) {
                    // If it's a directory, perform a recursive copy operation
                    fs::create_directories(destination);
                    for (const auto& entry : fs::recursive_directory_iterator(source)) {
                        fs::copy(entry.path(), destination / entry.path().lexically_relative(source));
                    }
                } else {
                    // If it's a file, perform a regular copy operation
                    fs::copy(source, destination);
                }
            } catch (const exception& e) {
                cout << "Error: " << e.what() << endl;
            }
        } else {
            cout << "Usage: cp <source> <destination>" << endl;
        }
    }


    string getCurrentDirectory() const {
        return current_directory;
    }
};

int main() {
    Shell myShell;

    while (true) {
        string command;
        cout << myShell.getCurrentDirectory() << " $ ";
        getline(cin, command);

        if (command == "exit") {
            break;
        }
        auto start = chrono::high_resolution_clock::now();
        myShell.execCommand(command);
        auto end = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<std::chrono::microseconds>(end - start);
        cout << "Time taken by function: " << duration.count() << " microseconds" << std::endl;
    }

    return 0;
}

    