#include "ProductManager.h"

#include<vector>
#include <algorithm>
#include<fstream>
#include<string>
#include<sstream>
#define C_YLLW "\033[33m"
#define C_NRML "\033[0m"

/**
* 생성자에서 Product.txt 파일이 존재하면, ','로 구분한 텍스트를 한줄씩 불러옴.
* @exception 파일이 없으면 로드되지 않음.
*/
ProductManager::ProductManager()
{
	ifstream file;
	file.open("Product.txt");
	if (!file.fail()) {
		while (!file.eof()) {
			vector<string> row = parseCSV(file, ',');
			if (row.size()) {
				int price = atoi(row[2].c_str());
				int stock = atoi(row[4].c_str());

				Product* p = new Product(row[0], row[1], price, row[3],stock);
				productList.push_back(p);
			}
		}
	}
	cout << "Product.txt을 로드하였습니다." << endl;
	Sleep(1000);
	file.close();
}
/**
* 소멸자에서 Product.txt 파일이 존재하면 productList 벡터의 ','로 구분한 텍스트 한줄 씩 저장
* @exception 파일이 없으면 파일 생성.
*/
ProductManager::~ProductManager()		//ProductManager 소멸자에서 파일 입력
{
	ofstream fs("Product.txt");
	if (!fs) {
		cout << "출력할 파일을 열 수 없습니다.";
		Sleep(1000);
	}

	for (auto it = productList.begin(); it != productList.end(); ++it)
	{
		fs << (*it)->getProductID() << ", "
			<< (*it)->getProductName() << ", "
			<< (*it)->getPrice() << ", "
			<< (*it)->getProductType() << ", "
			<< (*it)->getStock()<<endl;
	}
	cout << "Product.txt을 저장했습니다." << endl;
	Sleep(1000);
	fs.close();
}
/**
* @return 제품정보가 저장된 clientList벡터 반환
*/
vector<Product*>& ProductManager::getproductList()
{
	return productList;
}
/**
* 제품관리메인
* 화면에서 입력값을 받아 입력, 조회, 검색, 삭제, 변경 등 함수를 호출
* @exception 정해진 입력값이 아니면 함수 종료
*/
void ProductManager::ProductMainMenu()
{
	int back = 0;
	int product_menu;
	product_menu = cpmenu();	//제품관리메뉴에서 정해진 범위만 받는 함수

	switch (product_menu)
	{
	case 0:	//메인화면으로
		break;
	case 1:		//입력
		add_Product();
		break;
	case 2:		//조회
		Product_print();
		cout << "종료 (0) "; cin >> back;
		if (back == 0)break;
		break;
	case 3:		//검색
		search_Product();
		cout << "종료 (0) "; cin >> back;
		if (back == 0)break;
		break;
	case 4:		//삭제
		delete_Product();
		cout << "종료 (0) "; cin >> back;
		if (back == 0)break;
		break;
	case 5:		//변경
		update_product();
		cout << "종료 (0) "; cin >> back;
		if (back == 0)break;
		break;
	}
}
/**
* 제품정보입력
* 제품정보를 관리하는 productList벡터에 입력한 데이터를 추가
*
* @exception 제품코드(PK)를 중복검사 해 중복이면 추가되지 않도록 예외처리.
*/
void ProductManager::add_Product()
{
	string productID, productName, productType;
	int price = 0, stock=0;	

	system("cls");
	cout << LINE << endl;
	cout << "                                            신규 제품 등록" << endl;
	cout << LINE << endl;

	cout << "제품코드 (PK) : "; cin >> productID;
	cout << "제품명 : "; cin >> productName;
	while (1) {		// 가격 입력시 경계검사
		bool flag = false;
		cout << "가격 (0이면 종료됩니다.) :"; cin >> price;
		if (!cin) {
			cout << "[숫자만 입력해주세요 (종료 0)]" << endl;
			cin.clear();
			cin.ignore(INT_MAX, '\n');
			Sleep(1000);
		}
		else if (price < 0) {
			cout << "[0보다 같거나 같습니다.]" << endl;
		}
		else if (price == 0)	return;
		else flag = true;
		if(flag==true)break;
	}
	cout << "종류 : "; cin >> productType;

	while (1) {		// 재고 입력 시 경계검사
		bool flag = false;
		cout << "재고 : "; cin >> stock;
		if (!cin) {
			cout << "[숫자만 입력해주세요. ]" << endl;
			cin.clear();
			cin.ignore(INT_MAX, '\n');
			Sleep(1000);
		}
		else if (stock < 0) 
			cout << "[입력한 수량이 0보다 작습니다. 0이나 0보다 큰 수 입력.]"<<endl;
		else flag = true;
		if (flag == true)break;
	}

	while (1) {	// PK(제품ID) 중복검사
		bool flag = false;

		for (auto it = productList.begin(); it != productList.end(); ++it)
		{
			auto sch_PK = (*it)->getProductID();
			if (sch_PK == productID) {
				flag = true;
				break;
			}
		}
		if (flag == false)	break;
		else
		{
			cout << " 중복되는 제품 ID 입니다. 다시 입력해 주세요 (종료 0): ";
			cin >> productID;
			if (productID == "0")return;	//종료조건
		}
	}

	Product* newProduct = new Product(productID, productName, price, productType,stock);
	productList.push_back(newProduct);

	cout << "[제품 등록 완료]" << endl;

	Sleep(1000);	//Delay 1초
}

