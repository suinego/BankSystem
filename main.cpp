#include <cstring>
#include <iostream>
#include <string>

#include "raylib.h"
#include "src/BankFacade.hpp"
#include "unistd.h"

enum AppState {
  REGISTRATION,
  AUTHORIZATION,
  CLIENT_MAIN,
  CLIENT_TRANSACTION,
  ADMIN_MAIN,
  ADMIN_TRANSACTION,
  CREATE_ACCOUNT,
  TRANSFER,
  TOP_UP
};

int64_t selectedTextBox = 0;

void HandleTextInput(std::string& text, int64_t textBoxIndex, float posX,
                     float posY, float width, float height) {
  Rectangle textbox = {posX, posY, width, height};

  if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) &&
      CheckCollisionPointRec(GetMousePosition(), textbox)) {
    SetMouseCursor(MOUSE_CURSOR_IBEAM);
    selectedTextBox = textBoxIndex;
  }
  if (selectedTextBox == textBoxIndex) {
    int64_t key = GetKeyPressed();
    if (key >= 32 && key <= 125 && text.size() < 13) {
      text += static_cast<char>(key);
    } else if (key == KEY_BACKSPACE && !text.empty()) {
      text.pop_back();
    }
  }
}

//--------------------------REGISTRATION_WINDOW ------------------------

void DrawRegistrationWindow(bool& registration_done, std::string& first_name,
                            std::string& last_name, std::string& passport,
                            std::string& address, AppState& curr_state) {
  const int64_t inputBoxWidth = 400;
  const int64_t inputBoxHeight = 60;
  const int64_t buttonWidth = 300;
  const int64_t buttonHeight = 80;
  const int64_t spacing = 80;

  float screenWidth = GetScreenWidth();
  float screenHeight = GetScreenHeight();

  DrawText("Registration\n",
           screenWidth / 2 - MeasureText("Registration", 30) / 2 - 150, 50, 80,
           BLACK);

  HandleTextInput(first_name, 0, screenWidth / 2 - inputBoxWidth / 2, 220,
                  inputBoxWidth, inputBoxHeight);
  HandleTextInput(last_name, 1, screenWidth / 2 - inputBoxWidth / 2,
                  220 + spacing + 60, inputBoxWidth, inputBoxHeight);
  HandleTextInput(passport, 2, screenWidth / 2 - inputBoxWidth / 2,
                  220 + spacing * 2 + 120, inputBoxWidth, inputBoxHeight);
  HandleTextInput(address, 3, screenWidth / 2 - inputBoxWidth / 2,
                  220 + spacing * 3 + 180, inputBoxWidth, inputBoxHeight);

  DrawText("First Name:\n", screenWidth / 2 - inputBoxWidth / 2, 150, 40,
           BLACK);
  DrawRectangle(screenWidth / 2 - inputBoxWidth / 2, 220, inputBoxWidth,
                inputBoxHeight, DARKPURPLE);
  DrawText(first_name.c_str(), screenWidth / 2 - inputBoxWidth / 2 + 5, 225, 40,
           WHITE);

  DrawText("Last Name:\n", screenWidth / 2 - inputBoxWidth / 2, 220 + spacing,
           40, BLACK);
  DrawRectangle(screenWidth / 2 - inputBoxWidth / 2, 220 + spacing + 60,
                inputBoxWidth, inputBoxHeight, DARKPURPLE);
  DrawText(last_name.c_str(), screenWidth / 2 - inputBoxWidth / 2 + 5,
           225 + spacing + 60, 40, WHITE);

  DrawText("Passport:\n", screenWidth / 2 - inputBoxWidth / 2,
           220 + spacing * 2 + 60, 40, BLACK);
  DrawRectangle(screenWidth / 2 - inputBoxWidth / 2, 220 + spacing * 2 + 120,
                inputBoxWidth, inputBoxHeight, DARKPURPLE);
  DrawText(passport.c_str(), screenWidth / 2 - inputBoxWidth / 2 + 5,
           225 + spacing * 2 + 120, 40, WHITE);

  DrawText("Address:\n", screenWidth / 2 - inputBoxWidth / 2,
           220 + spacing * 3 + 120, 40, BLACK);
  DrawRectangle(screenWidth / 2 - inputBoxWidth / 2, 220 + spacing * 3 + 180,
                inputBoxWidth, inputBoxHeight, DARKPURPLE);
  DrawText(address.c_str(), screenWidth / 2 - inputBoxWidth / 2 + 5,
           225 + spacing * 3 + 180, 40, WHITE);

  Rectangle registerButtonRec = {screenWidth / 2 - buttonWidth / 2, 730,
                                 buttonWidth, buttonHeight};
  if (CheckCollisionPointRec(GetMousePosition(), registerButtonRec)) {
    DrawRectangleRec(registerButtonRec, PURPLE);
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
      registration_done = true;
      curr_state = CLIENT_MAIN;
    }
  } else {
    DrawRectangleRec(registerButtonRec, DARKPURPLE);
  }

  DrawText("Register", screenWidth / 2 - MeasureText("Register", 40) / 2,
           730 + buttonHeight / 2 - 20, 40, WHITE);

  DrawText("Already registered? Click here to login.",
           screenWidth / 2 -
               MeasureText("Already registered? Click here to login.", 20) / 2,
           830, 20, BLACK);

  Rectangle loginButtonRec = {screenWidth / 2 - buttonWidth / 2, 860,
                              buttonWidth, buttonHeight};
  if (CheckCollisionPointRec(GetMousePosition(), loginButtonRec)) {
    DrawRectangleRec(loginButtonRec, PURPLE);
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
      registration_done = true;
      curr_state = AUTHORIZATION;
    }
  } else {
    DrawRectangleRec(loginButtonRec, DARKPURPLE);
  }

  DrawText("Login", screenWidth / 2 - MeasureText("Login", 40) / 2,
           860 + buttonHeight / 2 - 20, 40, WHITE);
}

