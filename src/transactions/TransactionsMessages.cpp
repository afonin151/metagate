#include "TransactionsMessages.h"

#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonValue>
#include <QJsonObject>

#include "check.h"
#include "Log.h"

#include "Transaction.h"

namespace transactions {

QString makeGetBalanceRequest(const QString &address) {
    return "{\"id\":1,\"params\":{\"address\": \"" + address + "\"},\"method\":\"fetch-balance\", \"pretty\": false}";
}

BalanceInfo parseBalanceResponse(const QString &response) {
    const QJsonDocument jsonResponse = QJsonDocument::fromJson(response.toUtf8());
    CHECK(jsonResponse.isObject(), "Incorrect json ");
    const QJsonObject &json1 = jsonResponse.object();
    CHECK(json1.contains("result") && json1.value("result").isObject(), "Incorrect json: result field not found");
    const QJsonObject &json = json1.value("result").toObject();

    BalanceInfo result;

    CHECK(json.contains("address") && json.value("address").isString(), "Incorrect json: address field not found");
    result.address = json.value("address").toString();
    CHECK(json.contains("received"), "Incorrect json: received field not found");
    if (json.value("received").isDouble()) {
        result.received = QString::fromStdString(std::to_string(uint64_t(json.value("received").toDouble())));
    } else if (json.value("received").isString()) {
        result.received = json.value("received").toString();
    } else {
        throwErr("Incorrect json: received field not found");
    }
    CHECK(json.contains("spent"), "Incorrect json: spent field not found");
    if (json.value("spent").isDouble()) {
        result.spent = QString::fromStdString(std::to_string(uint64_t(json.value("spent").toDouble())));
    } else if (json.value("spent").isString()) {
        result.spent = json.value("spent").toString();
    } else {
        throwErr("Incorrect json: spent field not found");
    }
    CHECK(json.contains("count_received") && json.value("count_received").isDouble(), "Incorrect json: count_received field not found");
    result.countReceived = static_cast<uint64_t>(json.value("count_received").toDouble());
    CHECK(json.contains("count_spent") && json.value("count_spent").isDouble(), "Incorrect json: count_spent field not found");
    result.countSpent = static_cast<uint64_t>(json.value("count_spent").toDouble());
    CHECK(json.contains("currentBlock") && json.value("currentBlock").isDouble(), "Incorrect json: currentBlock field not found");
    result.currBlockNum = static_cast<uint64_t>(json.value("currentBlock").toDouble());

    return result;
}

QString makeGetHistoryRequest(const QString &address, bool isCnt, uint64_t cnt) {
    if (isCnt) {
        return "{\"id\":1,\"params\":{\"address\": \"" + address + "\"},\"method\":\"fetch-history\", \"pretty\": false}";
    } else {
        return "{\"id\":1,\"params\":{\"address\": \"" + address + "\", \"countTxs\": " + QString::fromStdString(std::to_string(cnt)) + "},\"method\":\"fetch-history\", \"pretty\": false}";
    }
}

QString makeGetTxRequest(const QString &hash) {
    return "{\"id\":1,\"params\":{\"hash\": \"" + hash + "\"},\"method\":\"get-tx\", \"pretty\": false}";
}

static Transaction parseTransaction(const QJsonObject &txJson) {
    Transaction res;

    CHECK(txJson.contains("from") && txJson.value("from").isString(), "Incorrect json: from field not found");
    res.from = txJson.value("from").toString();
    CHECK(txJson.contains("to") && txJson.value("to").isString(), "Incorrect json: to field not found");
    res.to = txJson.value("to").toString();
    CHECK(txJson.contains("value"), "Incorrect json: value field not found");
    if (txJson.value("value").isDouble()) {
        res.value = QString::fromStdString(std::to_string(uint64_t(txJson.value("value").toDouble())));
    } else if (txJson.value("value").isString()) {
        res.value = txJson.value("value").toString();
    }
    CHECK(txJson.contains("transaction") && txJson.value("transaction").isString(), "Incorrect json: transaction field not found");
    res.tx = txJson.value("transaction").toString();
    if (txJson.contains("data") && txJson.value("data").isString()) {
        res.data = txJson.value("data").toString();
    }
    CHECK(txJson.contains("timestamp") && txJson.value("timestamp").isDouble(), "Incorrect json: timestamp field not found");
    res.timestamp = static_cast<uint64_t>(txJson.value("timestamp").toDouble());
    if (txJson.contains("fee")) {
        if (txJson.value("fee").isDouble()) {
            res.fee = QString::fromStdString(std::to_string(static_cast<int64_t>(txJson.value("fee").toDouble())));
        } else if (txJson.value("fee").isString()) {
            res.fee = txJson.value("fee").toString();
        }
    }
    if (res.fee.isEmpty() || res.fee.isNull()) {
        res.fee = "0";
    }
    if (txJson.contains("nonce") && txJson.value("nonce").isDouble()) {
        res.nonce = txJson.value("nonce").toInt();
    }

    return res;
}

std::vector<Transaction> parseHistoryResponse(const QString &address, const QString &response) {
    const QJsonDocument jsonResponse = QJsonDocument::fromJson(response.toUtf8());
    CHECK(jsonResponse.isObject(), "Incorrect json ");
    const QJsonObject &json1 = jsonResponse.object();
    CHECK(json1.contains("result") && json1.value("result").isArray(), "Incorrect json: result field not found");
    const QJsonArray &json = json1.value("result").toArray();

    std::vector<Transaction> result;

    for (const QJsonValue &elementJson: json) {
        CHECK(elementJson.isObject(), "Incorrect json");
        const QJsonObject txJson = elementJson.toObject();

        Transaction res = parseTransaction(txJson);

        res.isInput = res.from == address;

        result.emplace_back(res);
        if (res.from == address && res.to == address) {
            Transaction res2 = res;
            res2.isInput = false;
            result.emplace_back(res2);
        }
    }

    return result;
}

QString makeSendTransactionRequest(QString to, QString value, QString nonce, QString data, QString fee, QString pubkey, QString sign) {
    QJsonObject request;
    request.insert("jsonrpc", "2.0");
    request.insert("method", "mhc_send");
    QJsonObject params;
    params.insert("to", to);
    params.insert("value", value);
    params.insert("nonce", nonce);
    params.insert("data", data);
    params.insert("fee", fee);
    params.insert("pubkey", pubkey);
    params.insert("sign", sign);
    request.insert("params", params);
    return QString(QJsonDocument(request).toJson(QJsonDocument::Compact));
}

QString parseSendTransactionResponse(const QString &response) {
    const QJsonDocument jsonResponse = QJsonDocument::fromJson(response.toUtf8());
    CHECK(jsonResponse.isObject(), "Incorrect json ");
    const QJsonObject &json1 = jsonResponse.object();
    CHECK_TYPED(!json1.contains("error") || !json1.value("error").isString(), TypeErrors::TRANSACTIONS_SERVER_SEND_ERROR, json1.value("error").toString().toStdString());

    CHECK(json1.contains("params") && json1.value("params").isString(), "Incorrect json: params field not found");
    return json1.value("params").toString();
}

Transaction parseGetTxResponse(const QString &response) {
    const QJsonDocument jsonResponse = QJsonDocument::fromJson(response.toUtf8());
    CHECK(jsonResponse.isObject(), "Incorrect json ");
    const QJsonObject &json1 = jsonResponse.object();
    CHECK(!json1.contains("error") || !json1.value("error").isObject(), json1.value("error").toObject().value("message").toString().toStdString());

    CHECK(json1.contains("result") && json1.value("result").isObject(), "Incorrect json: result field not found");
    const QJsonObject &obj = json1.value("result").toObject();
    CHECK(obj.contains("transaction") && obj.value("transaction").isObject(), "Incorrect json: transaction field not found");
    const QJsonObject &transaction = obj.value("transaction").toObject();
    return parseTransaction(transaction);
}

}
