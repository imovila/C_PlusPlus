/*
	Using DataCollection, 
	FileHandler, Validator
*/

#include "stdafx.h"
	#include<iostream>
	#include<string>
	#include<vector>
	#include<algorithm>
	#include<fstream>
	#include<regex>
	using namespace std;

struct Author
{
	string lastName;
	string firstName;
	Author()
	{
		lastName = "";
		firstName = "";
	}
	Author(string lastname, string firstname)
	{
		lastName = lastname;
		firstName = firstname;
	}
};
struct Date
{
	int day, month, year;
	Date():
		year(0),
		month(0),
		day(0){}
	Date(int year, int month, int day):
		year(year),
		month(month),
		day(day){}
};
struct Book
{
	string isbn;
	string codeBar;
	string title;
	int edition;
	int numbOfPages;
	Author author;
	string pubHouse;
	Date pubDate;
	Book():
		codeBar(""),
		isbn(""),
		title(""),
		edition(0),
		numbOfPages(0),
		pubHouse(""),
		pubDate(Date(0, 0, 0)),
		author(Author("", "")){}
	Book(string isbn, string codeBar, string title, int edition, 
		int numbOfPages, Author author, string pubHouse, Date pubDate) :
		isbn(isbn),
		codeBar(codeBar),
		title(title),
		edition(edition),
		numbOfPages(numbOfPages),
		author(author),
		pubHouse(pubHouse),
		pubDate(pubDate){}
};

template <class T>
struct DataCollection {
	vector<T> myVector;

	vector<T> myVectorForOld;

	void add(T myObj) {
		myVector.push_back(myObj);
	}

	void add() {
		T myObj = Book();
		cin >> myObj;
		add(myObj);
	}

	void add(vector<T> myVec) {
		for each(T t in myVec)
			add(t);
	}

	int size() {
		return myVector.size();
	}

	void show(vector<T> myObj) {
		int i(0);
		for (vector<T>::iterator itr = myObj.begin(); itr != myObj.end(); itr++)
			cout << "\n" << ++i << *itr << endl;
	}

	void display(const bool all) {
		if (all)
			show(myVector);
		else
			show(myVectorForOld);
	}

	void erase() {
		myVector.clear();
	}

	typename vector<T>::iterator getEndofContainer() {
		return myVector.end();
	}

	typename vector<T>::iterator search(string isbn) {
		return find(myVector.begin(), myVector.end(), isbn);
	}

	vector<T> search(Date date) {
		vector<T> myTmpVec;
		for each(T myObj in myVector)
		{
			if (date.month != -1) {
				if ((Date)myObj.pubDate == date)
					myTmpVec.push_back(myObj);
			}
			else
			{
				if (myObj.pubDate.year < date.year)
					myTmpVec.push_back(myObj);
			}
		}
		return myTmpVec;
	}

	void sortByIsbn() {
		sort(myVector.begin(), myVector.end(), byIsbn);
	}

	void sortByPubDate() {
		sort(myVector.begin(), myVector.end(), byPubDate);
	}

	int moveOld(int year) {
		vector<T> tmp = search(Date(year, -1, -1));
		myVectorForOld.insert(myVectorForOld.end(), tmp.begin(), tmp.end());
		return myVectorForOld.size();
	}

	vector<T> getObj() {
		return myVector;
	}

};

template <class T>
struct FileHandler {

	static const char * myfile() {
		return "dataCollectionFile.txt";
	}

	static void writeData(vector<T> myVector) {
		ofstream outFile(myfile(), ios::out);

		for each(T myObj in myVector)
		{
			outFile << myObj.isbn << "|";
			outFile << myObj.codeBar << "|";
			outFile << myObj.title << "|";
			outFile << myObj.edition << "|";
			outFile << myObj.numbOfPages << "|";
			outFile << myObj.author.firstName << "|";
			outFile << myObj.author.lastName << "|";
			outFile << myObj.pubHouse << "|";
			outFile << myObj.pubDate.year << "/" << myObj.pubDate.month;
			outFile << "/" << myObj.pubDate.day << "|";
		}
		outFile.close();
	}