//-------------------------------REGISTRATION----------------------------------------

void Registration(AppState& curr_state, BankFacade& bank,
                  ProccessSQL& sql_proccess) {
  std::string first_name;
  std::string last_name;
  std::string passport;
  std::string address;
  bool registration_done = false;

  while (!registration_done) {
    BeginDrawing();
    ClearBackground(PINK);

    DrawRegistrationWindow(registration_done, first_name, last_name, passport,
                           address, curr_state);
    if (curr_state != REGISTRATION) {
      break;
    }
    EndDrawing();
  }
  if (!first_name.empty() && !last_name.empty()) {
    bank.Registration(sql_proccess, first_name, last_name, passport, address);
  } else if (curr_state != AUTHORIZATION) {
    curr_state = REGISTRATION;
  }
}

//----------------------------AUTHORIZATION----------------------------

void DrawAuthorizationWindow(bool& authorization_done, std::string& first_name,
                             std::string& last_name, AppState& curr_state) {
  const int64_t inputBoxWidth = 400;
  const int64_t inputBoxHeight = 60;
  const int64_t buttonWidth = 300;
  const int64_t buttonHeight = 80;
  const int64_t spacing = 80;

  float screenWidth = GetScreenWidth();
  float screenHeight = GetScreenHeight();

  DrawText("Authorization\n",
           screenWidth / 2 - MeasureText("Authorization", 30) / 2 - 150, 50, 80,
           BLACK);

  HandleTextInput(first_name, 0, screenWidth / 2 - inputBoxWidth / 2, 220,
                  inputBoxWidth, inputBoxHeight);
  HandleTextInput(last_name, 1, screenWidth / 2 - inputBoxWidth / 2,
                  220 + spacing + 60, inputBoxWidth, inputBoxHeight);

  DrawText("First Name:\n", screenWidth / 2 - inputBoxWidth / 2, 150, 40,
           BLACK);
  DrawRectangle(screenWidth / 2 - inputBoxWidth / 2, 220, inputBoxWidth,
                inputBoxHeight, DARKPURPLE);
  DrawText(first_name.c_str(), screenWidth / 2 - inputBoxWidth / 2 + 5, 225, 40,
           WHITE);

  DrawText("Last Name:\n", screenWidth / 2 - inputBoxWidth / 2, 220 + spacing,
           40, BLACK);
  DrawRectangle(screenWidth / 2 - inputBoxWidth / 2, 220 + spacing + 60,
                inputBoxWidth, inputBoxHeight, DARKPURPLE);
  DrawText(last_name.c_str(), screenWidth / 2 - inputBoxWidth / 2 + 5,
           225 + spacing + 60, 40, WHITE);

  Rectangle loginButtonRec = {screenWidth / 2 - buttonWidth / 2, 500,
                              buttonWidth, buttonHeight};
  if (CheckCollisionPointRec(GetMousePosition(), loginButtonRec)) {
    DrawRectangleRec(loginButtonRec, PURPLE);
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
      curr_state = CLIENT_MAIN;
      authorization_done = true;
    }
  } else {
    DrawRectangleRec(loginButtonRec, DARKPURPLE);
  }

  DrawText("Login", screenWidth / 2 - MeasureText("Login", 40) / 2,
           500 + buttonHeight / 2 - 20, 40, WHITE);
}

