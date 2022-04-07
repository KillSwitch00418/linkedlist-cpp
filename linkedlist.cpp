#include <iostream>
#include <fstream>
#include <string>
using namespace std;

struct Person
{
	string social;
	string bDay;
	string firstName;
	string lastName;
	string zipcode;
};

struct Node
{
	Person person;

	Node *left;
	Node *right;

	Node(string ssn = NULL, string bd = NULL, string fn = NULL, string ln = NULL, string zip = NULL, Node *lt = NULL, Node *rt = NULL)
	{
		
		person.firstName = fn;
		person.lastName = ln;
		person.social = ssn;
		person.bDay = bd;
		person.zipcode = zip;
		left = lt;
		right = rt;
	}
};

class Btree
{
protected:
	Node *root;

public:
	Btree()
	{
		root = NULL;
	}

	void add(string ssn, string bd, string fn, string ln, string zip)
	{
		add(root, ssn, bd, fn, ln, zip);
	}

	void add(Node *&a, string ssn, string bd, string fn, string ln, string zip)
	{
		if (a == NULL)
		{
			a = new Node(ssn, bd, fn, ln, zip);
			return;
		}
		if (fn + ln < a->person.firstName + a->person.lastName)
			add(a->left, ssn, bd, fn, ln, zip);
		else
			add(a->right, ssn, bd, fn, ln, zip);
	}
	

	void add_loop(Node *&a, string ssn, string bd, string fn, string ln, string zip)
	{
		if (a == NULL)
		{
			a = new Node(ssn, bd, fn, ln, zip);
			return;
		}
		Node *prev = NULL;
		while (a != NULL)
		{
			prev = a;
			if (fn + ln < a->person.firstName + a->person.lastName)
				a = a->left;
			else
				a = a->right;
		}
		if (fn + ln < a->person.firstName + a->person.lastName)
			prev->left = new Node(ssn, bd, fn, ln, zip);
		else
			prev->right = new Node(ssn, bd, fn, ln, zip);
	}


	void print(Node *a)
	{
		if (a == NULL)
			return;
		print(a->left);
		cout << a->person.social + " " << a->person.bDay + " " <<  a->person.firstName + " " << a->person.lastName + " " << a->person.zipcode << endl;
		print(a->right);
	}

	void print()
	{
		print(root);
	}

	bool find_name(string firstName, string lastName)
	{
		Node *current = root;
		while (current != NULL)
		{
			if (current->person.firstName + current->person.lastName == firstName + lastName)
			{
				cout << current->person.social + " " << current->person.bDay + " " << current->person.firstName + " " << current->person.lastName + " " << current->person.zipcode << endl;
				return true;
			}
			else if (firstName + lastName < current->person.firstName + current->person.lastName)
				current = current->left;
			else
				current = current->right;
		}
		cout << "Person not found." << endl;
		return false;
	}

	void find_zipcode(string zipcode)
	{
		find_zipcode(root, zipcode);
	}
	
	void find_zipcode(Node *a, string zip)
	{
		if (a == NULL)
			cout << "Zipcode not found." << endl;
			return;
		find_zipcode(a->left, zip);
		if (zip == a->person.zipcode)
			cout << a->person.social + " " << a->person.bDay + " "  << a->person.firstName + " " << a->person.lastName + " " << a->person.zipcode << endl;
		find_zipcode(a->right, zip);
	}

	

	void find_oldest(Node *a, Person & person)
	{
		if (a == NULL)
			return;
		find_oldest(a->left, person);
		if (person.bDay > a->person.bDay)
		{
			person.firstName = a->person.firstName;
			person.lastName = a->person.lastName;
			person.social = a->person.social;
			person.bDay = a->person.bDay;
			person.zipcode = a->person.zipcode;
		}
		find_oldest(a->right, person);
	}

	void find_oldest()
	{
		if(root == NULL) return;
		Person person;
		person.zipcode = root->person.zipcode;
		person.bDay = root->person.bDay;
		person.social = root->person.social;
		person.firstName = root->person.firstName;
		person.lastName = root->person.lastName;
		find_oldest(root, person);
		cout << person.social + " " << person.bDay + " "  << person.firstName + " " << person.lastName + " " << person.zipcode << endl;
	}

	void delete_person(string fn, string ln)
	{
		delete_person(fn, ln, root);
	}

	bool delete_person(string firstName, string lastName, Node * & current)
	{
		Node * actual_root = root;
		while (current != NULL)
		{
			if (current->person.firstName + current->person.lastName == firstName + lastName)
			{ 
				//found it
				if (current->left == NULL && current->right == NULL)
				{	
					current= NULL;
					cout << "Deletion successsful." << endl;
					return true;
				}
				else if (current->left == NULL)
				{
					current = current ->right;
					cout << "Deletion successsful." << endl;
					return true;
				}
				else if  (current->right == NULL)
				{
					current = current ->left;
					cout << "Deletion successsful." << endl;
					return true;
				}
				else
				{
				Node * repl = find_smallest(current->right);
				current = repl;

				root = actual_root;
				cout << "Deletion successsful." << endl;
				return true;
				}
			}
			else if (firstName + lastName < current->person.firstName + current->person.lastName)
				current = current->left;
			else
				current = current->right;
		}
		cout << "Person not found." << endl;
		return false;
	}
	
	Node* find_smallest(Node * & b)
	{
		b = b -> right;
		if (b -> left != NULL)
			find_smallest(b -> left);
		else
			return b;
		return 0;
	}	
};

int main()
{

	ifstream myfile;
	myfile.open("/home/218/database.txt");
	int i = 0;
	Btree tree;
	string social, birth, first_name, last_name, zip_code;
	while (true)
	{
		if (myfile.fail())
			break;
		myfile >> social >> birth >> first_name >> last_name >> zip_code;
		if (social == "")
			break;
		tree.add(social, birth, first_name, last_name, zip_code);
	}
	myfile.close();

	cout << "data read successfully" << endl;

	string command;
	string answer1;
	string answer2;
	while (true)
	{
		cout << "Here is what you can do: type the following\n find: finds and prints all info of person\n all: prints info of everyone\n zipcode: creates list of people who live in given zipcode, then prints all their info\n oldest: prints name and zipcode of oldest person\n delete: finds and deletes all info of the given person\n exit: pretty obvious\n";
		
		cin >> command;
		if (command == "exit")
		{
			cout << "Goodbye." << endl;
			break;
		}
		

		if (command == "zipcode")
		{
			cout << "input zipcode:";
			cin >> answer1;
			tree.find_zipcode(answer1);
		}
		
		if (command == "find")
		{
			cout << "input firstname,then lastname:" << endl;
			cin >> answer1;
			cin >> answer2;

			tree.find_name(answer1, answer2);
		}

		if (command == "all")
		{
			tree.print();
		}

		if (command == "oldest")
		{
			tree.find_oldest();
		}
		
		if (command == "delete")
		{
			cout << "input firstname,then lastname:" << endl;;
			cin >> answer1;
			cin >> answer2;

			tree.delete_person(answer1, answer2);
		}
	}

	return 0;
}
