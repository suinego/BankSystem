set(ACCOUNT_SOURCES
    ../Account/Account.cpp
    ../Account/CreditAccount.cpp
    ../Account/DepositAccount.cpp
    ../Account/DebitAccount.cpp
)
add_library(Account ${ACCOUNT_SOURCES})

target_link_libraries(Account Transaction)

target_link_libraries(Account Client)