//-------------------------AUTHORIZATION------------------------

void Authorization(AppState& curr_state, BankFacade& bank,
                   ProccessSQL& sql_proccess) {
  std::string first_name;
  std::string last_name;
  bool authorization_done = false;

  while (!authorization_done) {
    BeginDrawing();
    ClearBackground(PINK);

    DrawAuthorizationWindow(authorization_done, first_name, last_name,
                            curr_state);

    EndDrawing();
  }
  if (!first_name.empty() && !last_name.empty()) {
    bank.Authorization(sql_proccess, first_name, last_name);
  } else {
    curr_state = REGISTRATION;
  }
}

//-----------------------------CLIENTMAIN----------------------------

void ClientMain(AppState& curr_state, BankFacade& bank,
                ProccessSQL& sql_proccess) {
  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(PINK);

    std::string data_client =
        sql_proccess.GetFirstName() + " " + sql_proccess.GetLastName();
    DrawText((data_client + "\n").c_str(),
             GetScreenWidth() / 2 - MeasureText(data_client.c_str(), 80) / 2,
             50, 80, BLACK);

    DrawRectangle(GetScreenWidth() / 2 - 350, 150, GetScreenWidth() / 2 - 250,
                  600, DARKPURPLE);
    DrawText("Accounts:\n",
             GetScreenWidth() / 2 - MeasureText("Accounts:", 80) / 2, 160, 50,
             WHITE);
    std::set<std::shared_ptr<Account>> accounts = sql_proccess.GetAccountsSet();
    int64_t y = 250;
    for (const auto& account : accounts) {
      DrawText(("Account ID: " + std::to_string(account->GetId())).c_str(),
               GetScreenWidth() / 2 - 300, y, 30, WHITE);
      DrawText(("Balance: " + std::to_string(int64_t(account->GetBalance())))
                   .c_str(),
               GetScreenWidth() / 2 + 50, y, 30, WHITE);
      y += 50;
    }

    float buttonWidth = 300;
    float buttonHeight = 70;
    float buttonSpacing = 20;
    float exitButtonX = 300;
    float exitButtonY = 800;

    // Exit Button
    Rectangle exitButtonRec = {exitButtonX, exitButtonY, buttonWidth,
                               buttonHeight};
    if (CheckCollisionPointRec(GetMousePosition(), exitButtonRec)) {
      DrawRectangleRec(exitButtonRec, PURPLE);
      if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        curr_state = REGISTRATION;
        break;
      }
    } else {
      DrawRectangleRec(exitButtonRec, DARKPURPLE);
    }

    DrawText("Log Out",
             exitButtonX + buttonWidth / 2 - MeasureText("Log Out", 40) / 2,
             exitButtonRec.y + buttonHeight / 2 - 20, 40, WHITE);

    // Transaction Button
    Rectangle transactionButtonRec = {exitButtonX + 500, exitButtonY,
                                      buttonWidth, buttonHeight};
    if (CheckCollisionPointRec(GetMousePosition(), transactionButtonRec)) {
      DrawRectangleRec(transactionButtonRec, PURPLE);
      if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        curr_state = CLIENT_TRANSACTION;
        break;
      }
    } else {
      DrawRectangleRec(transactionButtonRec, DARKPURPLE);
    }

    DrawText("Transaction",
             exitButtonX + 500 + buttonWidth / 2 -
                 MeasureText("Transaction", 40) / 2,
             exitButtonRec.y + buttonHeight / 2 - 20, 40, WHITE);

    // Create Button
    Rectangle createButtonRec = {exitButtonX + 1000, exitButtonY,
                                 buttonWidth + 80, buttonHeight};
    DrawRectangleRec(createButtonRec, DARKGRAY);
    if (CheckCollisionPointRec(GetMousePosition(), createButtonRec)) {
      DrawRectangleRec(createButtonRec, PURPLE);
      if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        curr_state = CREATE_ACCOUNT;
        break;
      }
    } else {
      DrawRectangleRec(createButtonRec, DARKPURPLE);
    }
    DrawText("Create Account",
             exitButtonX + 1000 + buttonWidth / 2 -
                 MeasureText("Transaction", 40) / 2,
             exitButtonRec.y + buttonHeight / 2 - 20, 40, WHITE);

    EndDrawing();
  }
}

