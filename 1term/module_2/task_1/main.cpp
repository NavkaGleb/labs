#include <fstream>
#include <iostream>
#include <vector>

using namespace std;

int main() {

    const char* file_name = "../c_file.txt";
    vector<string> file_copy;

    string line;

    ifstream r_file(file_name, ios_base::in);

    if (r_file.fail()) {

        cout << "problem with the file!\n";
        return 0;

    } else {

        while(getline(r_file, line))
            if (line.find("//") == string::npos)
                file_copy.push_back(line);

    }

    r_file.close();


    ofstream w_file(file_name, ios_base::out);

    if (w_file.fail()) {

        cout << "problem with the file!\n";
        return 0;

    } else {

        for (const auto& v_line : file_copy)
            w_file << v_line << endl;

        cout << "file overwritten successfully\n";
        cout << "check the file!\n";

    }

    w_file.close();

    return 0;

}
