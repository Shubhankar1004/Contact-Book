#include <bits/stdc++.h>

#define RESET "\033[0m"
#define BLACK "\033[30m"			  /* Black */
#define RED "\033[31m"				  /* Red */
#define GREEN "\033[32m"			  /* Green */
#define YELLOW "\033[33m"			  /* Yellow */
#define BLUE "\033[34m"				  /* Blue */
#define MAGENTA "\033[35m"			  /* Magenta */
#define CYAN "\033[36m"				  /* Cyan */
#define WHITE "\033[37m"			  /* White */
#define BOLDBLACK "\033[1m\033[30m"	  /* Bold Black */
#define BOLDRED "\033[1m\033[31m"	  /* Bold Red */
#define BOLDGREEN "\033[1m\033[32m"	  /* Bold Green */
#define BOLDYELLOW "\033[1m\033[33m"  /* Bold Yellow */
#define BOLDBLUE "\033[1m\033[34m"	  /* Bold Blue */
#define BOLDMAGENTA "\033[1m\033[35m" /* Bold Magenta */
#define BOLDCYAN "\033[1m\033[36m"	  /* Bold Cyan */
#define BOLDWHITE "\033[1m\033[37m"	  /* Bold White */
void clrscr()
{
	std::cout << "\033[2J\033[1;1H";
}

class Contact
{
private:
	std::string number;
	std::string email;
	int frequency;
public:
	std::string name;
	void setphone(std::string phoneno){
		std::regex rp("\\d{10}");
		auto isValidPhoneNumber = std::regex_match(phoneno, rp);
		if (!(isValidPhoneNumber))
		{
			clrscr();
			std::cout << RED << "\ninvalid phoneno" << RESET << std::endl;
			std::this_thread::sleep_for(std::chrono::seconds(2));
			return;
		}
		number=phoneno;
	}
	void setemail(std::string email){
		std::regex re("(\\w+)(\\.|_)?(\\w*)@(\\w+)(\\.(\\w+))+");
		auto isValidemail = std::regex_match(email, re);
		if (!(isValidemail))
		{
			clrscr();
			std::cout << RED << "\ninvalid email" << RESET << std::endl;
			std::this_thread::sleep_for(std::chrono::seconds(2));
			return;
		}
		this->email=email;
	}

	std::string getphoneno(){
		return number;
	}
	std::string getemail(){
		return email;
	}

	int freq(){
		return frequency;
	}


	Contact(std::string name, std::string number, std::string email)
	{
		this->name = name;
		setphone(number);
		setemail(email);
		frequency = 0;
	}
	void visited()
	{
		frequency++;
	}
};

class TrieNode
{
public:
	std::unordered_map<char, TrieNode *> child;
	TrieNode *parent;
	bool isLast;
	TrieNode()
	{
		child = std::unordered_map<char, TrieNode *>();
		for (char i = 'a'; i <= 'z'; i++)
		{
			child[i] = nullptr;
		}
		parent = nullptr;
		isLast = false;
	}
};
class Trie
{
public:
	TrieNode *root;
	Trie()
	{
		root = new TrieNode();
	}
	void insert(std::string s)
	{
		int len = s.length();
		TrieNode *itr = root;
		for (int i = 0; i < len; i++)
		{
			TrieNode *nextNode = itr->child[s[i]];
			if (nextNode == nullptr)
			{
				nextNode = new TrieNode();
				nextNode->parent = itr;
				itr->child[s[i]] = nextNode;
			}
			itr = nextNode;
			if (i == len - 1)
			{
				itr->isLast = true;
			}
		}
	}
	void deletenodes(TrieNode *curNode, TrieNode *deletetill, std::stack<char> letters)
	{
		TrieNode *prevnode = curNode->parent;
		bool flag = false;
		if (curNode != deletetill)
			flag = true;

		if (curNode == root)
			return;
		delete curNode->parent->child[letters.top()];
		curNode->parent->child[letters.top()] = nullptr;
		curNode = NULL;

		letters.pop();
		if (flag)
			deletenodes(prevnode, deletetill, letters);
	}
	void deletename(std::string str)
	{
		TrieNode *prevNode = root;
		TrieNode *deletetill = root;
		int len = str.length();
		std::string name;
		std::stack<char> letters;
		int i;
		for (i = 0; i < len; i++)
		{
			name += (char)str[i];
			char lastChar = name[i];
			letters.push(lastChar);
			bool deletetillflag = false;
			if (prevNode->isLast)
			{
				deletetillflag = true;
			}
			else
			{
				for (char j = 'a'; j <= 'z'; j++)
				{
					if ((prevNode->child[j] != nullptr) && lastChar != j)
					{
						deletetillflag = true;
						break;
					}
				}
			}

			TrieNode *curNode = prevNode->child[lastChar];
			if (deletetillflag)
			{
				deletetill = curNode;
			}
			if (curNode == NULL)
			{
				std::cout << "\nNo Results Found for " << name
						  << "\n";
				i++;
				break;
			}
			prevNode = curNode;
			if (i == len - 1)
			{
				char j;
				for (j = 'a'; j <= 'z'; j++)
				{
					if (prevNode->child[j] != nullptr)
					{
						prevNode->isLast = false;
						return;
					}
				}
			}
		}
		deletenodes(prevNode, deletetill, letters);
	}

