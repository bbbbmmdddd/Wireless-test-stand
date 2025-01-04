#include <Arduino.h>
#include <string>
#include <iostream>
#include <sstream>
#include <SPI.h>
#include <SD.h>
#include <FS.h>
#include <HX711.h>
#include <BluetoothSerial.h>

//music是移植过来的,直接删掉也没问题的哟~
//~~~~~~~~~~music~~~~~~~~~~~~~~~music~~~~~~~~~~~~~~music~~~~~~~~~~~~~~~~~music~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
int yesterday=0;
const int buzzer=15;
typedef struct data{
    note_t yinfu;
    uint16_t yingao;
    uint16_t shichang;
}note;
note melody[]={
    {NOTE_A,3,250},{NOTE_C,4,250},{NOTE_D,4,250},
    {NOTE_E,4,250},{NOTE_D,4,150},{NOTE_D,4,150},{NOTE_C,4,250},{NOTE_G,3,250},{NOTE_A,3,250},{NOTE_G,3,250},{NOTE_E,3,250},
    {NOTE_A,3,250},{NOTE_E,4,150},{NOTE_E,4,250},{NOTE_E,4,250},{NOTE_A,3,250},{NOTE_C,4,250},{NOTE_D,4,250},
    {NOTE_E,4,250},{NOTE_D,4,150},{NOTE_D,4,150},{NOTE_D,4,150},{NOTE_C,4,250},{NOTE_C,4,150},{NOTE_A,3,250},{NOTE_A,3,150},{NOTE_G,3,250},{NOTE_E,3,250},
    {NOTE_A,3,250},{NOTE_G,4,150},{NOTE_G,4,250},{NOTE_E,4,150},{NOTE_A,3,250},{NOTE_C,4,250},{NOTE_D,4,250},
    {NOTE_E,4,250},{NOTE_E,4,150},{NOTE_E,4,150},{NOTE_D,4,150},{NOTE_E,4,250},{NOTE_D,4,150},{NOTE_E,4,250},{NOTE_D,4,150},{NOTE_E,4,500},
    {NOTE_E,4,250},{NOTE_D,4,125},{NOTE_E,4,125},{NOTE_E,4,125},{NOTE_D,4,125},{NOTE_E,4,250},{NOTE_E,4,250},{NOTE_D,4,250},{NOTE_C,4,250},
    {NOTE_G,4,250},{NOTE_D,4,500},{NOTE_D,4,250},{NOTE_E,4,250},{NOTE_D,4,125},{NOTE_E,4,250},{NOTE_A,3,250},{NOTE_G,3,125},{NOTE_A,3,125},
    {NOTE_A,3,125},{NOTE_G,3,125},{NOTE_A,3,250},{NOTE_A,3,125},{NOTE_G,3,125},{NOTE_A,3,125},{NOTE_G,3,125},{NOTE_A,3,250},{NOTE_E,4,250},
    {NOTE_E,4,250},{NOTE_D,4,250},{NOTE_D,4,250},{NOTE_C,4,250},{NOTE_E,4,250},{NOTE_A,3,500},{NOTE_C,5,125},{NOTE_B,4,125},{NOTE_G,4,125},
    {NOTE_E,4,125},{NOTE_E,4,125},{NOTE_G,4,125},{NOTE_D,4,125},{NOTE_E,4,125},{NOTE_C,3,125},{NOTE_B,4,125},{NOTE_G,4,125},{NOTE_E,4,125},
    {NOTE_E,4,125},{NOTE_G,4,125},{NOTE_D,4,125},{NOTE_E,4,125},{NOTE_A,4,125},{NOTE_G,4,125},{NOTE_A,4,125},{NOTE_B,4,125},{NOTE_C,5,125},
    {NOTE_D,5,125},{NOTE_B,4,125},{NOTE_C,5,125},{NOTE_D,5,125},{NOTE_B,4,125},{NOTE_C,5,125},{NOTE_G,5,125},{NOTE_F,5,125},{NOTE_E,5,125},
    {NOTE_D,5,125},{NOTE_C,5,125},{NOTE_A,5,125},{NOTE_G,5,125},{NOTE_A,5,125},{NOTE_B,5,125},{NOTE_C,6,125},{NOTE_B,5,125},{NOTE_C,6,125},
    {NOTE_D,6,125},{NOTE_E,6,125},{NOTE_D,6,125},{NOTE_E,6,125},{NOTE_G,6,125},{NOTE_G,6,125},{NOTE_E,6,125},{NOTE_D,6,125},{NOTE_C,6,125},
    {NOTE_A,5,125},{NOTE_D,6,125},{NOTE_E,6,125},{NOTE_A,5,125},{NOTE_D,6,125},{NOTE_E,6,125},{NOTE_A,5,125},{NOTE_G,6,125},{NOTE_A,6,125},
    {NOTE_E,6,125},{NOTE_G,6,125},{NOTE_A,6,125},{NOTE_A,5,125},{NOTE_D,6,125},{NOTE_E,6,250},
    {NOTE_E,3,250},{NOTE_E,3,125},{NOTE_E,3,250},{NOTE_D,3,250},{NOTE_E,3,250},{NOTE_G,3,250},{NOTE_G,3,500},
    {NOTE_E,3,250},{NOTE_F,3,250},{NOTE_G,3,250},{NOTE_G,3,125},{NOTE_G,3,250},{NOTE_F,3,250},{NOTE_E,3,250},
    {NOTE_D,3,250},{NOTE_D,3,250},{NOTE_E,3,125},{NOTE_E,3,250},{NOTE_F,3,250},{NOTE_E,3,500},{NOTE_E,3,250},
    {NOTE_E,3,125},{NOTE_E,3,125},{NOTE_D,3,250},{NOTE_E,3,250},{NOTE_F,3,250},{NOTE_G,3,500},{NOTE_E,3,250},
    {NOTE_F,3,250},{NOTE_G,3,250},{NOTE_G,3,125},{NOTE_G,3,250},{NOTE_C,4,250},{NOTE_B,3,250},{NOTE_G,3,250},
    {NOTE_D,4,250},{NOTE_D,4,125},{NOTE_D,4,250},{NOTE_E,4,250},{NOTE_E,4,500},{NOTE_E,4,250},{NOTE_E,4,125},
    {NOTE_E,4,250},{NOTE_D,4,250},{NOTE_C,4,250},{NOTE_D,4,250},{NOTE_A,3,250},{NOTE_B,3,250},{NOTE_G,3,250},
    {NOTE_G,3,250},{NOTE_A,3,500},{NOTE_C,4,250},{NOTE_C,4,250},{NOTE_C,4,250},{NOTE_D,4,250},{NOTE_D,4,250},
    {NOTE_E,4,250},{NOTE_G,3,250},{NOTE_E,4,500},{NOTE_E,4,250},{NOTE_D,4,250},{NOTE_C,4,250},{NOTE_D,4,250},
    {NOTE_A,3,250},{NOTE_B,3,250},{NOTE_G,3,250},{NOTE_G,3,250},{NOTE_A,3,500},{NOTE_C,4,250},{NOTE_C,4,250},
    {NOTE_C,4,250},{NOTE_G,4,250},{NOTE_C,4,250},{NOTE_D,4,250},{NOTE_D,4,500},{NOTE_D,4,500},{NOTE_E,4,500},
    {NOTE_G,3,125},{NOTE_A,3,125},{NOTE_G,3,125},{NOTE_A,3,125},{NOTE_A,3,125},{NOTE_E,3,125},{NOTE_G,3,125},
    {NOTE_E,3,125},{NOTE_E,3,125},{NOTE_A,3,250},{NOTE_D,3,250},{NOTE_E,3,250},{NOTE_A,4,250},{NOTE_C,5,250},
    {NOTE_D,5,250},{NOTE_A,4,250},{NOTE_E,5,125},{NOTE_G,5,125},{NOTE_E,5,125},{NOTE_C,5,125},{NOTE_C,5,250},
    {NOTE_D,5,250},{NOTE_A,3,250},{NOTE_G,3,125},{NOTE_A,3,125},{NOTE_A,3,125},{NOTE_E,3,125},{NOTE_G,3,250},
    {NOTE_E,3,125},{NOTE_D,3,250},{NOTE_D,3,250},{NOTE_E,3,250},{NOTE_C,6,250},{NOTE_B,5,250},{NOTE_G,5,250},
    {NOTE_E,5,250},{NOTE_D,5,125},{NOTE_C,5,125},{NOTE_D,5,125},{NOTE_A,5,125},{NOTE_A,3,250},{NOTE_B,3,250},
    {NOTE_C,4,250},{NOTE_B,3,125},{NOTE_G,3,125},{NOTE_G,3,125},{NOTE_A,3,125},{NOTE_E,3,250},{NOTE_E,3,250},
    {NOTE_E,3,250},{NOTE_C,4,250},{NOTE_B,3,125},{NOTE_G,3,125},{NOTE_G,3,125},{NOTE_A,3,125},{NOTE_A,3,250},
    {NOTE_A,3,250},{NOTE_A,3,250},{NOTE_D,4,250},{NOTE_C,4,125},{NOTE_D,4,125},{NOTE_D,4,125},{NOTE_C,4,125},
    {NOTE_D,4,250},{NOTE_C,4,250},{NOTE_E,4,250},{NOTE_E,4,125},{NOTE_E,4,125},{NOTE_E,4,125},{NOTE_E,4,125},
    {NOTE_E,4,250},{NOTE_E,4,250},{NOTE_D,4,250},{NOTE_B,3,250},{NOTE_D,4,250},{NOTE_E,4,250},{NOTE_C,4,250},
    {NOTE_D,4,250},{NOTE_C,4,125},{NOTE_D,4,125},{NOTE_D,4,250},{NOTE_C,4,250},{NOTE_D,4,250},{NOTE_E,4,250},
    {NOTE_C,4,250},{NOTE_D,4,250},{NOTE_C,4,125},{NOTE_D,4,125},{NOTE_D,4,250},{NOTE_C,4,250},{NOTE_D,4,250},
    {NOTE_E,4,250},{NOTE_C,4,125},{NOTE_E,4,125},{NOTE_E,4,250},{NOTE_G,4,250},{NOTE_G,4,500},{NOTE_A,4,500},
    {NOTE_G,4,500},{NOTE_E,4,250},{NOTE_D,4,250},{NOTE_C,4,250},{NOTE_D,4,250},{NOTE_E,4,250},{NOTE_E,4,250},
    {NOTE_C,4,250},{NOTE_D,4,250},{NOTE_C,4,250},{NOTE_C,4,250},{NOTE_C,4,125},{NOTE_C,4,125},{NOTE_C,4,125},
    {NOTE_C,4,125},{NOTE_C,4,125},{NOTE_C,4,125},{NOTE_C,4,125},{NOTE_C,4,250},{NOTE_D,4,250},{NOTE_C,4,500},
    {NOTE_C,4,125},{NOTE_C,4,125},{NOTE_C,4,125},{NOTE_C,4,125},{NOTE_C,4,125},{NOTE_C,4,125},{NOTE_C,4,125},
    {NOTE_C,4,250},{NOTE_G,4,250},{NOTE_C,4,500},{NOTE_E,4,500},{NOTE_D,4,250},{NOTE_E,4,250},{NOTE_D,4,250},
    {NOTE_E,4,250},{NOTE_D,4,250},{NOTE_E,4,250},{NOTE_E,4,125},{NOTE_D,4,250},{NOTE_D,4,125},{NOTE_D,4,250},
    {NOTE_C,4,250},{NOTE_D,4,250},{NOTE_E,4,250},{NOTE_C,4,250},{NOTE_D,4,125},{NOTE_C,4,250},{NOTE_D,4,125},
    {NOTE_D,4,250},{NOTE_C,4,250},{NOTE_E,4,500},{NOTE_C,4,125},{NOTE_E,4,125},{NOTE_E,4,250},{NOTE_G,4,250},
    {NOTE_G,4,500},{NOTE_G,4,125},{NOTE_A,4,125},{NOTE_G,4,250},{NOTE_G,4,500},{NOTE_E,4,250},{NOTE_E,4,125},
    {NOTE_D,4,250},{NOTE_C,4,250},{NOTE_D,4,250},{NOTE_E,4,250},{NOTE_E,4,250},{NOTE_C,4,250},{NOTE_D,4,250},
    {NOTE_C,4,250},{NOTE_C,4,500},{NOTE_C,4,125},{NOTE_C,4,125},{NOTE_C,4,125},{NOTE_C,4,125},{NOTE_C,4,125},
    {NOTE_C,4,125},{NOTE_C,4,125},{NOTE_C,4,125},{NOTE_D,4,250},{NOTE_C,4,500},{NOTE_C,4,125},{NOTE_C,4,125},
    {NOTE_C,4,125},{NOTE_C,4,125},{NOTE_C,4,125},{NOTE_C,4,125},{NOTE_C,4,125},{NOTE_C,4,250},{NOTE_G,4,250},
    {NOTE_C,4,500},{NOTE_E,4,500},{NOTE_D,4,250},{NOTE_E,4,250},{NOTE_D,4,250},{NOTE_E,4,250},{NOTE_G,4,250},
    {NOTE_E,4,250},{NOTE_E,4,250},{NOTE_A,3,250},{NOTE_C,4,250},{NOTE_D,4,250},{NOTE_E,4,250},{NOTE_D,4,125},
    {NOTE_D,4,125},{NOTE_D,4,125},{NOTE_C,4,250},{NOTE_C,4,125},{NOTE_A,3,250},{NOTE_A,3,125},{NOTE_G,3,250},
    {NOTE_E,3,250},{NOTE_A,3,250},{NOTE_E,4,125},{NOTE_E,4,250},{NOTE_E,4,250},{NOTE_A,3,250},{NOTE_C,4,250},
    {NOTE_D,4,250},{NOTE_E,4,250},{NOTE_D,4,125},{NOTE_D,4,125},{NOTE_D,4,125},{NOTE_C,4,250},{NOTE_C,4,125},
    {NOTE_A,3,250},{NOTE_A,3,125},{NOTE_G,3,250},{NOTE_E,3,250},{NOTE_A,3,250},{NOTE_G,4,125},{NOTE_G,4,250},
    {NOTE_E,4,250},{NOTE_A,3,250},{NOTE_C,4,250},{NOTE_D,4,250},{NOTE_E,4,250},{NOTE_E,4,125},{NOTE_E,4,125},
    {NOTE_D,4,125},{NOTE_E,4,250},{NOTE_D,4,125},{NOTE_E,4,250},{NOTE_D,4,125},{NOTE_E,4,500},{NOTE_E,4,250},
    {NOTE_D,4,125},{NOTE_E,4,125},{NOTE_E,4,125},{NOTE_D,4,125},{NOTE_E,4,250},{NOTE_E,4,250},{NOTE_D,4,250},
    {NOTE_C,4,250},{NOTE_G,4,250},{NOTE_D,4,500},{NOTE_D,4,250},{NOTE_E,4,250},{NOTE_D,4,125},{NOTE_E,4,250},
    {NOTE_A,3,250},{NOTE_A,3,125},{NOTE_A,3,125},{NOTE_A,3,125},{NOTE_G,3,125},{NOTE_A,3,250},{NOTE_A,3,125},
    {NOTE_G,3,125},{NOTE_A,3,125},{NOTE_G,3,125},{NOTE_A,3,250},{NOTE_E,4,250},{NOTE_E,4,250},{NOTE_D,4,250},
    {NOTE_D,4,250},{NOTE_C,4,250},{NOTE_E,4,250},{NOTE_A,3,500},{NOTE_C,5,125},{NOTE_B,4,125},{NOTE_G,4,125},
    {NOTE_E,4,125},{NOTE_D,4,125},{NOTE_G,4,125},{NOTE_D,4,125},{NOTE_E,4,125},{NOTE_C,5,125},{NOTE_B,4,125},
    {NOTE_G,4,125},{NOTE_E,4,125},{NOTE_D,4,125},{NOTE_G,4,125},{NOTE_D,4,125},{NOTE_E,4,125},{NOTE_A,4,125},
    {NOTE_G,4,125},{NOTE_A,4,125},{NOTE_B,4,125},{NOTE_C,5,125},{NOTE_D,5,125},{NOTE_B,4,125},{NOTE_C,5,125},
    {NOTE_D,5,125},{NOTE_B,4,125},{NOTE_C,5,125},{NOTE_G,5,125},{NOTE_F,5,125},{NOTE_E,5,125},{NOTE_D,5,125},
    {NOTE_C,5,125},{NOTE_A,5,125},{NOTE_G,5,125},{NOTE_A,5,125},{NOTE_B,5,125},{NOTE_C,6,125},{NOTE_B,6,125},
    {NOTE_C,6,125},{NOTE_D,6,125},{NOTE_E,6,125},{NOTE_D,6,125},{NOTE_E,6,125},{NOTE_G,6,125},{NOTE_G,6,125},
    {NOTE_E,6,125},{NOTE_D,6,125},{NOTE_C,6,125},{NOTE_A,5,125},{NOTE_D,6,125},{NOTE_E,6,125},{NOTE_A,5,125},
    {NOTE_D,6,125},{NOTE_E,6,125},{NOTE_A,5,125},{NOTE_G,6,125},{NOTE_A,6,125},{NOTE_E,6,125},{NOTE_G,6,125},
    {NOTE_A,6,125},{NOTE_A,5,125},{NOTE_D,6,125},{NOTE_E,6,250},{NOTE_E,3,500},{NOTE_E,3,250},{NOTE_D,3,250},
    {NOTE_E,3,250},{NOTE_F,3,250},{NOTE_G,3,500},{NOTE_E,3,250},{NOTE_F,3,250},{NOTE_G,3,250},{NOTE_G,3,125},
    {NOTE_G,3,250},{NOTE_F,3,250},{NOTE_E,3,250},{NOTE_D,3,250},{NOTE_D,3,250},{NOTE_E,3,125},{NOTE_E,3,250},
    {NOTE_F,3,250},{NOTE_E,3,500},{NOTE_E,3,250},{NOTE_E,3,125},{NOTE_E,3,125},{NOTE_D,3,250},{NOTE_E,3,250},
    {NOTE_G,3,250},{NOTE_G,3,500},{NOTE_E,3,250},{NOTE_F,3,250},{NOTE_G,3,250},{NOTE_G,3,125},{NOTE_G,3,250},
    {NOTE_C,4,250},{NOTE_B,3,250},{NOTE_G,3,250},{NOTE_D,4,250},{NOTE_D,4,125},{NOTE_D,4,250},{NOTE_E,4,250},
    {NOTE_E,4,500},{NOTE_E,4,500},{NOTE_E,4,250},{NOTE_D,4,250},{NOTE_C,4,250},{NOTE_D,4,250},{NOTE_B,3,500},
    {NOTE_G,3,250},{NOTE_G,3,250},{NOTE_A,3,500},{NOTE_C,4,250},{NOTE_C,4,250},{NOTE_C,4,250},{NOTE_D,4,250},
    {NOTE_E,4,500},{NOTE_E,3,250},{NOTE_E,4,500},{NOTE_E,4,250},{NOTE_D,4,250},{NOTE_C,4,250},{NOTE_D,4,250},
    {NOTE_B,3,500},{NOTE_G,3,250},{NOTE_G,3,250},{NOTE_A,3,500},{NOTE_C,4,250},{NOTE_C,4,250},{NOTE_C,4,250},
    {NOTE_G,4,250},{NOTE_D,4,1000},{NOTE_D,4,250},{NOTE_E,4,250},{NOTE_E,4,250},{NOTE_A,3,250},{NOTE_G,3,125},
    {NOTE_A,3,125},{NOTE_A,3,125},{NOTE_E,3,125},{NOTE_G,3,250},{NOTE_D,3,125},{NOTE_E,3,250},{NOTE_A,2,125},
    {NOTE_D,3,250},{NOTE_E,3,250},{NOTE_A,4,250},{NOTE_C,5,250},{NOTE_D,5,250},{NOTE_A,4,250},{NOTE_E,5,125},
    {NOTE_G,5,125},{NOTE_E,5,125},{NOTE_C,5,125},{NOTE_C,5,250},{NOTE_D,5,250},{NOTE_A,3,250},{NOTE_G,3,125},
    {NOTE_A,3,125},{NOTE_A,3,125},{NOTE_E,3,125},{NOTE_G,3,250},{NOTE_D,3,125},{NOTE_E,3,125},{NOTE_D,3,250},
    {NOTE_D,3,250},{NOTE_E,3,250},{NOTE_C,6,250},{NOTE_B,5,250},{NOTE_G,5,250},{NOTE_E,5,250},{NOTE_D,5,125},
    {NOTE_C,5,125},{NOTE_D,5,125},{NOTE_A,5,125},{NOTE_A,3,250},{NOTE_B,3,250},{NOTE_C,4,250},{NOTE_B,3,125},
    {NOTE_G,3,125},{NOTE_G,3,125},{NOTE_A,3,125},{NOTE_E,3,250},{NOTE_E,3,250},{NOTE_E,3,250},{NOTE_C,4,250},
    {NOTE_C,3,125},{NOTE_B,3,125},{NOTE_G,3,125},{NOTE_G,3,125},{NOTE_A,3,250},{NOTE_A,3,250},{NOTE_A,3,250},
    {NOTE_A,4,250},{NOTE_D,4,125},{NOTE_C,4,125},{NOTE_D,4,125},{NOTE_D,4,125},{NOTE_C,4,250},{NOTE_D,4,250},
    {NOTE_C,4,500},{NOTE_E,4,250},{NOTE_E,4,125},{NOTE_E,4,125},{NOTE_E,4,125},{NOTE_E,4,125},{NOTE_E,4,250},
    {NOTE_E,4,250},{NOTE_D,4,250},{NOTE_B,3,250},{NOTE_D,4,250},{NOTE_E,4,500},{NOTE_D,4,500},{NOTE_E,4,250},
    {NOTE_F,4,125},{NOTE_F,4,250},{NOTE_E,4,250},{NOTE_D,4,500},{NOTE_E,4,1500},{NOTE_E,4,2000},{NOTE_B,3,250},
    {NOTE_G,3,250},{NOTE_B,3,250},{NOTE_C,4,250},{NOTE_B,3,250},{NOTE_C,4,250},{NOTE_D,4,250},{NOTE_E,4,250},
    {NOTE_E,4,250},{NOTE_D,4,125},{NOTE_C,4,125},{NOTE_E,4,250},{NOTE_A,4,250},{NOTE_G,4,250},{NOTE_E,4,250},
    {NOTE_D,4,250},{NOTE_C,4,250},{NOTE_A,3,250},{NOTE_E,4,125},{NOTE_E,4,250},{NOTE_D,4,250},{NOTE_E,4,250},
    {NOTE_B,3,125},{NOTE_B,3,250},{NOTE_G,3,250},{NOTE_C,4,250},{NOTE_G,4,125},{NOTE_G,4,250},{NOTE_F,4,250},
    {NOTE_E,4,250},{NOTE_D,4,125},{NOTE_D,4,250},{NOTE_C,4,250},{NOTE_C,4,250},{NOTE_G,3,250},{NOTE_C,4,250},
    {NOTE_E,4,250},{NOTE_G,4,250},{NOTE_E,4,250},{NOTE_B,3,250},{NOTE_D,4,500},{NOTE_C,4,250},{NOTE_D,4,250},
    {NOTE_C,4,125},{NOTE_D,4,125},{NOTE_D,4,250},{NOTE_C,4,250},{NOTE_D,4,250},{NOTE_E,4,250},{NOTE_C,4,250},
    {NOTE_D,4,250},{NOTE_C,4,125},{NOTE_D,4,125},{NOTE_D,4,250},{NOTE_C,4,250},{NOTE_D,4,250},{NOTE_E,4,250},
    {NOTE_C,4,125},{NOTE_E,4,125},{NOTE_E,4,250},{NOTE_G,4,250},{NOTE_G,4,500},{NOTE_A,4,500},{NOTE_G,4,500},
    {NOTE_E,4,250},{NOTE_E,4,125},{NOTE_D,4,250},{NOTE_C,4,250},{NOTE_D,4,250},{NOTE_E,4,250},{NOTE_E,4,250},
    {NOTE_C,4,250},{NOTE_D,4,250},{NOTE_C,4,250},{NOTE_C,4,500},{NOTE_C,4,125},{NOTE_C,4,125},{NOTE_C,4,125},
    {NOTE_C,4,125},{NOTE_C,4,125},{NOTE_C,4,125},{NOTE_C,4,125},{NOTE_C,4,125},{NOTE_C,4,125},{NOTE_D,4,250},
    {NOTE_C,4,500},{NOTE_C,4,125},{NOTE_C,4,125},{NOTE_C,4,125},{NOTE_C,4,125},{NOTE_C,4,125},{NOTE_C,4,125},
    {NOTE_C,4,125},{NOTE_C,4,250},{NOTE_G,4,250},{NOTE_C,4,500},{NOTE_E,4,500},{NOTE_D,4,250},{NOTE_E,4,250},
    {NOTE_D,4,250},{NOTE_E,4,250},{NOTE_D,4,250},{NOTE_E,4,250},{NOTE_E,4,125},{NOTE_D,4,250},{NOTE_D,4,125},
    {NOTE_D,4,250},{NOTE_C,4,250},{NOTE_D,4,250},{NOTE_E,4,250},{NOTE_C,4,250},{NOTE_D,4,125},{NOTE_C,4,250},
    {NOTE_D,4,125},{NOTE_D,4,250},{NOTE_C,4,250},{NOTE_E,4,500},{NOTE_C,4,125},{NOTE_E,4,125},{NOTE_E,4,250},
    {NOTE_G,4,250},{NOTE_G,4,500},{NOTE_G,4,125},{NOTE_A,4,250},{NOTE_G,4,500},{NOTE_E,4,250},{NOTE_E,4,125},
    {NOTE_D,4,250},{NOTE_C,4,250},{NOTE_D,4,250},{NOTE_E,4,250},{NOTE_E,4,250},{NOTE_C,4,250},{NOTE_D,4,250},
    {NOTE_C,4,250},{NOTE_C,4,500},{NOTE_C,4,125},{NOTE_C,4,125},{NOTE_C,4,125},{NOTE_C,4,125},{NOTE_C,4,125},
    {NOTE_C,4,125},{NOTE_C,4,125},{NOTE_C,4,125},{NOTE_D,4,250},{NOTE_C,4,500},{NOTE_C,4,125},{NOTE_C,4,125},
    {NOTE_C,4,125},{NOTE_C,4,125},{NOTE_C,4,125},{NOTE_C,4,125},{NOTE_C,4,125},{NOTE_C,4,250},{NOTE_G,4,250},
    {NOTE_C,4,500},{NOTE_E,4,500},{NOTE_D,4,250},{NOTE_E,4,250},{NOTE_D,4,250},{NOTE_E,4,250},{NOTE_G,4,250},
    {NOTE_E,4,250},{NOTE_E,4,250},{NOTE_A,3,250},{NOTE_C,4,250},{NOTE_D,4,250},{NOTE_E,4,250},{NOTE_D,4,125},
    {NOTE_D,4,125},{NOTE_D,4,125},{NOTE_C,4,250},{NOTE_C,4,125},{NOTE_A,3,250},{NOTE_A,3,125},{NOTE_G,3,250},
    {NOTE_E,3,250},{NOTE_A,3,250},{NOTE_E,4,125},{NOTE_E,4,250},{NOTE_E,4,250},{NOTE_A,3,250},{NOTE_C,4,250},
    {NOTE_D,4,250},{NOTE_E,4,250},{NOTE_D,4,125},{NOTE_D,4,125},{NOTE_D,4,125},{NOTE_C,4,250},{NOTE_C,4,125},
    {NOTE_A,3,250},{NOTE_A,3,125},{NOTE_G,3,250},{NOTE_E,3,250},{NOTE_A,3,250},{NOTE_G,4,125},{NOTE_G,4,250},
    {NOTE_E,4,250},{NOTE_A,3,250},{NOTE_C,4,250},{NOTE_D,4,250},{NOTE_E,4,250},{NOTE_E,4,125},{NOTE_E,4,125},
    {NOTE_D,4,125},{NOTE_E,4,250},{NOTE_D,4,125},{NOTE_E,4,250},{NOTE_D,4,125},{NOTE_E,4,500},{NOTE_E,4,250},
    {NOTE_D,4,125},{NOTE_E,4,125},{NOTE_E,4,125},{NOTE_D,4,125},{NOTE_E,4,250},{NOTE_E,4,250},{NOTE_D,4,250},
    {NOTE_C,4,250},{NOTE_G,4,250},{NOTE_D,4,500},{NOTE_D,4,250},{NOTE_E,4,250},{NOTE_D,4,125},{NOTE_E,4,250},
    {NOTE_A,3,250},{NOTE_A,3,125},{NOTE_A,3,125},{NOTE_A,3,125},{NOTE_G,3,125},{NOTE_A,3,250},{NOTE_A,3,125},
    {NOTE_G,3,125},{NOTE_A,3,125},{NOTE_G,3,125},{NOTE_A,3,250},{NOTE_E,4,250},{NOTE_E,4,250},{NOTE_D,4,250},
    {NOTE_D,4,250},{NOTE_C,4,250},{NOTE_E,4,250},{NOTE_A,3,500},{NOTE_E,4,250},{NOTE_A,3,250},{NOTE_G,3,250},
    {NOTE_A,3,250},{NOTE_E,4,250},{NOTE_A,3,250},{NOTE_G,3,250},{NOTE_A,3,250},{NOTE_A,3,250},{NOTE_E,4,250},
    {NOTE_E,5,250},{NOTE_D,5,250},{NOTE_C,5,250},{NOTE_B,4,250},{NOTE_G,4,250},{NOTE_A,3,250},{NOTE_A,3,125},
    {NOTE_A,3,125},{NOTE_A,3,125},{NOTE_G,3,125},{NOTE_A,3,250},{NOTE_A,3,125},{NOTE_G,3,125},{NOTE_A,3,125},
    {NOTE_G,3,125},{NOTE_A,3,250},{NOTE_F,4,125},{NOTE_G,4,125},{NOTE_G,4,250},{NOTE_E,4,250},{NOTE_D,4,250},
    {NOTE_C,4,250},{NOTE_E,4,500},{NOTE_A,3,500},{NOTE_C,5,125},{NOTE_B,4,125},{NOTE_G,4,125},{NOTE_E,4,125},
    {NOTE_E,4,125},{NOTE_G,4,125},{NOTE_D,4,125},{NOTE_E,4,125},{NOTE_C,5,125},{NOTE_B,4,125},{NOTE_G,4,125},
    {NOTE_E,4,125},{NOTE_E,4,125},{NOTE_G,4,125},{NOTE_D,4,125},{NOTE_E,4,125},{NOTE_A,4,125},{NOTE_G,4,125},
    {NOTE_A,4,125},{NOTE_B,4,125},{NOTE_C,5,125},{NOTE_D,5,125},{NOTE_B,4,125},{NOTE_C,5,125},{NOTE_D,5,125},
    {NOTE_B,4,125},{NOTE_C,5,125},{NOTE_G,5,125},{NOTE_F,5,125},{NOTE_E,5,125},{NOTE_D,5,125},{NOTE_C,5,125},
    {NOTE_A,5,125},{NOTE_G,5,125},{NOTE_A,5,125},{NOTE_B,5,125},{NOTE_C,6,125},{NOTE_B,5,125},{NOTE_C,6,125},
    {NOTE_D,6,125},{NOTE_E,6,125},{NOTE_D,6,125},{NOTE_E,6,125},{NOTE_G,6,125},{NOTE_G,6,125},{NOTE_E,6,125},
    {NOTE_D,6,125},{NOTE_C,6,125},{NOTE_A,5,125},{NOTE_D,6,125},{NOTE_E,6,125},{NOTE_A,5,125},{NOTE_D,6,125},
    {NOTE_E,6,125},{NOTE_A,5,125},{NOTE_G,6,125},{NOTE_A,6,125},{NOTE_E,6,125},{NOTE_G,6,125},{NOTE_A,6,125},
    {NOTE_A,5,125},{NOTE_D,6,125},{NOTE_E,6,250},{NOTE_C,5,250},{NOTE_B,4,250},{NOTE_G,4,250},{NOTE_A,4,250},
    {NOTE_E,4,250},{NOTE_G,4,250},{NOTE_D,4,250},{NOTE_E,4,250},{NOTE_C,4,250},{NOTE_D,4,250},{NOTE_B,3,250},
    {NOTE_C,4,250},{NOTE_G,3,250},{NOTE_B,3,250},{NOTE_E,3,250},{NOTE_G,3,250},{NOTE_A,3,500},{NOTE_E,4,500},
    {NOTE_D,4,500},{NOTE_E,4,500},{NOTE_B,3,500},{NOTE_A,3,500},{NOTE_C,4,500},{NOTE_D,4,500},{NOTE_E,4,500},
    {NOTE_G,4,500},{NOTE_A,4,500}
};
uint16_t tonesize = sizeof(melody)/sizeof(note);
//~~~~~~~~~~music~~~~~~~~~~~~~~~music~~~~~~~~~~~~~~music~~~~~~~~~~~~~~~~~music~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//music是移植过来的,直接删掉也没问题的哟~