/**
* 제품정보조회
* 제품정보를 관리하는 productList벡터에 입력된 데이터를 조회.
*/
void ProductManager::Product_print()
{
	system("cls");

	cout << LINE << endl;
	cout << "                                            제품정보조회" << endl;
	cout << LINE << endl;
	cout << "    제품코드 (PK)     /   제품명   /   가격     /    종류   /   재고" << endl;
	cout << LINE << endl;

	sort(productList.begin(), productList.end(), [](Product* a, Product* b) {		//제품ID 기준으로 정렬
		return a->getProductID() < b->getProductID();
		});

	for (auto it = productList.begin(); it != productList.end(); ++it)
	{
		showProductlist(*it);	// productList벡터 출력
	}

	cout << "[제품정보 조회 완료]" << endl;
	cout << LINE << endl;
	cout << "[총 " << productList.size() << "건의 제품정보가 있습니다]" << endl;
	cout << LINE << endl;
}

/**
* 제품정보검색
* 문자열을 입력받아 문자열이 고객정보의 입력된 데이터에 포함되는 데이터가 있으면 productList의 데이터 검색.
*
* @exception 정해진 입력값이 아닐 경우 예외처리
*/
void ProductManager::search_Product()		// 제품정보검색
{
	string input;
	int num = 0;
	bool flag = 0;

	system("cls");
	cout << LINE << endl;
	cout << "                                            제품 검색" << endl;
	cout << LINE << endl;
	for (auto it = productList.begin(); it != productList.end(); ++it)
	{
		showProductlist(*it);	//productList 벡터출력
	}
	cout << LINE << endl;
	cout << "1. 제품명 검색 2. 종류 검색 "; //cin >> num;

	num = pupdate_menu();	// 1. 제품명 검색 2. 종류 검색
	switch (num)
	{
	case 1:
		cout << "제품명 검색 : "; cin >> input;

		cout << LINE << endl;
		cout << "                                       [입력한 제품명 : " << input << " 검색결과]" << endl;
		cout << LINE << endl;
		cout << "      제품코드(PK)     /   제품명    /    가격    /    종류" << endl;
		cout << LINE << endl;

		for (auto it = productList.begin(); it != productList.end(); ++it)
		{
			auto sch_name = (*it)->getProductName();
			if (sch_name.find(input) != -1) {	//productList의 name에 입력한 문자열이 찾아지면 관련정보 출력
				flag = true;
				printf("%s", C_YLLW);		//글자색 찾은 데이터 노란색
				showProductlist(*it);		//productList 벡터출력
				printf("%s", C_NRML);		//글자 색 복귀
			}
		}
		if (flag == false)
			cout << "[입력하신 제품명이 없습니다]" << endl;
		cout << LINE << endl;
		break;

	case 2:
		cout << "종류 검색 : "; cin >> input;

		cout << LINE << endl;
		cout << "                                       [입력한 제품종류 : " << input << " 검색결과]" << endl;
		cout << LINE << endl;
		cout << "      제품코드(PK)     /   제품명    /    가격    /    종류   /   재고" << endl;
		cout << LINE << endl;

		for (auto it = productList.begin(); it != productList.end(); ++it)
		{
			auto sch_type = (*it)->getProductType();
			if (sch_type.find(input) != -1) {	//productList의 producttype에 입력한 문자열이 찾아지면 관련정보 출력
				flag = true;
				printf("%s", C_YLLW);		//글자색 찾은 데이터 노란색
				showProductlist(*it);		//productList 벡터출력
				printf("%s", C_NRML);		//글자 색 복귀
			}
		}
		if (flag == false)
			cout << "[입력하신 제품의 종류가 없습니다]" << endl;
		cout << LINE << endl;
		break;
	case 3:
		cout << "[메뉴 번호만 입력해주세요]" << endl;
		Sleep(1000);
		break;
	}
}

