set(TRANSACTION_SOURCES
    ../Transaction/Transaction.cpp
    ../Transaction/WithdrawTransaction.cpp
    ../Transaction/DepositTransaction.cpp
    ../Transaction/TransactionInvoker.cpp
)

add_library(Transaction ${TRANSACTION_SOURCES})

target_link_libraries(Transaction Account)
