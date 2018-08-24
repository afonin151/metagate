#ifndef TRANSACTIONSJAVASCRIPT_H
#define TRANSACTIONSJAVASCRIPT_H

#include <QObject>

#include <functional>

#include "TypedException.h"

#include "Transaction.h"

namespace transactions {

class Transactions;

class TransactionsJavascript
    : public QObject
{    
    Q_OBJECT
public:

    using Callback = std::function<void()>;

public:

    explicit TransactionsJavascript(QObject *parent = nullptr);

    void setTransactions(Transactions &trans) {
        transactionsManager = &trans;
    }

signals:

    void jsRunSig(QString jsString);

    void callbackCall(const Callback &callback);

signals:

    void newBalanceSig(const QString &address, const QString &currency, const BalanceInfo &balance);

    void sendedTransactionsResponseSig(QString requestId, QString server, QString response, TypedException error);

public slots:

    void onCallbackCall(const Callback &callback);

private slots:

    void onNewBalance(const QString &address, const QString &currency, const BalanceInfo &balance);

    void onSendedTransactionsResponse(QString requestId, QString server, QString response, TypedException error);

public slots:

    Q_INVOKABLE void registerAddress(QString address, QString currency, QString type, QString group, QString name);

    Q_INVOKABLE void registerAddresses(QString addressesJson);

    Q_INVOKABLE void getAddresses(QString group);

    Q_INVOKABLE void setCurrentGroup(QString group);

    Q_INVOKABLE void getTxs(QString address, QString currency, QString fromTx, int count, bool asc);

    Q_INVOKABLE void getTxsAll(QString currency, QString fromTx, int count, bool asc);

    Q_INVOKABLE void getTxs2(QString address, QString currency, int from, int count, bool asc);

    Q_INVOKABLE void getTxsAll2(QString currency, int from, int count, bool asc);

    Q_INVOKABLE void calcBalance(QString address, QString currency);

private:

    template<typename... Args>
    void makeAndRunJsFuncParams(const QString &function, const TypedException &exception, Args&& ...args);

    void runJs(const QString &script);

private:

    Transactions *transactionsManager;
};

}

#endif // TRANSACTIONSJAVASCRIPT_H