	void displayalphabetically(TrieNode *curNode, std::string prefix, std::vector<std::string> *arrangedcontacts)
	{
		if (curNode == nullptr)
		{
			return;
		}
		else
		{
			if (curNode->isLast)
			{
				arrangedcontacts->push_back(prefix);
			}
			for (char i = 'a'; i <= 'z'; i++)
			{
				if (curNode->child[i] != nullptr)
				{
					displayalphabetically(curNode->child[i], prefix + i, arrangedcontacts);
				}
			}
		}
	}

	void displayContactsUtil(TrieNode *curNode, std::string prefix, std::vector<std::string> *contactsWithPrefix)
	{
		if (curNode->isLast)
		{
			contactsWithPrefix->push_back(prefix);
		}
		for (char i = 'a'; i <= 'z'; i++)
		{
			TrieNode *nextNode = curNode->child[i];
			if (nextNode != nullptr)
			{
				displayContactsUtil(nextNode, prefix + i, contactsWithPrefix);
			}
		}
	}
	void getContacts(std::string str, std::vector<std::string> *arrangedcontacts)
	{
		TrieNode *prevNode = root;

		std::string prefix = "";
		int len = str.length();

		int i;
		for (i = 0; i < len; i++)
		{
			prefix += (char)str[i];
			char lastChar = prefix[i];
			TrieNode *curNode = prevNode->child[lastChar];
			if (curNode == NULL)
			{
				std::cout << "\nNo Results Found for " << str
						  << "\n";
				i++;
				break;
			}
			if (i == len - 1)
			{
				std::cout << "\nSuggestions based on " << prefix
						  << " are \n";
				displayContactsUtil(curNode, prefix, arrangedcontacts);
			}
			prevNode = curNode;
		}
	}
};

class PhoneBookDirectory
{
public:
	std::unordered_map<std::string, Contact *> phonebook;
	Trie *trie;
	std::vector<std::string> *frequentlyvisited;

	int Partition(std::vector<std::string> &v, int start, int end)
	{

		int pivot = end;
		int j = start;
		for (int i = start; i < end; ++i)
		{
			if (phonebook[v[i]]->freq() > phonebook[v[pivot]]->freq())
			{
				std::swap(v[i], v[j]);
				++j;
			}
		}
		swap(v[j], v[pivot]);
		return j;
	}

	void Quicksort(std::vector<std::string> &v, int start, int end)
	{

		if (start < end)
		{
			int p = Partition(v, start, end);
			Quicksort(v, start, p - 1);
			Quicksort(v, p + 1, end);
		}
	}

	PhoneBookDirectory()
	{
		phonebook = std::unordered_map<std::string, Contact *>();
		trie = new Trie();
		frequentlyvisited = new std::vector<std::string>;
	}
	void addContact()
	{
		std::string name;
		std::string phoneno;
		std::string email;
		clrscr();
		std::cout << BOLDCYAN << "ADD CONTACT" << RESET << std::endl;
		std::cout << "name: ";
		std::cin >> name;
		std::cout << "number: ";
		std::cin >> phoneno;
		std::regex rp("\\d{10}");
		auto isValidPhoneNumber = std::regex_match(phoneno, rp);
		if (!(isValidPhoneNumber))
		{
			clrscr();
			std::cout << RED << "\ninvalid phoneno" << RESET << std::endl;
			std::this_thread::sleep_for(std::chrono::seconds(2));
			return;
		}
		std::cout << "email: ";
		std::cin >> email;
		std::regex re("(\\w+)(\\.|_)?(\\w*)@(\\w+)(\\.(\\w+))+");
		auto isValidemail = std::regex_match(email, re);
		if (!(isValidemail))
		{
			clrscr();
			std::cout << RED << "\ninvalid email" << RESET << std::endl;
			std::this_thread::sleep_for(std::chrono::seconds(2));
			return;
		}
		if (!(phonebook.find(name) == phonebook.end()))
		{
			clrscr();
			std::cout << RED << "\ncontact with name " << name << " already exists" << RESET << std::endl;
			std::this_thread::sleep_for(std::chrono::seconds(2));
			return;
		}

		/////

		Contact *newcont = new Contact(name, phoneno, email);
		phonebook[name] = newcont;
		frequentlyvisited->push_back(name);
		trie->insert(name);
		clrscr();
		std::cout << GREEN << "\nadded " << name << "'s contact" << RESET << std::endl;
		std::this_thread::sleep_for(std::chrono::seconds(2));
	}