const int LOADCELL_DOUT_PIN = 34;//对应HX711模块的DO引脚
const int LOADCELL_SCK_PIN = 4;//对应HX711模块的CK引脚
const int SENSOR_CALIBRATION = -23.8846;//传感器校准值
const int TF_MOSI_PIN = 23;//对应TF卡模块的MOSI引脚
const int TF_MISO_PIN = 19;//对应TF卡模块的MISO引脚
const int TF_SCK_PIN = 18;//对应TF卡模块的SCK引脚
const int TF_CS_PIN = 5;//对应TF卡模块的CS引脚
const int LED_PIN = 2;//板载led
const int BUZZER_PIN = 15;//蜂鸣器引脚
const int FIRE_PIN = 27;//继电器引脚 

int fire_time=10;
//int key[] = {6,7,4,2,1,3,5};

std::string BTdata;
String BTData;
HX711 scale;
//File TF_password;
File TF;
BluetoothSerial SerialBT;
SPIClass CustomSPI;

/*
void Password(std::string& c,int kry[]){
  int len = c.size();
  for(int i = 0;i < len;i++){
    c[i] = c[i] ^ key[i % 7];
  }
}
*/

void HX711_init(){
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  scale.set_gain(128);
  scale.set_scale(SENSOR_CALIBRATION);
  scale.tare();

}

