#include <iostream>
#include <vector>
#include <windows.h>
#include <fstream>
#include <sstream>
#include <cstdio>

using namespace std;

struct Contact {
    int contact_id, user_id;
    string contact_name, contact_surname, contact_phone_number, contact_email, contact_address;
};

struct User {
    int user_id;
    string user_name, user_password;
};

string get_in_line() {
    string get_in = "";
    getline (cin, get_in);
    return get_in;
}

char get_in_char() {
    string get_in = "";
    char in_char = {0};
    while (true) {
        getline (cin, get_in);
        if (get_in.length() == 1) {
            in_char = get_in[0];
            break;
        }
        cout << "Thats not a single character. Try again" << endl;
    }
    return in_char;
}

int get_in_number () {
    string get_in = "";
    int number = 0;

    while (true) {
        getline(cin, get_in);
        stringstream myStream(get_in);

        if (myStream >> number) {
            break;
        }
        cout << "That not a number. Try again" << endl;
    }
    return number;
}

void convert_string_to_vector (vector <string> &vector_out, string string_in) {
    string s;
    stringstream ss (string_in);

    while (getline( ss, s, '|')) {
        vector_out.push_back (s);
    }
}

int extract_user_from_file_to_vector (vector <User> &users, string file_line) {
    User user;
    vector <string> extracted_vector;

    convert_string_to_vector (extracted_vector, file_line);

    user.user_id = stoi(extracted_vector[0]);
    user.user_name = extracted_vector[1];
    user.user_password = extracted_vector[2];

    users.push_back(user);
    return user.user_id;
}

int open_source_file_with_users (vector <User> &users){
    User user;
    vector <string> users_data_file_line_vector;
    string users_data_file_line;
    int users_file_line_number = 1, id_counter = 0;

    fstream users_data_file;
    users_data_file.open ("users_data_file.txt", ios::in | ios::app);

    if (users_data_file_line == "") {
        id_counter = 0;
    }
    while (getline(users_data_file, users_data_file_line)) {
        id_counter = extract_user_from_file_to_vector (users, users_data_file_line);
        users_file_line_number++;
    }
    users_data_file.close();
    return id_counter;
}

void add_new_user_to_file (User user) {
    fstream users_data_file;
    users_data_file.open("users_data_file.txt", ios::out | ios::app);

    if (!users_data_file.good()) {
        cout << "There is a problem with file!" << endl;
        Sleep(1500);
    }
    users_data_file << user.user_id << "|" << user.user_name << "|" << user.user_password << "|"  << endl;

    users_data_file.close();
}

int add_new_user (vector <User> &users, int new_user_id ){
    User user;
    user.user_id = new_user_id + 1;
    cout << "Enter data for the new user!" << endl;
    cout << "Username: ";
    user.user_name = get_in_line();
    cout << "Now create a password: ";
    user.user_password = get_in_line();

    add_new_user_to_file (user);

    cout << "Please remember this details to log in --> " + user.user_name + " " + user.user_password << endl;
    system("pause");

    users.push_back(user);

    return user.user_id;
}

int log_in_user (vector <User> &users){
    int actual_user_id = 0;

    cout << endl << "Enter a Username to log in: ";
    string check_user_name = get_in_line();
    bool check_user_name_status = false;
    bool check_user_password_status = false;

    for (size_t i = 0; i < users.size(); i++) {
        if (users[i].user_name == check_user_name) {
            check_user_name_status = true;
            cout << endl << "Enter a password: ";
            string check_user_password = get_in_line();
            if (users[i].user_password == check_user_password){
                check_user_password_status = true;
                cout << endl << "Logged in succesfully :) "<< endl << endl;
                actual_user_id = users[i].user_id;
            }
        }
    }
    if (check_user_name_status == false) {
        cout << endl << "Entered nickname was not found! :( " << endl << endl;
    }
    else if (check_user_password_status == false){
        cout << endl << "Wrong password! :( " << endl << endl;
    }
    system("pause");
    return actual_user_id;
}

void fill_out_working_space (vector <Contact> &contacts, vector <Contact> &working_contacts, int actual_user_id){
    Contact contact;

    for (size_t i = 0; i < contacts.size(); i++) {
        if (contacts[i].user_id == actual_user_id) {
            contact = contacts[i];
            working_contacts.push_back(contact);
        }
    }
}