//-------------------CREATE_ACCOUNT_WINDOW---------------------------

void DrawCreateAccountWindow(bool& account_creation_done,
                             std::string& account_type, int64_t& amount,
                             AppState& curr_state) {
  const int64_t inputBoxWidth = 400;
  const int64_t inputBoxHeight = 60;
  const int64_t buttonWidth = 300;
  const int64_t buttonHeight = 80;
  const int64_t spacing = 80;
  float screenWidth = GetScreenWidth();
  float screenHeight = GetScreenHeight();

  DrawText("Create Account\n",
           screenWidth / 2 - MeasureText("Create Account", 30) / 2 - 50, 50, 40,
           BLACK);

  // Account Type Selection
  DrawText("Select Account Type:", screenWidth / 2 - 170, 150, 30, BLACK);

  // Debit Button
  Rectangle debitButtonRec = {screenWidth / 2 - 400, 200, buttonWidth,
                              buttonHeight};
  if (CheckCollisionPointRec(GetMousePosition(), debitButtonRec)) {
    DrawRectangleRec(debitButtonRec, PURPLE);
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
      account_type = "Debit";
    }
  } else {
    DrawRectangleRec(debitButtonRec, DARKPURPLE);
  }
  DrawText("Debit", debitButtonRec.x + 50, debitButtonRec.y + 20, 30, WHITE);

  // Deposit Button
  Rectangle depositButtonRec = {screenWidth / 2 - 100, 200, buttonWidth,
                                buttonHeight};
  if (CheckCollisionPointRec(GetMousePosition(), depositButtonRec)) {
    DrawRectangleRec(depositButtonRec, PURPLE);
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
      account_type = "Deposit";
    }
  } else {
    DrawRectangleRec(depositButtonRec, DARKPURPLE);
  }
  DrawText("Deposit", depositButtonRec.x + 30, depositButtonRec.y + 20, 30,
           WHITE);

  // Credit Button
  Rectangle creditButtonRec = {screenWidth / 2 + 200, 200, buttonWidth,
                               buttonHeight};
  if (CheckCollisionPointRec(GetMousePosition(), creditButtonRec)) {
    DrawRectangleRec(creditButtonRec, PURPLE);
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
      account_type = "Credit";
      int64_t penaltyAmount = amount * 0.005;
      DrawText("Penalty:", screenWidth / 2 - 50, 320, 30, BLACK);
      std::string str_penalty = std::to_string(penaltyAmount);
    }
  } else {
    DrawRectangleRec(creditButtonRec, DARKPURPLE);
  }
  DrawText("Credit", creditButtonRec.x + 40, creditButtonRec.y + 20, 30, WHITE);

  // Draw the input box for amount
  Rectangle amountInputRec = {screenWidth / 2 - 200, 370, 400, inputBoxHeight};
  DrawRectangleRec(amountInputRec, DARKPURPLE);

  // Handle amount input
  std::string str_amount = std::to_string(amount);
  HandleTextInput(str_amount, 0, amountInputRec.x, amountInputRec.y,
                  amountInputRec.width, amountInputRec.height);
  amount = std::stod(str_amount);

  DrawText("Enter Amount:", screenWidth / 2 - 100, 320, 30, BLACK);
  DrawText(std::to_string(amount).c_str(), amountInputRec.x + 5,
           amountInputRec.y + 5, 40, WHITE);

  // Create Button
  Rectangle createButtonRec = {screenWidth / 2 - buttonWidth / 2, 500,
                               buttonWidth, buttonHeight};
  if (CheckCollisionPointRec(GetMousePosition(), createButtonRec)) {
    DrawRectangleRec(createButtonRec, PURPLE);
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
      account_creation_done = true;
      curr_state = CLIENT_MAIN;
    }
  } else {
    DrawRectangleRec(createButtonRec, DARKPURPLE);
  }
  DrawText("Create", createButtonRec.x + 50, createButtonRec.y + 20, 30, WHITE);
}