	static vector<T> readData()
	{
		ifstream inFile(myfile(), ios::in);
		string line; int q;
		vector<T> myVector;
		T myObj;

		if (inFile.is_open())
		{
			while (inFile >> q)
			{
				getline(inFile, myObj.isbn, '|');
				getline(inFile, myObj.codeBar, '|');
				getline(inFile, myObj.title, '|');
				getline(inFile, line, '|');
				myObj.edition = stoi(line);
				getline(inFile, line, '|');
				myObj.numbOfPages = stoi(line);
				getline(inFile, myObj.author.firstName, '|');
				getline(inFile, myObj.author.lastName, '|');
				getline(inFile, myObj.pubHouse, '|');
				getline(inFile, line, '|');
				string year = line.substr(0, line.find_first_of('/'));
				string day = line.substr(line.find_last_of('/') + 1, line.size());
				string month = line.substr(year.size() + 1, line.size() - day.size() - year.size() - 2);
				myObj.pubDate.year = stoi(year);
				myObj.pubDate.month = stoi(month);
				myObj.pubDate.day = stoi(day);
				myVector.push_back(myObj);
			}
			inFile.close();
		}
		else
			cout << "Error opening file";
		return myVector;
	}
};

struct Validator {

	struct infoValidation {
		static string isbn() { return " <Digits and length 8>"; }
		static string codeBar() { return " <Digits and length 10>"; }
		static string name() { return " <Leters, space and length between 3 and 30>"; }
		static string inT() { return " <Digits>"; }
		static string year() { return " <Digits and length 4>"; }
		static string month() { return " <[1][0-2] or [1-9]>"; }
		static string day() { return " <[1][0-9] or [2][0-9] or [3][0-1] or [1-9]>"; }
	};

	static bool validIsbn(string s) {
		return regex_match(s, regex("\\d{8}"));
	}

	static bool validCodeBar(string s) {
		return regex_match(s, regex("\\d{10}"));
	}

	static bool validName(string s) {
		return regex_match(s, regex("^([a-zA-Z]\\s?){3,30}$"));
	}

	static bool validInt(string s) {
		return regex_match(s, regex("\\d"));
	}

	static bool validYear(string s) {
		return regex_match(s, regex("\\d{4}"));
	}

	static bool validMonth(string s) {
		return regex_match(s, regex("^([1][0-2]|[1-9])$"));
	}

	static bool validDay(string s) {
		return regex_match(s, regex("^([1][0-9]|[2][0-9]|[3][0-1]|[1-9])$"));
	}

	static string errMessage() {
		return "\n\t\tData is not in the required format ! \n\t\tPlease retry !\n\t\t";
	}

};

inline istream & operator >> (istream &, Book &);
inline ostream & operator << (ostream &, Book &);
inline bool operator == (const Date &, const Date &);
inline bool operator == (const Book &, const string &);

void display(Book &);

inline bool operator > (Date &, Date &);

inline bool byIsbn(Book &, Book &);
inline bool byPubDate(Book &, Book &);