int open_user_login_interface (vector <User> users, vector <Contact> &contacts, vector <Contact> &working_contacts){
   int actual_user_id = 0;
   int user_id_counter = open_source_file_with_users (users);

   while (true) {
        system("cls");
        cout << ">>>   Address Book   <<<" << endl;
        cout << "------------------------" << endl;
        cout << ">>> Log In Interface <<<" << endl;
        cout << "------------------------" << endl << endl;
        cout << "1. Add new user " << endl;
        cout << "2. Log In " << endl << endl;
        cout << "9. Close " << endl << endl;

        cout << "Your choice: ";
        char menu_choice = get_in_char();

        switch(menu_choice) {
        case '1':
            user_id_counter = add_new_user (users, user_id_counter);
            break;
        case '2':
            actual_user_id = log_in_user (users);
            fill_out_working_space (contacts, working_contacts, actual_user_id);
            if (actual_user_id != 0) return actual_user_id;
            break;
        case '9':
            exit(0);
        }
    }
}

int extract_contact_from_file_to_vector (vector <Contact> &contacts, string file_line) {
    Contact contact;
    vector <string> extracted_vector;

    convert_string_to_vector (extracted_vector, file_line);

    contact.contact_id = stoi(extracted_vector[0]);
    contact.user_id = stoi(extracted_vector[1]);
    contact.contact_name = extracted_vector[2];
    contact.contact_surname = extracted_vector[3];
    contact.contact_phone_number = extracted_vector[4];
    contact.contact_email = extracted_vector[5];
    contact.contact_address = extracted_vector[6];

    contacts.push_back(contact);
    return contact.contact_id;
}

int open_source_file_with_contacts (vector <Contact> &contacts) {
    Contact contact;
    vector <string> address_book_file_line_vector;
    string address_book_file_line;
    int address_book_file_line_number = 1, id_counter = 0;

    fstream address_book_file;
    address_book_file.open ("address_book_file.txt", ios::in | ios::app);

    if (address_book_file_line == "") {
        id_counter = 0;
    }
    while (getline(address_book_file, address_book_file_line)) {
        id_counter = extract_contact_from_file_to_vector (contacts, address_book_file_line);
        address_book_file_line_number++;
    }
    address_book_file.close();
    return id_counter;
}

void add_new_contact_to_file (Contact contact) {
    fstream address_book_file;
    address_book_file.open("address_book_file.txt", ios::out | ios::app);

    if (!address_book_file.good()) {
        cout << "There is a problem with file!" << endl;
        Sleep(1500);
    }
    address_book_file << contact.contact_id << "|" << contact.user_id << "|" << contact.contact_name << "|" << contact.contact_surname
                      << "|" << contact.contact_phone_number << "|" << contact.contact_email << "|" <<contact.contact_address << "|" << endl;

    address_book_file.close();
}

int add_new_contact(vector <Contact> &contacts, vector <Contact> &working_contacts, int new_contact_id, int logged_user_id) {
    Contact contact;

    contact.contact_id = new_contact_id + 1;
    contact.user_id = logged_user_id;
    cout << "Please enter data for the new friend!" << endl;
    cout << "Name: ";
    contact.contact_name = get_in_line();
    cout << "Surname: ";
    contact.contact_surname = get_in_line();
    cout << "Telephone number: ";
    contact.contact_phone_number = get_in_line();
    cout << "E-mail: ";
    contact.contact_email = get_in_line();
    cout << "Address: ";
    contact.contact_address = get_in_line();

    add_new_contact_to_file (contact);

    cout << "You have a new friend--> " + contact.contact_name + " " + contact.contact_surname << endl;
    Sleep(1500);

    contacts.push_back(contact);
    working_contacts.push_back(contact);

    return contact.contact_id;
}

void print_contacts (vector <Contact> &contacts, int print_id) {
    cout << endl << "ID: " << contacts[print_id].contact_id << endl;
    cout << "Name: " + contacts[print_id].contact_name + " " + contacts[print_id].contact_surname << endl;
    cout << "Phone: " + contacts[print_id].contact_phone_number + "  E-mail: " + contacts[print_id].contact_email << endl;
    cout << "Address: " + contacts[print_id].contact_address << endl;
}