/**
* 제품정보삭제
* 제품PK 문자열을 입력받아 문자열이 제품정보의 PK와 일치하면 해당 productList의 데이터 삭제.
*/
void ProductManager::delete_Product()
{
	string input;
	bool flag = false;
	system("cls");
	cout << LINE << endl;
	cout << "                                            제품 정보 삭제                              " << endl;
	cout << LINE << endl;
	for (auto it = productList.begin(); it != productList.end(); ++it)
	{
		showProductlist(*it);	//productList 벡터 출력
	}
	cout << LINE << endl;
	cout << "삭제할 제품 ID 입력하세요 : "; cin >> input;

	for (auto it = productList.begin(); it != productList.end(); ++it)
	{
		auto sch_ID = (*it)->getProductID();
		if (sch_ID == input) {	//productList의 id와 입력한 문자열이 같으면 삭제
			flag = true;
			productList.erase(it);
			cout << input << " 에 대한 정보가 삭제되었습니다." << endl;
			cout << "[제품정보 삭제 완료]" << endl;
			break;
		}
	}
	if (flag == false)
		cout << "[입력하신 제품 ID가 없습니다]" << endl;
	cout << LINE << endl;
}

/**
* 제품정보변경
* 변경할 정보를 선택 후 입력받아 해당 productList의 데이터 변경.
*
* @exception 정해진 입력값이 아닐 경우 예외처리
*/
void ProductManager::update_product()
{
	string input;
	bool flag = false;
	int data;		//업데이트 시 변경할 데이터

	system("cls");
	cout << LINE << endl;
	cout << "                                            제품 정보 변경" << endl;
	cout << LINE << endl;
	for (auto it = productList.begin(); it != productList.end(); ++it)
	{
		showProductlist(*it);	//productList 출력
	}
	cout << LINE << endl;
	cout << "변경할 제품 코드 입력 : "; cin >> input;

	for (auto it = productList.begin(); it != productList.end(); ++it)
	{
		auto sch_name = (*it)->getProductID();		
		if (sch_name == input) {//productList의 id와 입력한 문자열이 같으면 정보변경
			flag = true;
			cout << "1. 가격 변경  2. 재고 변경 3. 제품명 변경" << endl;	
			int num = pupdate_menu();	// // 1 ~ 2 이외 예외값 입력 처리함수
			switch (num)
			{
			case 1:		//가격 변경
				cout << "변경할 가격 입력" << endl;
				data=update_data();
				if (data > 0) {
					(*it)->setPrice(data);
					cout << "[가격 변경 완료]" << endl;
				}
				else if (data == 0) {
					cout << "[범위가 벗어났습니다. 999999999 이하의 양수만 입력해주세요]" << endl;
					Sleep(1000);
				}
				
				break;

			case 2:		//재고 변경
				cout << "변경할 재고 수량 입력" << endl;
				data = update_data();
				if (data > 0) {
					(*it)->setStock(data);
					cout << "[재고 변경 완료]" << endl;
				}
				else if (data == 0) {
					(*it)->setStock(data);
					cout << "[재고 변경 완료]" << endl;
				}
				break;
			case 3:		//제품명 변경
				string up_name;
				cout << "변경할 제품명 입력 (종료 0)" << endl;	cin >> up_name;
				if (up_name == "0")	return;
				else {
					(*it)->setProductName(up_name);
					cout << "[제품명 변경 완료]" << endl;
				}
				break;
			}
		}
	}
		if (flag == false)
			cout << "[입력하신 제품 코드가 없습니다]" << endl;

		cout << LINE << endl;
	
}

