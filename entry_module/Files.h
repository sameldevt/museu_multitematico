#pragma once
#ifndef FILES
#define FILES

#define INITIAL_SCREEN "entry_resources\\ansi\\initial_screen.ans"
#define FINAL_SCREEN "entry_resources\\ansi\\final_screen.ans"
#define LOADING_SCREEN "entry_resources\\ansi\\loading_screen.ans"
#define THANK_YOU_SCREEN "entry_resources\\ansi\\thank_you_screen.ans"
#define WELCOME_SCREEN "entry_resources\\ansi\\welcome_screen.ans"
#define TICKET_MENU "entry_resources\\ansi\\ticket_menu.ans"
#define TICKET_TYPE_MENU "entry_resources\\ansi\\ticket_type_menu.ans"

#define MENU_MAIN "entry_resources\\ansi\\main_menu.ans"
#define MENU_TICKETS "entry_resources\\ansi\\ticket_menu.ans"
#define MENU_THEMES "entry_resources\\ansi\\themes.ans"
#define MENU_THEMES_CHOICE "entry_resources\\ansi\\themes_choice.ans"

#define MENU_PAYMENT "entry_resources\\ansi\\payment.ans"
#define MENU_PIX_PAYMENT "entry_resources\\ansi\\payment_pix.ans"
#define MENU_PIX_PAYMENT_OK "entry_resources\\ansi\\payment_pix_ok.ans"
#define MENU_PIX_PAYMENT_FAIL "entry_resources\\ansi\\payment_pix_fail.ans"

#define MENU_CARD_PAYMENT "entry_resources\\ansi\\payment_card.ans"
#define MENU_CARD_PAYMENT_NUMBER_FAIL "entry_resources\\ansi\\payment_card_fail_number.ans"
#define MENU_CARD_PAYMENT_CVC_FAIL "entry_resources\\ansi\\payment_card_fail_cvc.ans"
#define MENU_CARD_PAYMENT_OK "entry_resources\\ansi\\payment_card_ok.ans"

#define MENU_NORMAL_ENTRY "entry_resources\\ansi\\normal_entry.ans"
#define MENU_NORMAL_ENTRY_OK "entry_resources\\ansi\\normal_entry_ok.ans"

#define MENU_FREE_ENTRY "entry_resources\\ansi\\free_entry_menu.ans"

#define MENU_JUNIOR_ENTRY "entry_resources\\ansi\\junior_entry.ans"
#define MENU_JUNIOR_ENTRY_OK "entry_resources\\ansi\\junior_entry_ok.ans"
#define MENU_JUNIOR_ENTRY_FAIL "entry_resources\\ansi\\junior_entry_fail.ans"

#define MENU_SENIOR_ENTRY "entry_resources\\ansi\\senior_entry.ans"
#define MENU_SENIOR_ENTRY_OK "entry_resources\\ansi\\senior_entry_ok.ans"
#define MENU_SENIOR_ENTRY_FAIL "entry_resources\\ansi\\senior_entry_fail.ans"

#define MENU_HALFPRICE_ENTRY "entry_resources\\ansi\\half_entry_menu.ans"

#define MENU_DISABLED_PERSON_ENTRY "entry_resources\\ansi\\disabled_entry.ans"
#define MENU_DISABLED_PERSON_ENTRY_OK "entry_resources\\ansi\\disabled_entry_ok.ans"
#define MENU_DISABLED_PERSON_ENTRY_FAIL "entry_resources\\ansi\\disabled_entry_fail.ans"

#define MENU_STUDENT_ENTRY "entry_resources\\ansi\\student_entry.ans"
#define MENU_STUDENT_ENTRY_OK "entry_resources\\ansi\\student_entry_ok.ans"
#define MENU_STUDENT_ENTRY_FAIL "entry_resources\\ansi\\student_entry_fail.ans"

void loadLoadingScreen();

void loadScreen(char screen_path[100]);

void readFile(char file_path[100]);

void writeFile(char file_path[100], char input[]);

#endif