void HX711_read(){
    TF.println(scale.get_units());
}
 
void BT_init(){
  SerialBT.begin("无线试车台V2_by_bbbbmmdddd");
}

void Fire(bool a){
  if(a==1){
    digitalWrite(FIRE_PIN,LOW);
  }
  else if(a==0){
    digitalWrite(FIRE_PIN,HIGH);
  }
}

void led(bool a){
  if(a==1){
    digitalWrite(LED_PIN,HIGH);
  }
  else if(a==0){
    digitalWrite(LED_PIN,LOW);
  }
}

void times(int time){
  for(int i=0;i<time && !SerialBT.available();i++){
     delay(400);
     if(time > 1){
      String time1="倒计时"+ String(time-i) + "秒";
      SerialBT.println(time1);
      tone(BUZZER_PIN,988);
      delay(100);
      noTone(BUZZER_PIN);
      led(1);
      delay(500);
      led(0);
    }
    else{
      SerialBT.println("点火!");
    }   
  }
}

void TF_init(){
  CustomSPI.begin(TF_SCK_PIN, TF_MISO_PIN, TF_MOSI_PIN, TF_CS_PIN);
  pinMode(TF_CS_PIN,OUTPUT);
}

void setup() {
  pinMode(FIRE_PIN,OUTPUT);
  digitalWrite(FIRE_PIN,HIGH);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_PIN,OUTPUT);
  tone(BUZZER_PIN,532);
  delay(100);
  tone(BUZZER_PIN,587);
  delay(100);
  tone(BUZZER_PIN,659);
  delay(300);
  noTone(BUZZER_PIN);
  HX711_init();
  BT_init();
  TF_init();
  Serial.begin(115200);
  if(!SD.begin(TF_CS_PIN,CustomSPI, 1000000)){
    SerialBT.println("TF卡初始化失败!");
    tone(BUZZER_PIN,988);
    delay(3000);
    noTone(BUZZER_PIN);
  }
  else{
    SerialBT.println("TF卡初始化成功!");
  }
  TF = SD.open("/thrust.txt",FILE_WRITE);
  if(TF){
    TF.println("欢迎使用无线试车台V1.2_by:bbbbmmdddd");
    TF.close();
  }
  else {
    tone(BUZZER_PIN,988);
    delay(500);
    tone(BUZZER_PIN,988);
    delay(500);
    noTone(BUZZER_PIN);
  }
  //TF_password = SD.open("password.txt",FILE_WRITE);
  //SerialBT.println("欢迎使用无线试车台V1.2 请输入密码:");

}