void main()
{
	int option(0);
	bool checkStep = false;
	DataCollection<Book> myDC;

	do
	{
		system("cls");
		cout << "\n-- -- -- Choose an option -- -- --\n";
		cout << "\t1 - Add a new book" << endl;
		cout << "\t2 - Search for a book by ISBN" << endl;
		cout << "\t3 - Search for a book by published date" << endl;
		cout << "\t4 - Show all books from the library" << endl;
		cout << "\t5 - Sort books by ISBN (in ascending order)" << endl;
		cout << "\t6 - Sort books by published date (in descending order)" << endl;
		cout << "\t7 - Move the oldest books in another room (specific Year and less)" << endl;
		cout << "\t8 - Show old books" << endl;
		cout << "\t9 - Write all books data to file" << endl;
		cout << "\t10 - Read books data from file" << endl;
		cout << "\t11 - Exit the application" << endl;
		cout << "\n\tOption : ";
		cin >> option;

		system("cls");
		switch (option)
		{
			case 1:
			{
				char view = NULL;
				cout << "\n-- -- -- Books in the library before -- -- --\n";
				cout << "\n\t\t" << myDC.size() << " books" << endl;

				myDC.add();

				cout << "\n-- -- -- Books in the library after -- -- --\n";
				cout << "\n\t\t" << myDC.size() << " books" << endl;

				cout << "\n\tView (y/n): ";
				cin >> view;

				if ((view == 'y') || (view == 'Y'))
				{
					system("cls");
					myDC.display(1);
				}

				checkStep = true;
				break;
			}
			case 2:
			{
				if (checkStep)
				{
					string isbn("");
					cout << "\n\n\tEnter ISBN: ";
					cin >> isbn;

					system("cls");
					cout << "\n-- -- -- Books found by ISBN in the library -- -- --\n";
					auto itrBookfind = myDC.search(isbn);
					if (itrBookfind != myDC.getEndofContainer())
						cout << *itrBookfind;
					else
						cout << "\n\t\tNo data found !" << endl;
				}
				break;
			}
			case 3:
			{
				if (checkStep)
				{
					Date bookDate =  Date();
					cout << "\n\n\tEnter published date:";
					cout << "\n\t\tyear: ";
					cin >> bookDate.year;
					cout << "\t\tmonth: ";
					cin >> bookDate.month;
					cout << "\t\tday: ";
					cin >> bookDate.day;
					system("cls");
					cout << "\n-- -- -- Books found by published date in library -- -- --\n";
					auto findByDate = myDC.search(bookDate);
					if (findByDate.size() == 0)
						cout << "\n\t\tNo data found !" << endl;
					else
					{
						for_each(findByDate.begin(), findByDate.end(), display);
						cout << "\n\t\t____________________";
						cout << "\n\t\tTOTAL : " << findByDate.size() << " books !" << endl;
					}
				}
				break;
			}
			case 4:
			{
				if (checkStep)
				{
					cout << "\n-- -- -- All books in the library -- -- --\n";
					myDC.display(1);
				}
				break;
			}
			case 5:
			{
				if (checkStep)
				{
					cout << "\n-- -- -- Books sort by ISBN (in ascending order) -- -- --\n";
					myDC.sortByIsbn();
					myDC.display(1);
				}
				break;
			}
			case 6:
			{
				if (checkStep)
				{
					cout << "\n-- -- -- Books sort by published date (in descending order) -- -- --\n";
					myDC.sortByPubDate();
					myDC.display(1);
				}
				break;
			}
			case 7:
			{
				if (checkStep)
				{
					int year(0);
					cout << "\n-- -- -- Move the old books in another room (by year) -- -- --\n";
					cout << "\n\n\t\tEnter year: ";
					cin >> year;
					if (myDC.moveOld(year) != 0)
						cout << "\n\tMoved successfully " << endl;
					else
						cout << "\n\tMoved unsuccessfully. Check the year !" << endl;
				}
				break;
			}
			case 8:
			{
				cout << "\n-- -- -- All old books in the new room -- -- --\n";
				myDC.display(0);
				break;
			}
			case 9:
			{
				cout << "\n-- -- -- Write all books data to file -- -- --\n";
				FileHandler<Book>::writeData(myDC.getObj());
				cout << "\n\t\tSuccessful writing !" << endl;
				break;
			}
			case 10:
			{
				cout << "\n-- -- -- Read books data from file -- -- --\n";
				myDC.erase();
				myDC.add(FileHandler<Book>::readData());
				myDC.display(1);
				break;
			}
		}

		if ((!checkStep) && (option != 11))
		{
			cout << "\n\n\tCheck logical step !" << endl;
		}

		cout << endl << endl;
		system("pause");

	} while (option != 11);
}

