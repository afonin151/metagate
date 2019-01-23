#ifndef INIT_PROXY_H
#define INIT_PROXY_H

#include "../InitInterface.h"

#include <QObject>

#include <memory>
#include <future>

struct TypedException;

class WebSocketClient;

namespace proxy {
class Proxy;
class ProxyJavascript;
class WebSocketSender;
}

class MainWindow;

namespace initializer {

class InitializerJavascript;

class InitProxy: public InitInterface {
    Q_OBJECT
public:

    using Return = std::tuple<proxy::Proxy*, proxy::ProxyJavascript*, proxy::WebSocketSender*>;

    using Callback = std::function<void()>;

public:

    InitProxy(QThread *mainThread, Initializer &manager);

    ~InitProxy() override;

    void completeImpl() override;

    Return initialize(std::shared_future<WebSocketClient*> wssClient, std::shared_future<MainWindow*> mainWindow);

    static int countEvents() {
        return 3;
    }

    static int countCriticalEvents() {
        return 1;
    }

    static QString stateName();

private:

    void sendInitSuccess(const TypedException &exception);

    void sendUpnpStarted(bool isScipped, const TypedException &exception);

    void sendProxyStarted(bool isScipped, const TypedException &exception);

signals:

    void callbackCall(const InitProxy::Callback &callback);

    void upnpStarted(const TypedException &exception);

    void proxyStarted(const TypedException &exception);

private slots:

    void onCallbackCall(const InitProxy::Callback &callback);

    void onUpnpStarted(const TypedException &exception);

    void onProxyStarted(const TypedException &exception);

private:

    std::unique_ptr<proxy::Proxy> proxyManager;
    std::unique_ptr<proxy::ProxyJavascript> proxyJavascript;
    std::unique_ptr<proxy::WebSocketSender> proxyWebsocket;

};

}

#endif // INIT_PROXY_H