	void selectcontact(std::vector<std::string> *arrangedcontacts)
	{
		int index = 0;
		int selector;
		int action = 0;
		int operation;
		for (auto i : *arrangedcontacts)
		{
			std::cout << ++index << ": " << i << std::endl;
		}
		std::cout << "\nselect contact " << RED << "(or press 0 to exit)" << RESET ": ";
		std::cin >> selector;
		if (selector == 0)
			return;
		phonebook[arrangedcontacts->at(selector - 1)]->visited();
		clrscr();
		std::cout << BOLDCYAN << "VIEWING CONTACT" << RESET << std::endl;
		std::cout << "\n"
				  << arrangedcontacts->at(selector - 1) << "\t" << phonebook[arrangedcontacts->at(selector - 1)]->getphoneno() << "\t" << phonebook[arrangedcontacts->at(selector - 1)]->getemail() << "\n"
				  << std::endl;
		std::cout << RED << "1. delete\n"
				  << RESET << "2. edit\n3. exit" << std::endl;
		std::cin >> action;
		switch (action)
		{
		case 1:
			std::cout << arrangedcontacts->at(selector - 1);
			std::cout.flush();
			deletecontact(arrangedcontacts->at(selector - 1));
			break;
		case 2:
			editcontact(arrangedcontacts->at(selector - 1));
			break;

		default:
			return;
		}
	};

	void showfrequent()
	{
		clrscr();
		std::cout << BOLDCYAN << "FREQUENT CONTACTS" << RESET << std::endl;
		Quicksort(*frequentlyvisited, 0, frequentlyvisited->size() - 1);
		selectcontact(frequentlyvisited);
	}
	void displaysearchresults(std::vector<std::string> arrangedcontacts)
	{
		for (auto i : arrangedcontacts)
		{
			std::cout << i << "\n";
		}
	}
	void searchAndShowNoForAllCombination()
	{
		std::vector<std::string> arrangedcontacts;
		std::string query = "";
		char c;
		do
		{
			clrscr();
			std::cout << BOLDCYAN << "SEARCH" << RESET << std::endl;
			std::cout << "name: " << query << "|" << GREEN << "press 0 to select contact" << RESET;
			trie->getContacts(query, &arrangedcontacts);
			displaysearchresults(arrangedcontacts);
			std::cin >> c;
			if (c == '0')
				break;
			query = query + c;
			arrangedcontacts.clear();
		} while (true);
		clrscr();
		selectcontact(&arrangedcontacts);
	}

	void showalphabetically()
	{
		clrscr();
		std::cout << BOLDCYAN << "ALL CONTACTS" << RESET << std::endl;
		std::vector<std::string> arrangedcontacts;
		if (phonebook.empty())
		{
			std::cout << RED << "\ndirectory empty\n"
					  << RESET << std::endl;
			std::this_thread::sleep_for(std::chrono::seconds(2));
			return;
		}
		trie->displayalphabetically(trie->root, "", &arrangedcontacts);
		selectcontact(&arrangedcontacts);
	}

