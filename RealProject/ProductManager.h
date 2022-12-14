#ifndef __PRODUCTMANAGER_H__
#define __PRODUCTMANAGER_H__
#include "Product.h"
#include <vector>
/**
* 제품정보를 관리하는 클래스
* @author 홍성주
*/
class ProductManager {
public:
	ProductManager();

	void ProductMainMenu();	// 제품 메인 메뉴
	void add_Product();		// 제품 추가
	void Product_print();	// 조회
	void search_Product();	// 검색
	void delete_Product();	// 삭제
	void update_product();	// 정보 변경

	int cpmenu();			// 제품 관리 메뉴에서 정해진 범위만 받도록
	int pupdate_menu();	// 검색,변경 메뉴 입력 예외 처리
	int update_data(); // 예외 처리 함수
	void showProductlist(Product* productinfo);

	~ProductManager();

	vector<string> parseCSV(istream&, char);
	vector<Product*>& getproductList();
private:
	vector<Product*>productList;	// 제품정보가 저장되는 벡터
};
#endif