//~~~~~~~~~~music~~~~~~~~~~~~~~~music~~~~~~~~~~~~~~music~~~~~~~~~~~~~~~~~music~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void music(){
  ledcSetup(0,20000,10);
  ledcAttachPin(buzzer,0);
  for(int i=yesterday;i<tonesize;i++){
    ledcWriteNote(0,melody[i].yinfu,melody[i].yingao);
    delay(melody[i].shichang*0.77);
    yesterday=i;
    if(SerialBT.available()){
      break;
    }
  }
  if(yesterday >= tonesize-1){
    yesterday = 0;
  }
  delay(300);
  tone(BUZZER_PIN,988);
  delay(100);
  noTone(BUZZER_PIN);
  ledcWrite(0,0);
}
//~~~~~~~~~~music~~~~~~~~~~~~~~~music~~~~~~~~~~~~~~music~~~~~~~~~~~~~~~~~music~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void CallBTData(String Data );

void loop() {
  if(SerialBT.available()) {
    BTData = SerialBT.readStringUntil('\xff');
    SerialBT.println(BTData);//测试
    CallBTData(BTData);
  }

}

void CallBTData(String Data ){
  if(Data.substring(0,3) == "未定义功能"){
    SerialBT.println("未定义功能");
  }
  else if(Data.substring(0,3) == "未定义功能"){
    SerialBT.println("未定义功能");
  }
  else if(Data.substring(0,5) == "music"){
    SerialBT.println("已获取记录,播放中");
    music();
  }
  else if(Data.substring(0,4) == "STOP"){
    SerialBT.println("复位中...");
    tone(BUZZER_PIN,659);
    led(1);
    delay(100);
    tone(BUZZER_PIN,587);
    led(0);
    delay(100);
    tone(BUZZER_PIN,532);
    delay(300);
    ESP.restart();
  }
  else if(Data.substring(0,4) == "Fire"){
    tone(BUZZER_PIN,988);
    delay(200);
    tone(BUZZER_PIN,988);
    delay(200);
    noTone(BUZZER_PIN);
    SerialBT.println("请输入倒计时时间(单位秒)");
    while(!SerialBT.available()){
      delay(100);
    }
    BTData = SerialBT.readStringUntil('\xff');
    fire_time = BTData.toInt();
    times(fire_time);
    tone(BUZZER_PIN,988);
    delay(1000);
    noTone(BUZZER_PIN);
    Fire(1);
    SerialBT.println("开始采集!");
    led(1);
    TF = SD.open("/thrust.txt",FILE_WRITE);
    while(!SerialBT.available()){
      HX711_read();
    }
    Fire(0);
    TF.close();
    SerialBT.println("关闭采集!");
    led(0);
    tone(BUZZER_PIN,659);
    delay(100);
    noTone(BUZZER_PIN);
  }
}