void show_all_contacts (vector <Contact> &contacts) {
    if (contacts.size() == 0) {
        cout << endl << "There is no contacts yet, please add new contact :)" << endl << endl;
    } else {
        for (size_t i = 0; i < contacts.size(); i++) {
            print_contacts (contacts, i);
        }
    }
    system("pause");
}

void find_by_name (vector <Contact> &contacts) {
    cout << "Enter a name to find: ";
    string check_name = get_in_line();
    bool check_name_status = false;

    for (size_t i = 0; i < contacts.size(); i++) {
        if (contacts[i].contact_name == check_name) {
            print_contacts (contacts, i);
            check_name_status = true;
        }
    }

    if (check_name_status == false) {
        cout << endl << "There is no people with that name! " << endl << endl;
    }
    system("pause");
}

void find_by_surname (vector <Contact> &contacts) {
    cout << "Enter a surname to find: ";
    string check_surname = get_in_line();
    bool check_surname_status = false;

    for (size_t i = 0; i < contacts.size(); i++) {
        if (contacts[i].contact_surname == check_surname) {
            print_contacts (contacts, i);
            check_surname_status = true;
        }
    }

    if (check_surname_status == false) {
        cout << endl << "There is no people with that surname! " << endl << endl;
    }
    system("pause");
}

void update_txt_file (vector <Contact> &contacts) {
    char file_name[] = "address_book_file.txt";
    remove (file_name);

    fstream address_book_file;
    address_book_file.open("address_book_file.txt", ios::out | ios::app);
    for (size_t i = 0; i < contacts.size(); i++) {
        address_book_file << contacts[i].contact_id << "|" << contacts[i].user_id << "|" << contacts[i].contact_name << "|" << contacts[i].contact_surname
                          << "|" << contacts[i].contact_phone_number << "|" << contacts[i].contact_email << "|" <<contacts[i].contact_address << "|" << endl;
    }
    address_book_file.close();
}

void remove_contact (vector <Contact> &contacts, vector <Contact> &working_contacts, int logged_user_id) {
    int erased_id = 0, erased_position = 0;
    bool find_status = false;
    bool access_status = false;
    string deleted_name = "", deleted_surname = "";

    cout << "Please enter ID to remove: " << endl;
    erased_id = get_in_number();

    for (size_t i = 0; i < contacts.size(); i++) {
        if (contacts[i].contact_id == erased_id) {
                find_status = true;
                if (contacts[i].user_id == logged_user_id){
                    access_status = true;
                    deleted_name = contacts[i].contact_name;
                    deleted_surname = contacts[i].contact_surname;
                }
            break;
        } else {
            erased_position ++;
        }
    }
    if (access_status == true) {
        contacts.erase(contacts.begin() + erased_position);
        cout << "Deleted ID: " << erased_id << endl;
        cout << "Say bye bye to: " << deleted_name << " "<< deleted_surname << endl << " We will miss you <3"<< endl << endl;
        update_txt_file (contacts);
        working_contacts.clear();
        fill_out_working_space (contacts, working_contacts, logged_user_id);
    } else if (find_status == true && access_status == false){
        cout << "You can't remove this user !!!" << endl;
    } else {
        cout << "There is no contact with entered ID !!!" << endl;
        }
    system("pause");
}

string parameter_edition (string parameter_to_edit) {
    cout << "Please enter new parameter: " << endl;
    string new_parameter = get_in_line();
    parameter_to_edit = new_parameter;
    return parameter_to_edit;
}

