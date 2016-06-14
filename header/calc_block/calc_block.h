#ifndef CALC_BLOCK_H
#define CALC_BLOCK_H
	#include <math.h>
	#include <string.h>
	#include <sstream>
	#include <cstdlib>

	#define _USE_MATH_DEFINES
	
	/*Calc block*/
	void rounding(std::string* /*Массив значений*/, unsigned int /*Кол-во значений*/, unsigned int /*До какого числа округлять*/); // Округление
	std::string* calculation_formula(std::string /*сама формула*/, std::string* /*Массив значений*/, unsigned int /*кол-во значений*/, std::string /*Значение числа pi*/); // Расчёт по формуле
	std::string calculate_ex(std::string); // Расчёт
	std::string* get_fast_ops(); // Получение указателя на массив с возможными быстрыми операциями

	/*Long calc*/
	std::string un_e(std::string, int, int*, int*);
	bool find_comma(std::string); // Поиск запятой в строке
	std::string long_calc(std::string /*Первое число*/, std::string /*Второе число*/, int /*Код операция*/, bool /*Если необходимо получить версию подпрограммы*/); // Операция с высокой точностью
	std::string comma_converter(const char, std::string); // Замена запятой на точку и обратно
	std::string get_multiplication(std::string); // Подстановка необходимых умножений
#endif