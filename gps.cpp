#include "usart.h" // USART 헤더 파일 포함
#include <stdio.h> // 표준 입출력 라이브러리 포함
#include <string.h> // 문자열 처리 라이브러리 포함

#define GPSDATA 100 // GPS 데이터 수신 버퍼 크기 정의

float Time = 0; // 시간 변수
double Latitude = 0, Longitude = 0; // 위도와 경도 변수
uint8_t GPS_rx[GPSDATA] = {0}; // GPS 데이터 수신 배열 초기화
uint8_t GPSdebug = 0; // GPS 디버그 카운터

uint8_t checksum = 0; // 체크섬 변수
char NS = 0, EW = 0, Quality = 0, check = 0; // 방향과 품질 관련 변수

// DMM 형식의 위도를 도(degree)로 변환하는 함수
double convertToDegrees(double dmm) {
    int degrees = (int)(dmm / 100); // 정수 부분(도) 계산
    double minutes = dmm - (degrees * 100); // 분 계산
    return degrees + (minutes / 60); // 도로 변환하여 반환
}

// GPS 초기화 함수
void GPS_Init(void){
	  HAL_UART_Receive_DMA(&huart2, GPS_rx, GPSDATA); // DMA를 사용하여 GPS 데이터 수신 시작
}

// GPS 콜백 함수
void GPS_CallBack(void){
	GPSdebug++; // 디버그 카운터 증가
	static double LatitudeDMM = 0, LongitudeDMM = 0; // DMM 형식의 위도와 경도 변수
	// double LatitudeDMM = 3737.91482, LongitudeDMM = 12703.28406; // 테스트용 초기값 주석처리

	static double old_Latitude = 0, old_Longitude = 0; // 이전 위도와 경도 변수
	// uint8_t checksum = 0; // 체크섬 변수 주석처리

	// GPS 데이터 수신 배열에서 정보를 파싱하여 변수에 저장
	sscanf((char*)GPS_rx, "$GNRMC,%f,%c,%lf,%c,%lf,%c%*[^*]%*[*]%c%*s", 
		&Time, &Quality, &LatitudeDMM, &NS, &LongitudeDMM, &EW, &check);
	// 예시: $GNRMC,113817.20,A,3737.91482,N,12703.28403,E,0.029,,090424,,,A*6D

	// DMM 형식의 위도를 도 형식으로 변환
	Latitude = convertToDegrees(LatitudeDMM);
	Longitude = convertToDegrees(LongitudeDMM); // DMM 형식의 경도도 변환

	// 체크섬 계산
	/*for (int i = 1; i < GPSDATA; i++){
		if (GPS_rx[i] == '*') // '*' 문자까지 반복
			break;
		checksum ^= GPS_rx[i]; // 체크섬 계산
	}

	// 체크섬 비교 및 위도, 경도 갱신
	if (check != checksum){ // 체크섬이 일치하지 않으면
		GPSdebug++; // 디버그 카운터 증가
		Latitude = old_Latitude; // 이전 위도로 되돌림
		Longitude = old_Longitude; // 이전 경도로 되돌림
	}
	else // 체크섬이 일치하면
	{
		old_Latitude = Latitude; // 현재 위도를 이전 위도로 저장
		old_Longitude = Longitude; // 현재 경도를 이전 경도로 저장
	}*/

	// 다음 GPS 데이터를 수신하기 위한 준비
	HAL_UART_Receive_DMA(&huart2, GPS_rx, GPSDATA); // DMA를 사용하여 GPS 데이터 수신
}
