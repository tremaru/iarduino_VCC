#include "iarduino_VCC.h"																					//	
																											//
#ifndef MUX4																								//	Если IDE не знает что у микроконтроллера есть бит «MUX4», то ..
#define MUX4 4																								//	Указываем что бит «MUX4» является 4 битом регистра «ADMUX».
#endif																										//	У микроконтроллера «ATmega328» действительно нет такого бита, но он зарерервирован для совместимости, вот и совмещаем ))).
																											//	
//		Чтение напряжения питания в Вольтах:																//	Возвращаемое значение:	напряжение питания в Вольтах.
float	analogRead_VCC	(void){																				//	Параметр:				отсутствует.
			float       i = 0.0f;																			//	Определяем переменную для получения результата.
			uint8_t     j = 100;																			//	Определяем сколько значений АЦП требуется получить для усреднения результата.
			static bool f = true;																			//	Определяем флаг первого запуска функции.
		//	Устанавливаем Uвх АЦП с источника ИОН на 1,1В, а в качестве ИОН АЦП используем Vcc:				//	ИОН - источник опорного напряжения.
			#if (defined( __AVR_ATmega640__ ) || defined( __AVR_ATmega1280__ ) || defined( __AVR_ATmega1281__ ) || defined( __AVR_ATmega2560__ ) || defined( __AVR_ATmega2561__ ) || defined( __AVR_ATmega32U4__ ) || defined( __AVR_ATmega32U4__ ))
			#ifndef MUX5																					//	Если IDE не знает что у микроконтроллера есть бит «MUX5», то ..
			#error Просьба сообщить нам название вашей платы Arduino на электронный адрес: shop@iarduino.ru
			#endif																							//	У микроконтроллера «ATmega328» действительно нет такого бита, но он зарерервирован для совместимости, вот и совмещаем ))).
			ADCSRB &= ~(1<<MUX5);																			//	Для Arduino Mega, Leonardo и Micro, сбрасываем бит «MUX5» регистра «ADCSRB», так как «MUX[5-0]» должно быть равно 011110 (см. регистр «ADMUX»).
			#elif (!defined( __AVR_ATmega328__ ) && !defined( __AVR_ATmega328P__ ) && !defined( __AVR_ATmega1284__) && !defined( __AVR_ATmega1284P__) )	//	Если Плата Arduino не принадлежит к Mega, Leonardo, Micro и не принадлежит к Arduino MINI, NANO, UNO, ULTRA, то выводим сообщение об ошибке.
			#error Извините, но библиотека "iarduino_VCC" не поддерживает вашу плату Arduino!
			#endif																							//
			ADMUX   = (0<<REFS1)|(1<<REFS0)|(0<<ADLAR)|(1<<MUX4)|(1<<MUX3)|(1<<MUX2)|(1<<MUX1)|(0<<MUX0);	//	Устанавливаем биты регистра «ADMUX»: «REFS»=01 (ИОН=VCC), «ADLAR»=0 (выравнивание результата по правому краю), «MUX[4-0]»=11110 или «MUX[3-0]»=1110 (источником сигнала для АЦП является напряжение ИОН на 1,1 В).
		//	Устанавливаем задержку при первом запуске функции:												//	
			if(f){f=false; delay(50);}																		//	О первом запуске свидетельствует установленный флаг «f».
		//	Получаем несколько значений АЦП:																//	
			for(uint8_t k=0; k<j; k++){																		//	Выполняем цикл «j» раз.
			//	Запускаем преобразования АЦП:																//	
				ADCSRA |= (1<<ADEN )|(1<<ADSC );															//	Устанавливаем биты регистра «ADCSRA»: «ADEN»=1  (вкл АЦП), «ADSC» =1 (запускаем новое преобразование).
			//	Получаем данные АЦП:																		//	
				while(bit_is_set(ADCSRA,ADSC)){}; i += ADC;													//	Ждём завершения преобразования: о готовности результата свидетельствует сброс бита «ADSC» регистра «ADCSRA» и добавляем результат «ADC» к переменной «i».
			}	i /= j;																						//	Делим результат «i» на «j», так как мы получили его «j» раз.
		//	Рассчитываем напряжение питания:																//	АЦП = (Uвх/Vcc)*1023.	Напряжение Uвх мы брали с внутреннего ИОН на 1.1 В, значение которого возвращает функция analogSave_1V1(0).
			return (analogSave_1V1(0)/i) * 1023.0f;															//	Vcc = (Uвх/АЦП)*1023.	Значение АЦП мы сохранили в переменную «i».
}																											//	
																											//	
