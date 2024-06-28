#include <iostream>
#include <filesystem>
#include <fstream>
#include <chrono>
#include <iomanip>

namespace fs = std::filesystem;
using namespace std;

void displayMenu() {
    cout << "\nFile Manager Menu:" << endl;
    cout << "1. Navigate to Directory" << endl;
    cout << "2. View Files in Current Directory" << endl;
    cout << "3. Create Directory" << endl;
    cout << "4. Copy File" << endl;
    cout << "5. Move File" << endl;
    cout << "6. Delete File or Directory" << endl;
    cout << "7. Rename File or Directory" << endl;
    cout << "8. View File Content" << endl;
    cout << "9. Get File Info" << endl;
    cout << "10. Exit" << endl;
    cout << "Enter your choice: ";
}

void navigateToDirectory(fs::path& currentPath) {
    string dir;
    cout << "Enter the path to navigate to: ";
    cin >> dir;
    fs::path newPath = dir;
    if (fs::exists(newPath) && fs::is_directory(newPath)) {
        currentPath = newPath;
        cout << "Navigated to " << currentPath << endl;
    } else {
        cout << "Invalid directory path." << endl;
    }
}

void viewFilesInDirectory(const fs::path& currentPath) {
    cout << "Files and directories in " << currentPath << ":" << endl;
    for (const auto& entry : fs::directory_iterator(currentPath)) {
        cout << (fs::is_directory(entry.path()) ? "[DIR] " : "[FILE] ") << entry.path().filename().string() << endl;
    }
}

void createDirectory(const fs::path& currentPath) {
    string dirName;
    cout << "Enter the name of the directory to create: ";
    cin >> dirName;
    fs::path newDir = currentPath / dirName;
    if (fs::create_directory(newDir)) {
        cout << "Directory created: " << newDir << endl;
    } else {
        cout << "Failed to create directory." << endl;
    }
}

void copyFile(const fs::path& currentPath) {
    string source, destination;
    cout << "Enter the source file path: ";
    cin >> source;
    cout << "Enter the destination file path: ";
    cin >> destination;
    fs::path sourcePath = currentPath / source;
    fs::path destinationPath = currentPath / destination;
    try {
        fs::copy(sourcePath, destinationPath);
        cout << "File copied successfully." << endl;
    } catch (const fs::filesystem_error& e) {
        cout << "Error copying file: " << e.what() << endl;
    }
}

void moveFile(const fs::path& currentPath) {
    string source, destination;
    cout << "Enter the source file path: ";
    cin >> source;
    cout << "Enter the destination file path: ";
    cin >> destination;
    fs::path sourcePath = currentPath / source;
    fs::path destinationPath = currentPath / destination;
    try {
        fs::rename(sourcePath, destinationPath);
        cout << "File moved successfully." << endl;
    } catch (const fs::filesystem_error& e) {
        cout << "Error moving file: " << e.what() << endl;
    }
}

void deleteFileOrDirectory(const fs::path& currentPath) {
    string path;
    cout << "Enter the path of the file or directory to delete: ";
    cin >> path;
    fs::path deletePath = currentPath / path;
    try {
        if (fs::remove_all(deletePath)) {
            cout << "File/Directory deleted successfully." << endl;
        } else {
            cout << "Failed to delete file/directory." << endl;
        }
    } catch (const fs::filesystem_error& e) {
        cout << "Error deleting file/directory: " << e.what() << endl;
    }
}

void renameFileOrDirectory(const fs::path& currentPath) {
    string oldName, newName;
    cout << "Enter the current name: ";
    cin >> oldName;
    cout << "Enter the new name: ";
    cin >> newName;
    fs::path oldPath = currentPath / oldName;
    fs::path newPath = currentPath / newName;
    try {
        fs::rename(oldPath, newPath);
        cout << "Renamed successfully." << endl;
    } catch (const fs::filesystem_error& e) {
        cout << "Error renaming: " << e.what() << endl;
    }
}

void viewFileContent(const fs::path& currentPath) {
    string fileName;
    cout << "Enter the file name to view: ";
    cin >> fileName;
    fs::path filePath = currentPath / fileName;
    ifstream file(filePath);
    if (file.is_open()) {
        string line;
        cout << "Content of " << fileName << ":" << endl;
        while (getline(file, line)) {
            cout << line << endl;
        }
        file.close();
    } else {
        cout << "Failed to open file." << endl;
    }
}

void getFileInfo(const fs::path& currentPath) {
    string fileName;
    cout << "Enter the file name to get info: ";
    cin >> fileName;
    fs::path filePath = currentPath / fileName;
    if (fs::exists(filePath)) {
        auto ftime = fs::last_write_time(filePath);
        auto cftime = chrono::system_clock::to_time_t(chrono::file_clock::to_sys(ftime));
        cout << "File: " << filePath.filename() << endl;
        cout << "Size: " << fs::file_size(filePath) << " bytes" << endl;
        cout << "Last modified: " << put_time(localtime(&cftime), "%F %T") << endl;
    } else {
        cout << "File does not exist." << endl;
    }
}

int main() {
    fs::path currentPath = fs::current_path();
    int choice;

    while (true) {
        displayMenu();
        cin >> choice;

        switch (choice) {
            case 1:
                navigateToDirectory(currentPath);
                break;
            case 2:
                viewFilesInDirectory(currentPath);
                break;
            case 3:
                createDirectory(currentPath);
                break;
            case 4:
                copyFile(currentPath);
                break;
            case 5:
                moveFile(currentPath);
                break;
            case 6:
                deleteFileOrDirectory(currentPath);
                break;
            case 7:
                renameFileOrDirectory(currentPath);
                break;
            case 8:
                viewFileContent(currentPath);
                break;
            case 9:
                getFileInfo(currentPath);
                break;
            case 10:
                cout << "Exiting the file manager." << endl;
                return 0;
            default:
                cout << "Invalid choice. Please try again." << endl;
        }
    }
    return 0;
}