//-----------------ACCOUNT------------------------------------

void CreateAccount(AppState& curr_state, BankFacade& bank,
                   ProccessSQL& sql_proccess) {
  bool account_creation_done = false;
  std::string account_type;
  int64_t amount = 0;

  while (!account_creation_done) {
    BeginDrawing();
    ClearBackground(PINK);

    DrawCreateAccountWindow(account_creation_done, account_type, amount,
                            curr_state);

    EndDrawing();
  }

  bank.CreateAccount(sql_proccess, account_type, amount, 0);
}

//---------------------TOPUP--------------------------

void DrawTopUpWindow(AppState& curr_state, BankFacade& bank,
                     ProccessSQL& sql_proccess) {
  bool topup_done = false;
  int64_t selected_account_id = 0;
  int64_t amount = 0;

  while (!topup_done) {
    BeginDrawing();
    ClearBackground(PINK);

    DrawText("Top Up\n", GetScreenWidth() / 2 - MeasureText("Top Up", 80) / 2,
             50, 80, BLACK);

    const int64_t inputBoxWidth = 400;
    const int64_t inputBoxHeight = 60;
    const int64_t buttonWidth = 200;
    const int64_t buttonHeight = 60;
    const int64_t spacing = 80;

    float screenWidth = GetScreenWidth();
    float screenHeight = GetScreenHeight();

    std::string str_amount = std::to_string(amount);
    HandleTextInput(str_amount, 0, screenWidth / 2 - inputBoxWidth / 2, 220,
                    inputBoxWidth, inputBoxHeight);
    amount = std::stod(str_amount);

    DrawText("Enter Amount:", screenWidth / 2 - inputBoxWidth / 2, 150, 40,
             BLACK);
    DrawRectangle(screenWidth / 2 - inputBoxWidth / 2, 220, inputBoxWidth,
                  inputBoxHeight, DARKPURPLE);
    DrawText(std::to_string(amount).c_str(),
             screenWidth / 2 - inputBoxWidth / 2 + 5, 225, 40, WHITE);

    Rectangle topupButtonRec = {float(GetScreenWidth() / 2 - 100), 500, 200,
                                60};
    if (CheckCollisionPointRec(GetMousePosition(), topupButtonRec)) {
      DrawRectangleRec(topupButtonRec, PURPLE);
      if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && amount > 0) {
        bank.TopUp(sql_proccess, selected_account_id, amount);
        DrawText("Successful", topupButtonRec.x - 30, topupButtonRec.y - 30, 30,
                 WHITE);
        topup_done = true;
        curr_state = CLIENT_MAIN;
      }
    } else {
      DrawRectangleRec(topupButtonRec, DARKPURPLE);
    }
    DrawText("Top Up", topupButtonRec.x + 50, topupButtonRec.y + 20, 30, WHITE);

    Rectangle backButtonRec = {float(GetScreenWidth() / 2 - 100), 600, 200, 60};
    if (CheckCollisionPointRec(GetMousePosition(), backButtonRec)) {
      DrawRectangleRec(backButtonRec, PURPLE);
      if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        topup_done = true;
        curr_state = CLIENT_MAIN;
      }
    } else {
      DrawRectangleRec(backButtonRec, DARKPURPLE);
    }
    DrawText("Back", backButtonRec.x + 50, backButtonRec.y + 20, 30, WHITE);

    EndDrawing();
  }
}

//----------------------------TRANSACTION_WINDOW------------------------------------