/**
* productList에 입력된 데이터 출력함수
*/
void ProductManager::showProductlist(Product* productinfo)
{
	cout << "(PK)" << productinfo->getProductID() << "-> "
		<< productinfo->getProductName() << " / "
		<< productinfo->getPrice() << " / "
		<< productinfo->getProductType() << " / "
		<< productinfo->getStock() << "개" << endl;
}

/**
* 입력값 예외처리 함수
* @exception 정해진 입력값이 아닐 경우 예외처리
* @return 입력값 반환
*/
int ProductManager::cpmenu()
{
	int menu;
	cin >> menu;
	if (!cin) {		// cin 에 숫자만 입력 받도록
		cout << "[메뉴 번호만 입력해주세요]";
		cin.clear();
		cin.ignore(INT_MAX, '\n');
		Sleep(1000);
	}
	else if (menu >= 0 && menu < 6)
		return menu;
	else {
		cout << "[메뉴 번호만 입력해주세요]" << endl;
		Sleep(1000);
	}
	return 0;
}

/**
* 입력값 예외처리 함수
* @exception 정해진 입력값이 아닐 경우 예외처리
* @return 입력값 반환
*/
int ProductManager::pupdate_menu()
{
	int menu;
	cin >> menu;
	if (!cin) {		// cin 에 숫자만 입력 받도록
		cout << "[메뉴 번호만 입력해주세요]" << endl;
		cin.clear();
		cin.ignore(INT_MAX, '\n');
		Sleep(1000);
	}
	else if (menu > 0 && menu < 4)
		return menu;
	else {
		cout << "[메뉴 번호만 입력해주세요]" << endl;
		Sleep(1000);
	}
	return 0;
}

/**
* 입력값 예외처리 함수
* @exception 정해진 입력값이 아닐 경우 예외처리
* @return 입력값 반환
*/
int ProductManager::update_data()
{
	int data;
	cin >> data;
	if (!cin) {		// cin 에 숫자만 입력 받도록
		cout << "[범위가 벗어났습니다. 999999999 이하의 양수만 입력해주세요]";
		cin.clear();
		cin.ignore(INT_MAX, '\n');
		Sleep(1000);
	}
	else if (data >= 0 && data < 999999999)
		return data;
	else {
		cout << "[범위가 벗어났습니다. 999999999 이하의 양수만 입력해주세요]" << endl;
		Sleep(1000);
	}
	return 0;
}

/**
* CSV 파일의 형식을 한 행씩 가져옴
* @param istream& file 가져올 내용이 들어있는 파일
* @param char delimiter 구분 문자
* @return 한 행씩 반환
*/
vector<string> ProductManager::parseCSV(istream& file, char delimiter)
{
	stringstream ss;
	vector<string> row;
	string t = " \n\r\t";

	while (!file.eof()) {
		char c = file.get();
		if (c == delimiter || c == '\r' || c == '\n') {
			if (file.peek() == '\n') file.get();
			string s = ss.str();
			s.erase(0, s.find_first_not_of(t));
			s.erase(s.find_last_not_of(t) + 1);
			row.push_back(s);
			ss.str("");
			if (c != delimiter) break;
		}
		else {
			ss << c;
		}
	}
	return row;
}