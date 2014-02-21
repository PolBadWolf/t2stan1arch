
#include "main.h"
/*
#include "iom128.h"
#include "ina90.h"

#include "defect.h"
#include "eeprom_user.h"
#include "lcd_16.h"
*/

// датчик работа стана
#define work_pin    ( PINE & 1<<5 )

// счетчик для устранения дребезга
unsigned char work_count;
// верхний предел счетсчика
unsigned char work_up=50;
// тригер состояние датчиков дефектов
unsigned char work_tr;
// событие состояния датчика работы стана
unsigned char work_fg;


// массив сохранения расстояния до КК
unsigned int kk_sm[20];
// 8 - дефект
// 9 - стык

// флаг включение покраски
unsigned char kk_f_def;
unsigned char kk_f_styk;


// начальная настройка
void pulsar_init(void)
{
    kk_f_def=0;
    kk_f_styk=0;
    // загрузка растояния до КК
    // kk_load();
}

// чтение датчика
unsigned char work_test(void)
{
  return work_pin==0;
}

// от системного таймера ( для сигнала работа стана )
void pulsar_int(void)
{
  if ( work_test()==0 )
  {
    if ( work_count>0 )   work_count--;
    if ( work_count==0 && work_tr!=0 )
    if ( work_count==0 )
    {
      work_tr=0;
      work_fg=0;
    }
  }
  else
  {
    if ( work_count<work_up ) work_count++;
    if ( work_count==work_up && work_tr!=255 )
    {
      work_tr=255;
      work_fg=255;
    }
  }
//-------------------------------------------
    // мд-32
    if ( kk_f_def>0 && work_tr==255 )
    {
        DDRF |= (1<<1);
        PORTF &= ~(1<<1);
    }
    else
    {
        PORTF |= (1<<1);
    }
    // стыколов
    if ( kk_f_styk>0 && work_tr==255 )
    {
        DDRF |= (1<<2);
        PORTF &= ~(1<<2);
    }
    else
    {
        PORTF |= (1<<2);
    }

}

// цикл проверки дефекта
void pulsar_cicle(void)
{
    unsigned char kkf_def;
    unsigned char kkf_styk;
    unsigned char kkf_def_1;
    unsigned char kkf_styk_1;

    // kkf_def = d_def[kk_sm[8]] & ( (1<<1) | (1<<2) );
    // kkf_styk = d_def[kk_sm[9]] & ( (1<<0) | (1<<5) );
    
    kkf_def = d_def[ indx_norm(7) ] & (1<<1);
    kkf_styk = d_def[ indx_norm(7) ] & ( (1<<0) | (1<<3) );
    // отладка - вместо стыка покраска толщиномера
    //kkf_styk = d_def[ indx_norm(7) ] & ( (1<<4) | (1<<5) );
    //kkf_def = 0;
    
    // ShowDigitZ(12,3,d_def[kk_sm[8]] );

    if ( kkf_def >0 )   { kk_f_def =8; kkf_def_1 = kkf_def; }
    if ( kkf_styk>0 )   { kk_f_styk=8; kkf_styk_1 = kkf_styk; }
    if ( kk_f_def >0 )  { kk_f_def--;   kkf_def_1 = 0; }
    if ( kk_f_styk>0 )  { kk_f_styk--;  kkf_styk_1 = 0; }
    
    if ( kk_f_def>0 )
    {
        d_def[ indx_norm(0) ] |= kkf_def_1;
        d_def[ indx_norm(7) ] |= kkf_def_1;
    }
    if ( kk_f_styk>0 )
    {
        d_def[ indx_norm(0) ] |= kkf_styk_1;
        d_def[ indx_norm(7) ] |= kkf_styk_1;
    }
}