//		Рассчет напряжения ИОН вместо 1,1 В:																//	Возвращаемое значение:	рассчитанное напряжение ИОН вместо 1,1 В.
float	analogCalc_1V1	(float Vcc){																		//	Параметр:				измеренное напряжение питания в Вольтах.
			float       i = 0.0f;																			//	Определяем переменную для получения результата.
			uint8_t     j = 100;																			//	Определяем сколько значений АЦП требуется получить для усреднения результата.
			static bool f = true;																			//	Определяем флаг первого запуска функции.
		//	Устанавливаем Uвх АЦП с источника ИОН на 1,1В, а в качестве ИОН АЦП используем Vcc:				//	ИОН - источник опорного напряжения.
			#if (defined( __AVR_ATmega640__ ) || defined( __AVR_ATmega1280__ ) || defined( __AVR_ATmega1281__ ) || defined( __AVR_ATmega2560__ ) || defined( __AVR_ATmega2561__ ) || defined( __AVR_ATmega32U4__ ) || defined( __AVR_ATmega32U4__ ))
			#ifndef MUX5																					//	Если IDE не знает что у микроконтроллера есть бит «MUX5», то ..
			#error Просьба сообщить нам название вашей платы Arduino на электронный адрес: shop@iarduino.ru
			#endif																							//	У микроконтроллера «ATmega328» действительно нет такого бита, но он зарерервирован для совместимости, вот и совмещаем ))).
			ADCSRB &= ~(1<<MUX5);																			//	Для Arduino Mega, Leonardo и Micro, сбрасываем бит «MUX5» регистра «ADCSRB», так как «MUX[5-0]» должно быть равно 011110 (см. регистр «ADMUX»).
			#elif (!defined( __AVR_ATmega328__ ) && !defined( __AVR_ATmega328P__ ) && !defined( __AVR_ATmega1284__) && !defined( __AVR_ATmega1284P__) )	//	Если Плата Arduino не принадлежит к Mega, Leonardo, Micro и не принадлежит к Arduino MINI, NANO, UNO, ULTRA, то выводим сообщение об ошибке.
			#error Извините, но библиотека "iarduino_VCC" не поддерживает вашу плату Arduino!
			#endif																							//
			ADMUX   = (0<<REFS1)|(1<<REFS0)|(0<<ADLAR)|(1<<MUX4)|(1<<MUX3)|(1<<MUX2)|(1<<MUX1)|(0<<MUX0);	//	Устанавливаем биты регистра «ADMUX»: «REFS»=01 (ИОН=VCC), «ADLAR»=0 (выравнивание результата по правому краю), «MUX[4-0]»=11110 или «MUX[3-0]»=1110 (источником сигнала для АЦП является напряжение ИОН на 1,1 В).
		//	Устанавливаем задержку при первом запуске функции:												//	
			if(f){f=false; delay(50);}																		//	О первом запуске свидетельствует установленный флаг «f».
		//	Получаем несколько значений АЦП:																//	
			for(uint8_t k=0; k<j; k++){																		//	Выполняем цикл «j» раз.
			//	Запускаем преобразования АЦП:																//	
				ADCSRA |= (1<<ADEN )|(1<<ADSC );															//	Устанавливаем биты регистра «ADCSRA»: «ADEN»=1  (вкл АЦП), «ADSC» =1 (запускаем новое преобразование).
			//	Получаем данные АЦП:																		//	
				while(bit_is_set(ADCSRA,ADSC)){}; i += ADC;													//	Ждём завершения преобразования: о готовности результата свидетельствует сброс бита «ADSC» регистра «ADCSRA» и добавляем результат «ADC» к переменной «i».
			}	i /= j;																						//	Делим результат «i» на «j», так как мы получили его «j» раз.
		//	Рассчитываем реальное напряжение ИОН на 1,1В:													//	Uвх = (АЦП/1023)*Vcc.	Напряжение Uвх мы брали с внутреннего ИОН на 1.1 В, значение АЦП мы сохранили в переменную «i».
			return (i/1023.0f) * Vcc;																		//							Напряжение Vcc указано в качестве аргумента функции.
}																											//	
																											//	
//		Указание напряжения ИОН вместо 1,1 В (для дальнейших измерений Vcc):								//	Возвращаемое значение:	отсутствует.
float	analogSave_1V1	(float v){						 													//	Параметр:				напряжение ИОН вместо 1,1 В.
			static float ion_1V1 = 1.1f;																	//	Определяем напряжение ИОН по умолчанию 1,1 В.
			if(v==0){return ion_1V1;}else{ion_1V1 = v;}														//	Сохраняем или возвращаем значение ion_1V1.
}																											//	
