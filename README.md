# ATM_PROJECT


Project introduction:
This project aims to design and develop an Automated Teller Machine (ATM) system
that simulates the main functionalities of a real-world ATM, including card insertion,
PIN verification, transaction authorization, and balance update. The system consists
of two microcontroller units (MCUs) - one for the ATM and the other for the card -
that communicate with each other to execute the transaction process.

Project description :-
The system's functionality can be summarized in the following points:
• The ATM MCU will handle the transaction main flows, including displaying
welcome messages, requesting card insertion, entering PIN, and validating
transactions.
• The CARD MCU has two modes of operation - programming and user mode. In
programming mode, the MCU will request the user to enter the card PAN and PIN
and store them in the EEPROM. In user mode, the CARD MCU will send a trigger
signal to the ATM MCU to initiate the transaction flow.
• The project uses a hard-coded array of structures for accounts that contain the PAN,
account state (blocked/running), and balance.
• The maximum allowed limit for a transaction is hardcoded as $5000.00.
• The system will perform several checks on the database before finalizing the
transaction, including verifying the card PAN, checking if the card is blocked,
checking if the amount required exceeds the maximum daily limit, and verifying the
available balance.
• If all checks pass, the system will display the remaining balance and eject the card. If
any check fails, the system will display a declined message accordingly and initiate an
alarm if necessary.


Hardware components:
• ATM ECU • CARD ECU
1. ATM MCU 1. CARD MCU
2. 16 x 2 LCD 2. EEPROM
3. 3 x 3 Keypad 3. Serial Terminal
4. Buzzer
5. Enter/Set Button