	void deletecontact(std::string name)
	{
		trie->deletename(name);
		phonebook.erase(name);
		std::vector<std::string>::iterator itr;
		for (itr = frequentlyvisited->begin(); itr != frequentlyvisited->end(); itr++)
		{
			if (*itr == name)
			{
				frequentlyvisited->erase(itr);
				break;
			}
		}
		clrscr();
		std::cout << RED << "\ndeleted " << name << "'s contact" << RESET << std::endl;
		std::this_thread::sleep_for(std::chrono::seconds(2));
	}
	void editcontact(std::string name)
	{
		clrscr();
		char selector;
		std::cout << BOLDCYAN << "editing " << name << "..." << RESET << std::endl;
		std::string newname = name;
		std::string newnumber;
		std::string newemail;
		std::cout << BOLDWHITE << "\nedit name?: " << BOLDGREEN << "y" << RESET << "/" << BOLDRED << "n" << RESET << std::endl;
		std::cin >> selector;
		if (selector == 'y')
		{
			std::cout << BOLDWHITE << "type new name: " << RESET;
			std::cin >> newname;
			trie->deletename(name);
			trie->insert(newname);
			phonebook[newname] = phonebook[name];
			phonebook.erase(name);
			std::vector<std::string>::iterator itr;
			for (itr = frequentlyvisited->begin(); itr != frequentlyvisited->end(); itr++)
			{
				if (*itr == name)
				{
					frequentlyvisited->erase(itr);
					break;
				}
			}
			frequentlyvisited->push_back(newname);
		}
		std::cout << BOLDWHITE << "\nedit number?: " << BOLDGREEN << "y" << RESET << "/" << BOLDRED << "n" << RESET << std::endl;
		std::cin >> selector;
		if (selector == 'y')
		{
			std::cout << BOLDWHITE << "type new number: " << RESET;
			std::cin >> newnumber;
			phonebook[newname]->setphone(newnumber);
		}
		std::cout << BOLDWHITE << "\nedit email?: " << BOLDGREEN << "y" << RESET << "/" << BOLDRED << "n" << RESET << std::endl;
		std::cin >> selector;
		if (selector == 'y')
		{
			std::cout << BOLDWHITE << "type new number: " << RESET;
			std::cin >> newemail;
			phonebook[newname]->setemail(newemail);
		}
		clrscr();
		std::cout << GREEN << "\nupdated " << name << "'s contact" << RESET << std::endl;
		std::this_thread::sleep_for(std::chrono::seconds(2));
	}
};

class Account
{
public:
	std::string username;
	bool checkpass(std::string pass)
	{
		return pass == password;
	}
	PhoneBookDirectory *getdir()
	{
		return directory;
	}
	Account(std::string username, std::string password)
	{
		this->username = username;
		this->password = password;
		directory = new PhoneBookDirectory();
	}

private:
	std::string password;
	PhoneBookDirectory *directory;
};

void dashboard(Account *account)
{
	PhoneBookDirectory *directory = new PhoneBookDirectory();
	directory = account->getdir();
	bool loggedin = true;
	do
	{
		clrscr();
		std::cout << BOLDCYAN << "Hi " << account->username << "!" << RESET << std::endl;
		std::cout << "\n1. view all contacts\n2. search contact\n3. add contact\n4. show frequent contacts\n5. logout\n";
		int x;
		std::cin >> x;
		switch (x)
		{
		case 5:
			loggedin = false;
			break;
		case 4:
			directory->showfrequent();
			break;
		case 3:
			directory->addContact();
			break;

		case 2:
			directory->searchAndShowNoForAllCombination();
			break;

		case 1:
			directory->showalphabetically();
			break;

		default:
			break;
		}
	} while (loggedin);
}

void createaccount(std::unordered_map<std::string, Account *> &accounts)
{
	clrscr();
	std::cout << BOLDCYAN << "NEW ACCOUNT" << RESET << std::endl;
	std::string username;
	std::string password;
	std::cout << BOLDWHITE << "username: " << RESET;
	std::cin >> username;
	std::cout << BOLDWHITE << "password: " << RESET;
	std::cin >> password;
	Account *newaccount = new Account(username, password);
	accounts[username] = newaccount;
	dashboard(newaccount);
}

void login(std::unordered_map<std::string, Account *> &accounts)
{
	char ynselector;
	std::string username;
	std::string password;
	std::cout << BOLDCYAN << "CONTACTS APP" << RESET << std::endl;
	if (accounts.empty())
	{
		std::cout << "no accounts available\ncreate new account?: " << BOLDGREEN << "y" << RESET << "/" << BOLDRED << "n" << RESET << std::endl;
		std::cin >> ynselector;
		if (ynselector == 'y')
		{
			createaccount(accounts);
		}
	}
	else
	{
		int selector;
		std::cout << BOLDWHITE << "1. login to existing account\n2. create a new account\n3. quit \n"
				  << RESET;
		std::cin >> selector;
		if (selector == 1)
		{
			clrscr();
			std::cout << BOLDCYAN << "LOGIN" << RESET << std::endl;
			std::cout << BOLDWHITE << "username: " << RESET;
			std::cin >> username;
			std::cout << BOLDWHITE << "password: " << RESET;
			std::cin >> password;
			try
			{
				if (accounts[username]->checkpass(password))
				{
					dashboard(accounts[username]);
				}
				else
				{
					std::cout << RED << "password incorrect\n"
							  << RESET;
				}
			}
			catch (...)
			{
				std::cout << RED << "account doesnt exist" << RESET;
			}
		}
		else if (selector == 2)
		{
			createaccount(accounts);
		}
		else
		{
			exit(0);
		}
	}
}

int main()
{
	std::unordered_map<std::string, Account *> *accounts = new std::unordered_map<std::string, Account *>;
	do
	{
		clrscr();
		login(*accounts);
	} while (true);
}