inline istream & operator >> (istream & in, Book & myObj) {
	bool control = false;
	in.ignore();
	cout << endl;

	do{
		cout << "\tEnter ISBN: ";
		getline(in, myObj.isbn);
		control = Validator::validIsbn(myObj.isbn);
		if (!control)
			cout << Validator::errMessage() << Validator::infoValidation::isbn() << "\n" << endl;
	} while (!control);

	do {
		cout << "\tEnter Bar-Code: ";
		getline(in, myObj.codeBar);
		control = Validator::validCodeBar(myObj.codeBar);
		if (!control)
			cout << Validator::errMessage() << Validator::infoValidation::codeBar() << "\n" << endl;
	} while (!control);
	
	do {
		cout << "\tEnter Title: ";
		getline(in, myObj.title);
		control = Validator::validName(myObj.title);
		if (!control)
			cout << Validator::errMessage() << Validator::infoValidation::name() << "\n" << endl;
	} while (!control);

	do {
		cout << "\tEnter Number of Edition: ";
		in >> myObj.edition;
		control = Validator::validInt(to_string(myObj.edition));
		if (!control)
			cout << Validator::errMessage() << Validator::infoValidation::inT() << "\n" << endl;
	} while (!control);

	do {
		cout << "\tEnter Number of Pages: ";
		in >> myObj.numbOfPages;
		control = Validator::validInt(to_string(myObj.numbOfPages));
		if (!control)
			cout << Validator::errMessage() << Validator::infoValidation::inT() << "\n" << endl;
	} while (!control);
	
	in.ignore();

	do {
		cout << "\tEnter Autor:\n\t\tFirst Name: ";
		getline(in, myObj.author.firstName);
		control = Validator::validName(myObj.author.firstName);
		if (!control)
			cout << Validator::errMessage() << Validator::infoValidation::name() << "\n" << endl;
	} while (!control);

	do {
		cout << "\t\tLast Name: ";
		getline(in, myObj.author.lastName);
		control = Validator::validName(myObj.author.lastName);
		if (!control)
			cout << Validator::errMessage() << Validator::infoValidation::name() << "\n" << endl;
	} while (!control);
	
	do {
		cout << "\tPublished by: ";
		getline(in, myObj.pubHouse);
		control = Validator::validName(myObj.pubHouse);
		if (!control)
			cout << Validator::errMessage() << Validator::infoValidation::name() << "\n" << endl;
	} while (!control);

	cout << "\tPublished date:";
	do {
		cout << "\n\t\tYear: ";
		in >> myObj.pubDate.year;
		control = Validator::validYear(to_string(myObj.pubDate.year));
		if (!control)
			cout << Validator::errMessage() << Validator::infoValidation::year() << "\n" << endl;
	} while (!control);

	do {
		cout << "\t\tMonth: ";
		in >> myObj.pubDate.month;
		control = Validator::validMonth(to_string(myObj.pubDate.month));
		if (!control)
			cout << Validator::errMessage() << Validator::infoValidation::month() << "\n" << endl;
	} while (!control);

	do {
		cout << "\t\tDay: ";
		in >> myObj.pubDate.day;
		control = Validator::validDay(to_string(myObj.pubDate.day));
		if (!control)
			cout << Validator::errMessage() << Validator::infoValidation::day() << "\n" << endl;
	} while (!control);

	return in;
}

inline bool operator == (const Date & a, const Date & b) {
	bool isequal = ((a.year == b.year) && (a.month == b.month) && (a.day == b.day));
	return isequal;
}

inline bool operator == (const Book & book, const string & isbn) {
	return book.isbn == isbn;
}

inline ostream & operator << (ostream & out, Book * b) {
	out << "\n\tISBN: " << b->isbn << endl;
	out << "\tBar-Code: " << b->codeBar << endl;
	out << "\tTitle: " << b->title << endl;
	out << "\tNumber of Edition: " << b->edition << endl;
	out << "\tNumber of Pages: " << b->numbOfPages << endl;
	out << "\tAutor:\n\t\tFirst Name: " << b->author.firstName << endl;
	out << "\t\tLast Name: " << b->author.lastName << endl;
	out << "\tPublished by: " << b->pubHouse << endl;
	out << "\tPublished date: " << b->pubDate.year;
	out << "/" << b->pubDate.month << "/" << b->pubDate.day;
	out << endl;
	return out;
}

inline ostream & operator << (ostream & out, Book & b) {
	out << "\n\tISBN: " << b.isbn << endl;
	out << "\tBar-Code: " << b.codeBar << endl;
	out << "\tTitle: " << b.title << endl;
	out << "\tNumber of Edition: " << b.edition << endl;
	out << "\tNumber of Pages: " << b.numbOfPages << endl;
	out << "\tAutor:\n\t\tFirst Name: " << b.author.firstName << endl;
	out << "\t\tLast Name: " << b.author.lastName << endl;
	out << "\tPublished by: " << b.pubHouse << endl;
	out << "\tPublished date: " << b.pubDate.year;
	out << "/" << b.pubDate.month << "/" << b.pubDate.day;
	out << endl;
	return out;
}

void display(Book & book) {
	cout << book;
}

inline bool operator > (Date & a, Date & b) {
	return ((a.day > b.day) && (a.month >= b.month) && (a.year >= b.year));
}

inline bool byIsbn(Book & b1, Book & b2) {
	return b1.isbn < b2.isbn;
}

inline bool byPubDate(Book & b1, Book & b2) {
	return b1.pubDate > b2.pubDate;
}