void DrawTransferWindow(AppState& curr_state, BankFacade& bank,
                        ProccessSQL& sql_proccess) {
  bool transfer_done = false;
  int64_t sender_account_id = 0;
  int64_t receiver_account_id = 0;
  int64_t amount = 0;

  while (!transfer_done) {
    BeginDrawing();
    ClearBackground(PINK);

    DrawText("Transfer\n",
             GetScreenWidth() / 2 - MeasureText("Transfer", 80) / 2, 50, 80,
             BLACK);

    const int64_t inputBoxWidth = 400;
    const int64_t inputBoxHeight = 60;
    const int64_t buttonWidth = 200;
    const int64_t buttonHeight = 60;
    const int64_t spacing = 80;

    float screenWidth = GetScreenWidth();
    float screenHeight = GetScreenHeight();

    std::string str_sender_account_id = std::to_string(sender_account_id);
    HandleTextInput(str_sender_account_id, 0,
                    screenWidth / 2 - inputBoxWidth / 2, 220, inputBoxWidth,
                    inputBoxHeight);
    sender_account_id = std::stod(str_sender_account_id);

    std::string str_receiver_account_id = std::to_string(receiver_account_id);
    HandleTextInput(str_receiver_account_id, 1,
                    screenWidth / 2 - inputBoxWidth / 2, 220 + spacing + 60,
                    inputBoxWidth, inputBoxHeight);
    receiver_account_id = std::stod(str_receiver_account_id);

    std::string str_amount = std::to_string(amount);
    HandleTextInput(str_amount, 2, screenWidth / 2 - inputBoxWidth / 2,
                    220 + spacing * 2 + 120, inputBoxWidth, inputBoxHeight);
    amount = std::stod(str_amount);

    DrawText("Sender Account ID:", screenWidth / 2 - inputBoxWidth / 2, 150, 40,
             BLACK);
    DrawRectangle(screenWidth / 2 - inputBoxWidth / 2, 220, inputBoxWidth,
                  inputBoxHeight, DARKPURPLE);
    DrawText(std::to_string(sender_account_id).c_str(),
             screenWidth / 2 - inputBoxWidth / 2 + 5, 225, 40, WHITE);

    DrawText("Receiver Account ID:", screenWidth / 2 - inputBoxWidth / 2,
             220 + spacing, 40, BLACK);
    DrawRectangle(screenWidth / 2 - inputBoxWidth / 2, 220 + spacing + 60,
                  inputBoxWidth, inputBoxHeight, DARKPURPLE);
    DrawText(std::to_string(receiver_account_id).c_str(),
             screenWidth / 2 - inputBoxWidth / 2 + 5, 225 + spacing + 60, 40,
             WHITE);

    DrawText("Amount:", screenWidth / 2 - inputBoxWidth / 2,
             220 + spacing * 2 + 60, 40, BLACK);
    DrawRectangle(screenWidth / 2 - inputBoxWidth / 2, 220 + spacing * 2 + 120,
                  inputBoxWidth, inputBoxHeight, DARKPURPLE);
    DrawText(std::to_string(amount).c_str(),
             screenWidth / 2 - inputBoxWidth / 2 + 5, 225 + spacing * 2 + 120,
             40, WHITE);

    Rectangle transferButtonRec = {screenWidth / 2 - buttonWidth / 2, 700,
                                   buttonWidth, buttonHeight};
    if (CheckCollisionPointRec(GetMousePosition(), transferButtonRec)) {
      DrawRectangleRec(transferButtonRec, PURPLE);
      if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        if (sender_account_id != -1 && receiver_account_id != -1 &&
            amount > 0) {
          if (bank.Transaction(sql_proccess, sender_account_id,
                               receiver_account_id, amount)) {
            DrawText("Successful", transferButtonRec.x - 30,
                     transferButtonRec.y - 30, 30, WHITE);
          }
          transfer_done = true;
          curr_state = CLIENT_MAIN;
        }
      }
    } else {
      DrawRectangleRec(transferButtonRec, DARKPURPLE);
    }
    DrawText("Transfer", screenWidth / 2 - MeasureText("Transfer", 40) / 2,
             700 + buttonHeight / 2 - 20, 40, WHITE);

    Rectangle backButtonRec = {screenWidth / 2 - buttonWidth / 2, 800,
                               buttonWidth, buttonHeight};
    if (CheckCollisionPointRec(GetMousePosition(), backButtonRec)) {
      DrawRectangleRec(backButtonRec, PURPLE);
      if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        transfer_done = true;
        curr_state = CLIENT_MAIN;
      }
    } else {
      DrawRectangleRec(backButtonRec, DARKPURPLE);
    }
    DrawText("Back", screenWidth / 2 - MeasureText("Back", 40) / 2,
             800 + buttonHeight / 2 - 20, 40, WHITE);

    EndDrawing();
  }
}

//-------------------------------------TRANSACTION-----------------------