bool edition_menu_for_contact (vector <Contact> &contacts, int position_to_edit, bool edition_status) {
    while (true) {
        system("cls");
        cout << ">>> Editors menu <<<" << endl << endl;
        cout << "1. Change name " << endl;
        cout << "2. Change surname " << endl;
        cout << "3. Change phone number " << endl;
        cout << "4. Change e-mail " << endl;
        cout << "5. Change address " << endl;
        cout << "6. Back to main menu " << endl << endl;

        cout << "Your choice: ";
        char menu_choice = get_in_char();

        switch(menu_choice) {
        case '1':
            contacts[position_to_edit].contact_name = parameter_edition (contacts[position_to_edit].contact_name);
            edition_status = true;
            break;
        case '2':
            contacts[position_to_edit].contact_surname = parameter_edition (contacts[position_to_edit].contact_surname);
            edition_status = true;
            break;
        case '3':
            contacts[position_to_edit].contact_phone_number = parameter_edition (contacts[position_to_edit].contact_phone_number);
            edition_status = true;
            break;
        case '4':
            contacts[position_to_edit].contact_email = parameter_edition (contacts[position_to_edit].contact_email);
            edition_status = true;
            break;
        case '5':
            contacts[position_to_edit].contact_address = parameter_edition (contacts[position_to_edit].contact_address);
            edition_status = true;
            break;
        case '6':
            cout << "No changes have been made !!!" << endl ;
            break;
        }
        break;
    }
    return edition_status;
}

void edit_contact (vector <Contact> &contacts, vector <Contact> &working_contacts, int logged_user_id) {
    int id_to_edit = 0, position_to_edit = 0;
    bool find_status = false;
    bool edition_status = false;
    bool access_status = false;

    cout << "Please enter ID to edit: " << endl;
    id_to_edit = get_in_number();

    for (size_t i = 0; i < contacts.size(); i++) {
        if (contacts[i].contact_id == id_to_edit) {
            if (contacts[i].user_id == logged_user_id){
                find_status = true;
                access_status = true;
                position_to_edit = i;
                break;
            } else {
                    find_status = true;
                    cout << "You cant edit this contact !!!" << endl;
                    }
                }
        }
    if (access_status == true) {
        edition_status = edition_menu_for_contact (contacts, position_to_edit, edition_status);
        update_txt_file (contacts);
        working_contacts.clear();
        fill_out_working_space (contacts, working_contacts, logged_user_id);
        if (edition_status == true) cout << "Successfully edited ID: " << id_to_edit << endl;
    } else if (access_status == false && find_status == false){
        cout << "There is no contact with entered ID !!!" << endl;
    }
    system("pause");
}

void main_menu_interface (vector <Contact> &contacts, vector <Contact> &working_contacts, vector <User> &users, int logged_user_id, int id_counter){

    while (true) {
        system("cls");
        cout << ">>>   Address Book   <<<" << endl;
        cout << "------------------------" << endl;
        cout << ">>>    Main menu     <<<" << endl;
        cout << "------------------------" << endl;
        cout << "Logged In --> ID: " << logged_user_id << endl;
        cout << "------------------------" << endl << endl;
        cout << "1. Add new contact " << endl;
        cout << "2. Find by name " << endl;
        cout << "3. Find by surname " << endl;
        cout << "4. Show all " << endl;
        cout << "5. Remove contact " << endl;
        cout << "6. Edit contact " << endl;
        cout << "------------------------" << endl << endl;
        cout << "7. Change password " << endl;
        cout << "------------------------" << endl << endl;
        cout << "9. Log out" << endl << endl;

        cout << "Your choice: ";
        char menu_choice = get_in_char();

        switch(menu_choice) {
        case '1':
            id_counter = add_new_contact (contacts, working_contacts, id_counter, logged_user_id);
            break;
        case '2':
            find_by_name (working_contacts);
            break;
        case '3':
            find_by_surname (working_contacts);
            break;
        case '4':
            show_all_contacts (working_contacts);
            break;
        case '5':
            remove_contact (contacts, working_contacts, logged_user_id);
            break;
        case '6':
            edit_contact (contacts, working_contacts, logged_user_id);
            break;
        case '7':
            //change password function
            break;
        case '9':
            working_contacts.clear();
            logged_user_id = open_user_login_interface (users, contacts, working_contacts);
            break;
        }
    }
}

int main() {
    vector <Contact> contacts;
    vector <Contact> working_contacts;
    vector <User> users;

    int id_counter = open_source_file_with_contacts (contacts);
    int logged_user_id = open_user_login_interface (users, contacts, working_contacts);

    main_menu_interface (contacts, working_contacts, users, logged_user_id, id_counter);

    return 0;
}