void CreateTransaction(AppState& curr_state, BankFacade& bank,
                       ProccessSQL& sql_proccess) {
  bool transaction_done = false;
  int64_t selected_account_id = -1;
  int64_t amount = 0;

  while (!transaction_done) {
    BeginDrawing();
    ClearBackground(PINK);

    // Draw transaction window
    DrawText("Transaction\n",
             GetScreenWidth() / 2 - MeasureText("Transaction", 80) / 2, 50, 80,
             BLACK);

    // Display list of client's accounts
    DrawRectangle(GetScreenWidth() / 2 - 350, 150, GetScreenWidth() / 2 - 250,
                  600, DARKPURPLE);
    DrawText("Accounts:\n",
             GetScreenWidth() / 2 - MeasureText("Accounts:", 80) / 2, 160, 50,
             WHITE);
    std::set<std::shared_ptr<Account>> accounts = sql_proccess.GetAccountsSet();
    int64_t y = 250;
    for (const auto& account : accounts) {
      DrawText(("Account ID: " + std::to_string(account->GetId())).c_str(),
               GetScreenWidth() / 2 - 300, y, 30, WHITE);
      DrawText(("Balance: " + std::to_string(int64_t(account->GetBalance())))
                   .c_str(),
               GetScreenWidth() / 2 + 50, y, 30, WHITE);
      if (CheckCollisionPointRec(GetMousePosition(),
                                 {100, static_cast<float>(y), 200, 20})) {
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
          selected_account_id = account->GetId();
        }
      }
      y += 50;
    }

    // TopUp Button
    Rectangle topupButtonRec = {float(GetScreenWidth() / 2 - 300), 780, 200,
                                60};
    if (CheckCollisionPointRec(GetMousePosition(), topupButtonRec)) {
      DrawRectangleRec(topupButtonRec, PURPLE);
      if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        transaction_done = true;
        curr_state = TOP_UP;
        break;
      }
    } else {
      DrawRectangleRec(topupButtonRec, DARKPURPLE);
    }
    DrawText("TopUp", topupButtonRec.x + 50, topupButtonRec.y + 20, 30, WHITE);

    // Transfer Button
    Rectangle transferButtonRec = {float(GetScreenWidth() / 2 + 100), 780, 200,
                                   60};
    if (CheckCollisionPointRec(GetMousePosition(), transferButtonRec)) {
      DrawRectangleRec(transferButtonRec, PURPLE);
      if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        transaction_done = true;
        curr_state = TRANSFER;
      }
    } else {
      DrawRectangleRec(transferButtonRec, DARKPURPLE);
    }
    DrawText("Transfer", transferButtonRec.x + 20, transferButtonRec.y + 20, 30,
             WHITE);

    // Back Button
    Rectangle backButtonRec = {float(GetScreenWidth() / 2 - 100), 850, 200, 60};
    if (CheckCollisionPointRec(GetMousePosition(), backButtonRec)) {
      DrawRectangleRec(backButtonRec, PURPLE);
      if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        transaction_done = true;
        curr_state = CLIENT_MAIN;
      }
    } else {
      DrawRectangleRec(backButtonRec, DARKPURPLE);
    }
    DrawText("Back", backButtonRec.x + 50, backButtonRec.y + 20, 30, WHITE);

    EndDrawing();
  }
}

int main() {
  InitWindow(GetScreenWidth(), GetScreenHeight(), "VSBank");

  BankFacade bank;
  ProccessSQL sql_proccess;

  while (!WindowShouldClose()) {
    BeginDrawing();

    ClearBackground(PINK);

    AppState curr_state = REGISTRATION;

    while (!WindowShouldClose()) {
      if (curr_state == REGISTRATION) {
        Registration(curr_state, bank, sql_proccess);
      } else if (curr_state == AUTHORIZATION) {
        Authorization(curr_state, bank, sql_proccess);
      } else if (curr_state == CLIENT_MAIN) {
        ClientMain(curr_state, bank, sql_proccess);
      } else if (curr_state == CLIENT_TRANSACTION) {
        CreateTransaction(curr_state, bank, sql_proccess);
      } else if (curr_state == CREATE_ACCOUNT) {
        CreateAccount(curr_state, bank, sql_proccess);
      } else if (curr_state == TOP_UP) {
        DrawTopUpWindow(curr_state, bank, sql_proccess);
      } else if (curr_state == TRANSFER) {
        DrawTransferWindow(curr_state, bank, sql_proccess);
      } else if (curr_state == ADMIN_MAIN) {
      } else if (curr_state == ADMIN_TRANSACTION) {
      }
    }
    EndDrawing();
  }

  CloseWindow();

  return 0;
}
